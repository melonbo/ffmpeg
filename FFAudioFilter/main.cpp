#include "pch.h"
#include <string>
#include <memory>
#include <thread>
#include <iostream>
#include <windows.h>
#include <dshow.h>
#include <stdio.h>
using namespace std;


AVFormatContext *inputContext = NULL;
AVFormatContext *outputContext;
int64_t lastReadPacketTime;
AVFilterContext *buffersinkCtx;
AVFilterContext *buffersrcCtx;
AVFilterGraph *filterGraph;
AVCodecContext *outPutAudioEncContext = NULL;
int64_t audioCount = 0;
AVPacket packet;

static int interrupt_cb(void *ctx)
{
    int time_out = 3;
    if(av_gettime() - lastReadPacketTime > time_out*1000*1000)
    {
        return -1;
    }
    return 0;
}

int OpenInput(string inputUrl)
{
    inputContext = avformat_alloc_context();
    lastReadPacketTime = av_gettime();
    inputContext->interrupt_callback.callback = interrupt_cb;
    AVInputFormat *ifmt = av_find_input_format("dshow");
    AVDictionary *format_opts = NULL;
    av_dict_set_int(&format_opts, "audio_buffer_size", 20, 0);
    int ret = avformat_open_input(&inputContext, inputUrl.c_str(), ifmt, &format_opts);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Input file open input failed, %lld\n", ret);
        return ret;
    }

    ret = avformat_find_stream_info(inputContext, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Find input file stream inform failed\n");
    }
    else
    {
        ret = avcodec_open2(inputContext->streams[0]->codec, avcodec_find_decoder(inputContext->streams[0]->codec->codec_id), NULL);
        av_log(NULL, AV_LOG_FATAL, "Open input file %s success\n", inputUrl.c_str());
    }
    return ret;
}

int ReadPacketFromSource()
{
    return av_read_frame(inputContext, &packet);
}

int initAudioFilters()
{
    char args[512];
    int ret;
    AVFilter *abuffersrc  = avfilter_get_by_name("abuffer");
    AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();

    AVCodecContext *audioDecoderContext = inputContext->streams[0]->codec;
    if(!audioDecoderContext->channel_layout)
        audioDecoderContext->channel_layout = av_get_default_channel_layout(audioDecoderContext->channels);

    static const enum AVSampleFormat out_sample_fmts[] = { AV_SAMPLE_FMT_FLTP, AV_SAMPLE_FMT_NONE };
    static const int64_t out_channel_layouts[] = { audioDecoderContext->channel_layout, -1 };
    static const int out_sample_rates[] = {audioDecoderContext->sample_rate , -1 };


    AVRational time_base = inputContext->streams[0]->time_base;
    filterGraph = avfilter_graph_alloc();
    filterGraph->nb_threads = 1;

    //args为描述输入帧序列的参数信息
    sprintf_s(args, sizeof(args),
        "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%I64x",
        time_base.num, time_base.den, audioDecoderContext->sample_rate,
        av_get_sample_fmt_name(audioDecoderContext->sample_fmt), audioDecoderContext->channel_layout);

    ret = avfilter_graph_create_filter(&buffersrcCtx, abuffersrc, "in", args, NULL, filterGraph);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer source, ret = %d\n", ret);
        return ret;
    }

    /* buffer audio sink: to terminate the filter chain. */
    ret = avfilter_graph_create_filter(&buffersinkCtx, abuffersink, "out", NULL, NULL, filterGraph);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
        return ret;
    }

    ret = av_opt_set_int_list(buffersinkCtx, "sample_fmts", out_sample_fmts, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
        return ret;
    }

    ret = av_opt_set_int_list(buffersinkCtx, "channel_layouts", out_channel_layouts, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
        return ret;
    }

    ret = av_opt_set_int_list(buffersinkCtx, "sample_rates", out_sample_rates, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output sample rate\n");
        return ret;
    }

    /* Endpoints for the filter graph. */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx =  buffersrcCtx;;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersinkCtx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if ((ret = avfilter_graph_parse_ptr(filterGraph, "anull", &inputs, &outputs, NULL)) < 0)
        return ret;

    if ((ret = avfilter_graph_config(filterGraph, NULL)) < 0)
        return ret;

    av_buffersink_set_frame_size(buffersinkCtx, 1024);
    return 0;
}

int InitAudioEncoderCodec(AVCodecContext *inputAudioCodec)
{
    int ret = 0;
    AVCodec *  audioCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    outPutAudioEncContext = avcodec_alloc_context3(audioCodec);
    outPutAudioEncContext->codec = audioCodec;
    outPutAudioEncContext->sample_rate = inputAudioCodec->sample_rate;
    outPutAudioEncContext->channel_layout = inputAudioCodec->channel_layout;
    outPutAudioEncContext->channels = av_get_channel_layout_nb_channels(inputAudioCodec->channel_layout);
    if(outPutAudioEncContext->channel_layout == 0)
    {
        outPutAudioEncContext->channel_layout = AV_CH_LAYOUT_STEREO;
        outPutAudioEncContext->channels = av_get_channel_layout_nb_channels(outPutAudioEncContext->channel_layout);
    }
    outPutAudioEncContext->sample_fmt = audioCodec->sample_fmts[0];
    outPutAudioEncContext->codec_tag = 0;
    outPutAudioEncContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    ret = avcodec_open2(outPutAudioEncContext, audioCodec, 0);
    return ret;
}

AVFrame *DecodeAudio(AVPacket *packet, AVFrame *pSrcAudioFrame)
{
    AVStream * stream = inputContext->streams[0];
    AVCodecContext* codecContext = stream->codec;
    int gotFrame;
    AVFrame *filtFrame = NULL;
    auto length = avcodec_decode_audio4(codecContext, pSrcAudioFrame, &gotFrame, packet);
    if(length >= 0 && gotFrame != 0)
    {
        if (av_buffersrc_add_frame_flags(buffersrcCtx, pSrcAudioFrame, AV_BUFFERSRC_FLAG_PUSH) < 0) {
            av_log(NULL, AV_LOG_ERROR, "buffe src add frame error!\n");
            return NULL;
        }

        filtFrame = av_frame_alloc();
        int ret = av_buffersink_get_frame_flags(buffersinkCtx, filtFrame, AV_BUFFERSINK_FLAG_NO_REQUEST);
        if(ret < 0)
        {
            av_frame_free(&filtFrame);
            goto error;
        }
        return filtFrame;
    }

error:
    return NULL;
}

void av_packet_rescale_ts(AVPacket *pkt, AVRational src_tb, AVRational dst_tb)
{
    if (pkt->pts != AV_NOPTS_VALUE)
        pkt->pts = av_rescale_q(pkt->pts, src_tb, dst_tb);
    if (pkt->dts != AV_NOPTS_VALUE)
        pkt->dts = av_rescale_q(pkt->dts, src_tb, dst_tb);
    if (pkt->duration > 0)
        pkt->duration = av_rescale_q(pkt->duration, src_tb, dst_tb);
}

int WritePacket(AVPacket *packet)
{
    AVStream *inputStream = inputContext->streams[packet->stream_index];
    AVStream *outputStream = outputContext->streams[packet->stream_index];
    av_packet_rescale_ts(packet,inputStream->time_base,outputStream->time_base);
    return av_interleaved_write_frame(outputContext, packet);
}

int OpenOutput(string outUrl)
{
    int ret  = avformat_alloc_output_context2(&outputContext, NULL, "mpegts", outUrl.c_str());
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "open output context failed\n");
        goto Error;
    }

    ret = avio_open2(&outputContext->pb, outUrl.c_str(), AVIO_FLAG_WRITE,NULL, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "open avio failed");
        goto Error;
    }

    for(int i = 0; i < inputContext->nb_streams; i++)
    {
        AVStream * stream = avformat_new_stream(outputContext, outPutAudioEncContext->codec);
        stream->codec = outPutAudioEncContext;
        AVCodecContext *codecContext = inputContext->streams[0]->codec;
        if(ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "copy coddec context failed");
            goto Error;
        }
    }

    ret = avformat_write_header(outputContext, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "format write header failed");
        goto Error;
    }

    av_log(NULL, AV_LOG_FATAL, " Open output file success %s\n",outUrl.c_str());
    return ret ;
Error:
    if(outputContext)
    {
        for(int i = 0; i < outputContext->nb_streams; i++)
        {
            avcodec_close(outputContext->streams[i]->codec);
        }
        avformat_close_input(&outputContext);
    }
    return ret ;
}

void Init()
{
    av_register_all();
    avfilter_register_all();
    avformat_network_init();
    avdevice_register_all();
    av_log_set_level(AV_LOG_ERROR);
}

static char *dup_wchar_to_utf8(wchar_t *w)
{
    char *s = NULL;
    int l = WideCharToMultiByte(CP_UTF8, 0, w, -1, 0, 0, 0, 0);
    s = (char *)av_malloc(l);
    if (s)
        WideCharToMultiByte(CP_UTF8, 0, w, -1, s, l, 0, 0);
    return s;
}


int main(int argc, char *argv[])
{
    Init();
    AVFrame *pSrcAudioFrame = av_frame_alloc();
    string fileAudioInput = dup_wchar_to_utf8(L"audio=麦克风 (Realtek High Definition Au");
    int ret = OpenInput(fileAudioInput);
    printf("%s\n", fileAudioInput.c_str());

    if(ret < 0) goto Error;

    ret = initAudioFilters();
    if(ret < 0) goto Error;

    ret = InitAudioEncoderCodec(inputContext->streams[0]->codec);
    if(ret < 0) goto Error;

    ret = OpenOutput("D://aac.ts");
    if(ret < 0) goto Error;

    int got_output = 0;

    while(true)
    {
        memset(&packet, 0 , sizeof(packet));
        ReadPacketFromSource();
        AVFrame *filterFrame = DecodeAudio(&packet, pSrcAudioFrame);
        if(filterFrame)
        {
            AVPacket pkt;
            pkt.data = NULL;
            pkt.size = 0;
            ret = avcodec_encode_audio2(outPutAudioEncContext, &pkt, filterFrame, &got_output);
            if(ret >= 0 && got_output)
            {
                int streamTimeBase = outputContext->streams[pkt.stream_index]->time_base.den;
                int codecTimeBase = outputContext->streams[pkt.stream_index]->codec->time_base.den;
                pkt.pts = pkt.dts = (1024*streamTimeBase*audioCount)/codecTimeBase;
                audioCount++;
                if(&pkt)
                {
                    ret = WritePacket(&pkt);
                    if(ret>=0)
                    {
                        cout << "Write Packet Success!" << endl;
                    }
                    else
                    {
                        cout << "Write Packet Failed!" << endl;
                    }
                }
            }
        }
    }
Error:
    //Actually resource need to be released here
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(100));
    }

    return 0;
}

#include <iostream>
#include <iostream>
#include "pch.h"
#include <string>
#include <thread> 
#include <memory>
#include <fstream>
#include <condition_variable>
#include <Winsock2.h>
using namespace std;

AVFormatContext *context = NULL;
AVFormatContext *outputContext;
AVPacket packet;
int64_t lastPts = 0;
int64_t lastDts = 0;
int64_t lastFrameRealtime = 0;

int64_t firstPts = AV_NOPTS_VALUE;
int64_t startTime = 0;

AVCodecContext *outPutEncContext = NULL;
AVCodecContext *decoderContext = NULL;
#define SrcWidth 1920
#define SrcHeight 1080
#define DstWidth 640
#define DstHeight 480

struct SwsContext *pSwsContext;

AVFilterGraph *filter_graph = NULL;
AVFilterContext *buffersink_ctx = NULL;
AVFilterContext *buffersrc_ctx = NULL;

int interrupt_cb(void *ctx)
{
    return 0;
}

//string GetLocalIpAddress()
//{
//    char local[255] = {0};
//    gethostname(local, sizeof(local));
//    hostent* ph = gethostbyname(local);
//    if (ph == NULL)
//        return "";

//    in_addr addr;
//    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip

//    string localIP;
//    localIP.assign(inet_ntoa(addr));

//    return localIP;
//}

void Init()
{
    av_register_all();
    avfilter_register_all();
    avformat_network_init();
    avdevice_register_all();
    av_log_set_level(AV_LOG_ERROR);
}


int OpenInput(char *fileName)
{
    context = avformat_alloc_context();
    context->interrupt_callback.callback = interrupt_cb;
    AVDictionary *format_opts =  NULL;

    int ret = avformat_open_input(&context, fileName, NULL, &format_opts);
    if(ret < 0)
    {
        printf("avformat_open_input error %d\n", ret);
        return  ret;
    }
    ret = avformat_find_stream_info(context,NULL);
    av_dump_format(context, 0, fileName, 0);
    if(ret >= 0)
    {
        std::cout <<"open input stream successfully" << endl;
    }
    return ret;
}

int ReadPacketFromSource()
{
    return av_read_frame(context, &packet);
}

int OpenOutput(char *fileName)
{
    int ret = 0;
    ret  = avformat_alloc_output_context2(&outputContext, NULL, "mpegts", fileName);
    if(ret < 0)
    {
        goto Error;
    }
    ret = avio_open2(&outputContext->pb, fileName, AVIO_FLAG_READ_WRITE,NULL, NULL);
    if(ret < 0)
    {
        goto Error;
    }

    for(int i = 0; i < context->nb_streams; i++)
    {
        AVStream * stream = avformat_new_stream(outputContext, outPutEncContext->codec);
        stream->codec = outPutEncContext;
        if(ret < 0)
        {
            goto Error;
        }
    }
    av_dump_format(outputContext, 0, fileName, 1);
    ret = avformat_write_header(outputContext, NULL);
    if(ret < 0)
    {
        goto Error;
    }
    if(ret >= 0)
        cout <<"open output stream successfully" << endl;
    return ret ;
Error:
    if(outputContext)
    {
        avformat_close_input(&outputContext);
    }
    return ret ;
}

void CloseInput()
{
    if(context != NULL)
    {
        avformat_close_input(&context);
    }
}

void CloseOutput()
{
    if(outputContext != NULL)
    {
        for(int i = 0 ; i < outputContext->nb_streams; i++)
        {
            AVCodecContext *codecContext = outputContext->streams[i]->codec;
            avcodec_close(codecContext);
        }
        avformat_close_input(&outputContext);
    }
}

int InitEncoderCodec( int iWidth, int iHeight)
{
    AVCodec *  pH264Codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(NULL == pH264Codec)
    {
        printf("%s", "avcodec_find_encoder failed");
        return  -1;
    }
    outPutEncContext = avcodec_alloc_context3(pH264Codec);
    outPutEncContext->gop_size = 30;
    outPutEncContext->has_b_frames = 0;
    outPutEncContext->max_b_frames = 0;
    outPutEncContext->codec_id = pH264Codec->id;
    outPutEncContext->time_base.num =context->streams[0]->codec->time_base.num;
    outPutEncContext->time_base.den = context->streams[0]->codec->time_base.den;
    outPutEncContext->pix_fmt            = *pH264Codec->pix_fmts;
    outPutEncContext->width              =  iWidth;
    outPutEncContext->height             = iHeight;

    outPutEncContext->me_subpel_quality = 0;
    outPutEncContext->refs = 1;
    outPutEncContext->scenechange_threshold = 0;
    outPutEncContext->trellis = 0;
    AVDictionary *options = NULL;
    outPutEncContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    int ret = avcodec_open2(outPutEncContext, pH264Codec, &options);
    if (ret < 0)
    {
        printf("%s", "open codec failed");
        return  ret;
    }
    return 1;
}

int InitDecodeCodec(AVCodecID codecId)
{
    auto codec = avcodec_find_decoder(codecId);
    if(!codec)
    {
        return -1;
    }
    decoderContext = context->streams[0]->codec;
    if (!decoderContext) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    if (codec->capabilities & AV_CODEC_CAP_TRUNCATED)
        decoderContext->flags |= AV_CODEC_FLAG_TRUNCATED;
    int ret = avcodec_open2(decoderContext, codec, NULL);
    return ret;

}

bool DecodeVideo(AVPacket* packet, AVFrame* frame)
{
    int gotFrame = 0;
    auto hr = avcodec_decode_video2(decoderContext, frame, &gotFrame, packet);
    if(hr >= 0 && gotFrame != 0)
    {
        return true;
    }
    return false;
}

int InitFilter(AVCodecContext * codecContext)
{
    char args[512];
    int ret = 0;

    AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    AVFilter *buffersink = avfilter_get_by_name("buffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    string  filters_descr ="drawtext=fontfile=D\\\\:FreeSerif.ttf:fontsize=100:text=hello world:x=100:y=100";
    enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_YUV420P};

    filter_graph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* buffer video source: the decoded frames from the decoder will be inserted here. */
    snprintf(args, sizeof(args),
    "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
    codecContext->width,codecContext->height,AV_PIX_FMT_YUV420P,
    1, 25,1,1);

    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
        args, NULL, filter_graph);

    if (ret < 0) {
        cout << args << endl;
        if(buffersrc_ctx == NULL)
            av_log(NULL, AV_LOG_ERROR, "buffersrc_ctx is NULL\n");

        av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source, ret = %d\n", ret);
        goto end;
    }

    /* buffer video sink: to terminate the filter chain. */
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
        NULL, NULL, filter_graph);

    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink, %d\n", ret);
        goto end;
    }

    ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
        AV_PIX_FMT_YUV420P, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
        goto end;
    }

    /* Endpoints for the filter graph. */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;
    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr.c_str(),
        &inputs, &outputs, NULL)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        goto end;
    return ret;
end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    return ret;
}

int main()
{
    string fileInput = "Titanic.ts";
    string fileOutput = "draw.ts";
    Init();
    if(OpenInput((char *)fileInput.c_str()) < 0)
    {
        cout << "Open file Input failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }

    int ret = InitDecodeCodec(context->streams[0]->codecpar->codec_id);
    if(ret <0)
    {
        cout << "InitDecodeCodec failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }

    ret = InitEncoderCodec(decoderContext->width,decoderContext->height);
    if(ret < 0)
    {
        cout << "open eccoder failed ret is " << ret<<endl;
        cout << "InitEncoderCodec failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }

    ret = InitFilter(decoderContext);
    if(OpenOutput((char *)fileOutput.c_str()) < 0)
    {
        cout << "Open file Output failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }

    auto pSrcFrame = av_frame_alloc();
    auto  filterFrame = av_frame_alloc();
    int got_output = 0;
    int64_t  timeRecord = 0;
    int64_t  firstPacketTime = 0;

    while(true)
    {
        ret = ReadPacketFromSource();
        if(ret >= 0)
        {
            if(timeRecord == 0)
            {
                firstPacketTime = av_gettime();
                timeRecord++;
            }
            if(DecodeVideo(&packet,pSrcFrame))
            {

                if (av_buffersrc_add_frame_flags(buffersrc_ctx, pSrcFrame, AV_BUFFERSRC_FLAG_KEEP_REF) >= 0)
                {
                    if (av_buffersink_get_frame(buffersink_ctx, filterFrame) >= 0)
                    {
                        AVPacket *pTmpPkt = (AVPacket *)av_malloc(sizeof(AVPacket));
                        av_init_packet(pTmpPkt);
                        pTmpPkt->data = NULL;
                        pTmpPkt->size = 0;
                        ret = avcodec_encode_video2(outPutEncContext, pTmpPkt, filterFrame, &got_output);
                        if (ret >= 0 && got_output)
                        {
                            int ret = av_write_frame(outputContext, pTmpPkt);
                            av_packet_unref(pTmpPkt);
                        }
                    }
                }
            }
        }
        else
        {
            printf("ReadPacketFromSource failed %d\n", ret);
            break;
        }
    }
    CloseInput();
    CloseOutput();
    std::cout <<"Transcode file end!" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    return 0;
}


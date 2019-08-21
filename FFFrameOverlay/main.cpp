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

AVFormatContext * context[2];
AVFormatContext* outputContext;
AVPacket packet[2];
int64_t  lastPts = 0;
int64_t  lastDts = 0;
int64_t lastFrameRealtime = 0;

int64_t firstPts = AV_NOPTS_VALUE;
int64_t startTime = 0;

AVCodecContext*	outPutEncContext = NULL;
AVCodecContext *decoderContext[2];
#define SrcWidth 1920
#define SrcHeight 1080
#define DstWidth 640
#define DstHeight 480

struct SwsContext* pSwsContext;

AVFilterInOut *inputs;
AVFilterInOut *outputs;
AVFilterGraph * filter_graph = NULL;

AVFilterContext *inputFilterContext[2];
AVFilterContext *outputFilterContext = nullptr;
const char *filter_descr = "overlay=0:0";

int interrupt_cb(void *ctx)
{
    return 0;
}

void Init()
{
    av_register_all();
    avfilter_register_all();
    avformat_network_init();
    avdevice_register_all();
    av_log_set_level(AV_LOG_ERROR);
}


int OpenInput(char *fileName, int inputIndex)
{
    context[inputIndex] = avformat_alloc_context();
    context[inputIndex]->interrupt_callback.callback = interrupt_cb;
    AVDictionary *format_opts =  NULL;

    int ret = avformat_open_input(&context[inputIndex], fileName, NULL, &format_opts);
    if(ret < 0)
    {
        printf("avformat_open_input error %d\n", ret);
        return  ret;
    }
    ret = avformat_find_stream_info(context[inputIndex],NULL);
    av_dump_format(context[inputIndex], 0, fileName, 0);
    if(ret >= 0)
    {
        std::cout <<"open input stream successfully" << endl;
    }
    return ret;
}

//int ReadPacketFromSource(int inputIndex)
//{
//    return av_read_frame(context[inputIndex], &packet[inputIndex]);
//}

shared_ptr<AVPacket> ReadPacketFromSource(int inputIndex)
{
     std::shared_ptr<AVPacket> packet(static_cast<AVPacket*>(av_malloc(sizeof(AVPacket))), [&](AVPacket *p) { av_packet_free(&p); av_freep(&p); });
    av_init_packet(packet.get());
    int ret = av_read_frame(context[inputIndex], packet.get());
    if(ret >= 0)
    {
        return packet;
    }
    else
    {
        return NULL;
    }
}

int OpenOutput(char *fileName, int inputIndex)
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

    for(int i = 0; i < context[inputIndex]->nb_streams; i++)
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

void CloseInput(int inputIndex)
{
    if(context != NULL)
    {
        avformat_close_input(&context[inputIndex]);
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

int InitEncoderCodec( int iWidth, int iHeight, int inputIndex)
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
    outPutEncContext->time_base.num =context[inputIndex]->streams[0]->codec->time_base.num;
    outPutEncContext->time_base.den = context[inputIndex]->streams[0]->codec->time_base.den;
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

int InitDecodeCodec(AVCodecID codecId, int inputIndex)
{
	auto codec = avcodec_find_decoder(codecId);
	if(!codec)
	{
		return -1;
	}
	decoderContext[inputIndex] = context[inputIndex]->streams[0]->codec;
	if (!decoderContext) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}

	if (codec->capabilities & AV_CODEC_CAP_TRUNCATED)
		decoderContext[inputIndex]->flags |= AV_CODEC_FLAG_TRUNCATED; 
	int ret = avcodec_open2(decoderContext[inputIndex], codec, NULL);
	return ret;

}

bool DecodeVideo(AVPacket* packet, AVFrame* frame,int inputIndex)
{
    int gotFrame = 0;
    auto hr = avcodec_decode_video2(decoderContext[inputIndex], frame, &gotFrame, packet);
    if(hr >= 0 && gotFrame != 0)
    {
		frame->pts = packet->pts;
        return true;
    }
    return false;
}

int InitInputFilter(AVFilterInOut *input,const char *filterName,  int inputIndex)
{
    char args[512];
    memset(args,0, sizeof(args));
    AVFilterContext *padFilterContext = input->filter_ctx;

    auto filter = avfilter_get_by_name("buffer");
    auto codecContext = context[inputIndex]->streams[0]->codec;

    snprintf(args, sizeof(args),
    "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
    codecContext->width,codecContext->height,AV_PIX_FMT_YUV420P,
    1, 25,1,1);

    int ret = avfilter_graph_create_filter(&inputFilterContext[inputIndex],filter,filterName, args,
        NULL, filter_graph);

     if(ret < 0)  return ret;
     ret = avfilter_link(inputFilterContext[inputIndex],0,padFilterContext,input->pad_idx);
     return ret;
}

int InitOutputFilter(AVFilterInOut *output,const char *filterName)
{
    AVFilterContext *padFilterContext = output->filter_ctx;
     auto filter = avfilter_get_by_name("buffersink");


    int ret = avfilter_graph_create_filter(&outputFilterContext,filter,filterName, NULL,
        NULL, filter_graph);
     if(ret < 0)  return ret;
     ret = avfilter_link(padFilterContext,output->pad_idx,outputFilterContext,0);

     return ret;
}

void FreeInout()
{
    avfilter_inout_free(&inputs->next);
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
}

int main()
{
    string fileInput[2];
    fileInput[0] = "Titanic.ts";
    fileInput[1] = "0.jpg";
    string fileOutput = "draw.ts";
//    std::thread decodeTask;
    int numFrame=1;
    Init();
    for(int i = 0; i < 2; i++)
    {
        if(OpenInput((char *)fileInput[i].c_str(),i) < 0)
        {
            cout << "Open file Input failed!" << endl;
            this_thread::sleep_for(chrono::seconds(10));
            return 0;
        }
    }
    for(int i = 0; i < 2; i++)
    {
        int ret = InitDecodeCodec(context[i]->streams[0]->codec->codec_id,i);
        if(ret <0)
        {
            cout << "InitDecodeCodec failed!" << endl;
            this_thread::sleep_for(chrono::seconds(10));
            return 0;
        }
    }

    int ret = InitEncoderCodec(decoderContext[0]->width,decoderContext[0]->height,0);
    if(ret < 0)
    {
        cout << "open eccoder failed ret is " << ret<<endl;
        cout << "InitEncoderCodec failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }


    filter_graph = avfilter_graph_alloc();
    if(!filter_graph)
    {
        cout <<"graph alloc failed"<<endl;
        goto End;
    }

    avfilter_graph_parse2(filter_graph, filter_descr, &inputs, &outputs);
    InitInputFilter(inputs,"MainFrame",0);
    InitInputFilter(inputs->next,"OverlayFrame",1);
    InitOutputFilter(outputs,"output");
    FreeInout();

     ret = avfilter_graph_config(filter_graph, NULL);
    if(ret < 0)
    {
          goto End;
    }
    if(OpenOutput((char *)fileOutput.c_str(),0) < 0)
    {
        cout << "Open file Output failed!" << endl;
        this_thread::sleep_for(chrono::seconds(10));
        return 0;
    }

     AVFrame *pSrcFrame[2];
     AVFrame *inputFrame[2];
     pSrcFrame[0]= av_frame_alloc();
     pSrcFrame[1]= av_frame_alloc();
     inputFrame[0] = av_frame_alloc();
    inputFrame[1] = av_frame_alloc();
    auto  filterFrame = av_frame_alloc();
    int got_output = 0;
    int64_t  timeRecord = 0;
    int64_t  firstPacketTime = 0;
    int64_t outLastTime = av_gettime();
    int64_t inLastTime = av_gettime();
    int64_t videoCount = 0;

       std::thread decodeTask(thread([&]{
            bool ret = true;
            while(ret)
            {
                auto packet = ReadPacketFromSource(1);
                ret = DecodeVideo(packet.get(),pSrcFrame[1],1);
                if(ret) break;
            }
        }
        ));

    decodeTask.join();

    while(true)
    {
        outLastTime = av_gettime();
        auto packet = ReadPacketFromSource(0);
        if(packet)
        {
            if(DecodeVideo(packet.get(),pSrcFrame[0],0))
            {
                av_frame_ref( inputFrame[0],pSrcFrame[0]);
                if (av_buffersrc_add_frame_flags(inputFilterContext[0], inputFrame[0], AV_BUFFERSRC_FLAG_PUSH) >= 0)
                {
                    pSrcFrame[1]->pts = pSrcFrame[0]->pts;
                    //av_frame_ref( inputFrame[1],pSrcFrame[1]);
                    if(av_buffersrc_add_frame_flags(inputFilterContext[1],pSrcFrame[1], AV_BUFFERSRC_FLAG_PUSH) >= 0)
                    {
                        ret = av_buffersink_get_frame_flags(outputFilterContext, filterFrame,AV_BUFFERSINK_FLAG_NO_REQUEST);

                        if ( ret >= 0)
                        {
                            std::shared_ptr<AVPacket> pTmpPkt(static_cast<AVPacket*>(av_malloc(sizeof(AVPacket))), [&](AVPacket *p) { av_packet_free(&p); av_freep(&p); });
                            av_init_packet(pTmpPkt.get());
                            pTmpPkt->data = NULL;
                            pTmpPkt->size = 0;
                            ret = avcodec_encode_video2(outPutEncContext, pTmpPkt.get(), filterFrame, &got_output);
                            if (ret >= 0 && got_output)
                            {
                                cout << "frame : " << numFrame++ << endl;
                                int ret = av_write_frame(outputContext, pTmpPkt.get());
                            }
                            //this_thread::sleep_for(chrono::milliseconds(10));
                        }
                        av_frame_unref(filterFrame);
                    }
                }
            }
        }
        else break;
    }
End:

    CloseInput(0);
    CloseInput(1);
    CloseOutput();
    std::cout <<"Transcode file end!" << endl;
    this_thread::sleep_for(chrono::seconds(10));
    return 0;
}



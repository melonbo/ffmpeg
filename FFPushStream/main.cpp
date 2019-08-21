#include <QCoreApplication>
#include "pch.h" 
#include <thread>
#include <iostream>
using namespace std;

AVFormatContext *inputContext = NULL;
AVFormatContext *outputContext;
AVPacket packet;

int OpenInput(QString inputUrl)
{
    inputContext = avformat_alloc_context();
    AVDictionary *options = NULL;
    av_dict_set(&options, "rtsp_transport", "udp", 0);

    int ret = avformat_open_input(&inputContext, inputUrl.toLatin1().data(), NULL, &options);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Input file open input failed\n");
        return ret;
    }

    ret= avformat_find_stream_info(inputContext, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Find input file stream inform failed\n");
    }
    else
    {
        av_log(NULL, AV_LOG_FATAL, "Open input file  %s success\n", inputUrl.toLatin1().data());
    }
    return ret;
}

int ReadPacketFromSource()
{
    return av_read_frame(inputContext, &packet);
}

void av_packet_rescale_ts(AVPacket *pkt, AVRational src_tb, AVRational dst_tb)
{
    if(pkt->pts != AV_NOPTS_VALUE)
        pkt->pts = av_rescale_q(pkt->pts, src_tb, dst_tb);
    if(pkt->dts != AV_NOPTS_VALUE)
        pkt->dts = av_rescale_q(pkt->dts, src_tb, dst_tb);
    if(pkt->duration > 0)
        pkt->duration = av_rescale_q(pkt->duration, src_tb, dst_tb);
}

int WritePacket(AVPacket *packet)
{
    AVStream *inputStream = inputContext->streams[packet->stream_index];
    AVStream *outputStream = outputContext->streams[packet->stream_index];
    av_packet_rescale_ts(packet, inputStream->time_base, outputStream->time_base);
    return av_interleaved_write_frame(outputContext, packet);
}

int OpenOutput(QString outUrl)
{
    int ret = avformat_alloc_output_context2(&outputContext, NULL, "flv", outUrl.toLatin1().data());
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "open output context failed\n");
        goto Error;
    }
    ret = avio_open2(&outputContext->pb, outUrl.toLatin1().data(), AVIO_FLAG_READ_WRITE, NULL, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "open avio failed\n");
        goto Error;
    }
    for(int i=0; i < inputContext->nb_streams; i++)
    {
        AVStream *stream = avformat_new_stream(outputContext, inputContext->streams[i]->codec->codec);
        ret = avcodec_copy_context(stream->codec, inputContext->streams[i]->codec);
        if(ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "copy codec context failed\n");
            goto Error;
        }
    }

    ret = avformat_write_header(outputContext, NULL);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "format write header failed\n");
        goto Error;
    }

    av_log(NULL, AV_LOG_FATAL, "Open output file success %s\n", outUrl.toLatin1().data());
    return ret;

Error:
    if(outputContext)
    {
        for(int i=0; i<outputContext->nb_streams; i++)
        {
            avcodec_close(outputContext->streams[i]->codec);
        }
        avformat_close_input(&outputContext);
    }
    return ret;
}

void Init()
{
    av_register_all();
    avfilter_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_ERROR);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Init();
    int ret = OpenInput("rtsp://172.16.41.1:554/h264/ch1/main/av_stream");
    if(ret >= 0)
    {
        //ret = OpenOutput("rtmp://127.0.0.1:1935/live/stream0");
        ret = OpenOutput("udp://239.1.1.1:1234");
        if(ret < 0) goto Error;
    }

    while(true)
    {
        if(0 == ReadPacketFromSource())
        {
            if(WritePacket(&packet) >= 0)
                cout << "WritePacket Success!" << endl;
            else
                cout << "WritePacket Failed!" << endl;
        }
    }

Error:
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(100));
    }


    return a.exec();
}






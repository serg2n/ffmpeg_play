#ifndef FFREADER_H
#define FFREADER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QDebug>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavformat/avio.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libavutil/motion_vector.h>
#include <libavutil/frame.h>
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

#include "ffreader_global.h"

class FFREADERSHARED_EXPORT Ffreader:public QThread
{
   Q_OBJECT

public:
    enum
    {
        NOT_OPEN=-1,
        OPEN_SUCCESS=0,
        STREAM_NOT_OPEN,
        STREAM_INFO_FAILED,
        FRAME_READ_FAILED,
        CODEC_FAILED,
        COPY_ERROR,
        CODEC_OPEN_ERR,
        RGB_FRAME_ERR
    };
    Ffreader(QObject *parent = 0);
    ~Ffreader();
    int getOpenState(){return opened;}
signals:
    void sendOpenState(int);
    void signalFrameChanged(QPixmap);
private:
    int opened;
    AVFormatContext   *pFormatCtx ;
    int               indx, videoStream;
    AVCodecContext    *pCodecCtxOrig ;
    AVCodecContext    *pCodecCtx ;
    AVCodec           *pCodec ;
    AVFrame           *pFrame;
    AVFrame           *pFrameRGB;
    AVPacket          packet;
    int               frameFinished;
    int               numBytes;
    uint8_t           *buffer ;
    char* pUrl;
    struct SwsContext *sws_ctx ;
    int openStream(QString szStream);
    void clearPointers();
public slots:
    void initVideo(QString szVideo);
protected:
   void run();
};

#endif // FFREADER_H

#include "ffreader.h"


Ffreader::Ffreader(QObject *parent): QThread(parent)
{
    clearPointers();
 }
//----------------------------------------
Ffreader::~Ffreader()
{
}
//--------------------------------
void Ffreader::clearPointers()
{
    pFormatCtx = NULL;
    pCodecCtxOrig = NULL;
    pCodecCtx = NULL;
    pCodec = NULL;
    pFrame = NULL;
    pFrameRGB = NULL;
    buffer = NULL;
    sws_ctx = NULL;
    pUrl=NULL;
    opened=NOT_OPEN;
    indx=0;
    frameFinished=0;
}
//---------------------------------
void Ffreader::run()
{
     if(opened==OPEN_SUCCESS)
    {
        while (av_read_frame(pFormatCtx, &packet) >= 0)
        {
            // Это пакет видео потока?
               if (packet.stream_index == videoStream)
            {
                // Декодируем видео кадр
                avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
                // Мы получили видео кадр?
                       if (frameFinished)
                {
                    // Преобразуем кадр в RGB
                    sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                              pFrame->linesize, 0, pCodecCtx->height,
                              pFrameRGB->data, pFrameRGB->linesize);
                    QImage image( pFrameRGB->data[0],
                            pCodecCtx->width,
                            pCodecCtx->height,
                             pFrameRGB->linesize[0],
                            QImage::Format_RGB888);
                  emit signalFrameChanged(QPixmap::fromImage(image));
                 }
            }
            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);
        }
        // Освобождение памяти и закрытие кодеков
        av_free(buffer);
        av_frame_free(&pFrameRGB);
        av_frame_free(&pFrame);
        avcodec_close(pCodecCtx);
        avcodec_close(pCodecCtxOrig);
        avformat_close_input(&pFormatCtx);
    //    clearPointers();
    }
exec();
}
//------------------------------------------------
int Ffreader::openStream(QString szStream)
{
    int  nOpenVal=NOT_OPEN;
     pUrl= szStream.toLatin1().data();
    // Регистрируем все форматы и кодеки
    av_register_all();
    // Пробуем открыть видео файл
    if (avformat_open_input(&pFormatCtx,pUrl, NULL, NULL) != 0)
        return STREAM_NOT_OPEN;
    else
        nOpenVal=OPEN_SUCCESS;

    // Пробуем получить информацию о потоке
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return STREAM_INFO_FAILED;
    // Получаем подробную информацию о файле: продолжительность, битрейд, контейнер и прочее
    av_dump_format(pFormatCtx, 0, pUrl, 0);
    // Находим первый кард
    videoStream = -1;
    for (indx = 0;indx < (int)pFormatCtx->nb_streams;indx++)
    {
        if (pFormatCtx->streams[indx]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = indx;
            break;
        }
    }
    if (videoStream == -1)
        return FRAME_READ_FAILED; // Не нашли
    // Указатель куда будут сохраняться данные
    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
    // Находим подходящий декодер для файла
    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (pCodec == NULL)
        return CODEC_FAILED; // Декодер не найден
    // Копируем контекст
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0)
        return COPY_ERROR; // Ошибка копирования
    // Открываем кодек
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
        return CODEC_OPEN_ERR; // Не смогли открыть кодек
    // Здесь будет храниться кадр
    pFrame = av_frame_alloc();

    // Здесь храниться кадр преобразованный в RGB
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL)
        return RGB_FRAME_ERR;

    // Определяем необходимый размер буфера и выделяем память
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
                                  pCodecCtx->height);
    buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    //  Связуем кадр с вновь выделенным буфером.
    avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);
    // Инициализируем SWS context для программного преобразования полученного кадра в RGB
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width,
                             pCodecCtx->height,
                             AV_PIX_FMT_RGB24,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
                             );
    return nOpenVal;
}
//-----------------------------------
void Ffreader::initVideo(QString szVideo)
{
    opened=openStream(szVideo);
    emit sendOpenState(opened);
    moveToThread(this);             //после открытия запускаем поток
    start();
}

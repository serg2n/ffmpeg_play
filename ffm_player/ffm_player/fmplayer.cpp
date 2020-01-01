#include "fmplayer.h"
#include "ui_fmplayer.h"

FmPlayer::FmPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FmPlayer)
{
    ui->setupUi(this);
     this->setWindowFlags(Qt::FramelessWindowHint);
    framReader=new Ffreader();
    QObject::connect(
                this, SIGNAL(openVideoStream(QString)),
                framReader, SLOT(initVideo(QString)));
    QObject::connect(
                framReader, SIGNAL(sendOpenState(int)),
                this, SLOT(setOpenStatus(int)));
    QObject::connect(
                framReader, SIGNAL(signalFrameChanged(QPixmap)),
                this, SLOT(setVideoImage(QPixmap)));
   framReader->initVideo("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");

}

FmPlayer::~FmPlayer()
{
    delete ui;
}
//--------------------------------
void FmPlayer::setOpenStatus(int nStatus)
{
    qDebug()<<"nStatus"<<nStatus;
     ui->label_Pl->setText("nStatus "+QString::number(nStatus));
}
//------------------------------------------
void FmPlayer::setVideoImage(QPixmap pix)
{
    QRect rc=this->geometry();
    this->setGeometry(rc.left(),rc.top(),pix.width(),pix.height());
    ui->label_Pl->setGeometry(0,0,pix.width(),pix.height());
    ui->label_Pl->setPixmap(pix);
}

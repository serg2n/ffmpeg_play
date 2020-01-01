#ifndef FMPLAYER_H
#define FMPLAYER_H

#include <QMainWindow>

#include "ffreader.h"

namespace Ui {
class FmPlayer;
}

class FmPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit FmPlayer(QWidget *parent = nullptr);
    ~FmPlayer();
signals:
    void openVideoStream(QString szVstream);
private slots:
    void setOpenStatus(int nStatus);
    void setVideoImage(QPixmap pix);
private:
    Ui::FmPlayer *ui;
     Ffreader *framReader;
};

#endif // FMPLAYER_H

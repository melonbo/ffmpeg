#include "QPlayer.h"
#include "ui_QPlayer.h"
#include <QMessageBox>
#include <QDebug>
#include "pch.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>

int Thread_Play(void* lpParam);

QPlayer::QPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QPlayer)
{
    ui->setupUi(this);
    guiInit();
    m_rate = 0;
    m_demux = new demux(this);
//    connect(m_demux, SIGNAL(sigYUVFrame(void*)), this, SLOT(slot_SDLPresent(void*)));
    connect(m_demux, SIGNAL(sigRGBFrame(QImage)), this, SLOT(slot_RGBPresent(QImage)));
    connect(m_demux, SIGNAL(sigDuration(int)), this, SLOT(slot_slider_init(int)));
    connect(m_demux, SIGNAL(sigCurrentTime(int)), this, SLOT(slot_setCurrentTime(int)));
    m_demux->start();

    timerSpeed = new QTimer(this);
    connect(timerSpeed, SIGNAL(timeout()), this, SLOT(slot_timer_speed()));

//    Thread_Play((void*)ui->label->winId());

//    sdlInit();

}

QPlayer::~QPlayer()
{
    delete ui;
}

void QPlayer::on_btn_pause_clicked()
{
    if(m_demux->getPlayState() == 0)
    {
        SDL_Event user_event;
        user_event.type = SDL_KEYDOWN;
        user_event.key.keysym.sym = SDLK_p;
        SDL_PushEvent(&user_event);
    }
}

void QPlayer::on_btn_close_clicked()
{
    exit(0);
}

void QPlayer::on_btn_open_file_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("player"), "./");
    ui->lineEdit_url->setText(filename);
    m_demux->setFileName(filename);
}

void QPlayer::on_btn_play_clicked()
{
    if(m_demux->getPlayState() == 1)
    {
        SDL_Event user_event;
        user_event.type = SDL_KEYDOWN;
        user_event.key.keysym.sym = SDLK_p;
        SDL_PushEvent(&user_event);
    }
}


void QPlayer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(ui->label->x(), ui->label->y(), ui->label->width(), ui->label->height()); //先画成黑色

    int x = ui->label->x();
    int y = ui->label->y();

    painter.drawImage(QPoint(x,y),mImage); //画出图像
}


void QPlayer::sdlInit()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindowFrom((void*)ui->label->winId());
    /* 在window上创建一个render */
    sdlRenderer = SDL_CreateRenderer((SDL_Window*)window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    sdlTexture = SDL_CreateTexture((SDL_Renderer*)sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,640,272);
}

void QPlayer::slot_SDLPresent(void *pFrame)
{
    AVFrame *pFrameYUV = (AVFrame*)pFrame;
    SDL_UpdateTexture((SDL_Texture*)sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );
    SDL_RenderClear((SDL_Renderer*)sdlRenderer );
    SDL_RenderCopy((SDL_Renderer*)sdlRenderer, (SDL_Texture*)sdlTexture, NULL, NULL);
    SDL_RenderPresent((SDL_Renderer*)sdlRenderer );
}

void QPlayer::slot_RGBPresent(QImage img)
{
    mImage = img;
    ui->label->update();
}

int QPlayer::getPresentWidth()
{
    return ui->label->width();
}

int QPlayer::getPresentHeight()
{
    return ui->label->height();
}

void QPlayer::on_btn_forward_clicked()
{
    SDL_Event user_event;
    user_event.type = SDL_KEYDOWN;
    user_event.key.keysym.sym = SDLK_RIGHT;
    SDL_PushEvent(&user_event);

}

void QPlayer::on_btn_backward_clicked()
{
    SDL_Event user_event;
    user_event.type = SDL_KEYDOWN;
    user_event.key.keysym.sym = SDLK_LEFT;
    SDL_PushEvent(&user_event);
}

void QPlayer::guiInit()
{
    ui->label_rate->hide();
}

void QPlayer::slot_slider_init(int duration)
{
    qDebug("set duration to %d", duration);
    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(duration);
    ui->spinBox->setSingleStep(1);

    ui->hSlider->setMinimum(0);
    ui->hSlider->setMaximum(duration);
    ui->hSlider->setSingleStep(1);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->hSlider, SLOT(setValue(int)));
    connect(ui->hSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_setPlayValue(int)));
    ui->spinBox->setValue(0);
}

void QPlayer::on_hSlider_sliderPressed()
{
    if(m_demux->getPlayState() == 0)
    {
        SDL_Event user_event;
        user_event.type = SDL_KEYDOWN;
        user_event.key.keysym.sym = SDLK_p;
        SDL_PushEvent(&user_event);
    }
    sliderValuePress = ui->hSlider->value();
}



void QPlayer::on_hSlider_sliderReleased()
{
    SDL_Event user_event0;
    user_event0.type = SDL_KEYDOWN;
    user_event0.key.keysym.sym = SDLK_g;
    int val = sliderValueRelease - sliderValuePress;
    user_event0.user.data1 = val;
    SDL_PushEvent(&user_event0);

    qDebug("set play time %d, %d", sliderValuePress, sliderValueRelease);
    qDebug("play stat %d", m_demux->getPlayState());
    SDL_Delay(200);

    if(m_demux->getPlayState() == 1)
    {
        SDL_Event user_event;
        user_event.type = SDL_KEYDOWN;
        user_event.key.keysym.sym = SDLK_p;
        SDL_PushEvent(&user_event);
    }

}

void QPlayer::slot_setCurrentTime(int sec)
{
    ui->hSlider->setValue(sec);
}

void QPlayer::slot_setPlayValue(int val)
{
    ui->spinBox->setValue(val);
    sliderValueRelease = val;
}


void QPlayer::on_btn_speed_up_clicked()
{
    float rate = m_demux->getPlaySpeed();
    m_rate++;
    qDebug("playing speed x%d", pow(2,m_rate));
    m_demux->setPlaySpeedUp(pow(2,m_rate));
    if(m_rate>0)
    {
        ui->label_rate->show();
        ui->label_rate->setText(QString("%1倍速").arg(pow(2,m_rate)));
    }
    else if(m_rate<0)
    {
        ui->label_rate->show();
        ui->label_rate->setText(QString("1/%1倍速").arg(pow(2,m_rate*(-1))));
    }
    else
    {
        ui->label_rate->hide();
    }

}

void QPlayer::on_btn_speed_down_clicked()
{
    m_rate--;
    m_demux->setPlaySpeedUp(pow(2,m_rate));
    qDebug("playing speed x%d", pow(2,m_rate));
    if(m_rate>0)
    {
        ui->label_rate->show();
        ui->label_rate->setText(QString("%1倍速").arg(pow(2,m_rate)));
    }
    else if(m_rate<0)
    {
        ui->label_rate->show();
        ui->label_rate->setText(QString("1/%1倍速").arg(pow(2,m_rate*(-1))));
    }
    else
    {
        ui->label_rate->hide();
    }

}

void QPlayer::slot_timer_speed()
{
    SDL_Event user_event0;
    user_event0.type = SDL_KEYDOWN;
    user_event0.key.keysym.sym = SDLK_g;
    double val = 0.040;
    user_event0.user.data1 = &val;
    SDL_PushEvent(&user_event0);
}

void QPlayer::mousePressEvent(QMouseEvent *event)
{
//    qDebug("x=%d, y=%d", event->x(), event->y());
}

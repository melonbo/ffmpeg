#ifndef QPLAYER_H
#define QPLAYER_H

#include <QMainWindow>
#include "demux.h"
#include <QTimer>

namespace Ui {
class QPlayer;
}

class QPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QPlayer(QWidget *parent = 0);
    ~QPlayer();
    void sdlInit();
    void guiInit();


    int getPresentWidth();
    int getPresentHeight();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_btn_play_clicked();

    void on_btn_pause_clicked();

    void on_btn_close_clicked();

    void on_btn_speed_up_clicked();

    void on_btn_open_file_clicked();


    void slot_SDLPresent(void *data);
    void slot_RGBPresent(QImage img);


    void on_btn_forward_clicked();

    void on_btn_backward_clicked();

    void on_hSlider_sliderPressed();

    void on_hSlider_sliderReleased();

    void slot_slider_init(int duration);

    void slot_setCurrentTime(int sec);

    void slot_setPlayValue(int val);

    void on_btn_speed_down_clicked();

    void slot_timer_speed();

public:
    Ui::QPlayer *ui;
    void *m_pSDLWindow;
    void* sdlTexture;
    void* sdlRenderer;

private:
    void *window;
    void *render;
    void *surface;
    void *texture;
    demux *m_demux;
    QImage mImage;
    int sliderValuePress;
    int sliderValueRelease;
    QTimer *timerSpeed;
    double speed_val;
    int m_rate;
    QString filename;

};

#endif // QPLAYER_H

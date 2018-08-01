#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateTime>
#include <QTimer>
#include <QProcess>
#include <QComboBox>
#include <QDebug>
#include <QSpinBox>

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>


#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "can.h"
#include "consumer_producer.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void triggerred_on_ok_btn();
    void triggerred_on_cancle_btn();
    void triggerred_time_set_btn();
    void triggerred_time_get_btn();
    void triggerred_connect_btn();
    void triggerred_clear_btn();
    void triggerred_send_btn();
    void onTimeout();
    void uartRx();
    void uartTx(QByteArray msg);
    void refreshAdc();

    void changeBacklight(int value);

    void triggerred_pwm_btn();
    void triggerred_can0_btn();
    void triggerred_can1_btn();

    void triggerred_cp_btn();


private:
    QLabel *m_datetime;
    QComboBox *m_baudrate;
    QComboBox *m_databits;
    QComboBox *m_stopbits;
    QComboBox *m_parity;

    QTextEdit *m_recbuf;

    QPushButton *m_uart_btn;
    QPushButton *m_clear_btn;
    QPushButton *m_send_btn;
    QSerialPort *m_uart;
    QTimer *m_readTimer;

    QLineEdit *m_adc;
    QTimer *m_adcTimer;

    QSpinBox *m_backlight;
    QLineEdit *m_period;
    QLineEdit *m_cycle;
    QPushButton *m_pwmBtn;


    QTextEdit *m_canbuf0;
    QTextEdit *m_canbuf1;
    QPushButton *m_canBtn0;
    QPushButton *m_canBtn1;

    int s0_id;
    int s1_id;


    QPushButton *m_cpBtn;
};

#endif // MAINWINDOW_H

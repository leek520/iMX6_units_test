#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(480,272);
    //setWindowFlags(Qt::FramelessWindowHint);

    QTabWidget *tab = new QTabWidget;
    tab->tabBar()->setFixedSize(480,30);
    setCentralWidget(tab);

    //1.led
    QWidget *led_widget = new QWidget();
    tab->addTab(led_widget, "LED");

    QGridLayout *gbox = new QGridLayout(led_widget);

    QLabel *label = new QLabel("LED:");
    label->setFixedHeight(50);
    QComboBox *cbox = new QComboBox();
    cbox->setFixedHeight(50);
    cbox->addItems(QStringList()<<"LED1"<<"LED2");

    QPushButton *ok_btn = new QPushButton("on");
    ok_btn->setFixedHeight(50);
    QPushButton *cancel_btn = new QPushButton("off");
    cancel_btn->setFixedHeight(50);

    connect(ok_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_on_ok_btn()));
    connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_on_cancle_btn()));

    gbox->addWidget(label,0,1);
    gbox->addWidget(cbox,0,2);

    gbox->addWidget(ok_btn,1,1);
    gbox->addWidget(cancel_btn,1,2);

    //2.rtc
    QWidget *rtc_widget = new QWidget();
    tab->addTab(rtc_widget, "RTC");
    QGridLayout *gbox_rtc = new QGridLayout(rtc_widget);
    m_datetime = new QLabel();
    m_datetime->setAlignment(Qt::AlignCenter);
    m_datetime->setStyleSheet("border:1px green solid;");
    QPushButton *time_set_btn = new QPushButton("set time");
    time_set_btn->setFixedHeight(50);
    QPushButton *time_get_btn = new QPushButton("start time");
    time_get_btn->setFixedHeight(50);
    gbox_rtc->addWidget(m_datetime, 0, 0, 1, 2);
    gbox_rtc->addWidget(time_set_btn, 1, 0);
    gbox_rtc->addWidget(time_get_btn, 1, 1);
    connect(time_set_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_time_set_btn()));
    connect(time_get_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_time_get_btn()));


    //3.serial port
    QWidget *ser_widget = new QWidget();
    tab->addTab(ser_widget, "Serial");
    QGridLayout *gbox_ser_m = new QGridLayout(ser_widget);

    QGridLayout *gbox_ser = new QGridLayout();
    m_baudrate = new QComboBox;
    m_baudrate->addItems(QStringList()<<"9600"<<"115200");
    m_databits = new QComboBox;
    m_databits->addItems(QStringList()<<"5"<<"6"<<"7"<<"8");
    m_databits->setCurrentIndex(3);
    m_parity = new QComboBox;
    m_parity->addItems(QStringList()<<"None"<<"Odd"<<"Even");
    m_stopbits = new QComboBox;
    m_stopbits->addItems(QStringList()<<"1"<<"1.5"<<"2");

    m_uart_btn = new QPushButton("Connect");
    connect(m_uart_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_connect_btn()));
    m_clear_btn = new QPushButton("Clear");
    connect(m_clear_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_clear_btn()));
    m_send_btn = new QPushButton("Send");
    connect(m_send_btn, SIGNAL(clicked(bool)), this, SLOT(triggerred_send_btn()));

    m_uart_btn->setFixedHeight(50);
    m_clear_btn->setFixedHeight(50);
    m_send_btn->setFixedHeight(50);

    m_recbuf = new QTextEdit;

    QGridLayout *gbox_ser1 = new QGridLayout();
    gbox_ser1->addWidget(m_recbuf,0,0,1,2);
    gbox_ser1->addWidget(m_send_btn,1,0,1,1);
    gbox_ser1->addWidget(m_clear_btn,1,1,1,1);
    gbox_ser1->setRowStretch(0,4);
    gbox_ser1->setRowStretch(1,1);

    gbox_ser_m->addLayout(gbox_ser,0,0);
    gbox_ser_m->addLayout(gbox_ser1,0,1);

    gbox_ser->addWidget(new QLabel("Port:"),0,0);
    gbox_ser->addWidget(new QLabel("Baud rate:"),1,0);
    gbox_ser->addWidget(new QLabel("Data bits:"),2,0);
    gbox_ser->addWidget(new QLabel("Parity:"),3,0);
    gbox_ser->addWidget(new QLabel("Stop bits:"),4,0);


    gbox_ser->addWidget(new QLabel("Uart1"),0,1);
    gbox_ser->addWidget(m_baudrate,1,1);
    gbox_ser->addWidget(m_databits,2,1);
    gbox_ser->addWidget(m_parity,3,1);
    gbox_ser->addWidget(m_stopbits,4,1);
    gbox_ser->addWidget(m_uart_btn,5,0,1,2);


    //4.adc
    QWidget *adc_widget = new QWidget();
    tab->addTab(adc_widget, "ADC");
    QHBoxLayout *hbox_adc = new QHBoxLayout(adc_widget);
    m_adc = new QLineEdit;
    m_adc->setFixedWidth(100);
    m_adc->setAlignment(Qt::AlignCenter);
    hbox_adc->addWidget(m_adc);
    m_adcTimer = new QTimer(this);
    connect(m_adcTimer,SIGNAL(timeout()),this,SLOT(refreshAdc()));
    m_adcTimer->start(100);

    //5.pwm
    QWidget *pwm_widget = new QWidget();
    tab->addTab(pwm_widget, "PWM");
    QGridLayout *gbox_pwm = new QGridLayout(pwm_widget);
    m_backlight = new QSpinBox;
    m_backlight->setFixedSize(100,40);
    m_backlight->setRange(0,7);
    connect(m_backlight, SIGNAL(valueChanged(int)), SLOT(changeBacklight(int)));
    gbox_pwm->addWidget(new QLabel("backlight:"), 1, 0);
    gbox_pwm->addWidget(m_backlight,1,1,1,1,Qt::AlignLeft);

    m_period = new QLineEdit;
    m_period->setFixedSize(100,40);
    m_cycle = new QLineEdit;
    m_cycle->setFixedSize(100,40);
    m_pwmBtn = new QPushButton("Set PWM3");
    m_pwmBtn->setFixedHeight(40);
    connect(m_pwmBtn, SIGNAL(clicked(bool)), this, SLOT(triggerred_pwm_btn()));
    gbox_pwm->addWidget(new QLabel("period:"), 0, 2);
    gbox_pwm->addWidget(m_period,0,3);
    gbox_pwm->addWidget(new QLabel("ns"), 0, 4, Qt::AlignCenter);

    gbox_pwm->addWidget(new QLabel("duty_cycle:"), 1, 2);
    gbox_pwm->addWidget(m_cycle,1,3);
    gbox_pwm->addWidget(new QLabel("ns"), 1, 4, Qt::AlignCenter);

    gbox_pwm->addWidget(m_pwmBtn,2, 2, 1, 3);
    gbox_pwm->setContentsMargins(10,10,10,10);



    //5.can
    QWidget *can_widget = new QWidget();
    tab->addTab(can_widget, "CAN");
    QGridLayout *gbox_can = new QGridLayout(can_widget);

    m_canbuf0 = new QTextEdit;
    m_canbuf1 = new QTextEdit;

    m_canBtn0 = new QPushButton("CAN0 Open");
    m_canBtn0->setFixedHeight(40);
    connect(m_canBtn0, SIGNAL(clicked(bool)), this, SLOT(triggerred_can0_btn()));

    m_canBtn1 = new QPushButton("CAN1 Open");
    m_canBtn1->setFixedHeight(40);
    connect(m_canBtn1, SIGNAL(clicked(bool)), this, SLOT(triggerred_can1_btn()));

    gbox_can->addWidget(m_canbuf0,0,0);
    gbox_can->addWidget(m_canbuf1,0,1);
    gbox_can->addWidget(m_canBtn0,1,0);
    gbox_can->addWidget(m_canBtn1,1,1);
    gbox_can->setRowStretch(0,3);
    gbox_can->setRowStretch(1,1);


    //5.c_p
    QWidget *cp_widget = new QWidget();
    tab->addTab(cp_widget, "C_P");
    QGridLayout *gbox_cp = new QGridLayout(cp_widget);
    m_cpBtn = new QPushButton("cp Open");
    m_cpBtn->setFixedHeight(40);
    connect(m_cpBtn, SIGNAL(clicked(bool)), this, SLOT(triggerred_cp_btn()));
    gbox_cp->addWidget(m_cpBtn);

    gpio_export(LED);
    gpio_set_direction(LED, IN);

    onTimeout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::triggerred_on_ok_btn()
{
    gpio_set_value(LED, 1);
}

void MainWindow::triggerred_on_cancle_btn()
{
    gpio_set_value(LED, 0);
}

void MainWindow::triggerred_time_get_btn()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(10);
}

void MainWindow::triggerred_time_set_btn()
{
    // 修改系统时间很简单
    // 采用QProcess来实现
    QProcess::execute("date -s \"2018-07-12 09:00:00\""); // 0312 表示3月12日，1025表示当前时分
    QProcess::execute("hwclock"); // 同步系统时间
    QProcess::execute("sync"); // 保存配置

    onTimeout();
}

void MainWindow::onTimeout()
{
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy-MM-dd:hh:mm:ss:zzz");
    m_datetime->setText(localTime);
}


void MainWindow::triggerred_connect_btn()
{
    if (m_uart_btn->text()=="Disconnect")
    {
        qDebug()<<"Close!";
        m_uart_btn->setText("Connect");
        m_uart->close();
        m_readTimer->stop();
        return;
    }
    QSerialPort::BaudRate baudrate;
    QSerialPort::DataBits databits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;

    if (m_baudrate->currentText()=="9600")
    {
        baudrate = QSerialPort::Baud9600;
    }
    else if (m_baudrate->currentText()=="115200")
    {
        baudrate = QSerialPort::Baud115200;
    }
    else
    {
        baudrate = QSerialPort::Baud9600;
    }

    if (m_databits->currentText()=="5")
    {
        databits = QSerialPort::Data5;
    }
    else if (m_databits->currentText()=="6")
    {
        databits = QSerialPort::Data6;
    }
    else if (m_databits->currentText()=="7")
    {
        databits = QSerialPort::Data7;
    }
    else if (m_databits->currentText()=="8")
    {
        databits = QSerialPort::Data8;
    }
    else
    {
        databits = QSerialPort::Data8;
    }

    if (m_parity->currentText()=="None")
    {
        parity = QSerialPort::NoParity;
    }
    else if (m_parity->currentText()=="Odd")
    {
        parity = QSerialPort::OddParity;
    }
    else
    {
        parity = QSerialPort::EvenParity;
    }

    if (m_stopbits->currentText()=="1")
    {
        stopbits = QSerialPort::OneStop;
    }
    else if (m_stopbits->currentText()=="1.5")
    {
        stopbits = QSerialPort::OneAndHalfStop;
    }
    else
    {
        stopbits = QSerialPort::TwoStop;
    }

    m_uart = new QSerialPort();\
    /* 这里设置波特流等属性一定要在打开串口之后在进行，要不然设置不成功*/
    m_uart->setPortName("/dev/ttymxc1");
    if (!m_uart->open(QIODevice::ReadWrite))
    {
        qDebug()<<"Open /dev/ttymxc1 failed!";
        return;
    }
    m_uart->setBaudRate(baudrate);
    m_uart->setBaudRate(databits);
    m_uart->setParity(parity);
    m_uart->setStopBits(stopbits);
    m_uart->setFlowControl(QSerialPort::NoFlowControl);
    qDebug()<<baudrate<<databits<<parity<<stopbits;

    m_uart_btn->setText("Disconnect");
    m_readTimer = new QTimer(this);
    m_readTimer->start(100);
    connect(m_readTimer,SIGNAL(timeout()),this,SLOT(uartRx()));
}

void MainWindow::triggerred_clear_btn()
{
    m_recbuf->clear();
}

void MainWindow::triggerred_send_btn()
{
    QString msg = m_recbuf->toPlainText();
    QByteArray byte;
    byte = msg.toLatin1();
    if (!byte.isEmpty())
        uartTx(byte);
}
void MainWindow::uartRx()
{
    QByteArray temp = m_uart->readAll(); //读取串口缓冲区的所有数据给临时变量temp
    m_recbuf->insertPlainText(temp); //将串口的数据显示在窗口的文本浏览器中
    m_recbuf->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

    if (!temp.isEmpty())
        uartTx(temp);
}

void MainWindow::uartTx(QByteArray msg)
{
    m_uart->write(msg); //以ASCII码形式将数据写入串口
}

void MainWindow::refreshAdc()
{
    float voltage = 0.0;
    int value = adc_get_value(ADC_PORT);
    if (value > -1) {
        voltage = SCALE * value / 1000;
    }
    m_adc->setText(QString("%1 V").arg(QString::number(voltage, 'f', 4)));
}

void MainWindow::changeBacklight(int value)
{
    backlight_set_value(value);
}

void MainWindow::triggerred_pwm_btn()
{
    int period = m_period->text().toInt();
    int duty_cycle = m_cycle->text().toInt();

    pwm_export(PWM);
    pwm_enable(PWM);
    pwm_config(PWM,period,duty_cycle);

}

void MainWindow::triggerred_can0_btn()
{

    if (m_canBtn0->text() == "CAN0 Open")
    {
        s0_id = can_init(0, 250000);
        if (s0_id == -1) {
            printf("can0_init error!");
            return;
        }
        m_canBtn0->setText("CAN0 Send");


    }else{
        //开启两个线程，一个接收，一个发送
        extern pthread_mutex_t g_can0_mutex;
        extern int g_can0_fd;
        g_can0_fd = s0_id;
        pthread_mutex_init(&g_can0_mutex,NULL);
        create_can0_task();
        pthread_mutex_destroy(&g_can0_mutex);
    }
}
void MainWindow::triggerred_can1_btn()
{

    if (m_canBtn1->text() == "CAN1 Open")
    {
        s1_id = can_init(1, 250000);

        if (s1_id == -1) {
            printf("can1_init error!");
            return;
        }
        m_canBtn1->setText("CAN1 Receive");

    }else{
        //开启两个线程，一个接收，一个发送
        extern pthread_mutex_t g_can1_mutex;
        extern int g_can1_fd;
        g_can1_fd = s1_id;
        pthread_mutex_init(&g_can1_mutex,NULL);
        create_can1_task();
        pthread_mutex_destroy(&g_can1_mutex);

    }
}

void MainWindow::triggerred_cp_btn()
{
    c_p_test();
}

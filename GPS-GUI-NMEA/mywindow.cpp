#include "mywindow.h"
#include "ui_mywindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTextBrowser>
#include <QQuickItem>
#include <QPixmap>
#include <QtCharts>


MyWindow::MyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    ui->mapWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->mapWidget->show();

    ui->statusLabel->setStyleSheet("QLabel {background-color : blue;}");

    ui->formatComboBox->addItem("DMS");
    ui->formatComboBox->addItem("DD");
    ui->formatComboBox->addItem("DMM");
    ui->displayComboBox->addItem("Map");
    ui->displayComboBox->addItem("Constellation");

    QPolarChart *constellation = new QPolarChart();
    QLineSeries *series = new QLineSeries();
    QChartView *chartView = new QChartView(constellation);
    chartView->chart()->addSeries(series);
    chartView->chart()->createDefaultAxes();
    chartView->setParent(ui->constellationFrame);
    chartView->setFixedSize(ui->constellationFrame->size());

    auto mapObj = ui->mapWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant,QVariant)), mapObj, SLOT(setCenter(QVariant,QVariant)));

    QPixmap satPix("C:\\Users\\camer\\Desktop\\uBlox\\GPS-GUI-NMEA\\icons\\satellite-Icon.png");
    ui->satImage->setPixmap(satPix.scaled(50,50,Qt::KeepAspectRatio));
    QPixmap qtPix("C:\\Users\\camer\\Desktop\\uBlox\\GPS-GUI-NMEA\\icons\\qt-Icon.png");
    ui->qtImage->setPixmap(qtPix.scaled(50,50,Qt::KeepAspectRatio));

    ui->latDegrees->display("---");
    ui->latMinutes->display("--");
    ui->latSeconds->display("---");
    ui->lonDegrees->display("---");
    ui->lonMinutes->display("--");
    ui->lonSeconds->display("---");
    ui->utcNumber->display("--:--:--");
}

void MyWindow::updateLat(const QString latReading, const QString nsReading)
{
    latQuery = latReading.split(".");
    QString latSeconds_temp = latQuery[1];
    double latSeconds = latSeconds_temp.toDouble() / 1000 * 60;
    QString latMinutes = latQuery[0].right(2);
    QString latDegrees = latQuery[0];
    latDegrees.chop(2);
    //qDebug() << "Degrees: " << latDegrees << "   Minutes: " << latMinutes << "   Seconds:" << latSeconds;
    ui->latDegrees->display(latDegrees);
    ui->latDegrees_2->display(latDegrees);
    ui->latMinutes->display(latMinutes);
    ui->latSeconds->display(latSeconds);
    ui->nsLabel->setText(nsReading);
    ui->nsLabel_2->setText(nsReading);

    double latitudeDMM = latMinutes.toDouble() + latSeconds/60;
    ui->latMinutes_2->display(latitudeDMM);

    latitudeDD = latDegrees.toDouble() + latMinutes.toDouble()/60 + latSeconds/3600;
    if (nsReading == 'S')\
    {
        latitudeDD = -latitudeDD;
    }

    ui->ddLatNumber->display(latitudeDD);
}

void MyWindow::updateLon(const QString lonReading, const QString ewReading)
{

    lonQuery = lonReading.split(".");
    QString lonSeconds_temp = lonQuery[1];
    double lonSeconds = lonSeconds_temp.toDouble() / 1000 * 60;
    QString lonMinutes = lonQuery[0].right(2);
    QString lonDegrees = lonQuery[0];
    lonDegrees.chop(2);
    //qDebug() << "Degrees: " << lonDegrees << "   Minutes: " << lonMinutes << "   Seconds:" << lonSeconds;
    ui->lonDegrees->display(lonDegrees);
    ui->lonDegrees_2->display(lonDegrees);
    ui->lonMinutes->display(lonMinutes);
    ui->lonSeconds->display(lonSeconds);
    ui->ewLabel->setText(ewReading);
    ui->ewLabel_2->setText(ewReading);

    double longitudeDMM = lonMinutes.toDouble() + lonSeconds/60;
    ui->lonMinutes_2->display(longitudeDMM);

    longitudeDD = lonDegrees.toDouble() + lonMinutes.toDouble()/60 + lonSeconds/3600;
    if (ewReading == 'W')\
    {
        longitudeDD = -longitudeDD;
    }

    ui->ddLonNumber->display(longitudeDD);
}

void MyWindow::updateUtc(const QString utcReading)
{
     QString utchh = utcReading.left(2);
     QString utcmm = utcReading.left(4);
     utcmm.remove(0, 2);
     QString utcss = utcReading;
     utcss.remove(0, 4);
     //qDebug() << "hh: " << utchh << "   mm: " << utcmm << "   ss:" << utcss;
     QString utcTime = utchh + ":" + utcmm + ":" + utcss;
     ui->utcNumber->display(utcTime);
}

void MyWindow::on_pushButtonStart_clicked()
{
    if (initial_connect == true)
    {
        initial_connect = false;

        arduino = new QSerialPort(this);

       /*
        *  Testing code, prints the description, vendor id, and product id of all ports.
        *  Used it to determine the values for the arduino uno.
        *
        *
        *
       qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
       {
           qDebug() << "Description: " << serialPortInfo.description() << "\n";
           qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
           qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
           qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
           qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
       }
       */



        //Identify the port the arduino uno is on.

        bool arduino_is_available = false;
        QString arduino_uno_port_name;

        //  For each available serial port
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            //  check if the serialport has both a product identifier and a vendor identifier
            if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
                //  check if the product ID and the vendor ID match those of the arduino uno
                if((serialPortInfo.productIdentifier() == arduino_uno_pid)
                       && (serialPortInfo.vendorIdentifier() == arduino_uno_vid)){
                   arduino_is_available = true; //    arduino uno is available on this port
                   arduino_uno_port_name = serialPortInfo.portName();
               }
           }
       }


        //  Open and configure the arduino port if available

        if(arduino_is_available)
        {
            qDebug() << "Found the arduino port...\n";
            arduino->setPortName(arduino_uno_port_name);
            arduino->open(QSerialPort::ReadOnly);
            arduino->setBaudRate(QSerialPort::Baud115200);
            arduino->setDataBits(QSerialPort::Data8);
            arduino->setFlowControl(QSerialPort::NoFlowControl);
            arduino->setParity(QSerialPort::NoParity);
            arduino->setStopBits(QSerialPort::OneStop);

            ui->connectLabel->setText("<font color='green'>Connected</font>");
            ui->statusLabel->setStyleSheet("QLabel {background-color : green;}");
            QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));

        } else
        {
            initial_connect = true;
            qDebug() << "Couldn't find the correct port for the arduino.\n";
            QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
        }
    } else
    {
        ui->connectLabel->setText("<font color='green'>Connected</font>");
        ui->statusLabel->setStyleSheet("QLabel {background-color : green;}");
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }
}

MyWindow::~MyWindow()
{
    if(arduino->isOpen())
    {
        arduino->close();  //closes the port if open
    }
    delete ui;
}

void MyWindow::readSerial()
{
    //qDebug() << "serial port working..\n";
    serialData = arduino->readLine();
    QString temp = QString::fromStdString(serialData.toStdString());
    //qDebug() << temp;

    query = temp.split(",");
    //qDebug() << query;

    if (query[0] == "$GPGLL")
    {
        if (ui->gllCheckBox->isChecked()){
            add_to_textbox(temp);
        }
        MyWindow::updateLat(query[1], query[2]);
        MyWindow::updateLon(query[3], query[4]);
        MyWindow::updateUtc(query[5]);
    } else if (query[0] == "$GPGSV")
    {
        if (ui->gsvCheckBox->isChecked()){
            add_to_textbox(temp);
        }
        //do something, tbd
    } else if (query[0] == "$GPGGA")
    {
        if (ui->ggaCheckBox->isChecked()){
            add_to_textbox(temp);
        }
        //do something, tbd
    } else
    {
        if(!(ui->gllCheckBox->isChecked() || ui->ggaCheckBox->isChecked() || ui->gsvCheckBox->isChecked()))
        {
            add_to_textbox(temp);
        }
    }
}

void MyWindow::add_to_textbox(const QString message)
{
    ui->textBrowser->append(message);
}

void MyWindow::on_pushButtonStop_clicked()
{
    ui->connectLabel->setText("<font color='red'>Disconnected</font>");
    ui->statusLabel->setStyleSheet("QLabel {background-color : red;}");
    if (initial_connect == false)
    {
        QObject::disconnect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }
}

void MyWindow::on_pushButton_clicked()
{
    qApp->quit();
}

void MyWindow::on_setLocationButton_clicked()
{
    emit(setCenter(latitudeDD,longitudeDD));
    //qDebug() << latitudeDD << "  " << longitudeDD;
}

void MyWindow::on_formatComboBox_currentIndexChanged(int index)
{
    ui->lowerStackedWidget->setCurrentIndex(index);
}

void MyWindow::on_displayComboBox_currentIndexChanged(int index)
{
        ui->upperStackedWidget->setCurrentIndex(index);
}


#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMainWindow>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWindow; }
QT_END_NAMESPACE

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);
    ~MyWindow();

private slots:
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void readSerial();
    void updateLat(const QString, const QString);
    void updateLon(const QString, const QString);
    void updateUtc(const QString);
    void on_pushButton_clicked();
    void add_to_textbox(const QString);
    void on_setLocationButton_clicked();
    void on_formatComboBox_currentIndexChanged(int index);
    void on_displayComboBox_currentIndexChanged(int index);

private:
    Ui::MyWindow *ui;

    QSerialPort *arduino;
    QByteArray serialData;
    QStringList query;
    QStringList latQuery;
    QStringList lonQuery;
    QStringList utcQuery;
    double latitudeDD = 35.2688;
    double longitudeDD = -81.5352;
    bool initial_connect = true;

    static const quint16 arduino_uno_vid = 9025;
    static const quint16 arduino_uno_pid = 67;

signals:
    void setCenter(QVariant, QVariant);
};
#endif // MYWINDOW_H

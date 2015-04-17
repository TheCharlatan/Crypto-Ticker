#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QtGui>
#include <QThread>
#include <string>
#include <thread>
#include "data_parser.h"
#include "curl_wrapper_class.h"
#include "curl_worker_class.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void clear_alarm();
    void alarm();
    void curl_request(int Index);
    void plot_memory_stepping();
    QVector<double>  give_data();
    ~MainWindow();

signals:
   void  finished();
   void finished_okcoin(std::string okcoin_string);
   void finished_btcchina(std::string btcchina_string);
   void finished_bitfinex(std::string bitfinex_string);
   void finished_bitstamp(std::string bitstamp_string);

private slots:
    void plotter();
    void curl_timeout();
    void set_plot_data();
    void set_ui_details();
    void set_up_input();
    void set_down_input();
    void set_path();
    void set_price_range();
    void set_time_scale();
    void set_basic_information();
    void set_okcoin_data(QString okcoin_data);
    void set_btcchina_data(QString btcchina_data);
    void set_bitfinex_data(QString bitfinex_data);
    void set_bitstamp_data(QString bitstamp_data);

private:
    Ui::MainWindow *ui;
    QString label_text, up_bound, down_bound, alarm_path, ranges;
    float upper_bound, lower_bound;
    curl_worker* worker;
    QTimer * curl_timer;
    QThread* curl_thread;
    CppCurl okcoin_curler, btcchina_curler, bitstamp_curler, bitfinex_curler; //initializes the different curl object; more efficient than constant cleanup and re-setting of data
    std::string okcoin_string, bitfinex_string, btcchina_string, bitstamp_string; //passed to the curl object and returned, with the conentent of the tickers
    parsed_data current, okcoin_parsing, btcchina_parsing, bitstamp_parsing, bitfinex_parsing; //shows the current data
   QVector<double> history = QVector<double> (1001); //QVector initialized with C++11 copy-only member initialisers. The Vector holds the last price of the past 100 seconds.
   QVector<double>time = QVector<double>(1001); //This Vector holds the time, as in 100 seconds.
    int position; //position for the data, that is being appended to the plot
    double plot_time, plot_price; //hold the range of the plot time and plot  price
};

#endif // MAINWINDOW_H

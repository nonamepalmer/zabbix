#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <json.h>
#include <QDateTime>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString body = "{\"jsonrpc\": \"2.0\",\"method\": \"user.authenticate\",\"params\": {\"user\": \"Admin\",\"password\": \"zabbix\"";
    body +=   " },";
    body +=    "\"auth\": null,";
    body +=    "\"id\": 1";
    body +=   "}";

//    header('Cache-Control: no-cache, must-revalidate');
//    header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
//    header('Content-type: application/json');

    QByteArray dataToSend = body.toAscii();
    QVariantMap map;
    QVariantMap parameter;
//    parameter["user"]="Admin";
//    parameter["password"]="zabbix";

    parameter["user"]="Api";
    parameter["password"]="ApiAc3ss";


    map["jsonrpc"] = "2.0";
    map["method"] = "user.login";
    map["params"]=parameter;
    //map["params"]="{\"output\":\"extend\",\"sortfield\":\"name\"}";
    map["id"] = "1";

    using namespace QtJson;
    dataToSend = Json::serialize(map);


    networkAccessManager = new QNetworkAccessManager(this);

//    request.setUrl(QUrl("http://192.168.40.135/zabbix/api_jsonrpc.php"));
    request.setUrl(QUrl("http://swi19zabbixs/zabbix/api_jsonrpc.php"));
    request.setRawHeader("Content-Type","application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,dataToSend.size());
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(sendReportToServerReply(QNetworkReply*)));
    connect(networkAccessManager, SIGNAL(),this, SLOT(sendReportToServerReply(QNetworkReply*)));
    QNetworkReply *reply = networkAccessManager->post(request,dataToSend);




}

void MainWindow::sendReportToServerReply(QNetworkReply *reply){

     QVariant statusCodeV =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

      QVariant redirectionTargetUrl =reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    QByteArray result =reply->readAll();

    QString error = reply->errorString();

    if (reply->error ()==QNetworkReply::NoError){
        QString error ="fdaf";

    }
    QString zabbix (result);





    ui->plainTextEdit->appendPlainText(zabbix);

    //Say that we're using the QtJson namespace
    using namespace QtJson;
    bool ok;
    //json is a QString containing the JSON data
    QVariantMap result2 = Json::parse(zabbix, ok).toMap();

    if(!ok) {
        qFatal("An error occurred during parsing");
        exit(1);
    }

    ID= result2["result"].toString();

    foreach (QVariant plug, result2["result"].toList()) {
             qDebug() << "\t-" << plug.toString();
    }

    ID+="";

}



void MainWindow::on_pushButton_2_clicked()
{
    QVariantMap map;
    QVariantMap  parameter;

    parameter["output"]="extend";
    parameter["sortfield"]="name";

    map["jsonrpc"] = "2.0";
    map["method"] = "hostgroup.get";
    map["params"]=parameter;
    //map["params"]="{\"output\":\"extend\",\"sortfield\":\"name\"}";
    map["id"] = "1";
    map["auth"]= ID;


    QString body ="{\"jsonrpc\":\"2.0\",\"method\":\"hostgroup.get\",\"params\":{\"output\":\"extend\",\"sortfield\":\"name\"},\"id\":1,\"auth\":\"";
    body+=ID;
    body+="\"}";
    using namespace QtJson;
    QByteArray dataToSend = Json::serialize(map);
    //QByteArray dataToSend = body.toAscii();
    QString test (dataToSend);
    ui->plainTextEdit->appendPlainText(test);
    //QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);
    //QNetworkRequest request(QUrl("http://192.168.40.135/zabbix/api_jsonrpc.php"));
    //request.setRawHeader("Content-Type","application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,dataToSend.size());
    //connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(sendReportToServerReply(QNetworkReply*)));
    //connect(networkAccessManager, SIGNAL(),this, SLOT(sendReportToServerReply(QNetworkReply*)));
    QNetworkReply *reply = networkAccessManager->post(request,dataToSend);

}

void MainWindow::on_pushButton_Trigger_clicked()
{
    using namespace QtJson;
    QVariantMap map;
    QVariantMap  parameter;
    QVariantMap filter;

    QString host="[swi19zabbixs]";


    //filter["description"]="[]";
    filter ["host"]=host;


    parameter["output"]="extend";
    parameter["filter"]="\"host\":[\"swi19zabbixs\"],\"description\":[\"Info:\"]";

    map["id"] = "1";
    map["auth"]= ID;

    map["jsonrpc"] = "2.0";
    map["method"] = "trigger.get";
    map["params"]=parameter;






    QByteArray dataToSend = Json::serialize(map);

    QString test (dataToSend);
    ui->plainTextEdit->appendPlainText(test);

    request.setHeader(QNetworkRequest::ContentLengthHeader,dataToSend.size());

    QNetworkReply *reply = networkAccessManager->post(request,dataToSend);

}

void MainWindow::on_pushButton_History_clicked()
{
    using namespace QtJson;
    QVariantMap map;
    QVariantMap  parameter;




    int unixTime = 1234567890;
    QDateTime timestamp;
    timestamp.setTime_t(unixTime);
    qDebug() << timestamp.toString(Qt::SystemLocaleShortDate);


    QDateTime start = QDateTime::fromString (QString("2012-01-18T14:15:09"), Qt::ISODate);
    QDateTime end = QDateTime::fromString (QString("2012-02-28T14:15:09"), Qt::ISODate);
    QString startstring = QString::number(start.toTime_t());
    QString endstring = QString::number(end.toTime_t());


    parameter["history"]=0;
    parameter["output"]="extend";
    //parameter["itemids"]= "100100000018467";
    parameter["hostids"]="[\"nemesis3\"]";
    parameter["time_from"]= startstring;
    parameter["time_till"]= endstring;

    map["id"] = "1";
    map["auth"]= ID;

    map["jsonrpc"] = "2.0";
    map["method"] = "history.get";
    map["params"]=parameter;






    QByteArray dataToSend = Json::serialize(map);

    QString test (dataToSend);
    ui->plainTextEdit->appendPlainText(test);

    request.setHeader(QNetworkRequest::ContentLengthHeader,dataToSend.size());

    QNetworkReply *reply = networkAccessManager->post(request,dataToSend);

}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();
    void sendReportToServerReply(QNetworkReply*);

    void on_pushButton_2_clicked();

    void on_pushButton_Trigger_clicked();

    void on_pushButton_History_clicked();

private:
    Ui::MainWindow *ui;
    QString ID;
    QNetworkAccessManager *networkAccessManager;
    QNetworkRequest request;
};

#endif // MAINWINDOW_H

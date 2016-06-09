#ifndef DESKTOP_H
#define DESKTOP_H

#include <QWidget>
#include<QFileDialog>
#include<QDesktopServices>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QMessageBox>
#include<QDragEnterEvent>
#include<QMimeData>
namespace Ui {
class desktop;
}

class desktop : public QWidget
{
    Q_OBJECT

public:
    explicit desktop(QWidget *parent = 0);
    void setname(QString&);
    ~desktop();

private slots:
    void on_pushButton_clicked();

    void on_icon_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_youjian_clicked();

private:
    Ui::desktop *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // DESKTOP_H

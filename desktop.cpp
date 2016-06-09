#include "desktop.h"
#include "ui_desktop.h"

QString home=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
QString  youjian=home+"/.local/share/nautilus/scripts/创建桌面图标";
bool  kaiqi=false;
desktop::desktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::desktop)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/desk.png"));
    setAcceptDrops(true);
    ui->nameedit->setAcceptDrops(false);
    ui->commentedit->setAcceptDrops(false);
    ui->exeedit->setAcceptDrops(false);
    setLayout(ui->verticalLayout);
    QString cates="Network|AudioVideo|Graphics|Office|Development|System|Utility|";
    QStringList data=cates.split("|");
    for(int i=0;i<data.length();i++)
    {
        ui->categ->setItemData(i,QVariant(data.at(i)),Qt::UserRole);
    }
    QFile file(youjian);
    if(file.exists()){
        ui->youjian->setChecked(true);
        kaiqi=true;
        ui->youjian->setText("取消右键菜单");
    }//默认显示开启右键菜单
}

desktop::~desktop()
{
    delete ui;
}

void desktop::on_pushButton_clicked()
{
    QString name=QFileDialog::getOpenFileName(this,"选择程序",home,"All (*)");
    ui->exeedit->setText(name);
    ui->exeedit->setToolTip(name);
}

void desktop::on_icon_clicked()
{
    QString name=QFileDialog::getOpenFileName(this,"选择图标",home,"Images (*.png *.bmp *.jpg);;ALL (*)");
    ui->icon->setIcon(QIcon(name));
    ui->icon->setToolTip(name);
}

void desktop::on_buttonBox_accepted()
{
    QString type="Application";
    QString target_type="Exec=";
    QString desk=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString name=ui->nameedit->text();
    QString comment=ui->commentedit->text();
    QString exe=ui->exeedit->text();
    QString icon=ui->icon->toolTip();
    QString cat=ui->categ->currentData(Qt::UserRole).toString();
    if(name.isEmpty()||exe.isEmpty()){
        QMessageBox::warning(0,"无法创建","“名称”和“命令“不能为空！",QMessageBox::Yes);
        return;
    }
    if(exe.indexOf(QRegularExpression("^http[s]{0,1}://"))!=-1) {
        type="Link";
        target_type="URL=";
    }
    desk+="/"+name+".desktop";
    QFile desktop(desk);
    if(desktop.open(QIODevice::WriteOnly))
    {
        desktop.setPermissions(QFileDevice::ExeUser|QFileDevice::ReadUser|QFileDevice::WriteUser|QFileDevice::ReadGroup|QFileDevice::ReadOther);
        QTextStream out(&desktop);
        out<<"[Desktop Entry]"<<endl;
        out<<"Type="<<type<<endl;
        out<<"Name="<<name<<endl;
        out<<"Comment="<<comment<<endl;
        out<<target_type<<exe<<endl;
        out<<"Icon="<<icon<<endl;
        out<<"Categories="<<cat;
        out.flush();
        desktop.close();
        this->close();
    }
    else {
        QMessageBox::warning(0,"创建失败","快捷方式创建失败！",QMessageBox::Yes);
    }
}

void desktop::on_buttonBox_rejected()
{
    int result=QMessageBox::question(0,"放弃创建","是否放弃创建快捷方式？",QMessageBox::No,QMessageBox::Yes);
    if(result==QMessageBox::Yes) close();
}
void desktop::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"dragenter!";
        event->acceptProposedAction();
}

void desktop::dropEvent(QDropEvent *event)
{

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    qDebug()<<fileName;
    ui->exeedit->setText(fileName);
}

void desktop::setname(QString &n){
    ui->exeedit->setText(n);
}

void desktop::on_youjian_clicked()
{
    kaiqi=!kaiqi;
    if(kaiqi)
    {
        QFile file(youjian);
        if(file.open(QIODevice::WriteOnly))
        {
            file.setPermissions(QFileDevice::ExeUser|QFileDevice::ReadUser|QFileDevice::WriteUser|QFileDevice::ReadGroup|QFileDevice::ReadOther);
            QTextStream out(&file);
            QString cmd=QApplication::applicationFilePath()+" $NAUTILUS_SCRIPT_SELECTED_FILE_PATHS";
            out<< cmd;
            out.flush();
            file.close();
//            cmd="chmod 777 "+youjian;
//            system(cmd.toStdString().c_str());
            ui->youjian->setText("取消右键菜单");
        }
        else{
            QMessageBox::warning(0,"开启失败","无法开启右键菜单功能",QMessageBox::Yes);
            ui->youjian->setChecked(false);
        }
    }
    else{
        QString cmd="rm \""+youjian+"\"";
        system(cmd.toStdString().c_str());
        ui->youjian->setText("开启右键菜单");
    }
}

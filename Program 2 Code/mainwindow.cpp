#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>
#include <QFileDialog>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hf=new huffman();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_This_triggered()
{
    QMessageBox qb;
    qb.setWindowTitle(QString("About This Project"));
    qb.setText(QString("Author: Shenxiao Mei\nProgram 2. Huffman Exercise.\nThank you for your time reviewing this program!"));
    qb.exec();
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
           this,
           tr("Open Document"),
           QDir::currentPath(),
           tr("Text files (*.txt *.rtf *.h *.cpp)") );
    std::string newFile = filename.toStdString();

    struct stat filestatus;
    stat( newFile.c_str(), &filestatus );
    long prev = filestatus.st_size;
    if(newFile.size()>0){
        hf->load(newFile);
        int prev = hf->info.length();
        hf->encode(newFile);
        stat( newFile.c_str(), &filestatus );
        long now = filestatus.st_size;
        ui->label->setText(QString::fromStdString("Compressed File View, Size: "+std::to_string(prev)+" -> "+std::to_string(now)));
        ui->textEdit->setText(QString::fromStdString(hf->info));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
           this,
           tr("Open Document"),
           QDir::currentPath(),
           tr("Text files (*.txt *.rtf *.h *.cpp)") );
    std::string newFile = filename.toStdString();
    struct stat filestatus;
    stat( newFile.c_str(), &filestatus );
    long prev = filestatus.st_size;
    if(newFile.size()>0){
        hf->decode(newFile);
        stat( newFile.c_str(), &filestatus );
        long now = filestatus.st_size;
        ui->label->setText(QString::fromStdString("Decompressed File View, Size: "+std::to_string(prev)+" -> "+std::to_string(now)));
        ui->textEdit->setText(QString::fromStdString(hf->info));
    }
}

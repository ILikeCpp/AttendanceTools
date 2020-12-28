#include "widget.h"
#include "ui_widget.h"
#include "ImportWorker.h"
#include "ExportWorker.h"
#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initThread();
}

Widget::~Widget()
{
    m_importThread.quit();
    m_importThread.wait();

    m_exportThread.quit();
    m_exportThread.wait();

    delete ui;
}

void Widget::on_pushButton_import_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"),"./","Excel (*.xlsx)");
    if (fileName.isEmpty())
        return;

    qDebug() << "fileName = " << fileName;

    emit signalImport(fileName);

    ui->progressBar->setValue(0);
}

void Widget::on_pushButton_export_clicked()
{
    ui->progressBar->setValue(0);

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"),"./","Excel (*.xlsx)");

    if (fileName.isEmpty())
        return;

    qDebug() << "fileName = " << fileName;

    emit signalExportFileName(fileName);
    emit signalExportData();
}

void Widget::slotImportProgress(int progress, int total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(progress);
}

void Widget::slotExportProgress(int progress, int total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(progress);
}

void Widget::initThread()
{
    ImportWorker *importWorker = new ImportWorker();
    importWorker->moveToThread(&m_importThread);
    connect((&m_importThread), &QThread::started, importWorker, &ImportWorker::slotStart);
    connect((&m_importThread), &QThread::finished, importWorker, &ImportWorker::deleteLater);
    connect(this, &Widget::signalImport, importWorker, &ImportWorker::slotImport);
    connect(importWorker, &ImportWorker::signalImportProgress, this, &Widget::slotImportProgress);
    connect(this, &Widget::signalExportData, importWorker, &ImportWorker::slotExportData);

    ExportWorker *exportWorker = new ExportWorker();
    exportWorker->moveToThread(&m_exportThread);
    connect((&m_exportThread), &QThread::started, exportWorker, &ExportWorker::slotStart);
    connect((&m_exportThread), &QThread::finished, exportWorker, &ExportWorker::deleteLater);
    connect(this, &Widget::signalExportFileName, exportWorker, &ExportWorker::slotExportFileName);
    connect(importWorker, &ImportWorker::signalExportData, exportWorker, &ExportWorker::slotExportData);
    connect(exportWorker, &ExportWorker::signalExportProgress, this, &Widget::slotExportProgress);

    m_importThread.start();
    m_exportThread.start();
}

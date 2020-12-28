#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

namespace Ui {
class Widget;
}

class ImportWorker;
class ExportWorker;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_import_clicked();

    void on_pushButton_export_clicked();

public slots:
    void slotImportProgress(int progress, int total);
    void slotExportProgress(int progress, int total);

signals:
    void signalImport(const QString fileName);
    void signalExportFileName(const QString fileName);
    void signalExportData();

private:
    void initThread();

private:
    Ui::Widget *ui;
    QString m_import;
    QThread m_importThread;
    QThread m_exportThread;
};

#endif // WIDGET_H

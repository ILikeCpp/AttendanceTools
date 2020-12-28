#ifndef EXPORTWORKER_H
#define EXPORTWORKER_H

#include <QObject>
#include <QMultiMap>

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"

class ExportWorker : public QObject
{
    Q_OBJECT
public:
    explicit ExportWorker(QObject *parent = nullptr);
    ~ExportWorker();

signals:
    void signalExportProgress(int progress, int total);

public slots:
    void slotStart();
    void slotExportFileName(const QString fileName);
    void slotExportData(QMultiMap<QString, QString> workerMap,
                        QMultiMap<QString, QString> workerInfoMap);


private:
    void initXlsxTemplate(QStringList sheetNames, QMultiMap<QString, QString> &workerMap);
    void writeData2Xlsx(QXlsx::Document &xlsx,
                        QStringList workers,
                        QMultiMap<QString, QString> &workerInfoMap);

    QString m_fileName;
};

#endif // EXPORTWORKER_H

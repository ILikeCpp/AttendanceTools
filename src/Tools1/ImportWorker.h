#ifndef IMPORTWORKER_H
#define IMPORTWORKER_H

#include <QObject>
#include <QMultiMap>
#include <QJsonObject>

class ImportWorker : public QObject
{
    Q_OBJECT
public:
    explicit ImportWorker(QObject *parent = nullptr);
    ~ImportWorker();

signals:
    void signalImportProgress(int progress, int total);
    void signalExportData(QMultiMap<QString, QString> workerMap,
                        QMultiMap<QString, QString> workerInfoMap);

public slots:
    void slotStart();
    void slotImport(const QString fileName);
    void slotExportData();

private:
    QMultiMap<QString, QString> m_workerMap;
    QMultiMap<QString, QString> m_workerInfoMap;
};

#endif // IMPORTWORKER_H

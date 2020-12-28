#include "ImportWorker.h"
#include <QDebug>
#include <QDateTime>
#include <QDate>

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"

ImportWorker::ImportWorker(QObject *parent) : QObject(parent)
{
    qRegisterMetaType< QMultiMap<QString, QString> >("QMultiMap<QString, QString>");
}

ImportWorker::~ImportWorker()
{
    qDebug() << "ImportWorker::~ImportWorker";
}

void ImportWorker::slotStart()
{
    qDebug() << "ImportWorker::slotStart";
}

void ImportWorker::slotImport(const QString fileName)
{
    m_workerMap.clear();
    m_workerInfoMap.clear();

    //打开excel
    QXlsx::Document xlsx(fileName);

    QXlsx::CellRange range = xlsx.dimension();

    int rowCount = range.rowCount();

    qDebug() << "range row:" << rowCount;

    //读取每一行
    for (int row = 2; row <= rowCount; ++row)
    {
        QString groupName = xlsx.read(row,1).toString();
        QString workerName = xlsx.read(row,2).toString();
        QString time = xlsx.read(row,3).toString();
        QDateTime datetime = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
        QString date = datetime.date().toString("yyyy-MM-dd");

        if (!m_workerMap.contains(groupName, workerName))
        {
            m_workerMap.insert(groupName, workerName);
        }

        if (!m_workerInfoMap.contains(workerName, date))
        {
            m_workerInfoMap.insert(workerName, date);
        }

        emit signalImportProgress(row, rowCount);
    }

    qDebug() << "m_workerMap size = " << m_workerMap.size()
             << "m_workerInfoMap size = " << m_workerInfoMap.size();
}

void ImportWorker::slotExportData()
{
    emit signalExportData(m_workerMap, m_workerInfoMap);
}

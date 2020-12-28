#include "ExportWorker.h"
#include <QDebug>
#include <QFile>
#include <QDate>

#include "xlsxabstractsheet.h"

ExportWorker::ExportWorker(QObject *parent) : QObject(parent)
{

}

ExportWorker::~ExportWorker()
{
    qDebug() << "ExportWorker::~ExportWorker";
}

void ExportWorker::slotStart()
{
    qDebug() << "ExportWorker::slotStart";
}

void ExportWorker::slotExportFileName(const QString fileName)
{
    m_fileName = fileName;
}

void ExportWorker::slotExportData(QMultiMap<QString, QString> workerMap, QMultiMap<QString, QString> workerInfoMap)
{
    QStringList sheetNames(workerMap.keys());
    //初始化导出模板
    initXlsxTemplate(sheetNames, workerMap);

    QXlsx::Document xlsx(m_fileName);

    int progress = 0;
    foreach (QString sheet, sheetNames) {
        progress++;
        xlsx.selectSheet(sheet);
        QStringList workers(workerMap.values(sheet));
        int size = workers.size();
        int pos = 0;
        if (size > 15)
        {
            QStringList temp = workers.mid(pos,15);
            //写入数据
            writeData2Xlsx(xlsx, temp, workerInfoMap);

            pos += 15;
            size -= 15;
            int count = 1;
            while(size > 0)
            {
                count += 1;
                xlsx.selectSheet(QString("%1_%2").arg(sheet).arg(count));

                if (size > 15)
                {
                    QStringList temp = workers.mid(pos,15);
                    //写入数据
                    writeData2Xlsx(xlsx, temp, workerInfoMap);
                }
                else
                {
                    QStringList temp = workers.mid(pos,size);
                    //写入数据
                    writeData2Xlsx(xlsx, temp, workerInfoMap);
                }


                pos += 15;
                size -= 15;
            }
        }
        else
        {
            writeData2Xlsx(xlsx, workers, workerInfoMap);
        }

        emit signalExportProgress(progress,sheetNames.size());
    }

    qDebug() << xlsx.save();
}

void ExportWorker::initXlsxTemplate(QStringList sheetNames, QMultiMap<QString, QString> &workerMap)
{
    //打开excel
    QXlsx::Document xlsx(":/excel/考勤表示例.xlsx");

    QString model = xlsx.sheetNames().first();
    xlsx.selectSheet(model);

    QXlsx::CellRange range = xlsx.dimension();
    int col = range.columnCount();
    int row = range.rowCount();

    QList<QXlsx::Format> colFormatList;
    QList<int> colWidthList;
    QList<QXlsx::Format> rowFormatList;
    QList<int> rowHeightList;

    for (int i = 0; i < col; ++i)
    {
        colFormatList.append(xlsx.columnFormat(i+1));
        colWidthList.append(xlsx.columnWidth(i+1));
    }

    for (int i = 0; i < row; ++i)
    {
        rowFormatList.append(xlsx.rowFormat(i+1));
        rowHeightList.append(xlsx.rowHeight(i+1));
    }

    foreach (QString sheet, sheetNames) {
        //超出15个要多拷贝几份
        xlsx.copySheet(model, sheet);
        QStringList workers(workerMap.values(sheet));
        int size = workers.size();
        size -= 15;
        int count = 1;
        while(size > 0)
        {
            count += 1;
            xlsx.copySheet(model, QString("%1_%2").arg(sheet).arg(count));
            size -= 15;
        }
    }

    foreach (QString sheet, sheetNames) {
        xlsx.selectSheet(sheet);

        xlsx.write(2,1, QString("班组：%1班组").arg(sheet));

        for (int i = 0; i < col; ++i)
        {
            xlsx.setColumnFormat(i+1, colFormatList.at(i));
            xlsx.setColumnWidth(i+1, colWidthList.at(i));
        }

        for (int i = 0; i < row; ++i)
        {
            xlsx.setRowFormat(i+1, rowFormatList.at(i));
            xlsx.setRowHeight(i+1, rowHeightList.at(i));
        }

        QStringList workers(workerMap.values(sheet));
        int size = workers.size();
        size -= 15;
        int count = 1;
        while(size > 0)
        {
            count += 1;
            xlsx.selectSheet(QString("%1_%2").arg(sheet).arg(count));

            xlsx.write(2,1, QString("班组：%1班组").arg(sheet));

            for (int i = 0; i < col; ++i)
            {
                xlsx.setColumnFormat(i+1, colFormatList.at(i));
                xlsx.setColumnWidth(i+1, colWidthList.at(i));
            }

            for (int i = 0; i < row; ++i)
            {
                xlsx.setRowFormat(i+1, rowFormatList.at(i));
                xlsx.setRowHeight(i+1, rowHeightList.at(i));
            }

            size -= 15;
        }
    }

    xlsx.deleteSheet(model);

    bool result = xlsx.saveAs(m_fileName);

    qDebug() << "result = " << result;
}

void ExportWorker::writeData2Xlsx(QXlsx::Document &xlsx , QStringList workers, QMultiMap<QString, QString> &workerInfoMap)
{
    int count = workers.size();//<=15

    for (int i = 0; i < count; ++i)
    {
        QString worker = workers.at(i);
        xlsx.write(i+5, 2, worker);

        QStringList dates(workerInfoMap.values(worker));

        foreach (QString date, dates) {
            QDate qdate = QDate::fromString(date, "yyyy-MM-dd");
            int day = qdate.day();
            xlsx.write(i+5, 2+day, "√");
        }

        xlsx.write(i+5, 34, dates.size());
    }
}

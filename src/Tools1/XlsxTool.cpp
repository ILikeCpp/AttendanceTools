#include "XlsxTool.h"
#include <QDebug>
#include <QDateTime>

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"

XlsxTool *XlsxTool::instance()
{
    static XlsxTool tools;
    return &tools;
}

void XlsxTool::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void XlsxTool::writeXlsx()
{
    //打开excel
    QXlsx::Document xlsx(m_fileName);

    QXlsx::CellRange range = xlsx.dimension();

    qDebug() << "range row:" << range.rowCount() << "range last row:" << range.lastRow();

    //保存到文件

    bool ok = xlsx.saveAs(m_fileName);

    qDebug() << "XlsxTool::writeXlsx ok:" << ok;
}

XlsxTool::XlsxTool(QObject *parent) : QObject(parent)
{

}

XlsxTool::~XlsxTool()
{
    qDebug() << "XlsxTool::~XlsxTool";
}

#ifndef XLSXTOOL_H
#define XLSXTOOL_H

#include <QObject>

class XlsxTool : public QObject
{
    Q_OBJECT
public:
    static XlsxTool *instance();

    void setFileName(const QString &fileName);
    void writeXlsx();

signals:

public slots:

private:
    explicit XlsxTool(QObject *parent = nullptr);
    XlsxTool(const XlsxTool &); // 禁止拷贝
    XlsxTool & operator= (const XlsxTool &); // 禁止赋值
    ~XlsxTool();

private:
    QString m_fileName;
};

#endif // XLSXTOOL_H

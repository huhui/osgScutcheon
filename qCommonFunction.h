#ifndef QCOMMONFUNCTION_H
#define QCOMMONFUNCTION_H

#include "Windows.h"
#include "WinNls.h"
#include <iostream>
#include <QString>
#include <osg/Referenced>
#include <osgText/String>
#include <osgText/Text>

class CommonFunction
{
public:
    CommonFunction();
    ~CommonFunction();
    std::string unicodeToUTF8(const std::wstring &src);
    std::wstring gb2312ToUnicode(const std::string &src);
    std::string gb2312ToUTF8(const std::string &src);
    std::string qstringToOsgTextToStdString(QString &srcString);
    osgText::String qstringToOsgTextString(const QString &srcString);
    QString asciiToUTF8(std::string &src);
};

#endif // QCOMMONFUNCTION_H

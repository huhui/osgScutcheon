#include "qCommonFunction.h"

CommonFunction::CommonFunction()
{

}

CommonFunction::~CommonFunction()
{

}

//wstring转string
std::string CommonFunction:: unicodeToUTF8(const std::wstring &src)
{
    std::string result;
    int n = WideCharToMultiByte(CP_UTF8,0,src.c_str(),-1,0,0,0,0);
    result.resize(n);
    ::WideCharToMultiByte(CP_UTF8,0,src.c_str(),-1,(char*)result.c_str(),result.length(),0,0);
    return result;
}
//string转wstring
std::wstring CommonFunction:: gb2312ToUnicode(const std::string &src)
{
    std::wstring result;
    int n = MultiByteToWideChar(CP_ACP,0,src.c_str(),-1,NULL,0);
    result.resize(n);
    ::MultiByteToWideChar(CP_ACP,0,src.c_str(),-1,(LPWSTR)result.c_str(),result.length());
    return result;
}
//先string转wstring
//再wstring转string
std::string CommonFunction::gb2312ToUTF8(const std::string &src)
{
    std::string result;
    result = unicodeToUTF8(gb2312ToUnicode(src));
    return result;
}

std::string CommonFunction::qstringToOsgTextToStdString(QString &srcString)
{
    osgText::String dstString;
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");
    text->setFont(font);
    const std::string tmpStr = srcString.toStdString();
    text->setText(tmpStr, osgText::String::ENCODING_UTF8);
    dstString = text->getText();
    return dstString.createUTF8EncodedString();
}

QString CommonFunction::asciiToUTF8(std::string &src)
{
    //ANSI转unicode
    setlocale(LC_ALL, "chs");
    wchar_t wcsStr[100];
    swprintf(wcsStr, L"%S",src.c_str());
    //unicode转utf-8
    QString placeName = QString::fromWCharArray(wcsStr);
    return placeName;
    //return "";
}

osgText::String CommonFunction::qstringToOsgTextString(const QString &srcString)
{
    osgText::String dstString;
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");
    text->setFont(font);
    const std::string tmpStr = srcString.toStdString();
    text->setText(tmpStr, osgText::String::ENCODING_UTF8);
    dstString = text->getText();
    return dstString;//.createUTF8EncodedString()
}


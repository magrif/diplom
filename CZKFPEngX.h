#include <QVariant>
#include <QColor>
#include <afxres.h>
#include <oleauto.h>
#include <QAxBase>
#include <QAxObject>
#include <QDebug>
#include <QUuid>
#include "windows.h"
#include "comutil.h"
#include <QBitmap>
#include "VrBio.h"

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CZKFPEngX

void callback(int ev, const char* reader, VrBio_BiometricImage* data, const void* userData);

class CZKFPEngX : public QAxObject
{
    Q_OBJECT
    IDispatch *iface;
    VrBio_BiometricImage *img;

public:
    CZKFPEngX()
    {
        setControl(QString::fromUtf8("{CA69969C-2F27-41D3-954D-A48B941C3BA7}"));
        connect(this,SIGNAL(exception(int,QString,QString,QString)),this,SLOT(slotException(int,QString,QString,QString)));

        iface = 0;
        QUuid id(QString::fromUtf8("{161A8D2D-3DDE-4744-BA38-08F900D10D6D}"));
        queryInterface(id,(void **)&iface);
        if (iface)
        {
            qDebug() << "Interface is loaded";
        }

        if (VInstallLicense("0000-03d7-c2eb-f4ef-3852", NULL) != VRBIO_SUCCESS)
        {
            qDebug() << "License not loaded";
        }

        VStartSDK(callback);
        VInitializeBiometricImage(&img);

        int N(-1);
        const char **list;
        VListDevices(&list,&N);
        qDebug() << "Устройств найдено:" << N;
        VStringListFree(&list);

        //int size;
        //char *tpl;
        //qDebug() << VLoadImageFromFile("fingerprint.jpg",600,&img);
        //qDebug() << VExtract(img,&tpl,&size,VRBIO_IMAGEFORMAT_JPEG);
        img->biometricModality = 0;
        img->bufferSize = 116768;
        img->channels = 1;
        img->compressionRate = 15;
        img->fingerQuality = 4;
        img->width = 328;
        img->height = 356;
        img->imageFormat = VRBIO_IMAGEFORMAT_BMP;
        img->scannerType = 0;
        img->ISO197944_parameters->compressionMode = 0;
        img->ISO197944_parameters->fingerPosition = 0;
        img->ISO197944_parameters->impressionType = 0;
        img->resolution = 800;

    }
    ~CZKFPEngX()
    {
        //Выгрузить интерфейс
        if (iface)
            iface->Release();
        VDestroyBiometricImage(&img);
        clear();
    }

    int get_EnrollCount()
	{
        int result = dynamicCall("EnrollCount()").toInt();
        return result;
	}
    void put_EnrollCount(int newValue)
	{
        dynamicCall("EnrollCount", newValue);
	}
    bool VerFinger(VARIANT* regTemplate, VARIANT verTemplate, BOOL ADoLearning, BOOL* ARegFeatureChanged)
	{
        /*QList<QVariant> var;
        var << regTemplate << verTemplate << QVariant(ADoLearning) << QVariant(ARegFeatureChanged);
        bool result = dynamicCall("VerFinger(QVariant&,const QVariant&,bool,bool&)", var).toBool();
        regTemplate = var.at(0);
        ARegFeatureChanged = var.at(3).toBool();
        return result;*/

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[4];
        //regTemplate.vt = VT_EMPTY;

        pDispParams.cArgs = 4;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_BOOL | VT_BYREF;
        pDispParams.rgvarg[0].boolVal = *ARegFeatureChanged;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_BOOL;
        pDispParams.rgvarg[1].boolVal = ADoLearning;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_VARIANT;
        pDispParams.rgvarg[2].pvarVal = &verTemplate;

        VariantInit(&pDispParams.rgvarg[3]);
        pDispParams.rgvarg[3].vt = VT_VARIANT | VT_BYREF;
        pDispParams.rgvarg[3].pvarVal = regTemplate;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;

        /*HRESULT hr = */iface->Invoke(0x8, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        result = pVarResult.boolVal;
        return (bool)result;


	}
    bool VerRegFingerFile(QString regTemplateFile, QVariant verTemplate, bool ADoLearning, bool& ARegFeatureChanged)
	{
        QList<QVariant> var;
        var << regTemplateFile << verTemplate << QVariant(ADoLearning) << QVariant(ARegFeatureChanged);
        bool result = dynamicCall("VerRegFingerFile(const QString&, const QVariant&, bool, bool&)",regTemplateFile, verTemplate, ADoLearning, ARegFeatureChanged).toBool();
        ARegFeatureChanged = var.at(3).toBool();
        return result;
	}
    void PrintImageAt(int hdc, int x, int y, int aWidth, int aHeight)
	{
        dynamicCall("PrintImageAt(int,int,int,int,int)", hdc, x, y, aWidth, aHeight);
	}
    void PrintImageEllipseAt(int hdc, int x, int y, int aWidth, int aHeight, QColor bkColor)
	{
        dynamicCall("PrintImageEllipseAt(int, int , int , int , int , const QColor&)", hdc, x, y, aWidth, aHeight, bkColor);
	}
	void BeginEnroll()
	{
        dynamicCall("BeginEnroll()");
	}
    bool SaveTemplate(QString FileName, QVariant ATemplate)
	{
        bool result = dynamicCall("SaveTemplate(const QString&,const QVariant&)", FileName, ATemplate).toBool();
        return result;
	}
    void SaveBitmap(QString FileName)
	{
        dynamicCall("SaveBitmap(const QString&)",FileName);
	}
    void SaveJPG(QString FileName)
	{
        dynamicCall("SaveJPG(const QString&)",FileName);
	}
    int InitEngine()
	{
        int result = dynamicCall("InitEngine()").toInt();
        return result;
	}
    int get_SensorIndex()
	{
        int result = dynamicCall("SensorIndex()").toInt();
        return result;
	}
    void put_SensorIndex(int newValue)
	{
        dynamicCall("SensorIndex", newValue);
	}
	void CancelEnroll()
	{
        dynamicCall("CancelEnroll()");
	}
    int CreateFPCacheDB()
	{
        int result = dynamicCall("CreateFPCacheDB()").toInt();
        return result;
	}
    void FreeFPCacheDB(int fpcHandle)
	{
        dynamicCall("FreeFPCacheDB(int)", fpcHandle);

	}
    int AddRegTemplateToFPCacheDB(int fpcHandle, int FPID, QVariant pRegTemplate)
	{
        int result = dynamicCall("AddRegTemplateToFPCacheDB(int,int,const QVariant&)", fpcHandle, FPID, pRegTemplate).toInt();
        return result;
	}
    int RemoveRegTemplateFromFPCacheDB(int fpcHandle, int FPID)
	{
        int result = dynamicCall("RemoveRegTemplateFromFPCacheDB(int,int)", fpcHandle, FPID).toInt();
        return result;
	}
    int AddRegTemplateFileToFPCacheDB(int fpcHandle, int FPID, QString pRegTemplateFile)
	{
        int result = dynamicCall("AddRegTemplateFileToFPCacheDB(int,int,const QString&)", fpcHandle, FPID, pRegTemplateFile).toInt();
        return result;
	}
    int get_Threshold()
	{
        int result = dynamicCall("Threshold()").toInt();
        return result;
	}
    void put_Threshold(int newValue)
	{
        dynamicCall("Threshold", newValue);
	}
    bool DongleIsExist()
	{
        bool result = dynamicCall("DongleIsExist()").toBool();
        return result;
	}
    int DongleUserID()
	{
        int result = dynamicCall("DongleUserID()").toInt();
        return result;
	}
    bool DongleSeed(int& lp2, int& p1, int& p2, int& p3, int& p4)
	{
        QList<QVariant> var;
        var << QVariant(lp2) << QVariant(p1) << QVariant(p2) << QVariant(p3) << QVariant(p4);
        bool result = dynamicCall("DongleSeed(int&, int&, int&, int&, int&)",var).toBool();
        lp2 = var.at(0).toInt();
        p1 = var.at(1).toInt();
        p2 = var.at(2).toInt();
        p3 = var.at(3).toInt();
        p4 = var.at(4).toInt();
        return result;
	}
    bool DongleMemRead(int& p1, int& p2, QVariant& buf)
	{
        QList<QVariant> var;
        var << QVariant(p1) << QVariant(p2) << buf;
        bool result = dynamicCall("DongleMemRead(int&,int&,QVariant&)",var).toBool();
        p1 = var.at(0).toInt();
        p2 = var.at(1).toInt();
        buf = var.at(2);
        return result;
	}
    bool DongleMemWrite(int& p1, int& p2, QVariant& buf)
	{
        QList<QVariant> var;
        var << QVariant(p1) << QVariant(p2) << buf;
        bool result = dynamicCall("DongleMemWrite(int&,int&,QVariant&)",var).toBool();
        p1 = var.at(0).toInt();
        p2 = var.at(1).toInt();
        buf = var.at(2);
        return result;
	}
    bool VerFingerFromFile(QString regTemplateFile, QString verTemplateFile, bool ADoLearning, bool& ARegFeatureChanged)
	{
        QList<QVariant> var;
        var << QVariant(regTemplateFile) << QVariant(verTemplateFile) << QVariant(ADoLearning) << QVariant(ARegFeatureChanged);
        bool result = dynamicCall("VerFingerFromFile(const QString&,const QString&,bool,bool&)",var).toBool();
        ARegFeatureChanged = var.at(3).toBool();
        return result;
	}
    QString get_VerTplFileName()
	{
        QString result = dynamicCall("VerTplFileName()").toString();
        return result;
	}
    void put_VerTplFileName(QString newValue)
	{
        dynamicCall("VerTplFileName",newValue);
	}
    QString get_RegTplFileName()
	{
        QString result = dynamicCall("RegTplFileName()").toString();
        return result;
	}
    void put_RegTplFileName(QString newValue)
	{
        dynamicCall("RegTplFileName",newValue);
	}
    QVariant GetTemplate()
	{
        DISPPARAMS pDispParams;
        pDispParams.cArgs = 0;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = 0;

        VARIANT pVarResult;
        pVarResult.vt = VT_EMPTY;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)-2;

        qDebug() << iface->Invoke(0x2d, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        long iLBound, iUBound, sz;
        char * data;
        qDebug() << SafeArrayGetLBound(pVarResult.parray, pVarResult.parray->cDims, &iLBound);
        qDebug() << SafeArrayGetUBound(pVarResult.parray, pVarResult.parray->cDims, &iUBound);
        qDebug() << SafeArrayAccessData(pVarResult.parray,(void**)(&data));
        sz = iUBound - iLBound + 1;
        QByteArray ba = QByteArray::fromRawData(data,sz);
        QString s(ba);
        qDebug () << s;

        QVariant result = dynamicCall("GetTemplate()");
        return result;
	}
    bool GetFingerImage(QImage& FPImage)
	{
        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[1];
        VARIANT AFingerImage;
        AFingerImage.vt = VT_EMPTY;

        pDispParams.cArgs = 1;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;
        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_VARIANT | VT_BYREF;
        pDispParams.rgvarg[0].pvarVal = &AFingerImage;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;

        HRESULT hr = iface->Invoke(0x2e, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        if(FAILED(hr)) return false;
        long iLBound, iUBound, sz;
        char * data;
        hr = SafeArrayGetLBound(AFingerImage.parray, AFingerImage.parray->cDims, &iLBound);
        hr = SafeArrayGetUBound(AFingerImage.parray, AFingerImage.parray->cDims, &iUBound);
        hr = SafeArrayAccessData(AFingerImage.parray,(void**)(&data));
        if(FAILED(hr)) return false;
        sz = iUBound - iLBound + 1;
        if(FAILED(sz)) return false;
        QByteArray ba = QByteArray::fromRawData(data,sz); 
        FPImage.loadFromData(ba);
        hr = SafeArrayUnaccessData(AFingerImage.parray);
        if(FAILED(hr)) return false;

        //Формируем шаблон отпечатка пальца
        int size;
        char *tpl;
        img->buffer = new unsigned char[sz + 1];
        memcpy(img->buffer, FPImage.bits(), sz);
        img->bufferSize = sz;
        qDebug() << VExtract(img,&tpl,&size,VRBIO_TEMPLATEFORMAT_ISO);
        QByteArray tpt(tpl,size);
        qDebug() << tpt.toHex();

        delete [] img->buffer;
        VTemplateFree(&tpl);

        result = pVarResult.boolVal;
        return (bool)result;

	}
    int get_OneToOneThreshold()
	{
        int result = dynamicCall("OneToOneThreshold()").toInt();
        return result;
	}
    void put_OneToOneThreshold(int newValue)
	{
        dynamicCall("OneToOneThreshold",newValue);
	}
    bool IsOneToOneTemplate(QVariant ATemplate)
	{
        bool result = dynamicCall("IsOneToOneTemplate(const QVariant&)",ATemplate).toBool();
        return result;
	}
    void ModifyTemplate(QVariant& ATemplate, bool AOneToOne)
	{
        //static BYTE parms[] = VTS_PVARIANT VTS_BOOL ;
        //InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ATemplate, AOneToOne);

        dynamicCall("ModifyTemplate(QVariant&,bool)", ATemplate, AOneToOne);
	}
	void FlushFPImages()
	{
        dynamicCall("FlushFPImages()");
	}
    bool get_Active()
	{
        bool result = dynamicCall("Active()").toBool();
        return result;
	}
    void put_Active(bool newValue)
	{
        dynamicCall("Active",newValue);
	}
    bool get_IsRegister()
	{
        bool result = dynamicCall("IsRegister()").toBool();
        return result;

	}
    void put_IsRegister(bool newValue)
	{
        dynamicCall("IsRegister",newValue);
	}
    int get_EnrollIndex()
	{
        int result = dynamicCall("EnrollIndex()").toInt();
        return result;
	}
    void put_EnrollIndex(int newValue)
	{
        dynamicCall("EnrollIndex",newValue);
	}
    QString get_SensorSN()
	{
        QString result = dynamicCall("SensorSN()").toString();
        return result;
	}
    void put_SensorSN(QString newValue)
	{
        dynamicCall("SensorSN",newValue);
	}
    QString get_FPEngineVersion()
	{
        QString result = dynamicCall("FPEngineVersion()").toString();
        return result;
	}
    void put_FPEngineVersion(QString newValue)
	{
        dynamicCall("FPEngineVersion", newValue);
	}
    int get_ImageWidth()
	{
        int result = dynamicCall("ImageWidth()").toInt();
        return result;
	}
    void put_ImageWidth(int newValue)
	{
        dynamicCall("ImageWidth",newValue);
	}

    int get_ImageHeight()
	{
        int result = dynamicCall("ImageHeight()").toInt();
        return result;
	}
    void put_ImageHeight(int newValue)
	{
        dynamicCall("ImageHeight",newValue);
	}
	long get_SensorCount()
	{
        int result = dynamicCall("SensorCount()").toInt();
        return result;
	}
    void put_SensorCount(int newValue)
	{
        dynamicCall("SensorCount",newValue);
	}
    int get_TemplateLen()
	{
        int result = dynamicCall("TemplateLen()").toInt();
        return result;
	}
    void put_TemplateLen(int newValue)
	{
        dynamicCall("TemplateLen",newValue);
	}
    bool get_EngineValid()
	{
        bool result = dynamicCall("EngineValid()").toBool();
        return result;
	}
    void put_EngineValid(bool newValue)
	{
        dynamicCall("EngineValid",newValue);
	}
    bool DecodeTemplate(QString ASour, QVariant& ADest)
	{
        bool result = dynamicCall("DecodeTemplate(const QString&,QVariant&)",ASour,ADest).toBool();
        return result;
	}
    bool EncodeTemplate(QVariant ASour, QString& ADest)
	{
        //bool result;
        //static BYTE parms[] = VTS_VARIANT VTS_PBSTR ;
        //InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &ASour, ADest);
        //return result;

        bool result = dynamicCall("EncodeTemplate(const QVariant&,QString&)",ASour,ADest).toBool();
        return result;
	}
	void BeginCapture()
	{
        dynamicCall("BeginCapture()");
	}
	void CancelCapture()
	{
        dynamicCall("CancelCapture()");
	}
	void EndEngine()
	{
        dynamicCall("EndEngine()");
	}
    QVariant DecodeTemplate1(QString ASour)
	{
        QVariant result = dynamicCall("DecodeTemplate1(const QString&",ASour);
        return result;
	}
    QString EncodeTemplate1(QVariant ASour)
	{
        //QString result;
        //static BYTE parms[] = VTS_VARIANT ;
        //InvokeHelper(0x21, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &ASour);
        //return result;

        QString result = dynamicCall("EncodeTemplate1(const QVariant&)",ASour).toString();
        return result;
	}
    int AddRegTemplateStrToFPCacheDB(int fpcHandle, int FPID, QString ARegTemplateStr)
	{
        int result = dynamicCall("AddRegTemplateStrToFPCacheDB(int,int,const QString&",fpcHandle,FPID,ARegTemplateStr).toInt();
        return result;
	}
    bool VerFingerFromStr(QString& regTemplateStr, QString verTemplateStr, bool ADoLearning, bool& ARegFeatureChanged)
	{
        QList<QVariant> var;
        var << QVariant(regTemplateStr) << QVariant(verTemplateStr) << QVariant(ADoLearning) << QVariant(ARegFeatureChanged);
        bool result = dynamicCall("VerFingerFromStr(QString&,const QString&,bool,bool&)",var).toBool();
        regTemplateStr = var.at(0).toString();
        ARegFeatureChanged = var.at(3).toBool();
        return result;
    }
    QString GetTemplateAsString()
	{
        QString result = dynamicCall("GetTemplateAsString()").toString();
        return result;
	}
    bool IsOneToOneTemplateStr(QString ATemplate)
	{
        bool result = dynamicCall("IsOneToOneTemplateStr(const QString&)",ATemplate).toBool();
        return result;
	}
    void ModifyTemplateStr(QString& ATemplate, bool AOneToOne)
	{
        QList<QVariant> var;
        var << QVariant(ATemplate) << QVariant(AOneToOne);
        dynamicCall("ModifyTemplateStr(QString&,bool)",var);
        ATemplate = var.at(0).toString();
	}
    void SaveTemplateStr(QString FileName, QString ATemplateStr)
    {
        dynamicCall("SaveTemplateStr(const QString&,const QString&)", FileName,ATemplateStr);
	}
    void GetTemplateCount(int AFPHandle, int& AOneToOneCnt, int& ATotalCnt)
	{
        QList<QVariant> var;
        var << QVariant(AFPHandle) << QVariant(AOneToOneCnt) << QVariant(ATotalCnt);
        dynamicCall("GetTemplateCount(int,int&,int&)", var);
        AOneToOneCnt = var.at(1).toInt();
        ATotalCnt = var.at(2).toInt();
	}
    int IdentificationInFPCacheDB(int fpcHandle, QVariant pVerTemplate, int& Score, int& ProcessedFPNumber)
	{
        QList<QVariant> var;
        var << QVariant(fpcHandle) << pVerTemplate << QVariant(Score) << QVariant(ProcessedFPNumber);
        int result = dynamicCall("IdentificationInFPCacheDB(int,QVariant,int&,int&)",var).toInt();
        Score = var.at(2).toInt();
        ProcessedFPNumber = var.at(3).toInt();
        return result;
	}
    int IdentificationFromFileInFPCacheDB(int fpcHandle, QString pVerTemplateFile, int& Score, int& ProcessedFPNumber)
	{
        QList<QVariant> var;
        var << QVariant(fpcHandle) << QVariant(pVerTemplateFile) << QVariant(Score) << QVariant(ProcessedFPNumber);
        int result = dynamicCall("IdentificationFromFileInFPCacheDB(int,const QString&,int&,int&)",var).toInt();
        Score = var.at(2).toInt();
        ProcessedFPNumber = var.at(3).toInt();
        return result;
	}
    int IdentificationFromStrInFPCacheDB(int fpcHandle, QString AVerTemplateStr, int& Score, int& ProcessedFPNumber)
	{
        QList<QVariant> var;
        var << QVariant(fpcHandle) << QVariant(AVerTemplateStr) << QVariant(Score) << QVariant(ProcessedFPNumber);
        int result = dynamicCall("IdentificationFromStrInFPCacheDB(int,const QString&,int&,int&)",var).toInt();
        Score = var.at(2).toInt();
        ProcessedFPNumber = var.at(3).toInt();
        return result;
	}
    void SetAutoIdentifyPara(bool AAutoIdentify, int ACacheDBHandle, int AScore)
	{
        dynamicCall("SetAutoIdentifyPara(bool,int,int)",AAutoIdentify,ACacheDBHandle,AScore);
	}
    void SetImageDirection(bool AIsImageChange)
	{
        dynamicCall("SetImageDirection(bool)",AIsImageChange);
	}
    bool IsOneToOneTemplateFile(QString ATemplateFileName)
	{
        bool result = dynamicCall("CompressTemplate(const QString&)",ATemplateFileName).toBool();
        return result;
	}
    void ModifyTemplateFile(QString ATemplateFileName, bool AOneToOne)
	{
        dynamicCall("ModifyTemplateFile(const QString&,bool)",ATemplateFileName,AOneToOne);
	}
    QVariant GetVerTemplate()
	{
        QVariant result = dynamicCall("GetVerTemplate()");
        return result;
	}
    int GetVerScore()
	{
        int result = dynamicCall("GetVerScore()").toInt();
        return result;
	}
    bool AddImageFile(QString AFileName, bool ADPI)
	{
        bool result = dynamicCall("CompressTemplate(const QString&,bool)",AFileName,ADPI).toBool();
        return result;
	}
    void SetOneToOnePara(int ADoLearning, int AMatchSecurity)
	{
        dynamicCall("SetOneToOnePara(int,int)",ADoLearning,AMatchSecurity);
	}
    QVariant CompressTemplate(QString ATemplate)
	{
        QVariant result = dynamicCall("CompressTemplate(const QString&)",ATemplate);
        return result;
	}
    QVariant ConvertAttTemplate(QVariant ATemplate)
	{
        QVariant result = dynamicCall("ConvertAttTemplate(const QVariant&)",ATemplate);
        return result;
	}
    bool get_ForceSecondMatch()
	{
        bool result = dynamicCall("ForceSecondMatch()").toBool();
        return result;
	}
    void put_ForceSecondMatch(bool newValue)
	{
        dynamicCall("ForceSecondMatch",newValue);
	}
    bool AddBitmap(int BitmapHandle, int ValidRectX1, int ValidRectY1, int ValidRectX2, int ValidRectY2, int DPI)
	{
        bool result = dynamicCall("AddBitmap(int,int,int,int,int,int)",BitmapHandle,ValidRectX1,ValidRectY1,ValidRectX2,ValidRectY2,DPI).toBool();
        return result;
	}
    void UsingXTFTemplate(bool ADoUsingXTFTemplate)
	{
        dynamicCall("UsingXTFTemplate(bool)",ADoUsingXTFTemplate);
	}
    int ExtractImageFromURU4000(QString AOriImageBuf, int Size, bool AAutoIdentify, QVariant& iResult)
	{
        int result = dynamicCall("ExtractImageFromURU4000(const QString&,int,bool,QVariant&)",AOriImageBuf,Size,AAutoIdentify,iResult).toInt();
        return result;
	}
    QVariant ConvertToBiokey(QVariant OriTemplate)
	{
        QVariant result = dynamicCall("ConvertToBiokey(const QVariant)",OriTemplate);
        return result;
	}
    bool GenRegTemplateAsStringFromFile(QString AImageFileName, int ADPI, QString& ADest)
	{
        QList<QVariant> vars;
        vars << QVariant(AImageFileName) << QVariant(ADPI) << QVariant(ADest);
        bool result = dynamicCall("GenRegTemplateAsStringFromFile(const QString&,int,QString&)",vars).toBool();
        ADest = vars.at(2).toString();
        return result;
	}
    bool GenVerTemplateAsStringFromFile(QString AImageFileName, int ADPI, QString& ADest)
	{
        QList<QVariant> vars;
        vars << QVariant(AImageFileName) << QVariant(ADPI) << QVariant(ADest);
        bool result = dynamicCall("GenVerTemplateAsStringFromFile(const QString&,int,QString&)",vars).toBool();
        ADest = vars.at(2).toString();
        return result;
	}
    int ExtractImageFromURU(QString AOriImageStr, int Size, bool AAutoIdentify, QVariant& iResult)
	{
        QList<QVariant> vars;
        vars << QVariant(AOriImageStr) << QVariant(Size) << QVariant(AAutoIdentify) << QVariant(iResult);
        int result = dynamicCall("ExtractImageFromURU(const QString&,int,bool,QVariant&)",vars).toInt();
        iResult = vars.at(3);
        return result;
	}
    QString get_Vendor()
	{
        QString result = dynamicCall("Vendor()").toString();
        return result;
	}
    QString get_ProduceName()
	{
        QString result = dynamicCall("ProduceName()").toString();
        return result;
	}
    int SetTemplateLen(QVariant& ATemplate, int ALen)
	{
        //long result;
        //static BYTE parms[] = VTS_PVARIANT VTS_I4 ;
        //InvokeHelper(0xd0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ATemplate, ALen);
        //return result;
        int result = dynamicCall("SetTemplateLen(QVariant&,int)",ATemplate,ALen).toInt();
        return result;
	}
    int ControlSensor(int ACode, int AValue)
	{
        int result = dynamicCall("ControlSensor(int,int)",ACode,AValue).toInt();
        return result;
	}
    int get_IsSupportAuxDevice()
	{
        int result = dynamicCall("IsSupportAuxDevice()").toInt();
        return result;
	}
    int ExtractImageFromTerminal(QVariant AOriImage, int Size, bool AAutoIdentify, QVariant& iResult)
	{
        //long result;
        //static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_BOOL VTS_PVARIANT ;
        //InvokeHelper(0xd3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, &AOriImage, Size, AAutoIdentify, iResult);
        //return result;

        int result = dynamicCall("ExtractImageFromTerminal(const QVariant&,int,bool,QVariant&)",AOriImage,Size,AAutoIdentify,iResult).toInt();
        return result;
	}
	long CreateFPCacheDBEx()
	{
        int result = dynamicCall("CreateFPCacheDBEx()").toInt();
        return result;
	}
    void FreeFPCacheDBEx(int fpcHandle)
	{
        dynamicCall("FreeFPCacheDBEx(int)",fpcHandle);
	}
    int AddRegTemplateToFPCacheDBEx(int fpcHandle, int FPID, QVariant pRegTemplate, QVariant pRegTemplate10)
	{
        int result = dynamicCall("AddRegTemplateToFPCacheDBEx(int,int,const QVariant&,const QVariant&)",fpcHandle,FPID,pRegTemplate,pRegTemplate10).toInt();
        return result;
	}
    int RemoveRegTemplateFromFPCacheDBEx(int fpcHandle, int FPID)
	{
        int result = dynamicCall("RemoveRegTemplateFromFPCacheDBEx(int,int)",fpcHandle,FPID).toInt();
        return result;
	}
    int AddRegTemplateFileToFPCacheDBEx(int fpcHandle, int FPID, QString pRegTemplateFile, QString pRegTemplate10File)
	{
        int result = dynamicCall("AddRegTemplateFileToFPCacheDBEx(int,int,const QString&,const QString&)",fpcHandle,FPID,pRegTemplateFile,pRegTemplate10File).toInt();
        return result;
	}
    QVariant GetTemplateEx(QString AFPEngineVersion)
	{
        QVariant result = dynamicCall("GetTemplateEx(const QString&)",AFPEngineVersion);
        return result;
	}
    int AddRegTemplateStrToFPCacheDBEx(int fpcHandle, int FPID, QString ARegTemplateStr, QString ARegTemplate10Str)
	{
        int result = dynamicCall("AddRegTemplateStrToFPCacheDBEx(int,int,const QString&,const QString&)",fpcHandle,FPID,ARegTemplateStr,ARegTemplate10Str).toInt();
        return result;
	}
    QString GetTemplateAsStringEx(QString AFPEngineVersion)
	{
        QString str = dynamicCall("GetTemplateAsStringEx(const QString&)",AFPEngineVersion).toString();
        return str;
	}
    QVariant GetVerTemplateEx(QString AFPEngineVersion)
	{
        QVariant result = dynamicCall("GetVerTemplateEx(const QString&)",AFPEngineVersion);
        return result;


	}
    bool MF_GET_SNR(int commHandle, int DeviceAddress, unsigned char mode, unsigned char RDM_halt, unsigned char* snr, unsigned char* Value)
	{
        //BOOL result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_UI1 VTS_UI1 VTS_PUI1 VTS_PUI1 ;
        //InvokeHelper(0xdd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, mode, RDM_halt, snr, Value);
        //return result;

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[6];

        pDispParams.cArgs = 6;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = Value;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[1].pbVal = snr;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_UI1;
        pDispParams.rgvarg[2].bVal = RDM_halt;

        VariantInit(&pDispParams.rgvarg[3]);
        pDispParams.rgvarg[3].vt = VT_UI1;
        pDispParams.rgvarg[3].bVal = mode;

        VariantInit(&pDispParams.rgvarg[4]);
        pDispParams.rgvarg[4].vt = VT_I4;
        pDispParams.rgvarg[4].intVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[5]);
        pDispParams.rgvarg[5].vt = VT_I4;
        pDispParams.rgvarg[5].intVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xdd, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    bool MF_GetSerNum(int commHandle, int DeviceAddress, unsigned char* buffer)
	{
        //BOOL result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_PUI1 ;
        //InvokeHelper(0xde, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, buffer);
        //return result;
        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[3];

        pDispParams.cArgs = 3;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = buffer;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_I4;
        pDispParams.rgvarg[1].intVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_I4;
        pDispParams.rgvarg[2].intVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xde, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    bool MF_SetSerNum(long commHandle, long DeviceAddress, unsigned char * newValue, unsigned char * buffer)
	{
        //bool result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_PUI1 VTS_PUI1 ;
        //InvokeHelper(0xdf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, newValue, buffer);
        //return result;

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[4];

        pDispParams.cArgs = 4;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = buffer;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[1].pbVal = newValue;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_I4;
        pDispParams.rgvarg[2].lVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[3]);
        pDispParams.rgvarg[3].vt = VT_I4;
        pDispParams.rgvarg[3].lVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xdf, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    bool MF_GetVersionNum(long commHandle, long DeviceAddress, unsigned char * VersionNum)
	{
        //bool result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_PUI1 ;
        //InvokeHelper(0xe0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, VersionNum);
        //return result;

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[3];

        pDispParams.cArgs = 3;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = VersionNum;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_I4;
        pDispParams.rgvarg[1].lVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_I4;
        pDispParams.rgvarg[2].lVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xe0, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    bool MF_PCDRead(long commHandle, long DeviceAddress, unsigned char mode, unsigned char blkIndex, unsigned char blkNum, unsigned char * key, unsigned char * buffer)
	{
        //bool result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_UI1 VTS_UI1 VTS_UI1 VTS_PUI1 VTS_PUI1 ;
        //InvokeHelper(0xe1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, mode, blkIndex, blkNum, key, buffer);
        //return result;

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[7];

        pDispParams.cArgs = 7;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = buffer;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[1].pbVal = key;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_UI1;
        pDispParams.rgvarg[2].bVal = blkNum;

        VariantInit(&pDispParams.rgvarg[3]);
        pDispParams.rgvarg[3].vt = VT_UI1;
        pDispParams.rgvarg[3].bVal = blkIndex;

        VariantInit(&pDispParams.rgvarg[4]);
        pDispParams.rgvarg[4].vt = VT_UI1;
        pDispParams.rgvarg[4].bVal = mode;

        VariantInit(&pDispParams.rgvarg[5]);
        pDispParams.rgvarg[5].vt = VT_I4;
        pDispParams.rgvarg[5].lVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[6]);
        pDispParams.rgvarg[6].vt = VT_I4;
        pDispParams.rgvarg[6].lVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xe1, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    bool MF_PCDWrite(int commHandle, int DeviceAddress, unsigned char mode, unsigned char blkIndex, unsigned char blkNum, unsigned char * key, unsigned char * buffer)
	{
        //bool result;
        //static BYTE parms[] = VTS_I4 VTS_I4 VTS_UI1 VTS_UI1 VTS_UI1 VTS_PUI1 VTS_PUI1 ;
        //InvokeHelper(0xe2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commHandle, DeviceAddress, mode, blkIndex, blkNum, key, buffer);
        //return result;

        int result;
        DISPPARAMS pDispParams;
        VARIANTARG vArg[7];

        pDispParams.cArgs = 7;
        pDispParams.cNamedArgs = 0;
        pDispParams.rgdispidNamedArgs = 0;
        pDispParams.rgvarg = vArg;

        VariantInit(&pDispParams.rgvarg[0]);
        pDispParams.rgvarg[0].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[0].pbVal = buffer;

        VariantInit(&pDispParams.rgvarg[1]);
        pDispParams.rgvarg[1].vt = VT_UI1 | VT_BYREF;
        pDispParams.rgvarg[1].pbVal = key;

        VariantInit(&pDispParams.rgvarg[2]);
        pDispParams.rgvarg[2].vt = VT_UI1;
        pDispParams.rgvarg[2].bVal = blkNum;

        VariantInit(&pDispParams.rgvarg[3]);
        pDispParams.rgvarg[3].vt = VT_UI1;
        pDispParams.rgvarg[3].bVal = blkIndex;

        VariantInit(&pDispParams.rgvarg[4]);
        pDispParams.rgvarg[4].vt = VT_UI1;
        pDispParams.rgvarg[4].bVal = mode;

        VariantInit(&pDispParams.rgvarg[5]);
        pDispParams.rgvarg[5].vt = VT_I4;
        pDispParams.rgvarg[5].intVal = DeviceAddress;

        VariantInit(&pDispParams.rgvarg[6]);
        pDispParams.rgvarg[6].vt = VT_I4;
        pDispParams.rgvarg[6].intVal = commHandle;

        VARIANT pVarResult;
        EXCEPINFO pExcepInfo;
        UINT puArgErr = (UINT)0;
        HRESULT hr = iface->Invoke(0xe2, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &pDispParams, &pVarResult, &pExcepInfo, &puArgErr);
        qDebug() << hr;
        result = pVarResult.boolVal;
        return (bool)result;
	}
    int get_ReservedParam()
	{
        int result = dynamicCall("ReservedParam()").toInt();
		return result;
	}
    bool get_IsReturnNoLic()
	{
        bool result = dynamicCall("IsReturnNoLic()").toBool();
		return result;
	}
    void put_IsReturnNoLic(bool newValue)
	{
        dynamicCall("IsReturnNoLic",newValue);
	}
    int get_LastQuality()
	{
        int result = dynamicCall("LastQuality()").toInt();
        return result;
	}
    int get_LowestQuality()
	{
        int result = dynamicCall("LowestQuality()").toInt();
        return result;
	}
    void put_LowestQuality(int newValue)
	{
        dynamicCall("LowestQuality",newValue);
	}

    void DEB(const QString& str)
    {
        qDebug() << str;
    }

    void DEB(HRESULT hr)
    {
        switch (hr) {
        case S_OK:
            qDebug() << "HRESULT = True result"; break;
        case S_FALSE:
            qDebug() << "HRESULT = False result"; break;
        case DISP_E_EXCEPTION:
            qDebug() << "DISP_E_EXCEPTION";/* << pExcepInfo.bstrDescription
                 << pExcepInfo.bstrHelpFile
                 << pExcepInfo.bstrSource
                 << pExcepInfo.dwHelpContext
                 << pExcepInfo.pfnDeferredFillIn
                 << pExcepInfo.pvReserved
                 << pExcepInfo.scode
                 << pExcepInfo.wCode
                 << pExcepInfo.wReserved;*/ break;
        default:
            qDebug() << "default" << hr; break;
        }
    }

signals:
    void OnFeatureInfo(int);
    void OnImageReceived(bool&);
    void OnEnroll(bool,QVariant);
    void OnCapture(bool,QVariant);
    void OnCaptureToFile(bool);
    void OnEnrollToFile(bool);

    void OnFingerTouching();
    void OnFingerLeaving();

    void append(QString);

private slots:
    void slotException(int code, const QString & source, const QString & desc, const QString & help);


};

Q_DECLARE_METATYPE(VARIANT)

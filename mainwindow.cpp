#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAxWidget>
#include <QDebug>
#include <QMessageBox>
#include <windows.h>
#include <cstring>

CDemoDlg::CDemoDlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    enableButton(true);
    //ui->widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //ui->widget->setObjectName(QString::fromUtf8("IDC_ZKFPENGX2"));

    m_Used = "";
    m_SN = "";
    m_COUNT ="";
    m_Hint = "";
    m_Block = 1;
    m_WriteBlock = 1;
    m_fpPin = 1;
    m_Pwidth5 = false;
    s_zkfp = "";
    sRegTemplate = "";
    sRegTemplate10 = "";

    //h = (HWND)ui->widget->winId();
    //hdc = GetWindowDC(h);
    hh = (HWND)ui->widget_2->winId();
    _hdc = GetWindowDC(hh);

    //Сигналы от кнопок
    connect(ui->IDC_BTNInit,SIGNAL(clicked(bool)),this,SLOT(OnBTNInit()));
    connect(ui->IDC_BTNSave,SIGNAL(clicked(bool)),this,SLOT(OnBTNSave()));
    connect(ui->IDC_BTNReg,SIGNAL(clicked(bool)),this,SLOT(OnBTNReg()));
    connect(ui->IDC_BTNIdentify,SIGNAL(clicked(bool)),this,SLOT(OnBTNIdentify()));
    connect(ui->IDC_BTNVer,SIGNAL(clicked(bool)),this,SLOT(OnBTNVer()));
    connect(ui->IDC_BUTTON1,SIGNAL(clicked(bool)),this,SLOT(OnSetRFIDReaderAddr()));
    connect(ui->IDC_BUTTON2,SIGNAL(clicked(bool)),this,SLOT(OnGetCardSN()));
    connect(ui->IDC_BUTTON3,SIGNAL(clicked(bool)),this,SLOT(OnReadcard()));
    connect(ui->IDC_BUTTON4,SIGNAL(clicked(bool)),this,SLOT(OnWriteCard()));
    connect(ui->IDC_BUTTON5,SIGNAL(clicked(bool)),this,SLOT(OnTemplateToCard()));
    connect(ui->IDC_BUTTON6,SIGNAL(clicked(bool)),this,SLOT(OnButton6()));
    connect(ui->IDC_BTNRED,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnred()));
    connect(ui->IDC_BTNGREEN,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtngreen()));
    connect(ui->IDC_BTNBEEP,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnbeep()));
    connect(ui->IDC_BTNWRITEPWD,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnwritepwd()));
    connect(ui->IDC_BTNREADPWD,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnreadpwd()));
    connect(ui->IDC_RADIO9,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedRadio9()));
    connect(ui->IDC_RADIO10,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedRadio10()));
    connect(ui->IDC_BTNDisConnect,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtndisconnect()));
    connect(ui->IDC_BTNREGBYIMAGE,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnregbyimage()));
    connect(ui->IDC_BTNIDENBYIMAGE,SIGNAL(clicked(bool)),this,SLOT(OnBnClickedBtnidenbyimage()));

    connect(ui->test,SIGNAL(clicked(bool)),this,SLOT(test()));

    //События от устройства
    connect(&zkfpEng,SIGNAL(OnFeatureInfo(int)), SLOT(featureInfo(int)));
    connect(&zkfpEng,SIGNAL(OnImageReceived(bool&)), SLOT(imageReceived(bool&)));
    connect(&zkfpEng,SIGNAL(OnEnroll(bool,QVariant)), SLOT(enroll(bool,QVariant)));
    connect(&zkfpEng,SIGNAL(OnCapture(bool,QVariant)), SLOT(capture(bool,QVariant)));
    connect(&zkfpEng,SIGNAL(OnCaptureToFile(bool)), SLOT(captureToFile(bool)));
    connect(&zkfpEng,SIGNAL(OnEnrollToFile(bool)), SLOT(enrollToFile(bool)));

    connect(&zkfpEng, SIGNAL(OnFingerTouching()), SLOT(fingerTouching()));
    connect(&zkfpEng, SIGNAL(OnFingerLeaving()), SLOT(fingerLeaving()));

    connect(&zkfpEng, SIGNAL(append(QString)), ui->IDC_EDHint, SLOT(append(QString)));
}

void CDemoDlg::test()
{
    VARIANT q,s; WINBOOL qq,ss;
    zkfpEng.VerFinger(&q,s,qq,&ss);
}

void CDemoDlg::getFPImage()
{
    QImage img;
    zkfpEng.GetFingerImage(img);
    FPImage = img;
    //if(!img.save("D:/image.bmp"))
    //    qDebug() << "ERROR SAVE IMAGE";
}

CDemoDlg::~CDemoDlg()
{
    delete ui;
}

void CDemoDlg::OnBTNInit()
{
    int i;
    zkfpEng.put_SensorIndex(0);
    if (zkfpEng.InitEngine() == 0)
    {
        enableButton(false);
        if (ui->IDC_RADIO9->isChecked() != 0)
            zkfpEng.put_FPEngineVersion("9");
        else
            zkfpEng.put_FPEngineVersion("10");
        fpcHandle = zkfpEng.CreateFPCacheDBEx();
        m_SN = zkfpEng.get_SensorSN();
        i = zkfpEng.get_SensorIndex();
        m_Used = QString::number(i);
        i = zkfpEng.get_SensorCount();
        m_COUNT = QString::number(i);
        ui->IDC_EDCount->setText(QString(m_COUNT));
        ui->IDC_EDASN->setText(QString(m_SN));
        ui->IDC_EDUsed->setText(QString(m_Used));
        QMessageBox::information(0, "Initial Succeed", "Initial Succeed");
    }
    else
    {
        zkfpEng.EndEngine();
        QMessageBox::information(0, "Initial Failed", "Initial Failed");
    }
    FMatchType = 2;
}

void CDemoDlg::OnBTNSave()
{
    if (ui->IDC_RADIOBMP->isChecked() != 0)
        zkfpEng.SaveBitmap("fingerprint.bmp");
    else
        zkfpEng.SaveJPG("fingerprint.jpg");
    QMessageBox::information(0, "Fingerprint Image saved", "Fingerprint Image saved");
}

void CDemoDlg::OnBTNReg()
{
    zkfpEng.CancelEnroll();
    zkfpEng.put_EnrollCount(3);
    zkfpEng.BeginEnroll();

    ui->IDC_EDHint->append("start register");
}

void CDemoDlg::OnBTNIdentify()
{
    // TODO: Add your control notification handler code here
    if (zkfpEng.get_IsRegister())
        zkfpEng.CancelEnroll();
    ui->IDC_EDHint->append("Identify(1:N)");
    FMatchType = 2;
}

void CDemoDlg::OnBTNVer()
{
    // TODO: Add your control notification handler code here
    if (zkfpEng.get_IsRegister())
        zkfpEng.CancelEnroll();
    ui->IDC_EDHint->append("Verify(1:1)");
    FMatchType = 1;
}

void CDemoDlg::OnClose()
{
    zkfpEng.EndEngine();
    if(fpcHandle > 0)
    {
        zkfpEng.FreeFPCacheDBEx(fpcHandle);
    }
}

/**
Get Reader Address and Serial number
*/
QString CDemoDlg::GetSnr()
{
    BYTE buf[128];
    memset(buf,1,128);
    buf[1] = 0x00;

    if(zkfpEng.MF_GetSerNum(0, 0, buf))
    {
        QString str;
        str = QString("%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(buf[0],0,16)
                .arg(buf[1],0,16)
                .arg(buf[2],0,16)
                .arg(buf[3],0,16)
                .arg(buf[4],0,16)
                .arg(buf[5],0,16)
                .arg(buf[6],0,16)
                .arg(buf[7],0,16)
                .arg(buf[7],0,16);
        return str;
    }
    else
        return "";
}

void CDemoDlg::OnSetRFIDReaderAddr()
{
    ui->IDC_EDHint->append("Get Reader SN:" + GetSnr());
}

int CDemoDlg::GetCardByteSerial(BYTE  *pSerial)
{
    BYTE buf[128];
    BYTE snr[16];
    memset(buf,0,128);
    memset(snr,0,16);

    if(!zkfpEng.MF_GET_SNR(0, 0, 0x26, 0x00, snr, buf))
        return 1;
    memcpy(pSerial, buf, 4);
    return 0;
}

/*
Get card serial number
*/
QString CDemoDlg::GetCardStrSerial()
{
    QString strre;
    BYTE buf[128];
    unsigned long nSerial;

    if(GetCardByteSerial(buf) == 0)
    {
        nSerial = ((buf[0] * 256 + buf[1])*256 + buf[2])* 256 +buf[3];

        if(nSerial > 0x7f000000)
            strre = QString("%1%2%3%4%5%6%7%8%9")
                    .arg(buf[0],0,16)
                    .arg(buf[1],0,16)
                    .arg(buf[2],0,16)
                    .arg(buf[3],0,16);//,nSerial);
        else
            strre = QString("%1%2%3%4%5%6%7%8%9")
                    .arg(buf[0],0,16)
                    .arg(buf[1],0,16)
                    .arg(buf[2],0,16)
                    .arg(buf[3],0,16)
                    .arg(nSerial,0,16);


        return strre;
    }
    return "";
}

void CDemoDlg::OnGetCardSN()
{
    ui->IDC_EDHint->append("Get Card SN:" + GetCardStrSerial());
}

int CDemoDlg::ReadBLOCK(int blockIndex, BYTE *pData, unsigned char *key)
{
    BYTE dataBuf[1024];
    BYTE len;

    if(blockIndex<0 || blockIndex>63)// mifare s50card 0~63
        return 10;
    memset(dataBuf, 0, 1024);
    len = 1;

    if(!zkfpEng.MF_PCDRead(0 ,0, 0, blockIndex, len, key, dataBuf))
        return 1;
    memcpy(pData, dataBuf, 16);
    return 0;
}

int CDemoDlg::ReadBlock2(BYTE blockIndex, int mod, BYTE *pData, int blockLen)
{
    Q_UNUSED(mod);
    BYTE *buf;
    BYTE pKey[6];

    BYTE nAdd,BlockNum;
    int i;

    nAdd = blockIndex;
    buf = pData;

    for(i = 0;i< blockLen;)
    {
        // S50 CardЈє1024BytesЈ¬16 SectorsЈ¬each sector 4 blocks
        // S70 CardЈє4096BytesЈ¬40 SectorsЈ¬the first 32 secotrs have per4 blockЈ¬last 8 have per16 blocks.
        if(nAdd >= 64)
            return -1;

        if(nAdd == 0) // In first secotr, block 0 for card number... Only 2 block can use.
        {
            nAdd = 1;
            BlockNum = 2;
        }
        else if(nAdd<32 *4) // s50 card
        {
            if(nAdd%4 == 3)
                nAdd++;
            BlockNum = 3;
        }
        else if(nAdd> 32*4) // s70 card
        {
            if(nAdd%16 == 15)
                nAdd++;
            BlockNum = 3;
        }

        memset(pKey, 0xFF, 6); // As 0xffffffffffff for keyA

        if(!zkfpEng.MF_PCDRead(0, 0, 0, nAdd, BlockNum, pKey, buf))
            return 1;

        nAdd += BlockNum;
        buf = buf + 16*BlockNum;
        i += BlockNum;
    }
    return 0;
}

int CDemoDlg::WriteBLOCK(BYTE blockIndex, BYTE *pData, unsigned char *key)
{
    if(blockIndex<0 || blockIndex>64) // mifare s50card 0~63
        return 10;
    if(blockIndex%4 == 3)	// Note, it's password block
        return 11;

    if(!zkfpEng.MF_PCDWrite(0, 0, 0, blockIndex, 1, key, pData))
        return 1;
    return 0;
}

int CDemoDlg::WriteBlock2(BYTE blkIndex, int mod, BYTE *pData, int blockLen)
{
    Q_UNUSED(mod);
    BYTE *buf;
    BYTE pKey[6];

    BYTE nAdd, BlockNum;
    int i;

    nAdd = blkIndex;

    buf = pData;

    for(i = 0; i< blockLen;)
    {
        // S50 CardЈє1024BytesЈ¬16 SectorsЈ¬each sector 4 blocks
        // S70 CardЈє4096BytesЈ¬40 SectorsЈ¬the first 32 secotrs have per4 blockЈ¬last 8 have per16 blocks.
        if(nAdd >= 64)
            return -1;

        if(nAdd==0) // In first secotr, block 0 for card number... Only 2 block can use.
        {
            nAdd=1;
            BlockNum = 2;
        }
        else if(nAdd<32 *4) // s50 card
        {
            if(nAdd%4 == 3)
                nAdd++;
            BlockNum = 3;
        }
        else if(nAdd> 32*4) // s70 card
        {
            if(nAdd%16 == 15)
                nAdd++;
            BlockNum = 3;
        }

        memset(pKey, 0xff, 6); //0xffffffffffff as keyA

        if(!zkfpEng.MF_PCDWrite(0, 0, 0, nAdd, BlockNum, pKey, buf))
        {
                return 1;
        }
        nAdd += BlockNum;
        buf = buf + 16*BlockNum;
        i += BlockNum;
    }
    return 0;
}

void CDemoDlg::OnReadcard()
{
    QString temp;
    BYTE buf[256];
    BYTE key[6];
    int Result,Block;

    //UpdateData(TRUE);
    Block = m_Block;
    memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA
    Result =  ReadBLOCK(Block, buf, key);
    if(Result == 0)
        temp = QString("BLOCK = 0x%1, Data=0x %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17")
                .arg(Block,0,16)
                .arg(buf[0],0,16)
                .arg(buf[1],0,16)
                .arg(buf[2],0,16)
                .arg(buf[3],0,16)
                .arg(buf[4],0,16)
                .arg(buf[5],0,16)
                .arg(buf[6],0,16)
                .arg(buf[7],0,16)
                .arg(buf[8],0,16)
                .arg(buf[9],0,16)
                .arg(buf[10],0,16)
                .arg(buf[11],0,16)
                .arg(buf[12],0,16)
                .arg(buf[13],0,16)
                .arg(buf[14],0,16)
                .arg(buf[15],0,16);
    else
         temp = QString("Errorcode = %1").arg(Result);
    ui->IDC_EDHint->append("Read card:" + temp);
}

void CDemoDlg::OnWriteCard()
{
    QString temp;
    BYTE buf[16];
    BYTE key[6];
    int Result,Block;

    //UpdateData(TRUE);
    if(m_WriteBlock%4 ==3 || m_WriteBlock == 0)
    {
        QMessageBox::information(0, "parameter error!", "parameter error!");
        m_WriteBlock = 1;
        //UpdateData(FALSE);

    }
    Block = m_WriteBlock;
    memset(buf, 0x66, 6);
    // FF 07 80 69 as default
    buf[6] = 0xFF;
    buf[7] = 0x07;
    buf[8] = 0x80;
    buf[9] = 0x69;
    memset(&buf[10], 0xFF, 6);

    memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA
    Result =  WriteBLOCK(Block, buf, key);
    if(Result == 0)
        temp = QString("Block = 0x%1").arg(Block,0,16);
    else
        temp = QString("Errorcode = %1").arg(Result);
    ui->IDC_EDHint->append("Write card:" + temp);
}

void CDemoDlg::OnTemplateToCard()
{
        QString temp;
        int Result, DataLen, blockLen;
        BYTE buf[2000];
        BYTE buf2[2024];
        FILE *fp;

        //UpdateData(TRUE);
        // Get Reader Address and Serial number
        temp = GetSnr();
        if(temp == "")
        {
            QMessageBox::information(0, "cardmodule fault!", "cardmodule fault!");
            return;
        }

        // Get card serial number
        temp = GetCardStrSerial();
        if(temp == "")
        {
            QMessageBox::information(0, "no card or card error!", "no card or card error!");
            return;
        }

        // Save fingerprint.tpl to card
        DataLen = 0;
        if(!(fp = fopen("fingerprint.tpl", "rb")) || !FingerCount)
        {
             QMessageBox::information(0, "please register finger first!", "please register finger first!");
             return;
        }
        fseek(fp, 0, SEEK_SET);
        // A template is not more than 2000 Bytes
        DataLen =  fread( (void *)buf, sizeof(unsigned char), 2000, fp);
        fclose(fp);

        // Content structure
        // mark(2B) + UserID(2B/4B)+TemplateSize(2B)+FingerIndex[2B]+Template
        if(!m_Pwidth5) // 9 PinWidth
        {
            // mark
            buf2[0] = 'Z'+16;
            buf2[1] = 1;

            // UserID
            memcpy(&buf2[2],(void *)&m_fpPin, 4);

            // Template size
            buf2[6] = DataLen%256;
            buf2[7] = DataLen/256;

            // FingerIndex
            buf2[8] = 0x0f;
            buf2[9] = 0xff;

            // Template
            memcpy(&buf2[10], buf, DataLen);

            //ґУїй0їЄКјРґЖрЈ¬
            DataLen += 10;
            if(DataLen > 16*3*16 -16)	//s50їЁЧо¶а±Јґж16*3*16 -16ёцЧЦЅЪ
                DataLen = 16*3*16 -16;
        }
        else  // 5 PinWidth
        {
            // mark
            buf2[0] = 'Z'+16;
            buf2[1] = 1;

            // UserID
            memcpy(&buf2[2], (void *)&m_fpPin, 2);

            // Template size
            buf2[4] = DataLen%256;//4.5
            buf2[5] = DataLen/256;//

            // Fingerindex
            buf2[6] = 0x0f;
            buf2[7] = 0xff;

            // Template
            memcpy(&buf2[8], buf, DataLen);

            DataLen += 8;
            if(DataLen > 16*3*16 -16)
                DataLen = 16*3*16 -16;
        }

        blockLen = DataLen/16;
        if((DataLen % 16) > 0)
            blockLen += 1;
        Result = WriteBlock2(0, 0, buf2, blockLen);

        if(Result != 0)
        {
            QMessageBox::information(0, "write card error!", "write card error!");
        }
        else
        {
            QMessageBox::information(0, "write card sucess!", "write card sucess!");
        }
}



void CDemoDlg::OnButton6()
{
    int Result, DataLen;
    int Score;
    int ProcessedFPNumber;
    FILE *fp;

    BYTE buf[2000] = {0};
    BYTE buf2[2024] = {0};
    //UpdateData(TRUE);

    Result = ReadBlock2(0, 0, buf2, 608/16); // not more than 608 bytes

    if(Result != 0)
    {
        QMessageBox::information(0, "Err", "read card error,please put card and operate it once again!");
        return;
    }

    // Content structure
    // mark(2B) + UserID(2B/4B)+TemplateSize(2B)+FingerIndex[2B]+Template

    if(buf2[0] != 'Z'+16  && buf2[1] != 1)
    {
        QMessageBox::information(0, "Err", "not fingertemplate card");
        return;
    }

    if(!m_Pwidth5) // 9 PinWidth
    {
        // UserID
        memcpy((void *)&m_fpPin,&buf2[2], 4);

        // Templaet size
        DataLen = buf2[6] + buf2[7] *256;

        // fingerIndex.
        //buf2[8] ;
        //buf2[9] ;

        if(DataLen > 752-16) //s50 valid capacity not more than 752bytes(16*3*16-16)
        {
             QMessageBox::information(0, "Err", "Fingertemplate abnormal, its length is larger than 752 bytes");
             return;
        }
        memcpy(buf, &buf2[10], DataLen);
    }
    else // 5 PinWidth
    {
        // UserID
        memcpy((void *)&m_fpPin, &buf2[2], 2);

        // Template size
        DataLen = buf2[4] + buf2[5] *256;

        // fingerIndex
        //buf2[6] ;
        //buf2[7] ;

        if(DataLen > 752-16)
        {
             QMessageBox::information(0, "Err", "Fingertemplate abnormal, its length is larger than 752 bytes");
             return;
        }
        memcpy(buf, &buf2[8], DataLen);
    }

    // Save template to file
    if(!(fp = fopen("fingerprint2.tpl", "wb")))
    {
        QMessageBox::information(0, "Err", "write fingertemplate file fail!");
        return;
    }
    fseek(fp, 0, SEEK_SET);
    DataLen = fwrite( (void *)buf, sizeof(unsigned char), DataLen, fp);
    fclose(fp);

    Score = 8;
    ProcessedFPNumber = 1;

    if(FMatchType == 2)
    {
        Result = zkfpEng.IdentificationFromFileInFPCacheDB(fpcHandle, "fingerprint2.tpl", Score, ProcessedFPNumber);
        if(Result == -1)
        {
            QMessageBox::information(0, "Err", "identificate error!");
            return ;
        }
        else
        {
            QString str("Identify Succeed ID = %1 Score = %2  Processed Number = %3");
            str = str.arg(Result).arg(Score).arg(ProcessedFPNumber);
            QMessageBox::information(0, "Success", str);
        }
    }
    else
    {
        QString str("sucess USER PIN = %1 please press finger!");
        str = str.arg(m_fpPin);

        FMatchType = 1;
        QMessageBox::information(0, "Success", str);
        bPollCardOK = true;
    }
}


/*
ControlSensor(ACode As Long; AValue As Long)
If ACode=11, control the green light, if itЎЇs 12, control the red light, if itЎЇs 13,
control the beep.
If AValue=1, lights on, if itЎЇs 0, lights off.
*/
void CDemoDlg::OnBnClickedBtnred()
{
    zkfpEng.ControlSensor(12, 1);
    zkfpEng.ControlSensor(12, 0);
}

void CDemoDlg::OnBnClickedBtngreen()
{
    zkfpEng.ControlSensor(11, 1);
    zkfpEng.ControlSensor(11, 0);
}

void CDemoDlg::OnBnClickedBtnbeep()
{
    zkfpEng.ControlSensor(13, 1);
    zkfpEng.ControlSensor(13, 0);
}

void CDemoDlg::OnBnClickedBtnwritepwd()
{
    QString temp;
    BYTE buf[16];
    BYTE key[6];

    // keyA
    memset(buf, 0x66, 6);
    // FF 07 80 69 as default
    buf[6] = 0xFF;
    buf[7] = 0x07;
    buf[8] = 0x80;
    buf[9] = 0x69;

    // keyB
    memset(&buf[10], 0xFF, 6);

    memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA

    // Here we only modify the first sector's keyA
    if(zkfpEng.MF_PCDWrite(0, 0, 0, 3, 1, key, buf))
    {
        temp = QString("BLOCK = 0x%1, success ").arg(3,0,16);
    }
    else
        temp = QString("Errorcode = %1").arg(buf[0]);
    ui->IDC_EDHint->append("Write card:" +  temp);
}

void CDemoDlg::OnBnClickedBtnreadpwd()
{
    QString temp;
    BYTE buf[256];
    BYTE key[6];
    int Result,Block;

    //UpdateData(TRUE);
    Block = m_Block;

    memset(key, 0x66, 6); // 0xFFFFFFFFFFFF as keyA
    Result =  ReadBLOCK(Block, buf, key);
    if(Result == 0)
        temp = QString("BLOCK = 0x%1, Data=0x %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17")
                .arg(Block,0,16)
                .arg(buf[0],0,16)
                .arg(buf[1],0,16)
                .arg(buf[2],0,16)
                .arg(buf[3],0,16)
                .arg(buf[4],0,16)
                .arg(buf[5],0,16)
                .arg(buf[6],0,16)
                .arg(buf[7],0,16)
                .arg(buf[8],0,16)
                .arg(buf[9],0,16)
                .arg(buf[10],0,16)
                .arg(buf[11],0,16)
                .arg(buf[12],0,16)
                .arg(buf[13],0,16)
                .arg(buf[14],0,16)
                .arg(buf[15],0,16);
    else
        temp = QString("Errorcode = %1").arg(Result);
    ui->IDC_EDHint->append("Read card:" +  temp);
}

void CDemoDlg::OnBnClickedRadio9()
{
    zkfpEng.put_FPEngineVersion("9");
}

void CDemoDlg::OnBnClickedRadio10()
{
    zkfpEng.put_FPEngineVersion("10.0");
}

void CDemoDlg::enableButton(bool bEnable)
{
    ui->IDC_BTNDisConnect->setEnabled(!bEnable);
    ui->IDC_BTNInit->setEnabled(bEnable);
    ui->IDC_BTNSave->setEnabled(!bEnable);
    ui->IDC_BTNReg->setEnabled(!bEnable);
    ui->IDC_BTNVer->setEnabled(!bEnable);
    ui->IDC_BTNREGBYIMAGE->setEnabled(!bEnable);
    ui->IDC_BTNIdentify->setEnabled(!bEnable);
    ui->IDC_BTNIDENBYIMAGE->setEnabled(!bEnable);
    ui->IDC_BTNRED->setEnabled(!bEnable);
    ui->IDC_BTNGREEN->setEnabled(!bEnable);
    ui->IDC_BTNBEEP->setEnabled(!bEnable);
    ui->IDC_BUTTON5->setEnabled(!bEnable);
    ui->IDC_BUTTON6->setEnabled(!bEnable);
    ui->IDC_BUTTON1->setEnabled(!bEnable);
    ui->IDC_BUTTON2->setEnabled(!bEnable);
    ui->IDC_BUTTON3->setEnabled(!bEnable);
    ui->IDC_BTNREADPWD->setEnabled(!bEnable);
    ui->IDC_BUTTON4->setEnabled(!bEnable);
    ui->IDC_BTNWRITEPWD->setEnabled(!bEnable);
}


void CDemoDlg::OnBnClickedBtndisconnect()
{
    zkfpEng.EndEngine();
    enableButton(true);
}

void CDemoDlg::OnBnClickedBtnregbyimage()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открыть", "C:/Qtprojects/GoodHash");
    if(filename.isEmpty())
        return;

    if(zkfpEng.get_IsRegister())
        zkfpEng.CancelEnroll();
    zkfpEng.put_EnrollCount(1);
    zkfpEng.BeginEnroll();
    if(!zkfpEng.AddImageFile(filename, 500))
    {
    QMessageBox::information(0, "Err", "Extract failed or not using the standard version of ZKFinger SDK");
    }
}

void CDemoDlg::OnBnClickedBtnidenbyimage()
{
        QString filename = QFileDialog::getOpenFileName(this, "Открыть", "C:/Qtprojects/GoodHash");
        if(filename.isEmpty())
            return;
        if(zkfpEng.get_IsRegister())
            zkfpEng.CancelEnroll();

        zkfpEng.BeginCapture();

        if(!zkfpEng.AddImageFile(filename, 500))
        {
            QMessageBox::information(0, "Err", "Extract failed or not using the standard version of ZKFinger SDK");
        }
}

void CDemoDlg::featureInfo(int AQuality)
{
    QString sTemp;
    int i(-88);

    sTemp = "";
    if (zkfpEng.get_IsRegister())
    {
        i = zkfpEng.get_EnrollIndex();
        sTemp = "Register Status: still press finger ";
        sTemp = sTemp + QString::number(i) + " times";
    }

    sTemp = sTemp + " Fingerprint quality";
    i = zkfpEng.get_LastQuality();
    if (AQuality != 0)
        sTemp = sTemp + " not good, quality="  + QString::number(i);
    else
        sTemp = sTemp +  " good, quality=" + QString::number(i);
    ui->IDC_EDHint->append(sTemp);
}

void CDemoDlg::imageReceived(bool& AImageValid)
{
    Q_UNUSED(AImageValid);
    getFPImage();
    update();
    /*
    zkfpEng.PrintImageAt((int)hdc, 0, 0, zkfpEng.get_ImageWidth(), zkfpEng.get_ImageHeight());
    zkfpEng.PrintImageEllipseAt((int)_hdc, 0, 0, zkfpEng.get_ImageWidth(), zkfpEng.get_ImageHeight(), QColor(Qt::black));
    */
}

void CDemoDlg::enroll(bool ActionResult, QVariant ATemplate)
{
    QVariant pTemplate;
    Q_UNUSED(ATemplate);

    if (!ActionResult)
        QMessageBox::information(0,"Register Failed","Register Failed");
    else
    {
        // After enroll, you can get 9.0&10.0 template at the same time
        sRegTemplate = zkfpEng.GetTemplateAsStringEx("9");
        sRegTemplate10 = zkfpEng.GetTemplateAsStringEx("10");

        if(sRegTemplate.size() > 0)
        {
            if(sRegTemplate10.size() > 0)
                zkfpEng.AddRegTemplateStrToFPCacheDBEx(fpcHandle, FingerCount, sRegTemplate, sRegTemplate10);
            else
                QMessageBox::information(0,"Register Failed","Register 10.0 failed, template length is zero");

            pTemplate = zkfpEng.DecodeTemplate1(sRegTemplate);

            // Note: 10.0 Template can not be compressed
            zkfpEng.SetTemplateLen(pTemplate, 602);
            zkfpEng.SaveTemplate("fingerprint.tpl", pTemplate);

            FingerCount = FingerCount + 1;

            QMessageBox::information(0,"Register Succeed", "Register Succeed");
        }
        else
        {
            QMessageBox::information(0,"Register Failed, template length is zero", "Register Failed, template length is zero");
        }
  }
}

void CDemoDlg::capture(bool ActionResult, QVariant ATemplate)
{
    Q_UNUSED(ActionResult);
    int id;
    int Score, ProcessNum;
    QString sTemp;
    ui->IDC_EDHint->append("Acquired Fingerprint template");

    if (FMatchType == 1)
    {
        if(bPollCardOK)
        {
            bool ARegFeatureChanged;
            QString tmp("fingerprint2.tpl");
            if(zkfpEng.VerRegFingerFile(tmp, ATemplate, false, ARegFeatureChanged))
            {
                ui->IDC_EDHint->append("Verify Succeed(from Mifare Card)!");
            }
            else
            {
                ui->IDC_EDHint->append("Verify Failed");
            }
            bPollCardOK = false;
        }
        else
        {
            sTemp = zkfpEng.GetTemplateAsString();
            QString bTemp;
            if (ui->IDC_RADIO9->isChecked() != 0)
            {
                bTemp = sRegTemplate;
            }
            else
            {
                bTemp = sRegTemplate10;
            }
            bool flag;
            if (zkfpEng.VerFingerFromStr(bTemp, sTemp, false, flag))
            {
                ui->IDC_EDHint->append("Verify Succeed(from fingerprint reader)");
            }
            else
            {
                ui->IDC_EDHint->append("Verify Failed");
            }
        }
    }
    else if (FMatchType == 2) // 1:N
    {
        Score = 8;
        id = zkfpEng.IdentificationInFPCacheDB(fpcHandle, ATemplate, Score, ProcessNum);
        if (id == -1)
        {
            ui->IDC_EDHint->append("Identify Failed");
        }
        else
        {
            QString buf("Identify Succeed ID = %1 Score = %2  Processed Number = %3");
            ui->IDC_EDHint->append(buf.arg(QString::number(id)).arg(QString::number(Score)).arg(QString::number(ProcessNum)));
        }
    }
}

void CDemoDlg::captureToFile(bool ActionResult){Q_UNUSED(ActionResult);}

void CDemoDlg::enrollToFile(bool ActionResult){Q_UNUSED(ActionResult);}

void CDemoDlg::fingerTouching(){}

void CDemoDlg::fingerLeaving(){}


void CDemoDlg::paintEvent(QPaintEvent * event)
{
    QMainWindow::paintEvent(event);
    if(!FPImage.isNull())
    {
        QPainter painter(this);
        painter.drawImage(369,29,FPImage);
        painter.end();
    }
}

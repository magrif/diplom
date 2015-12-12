#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CZKFPEngX.h>
#include <QFileDialog>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class CDemoDlg: public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QImage FPImage;
    HWND h, hh;
    HDC hdc, _hdc;

public:
        CDemoDlg(QWidget *pParent = NULL);	// standard constructor
        ~CDemoDlg();

        long FingerCount;
        long fpcHandle;

        long FMatchType;

        QString s_zkfp;
        QString sRegTemplate;
        QString sRegTemplate10;

        bool bPollCardOK;

        CZKFPEngX zkfpEng;
        QString	m_Used;
        QString	m_SN;
        QString	m_COUNT;
        QString	m_Regname;
        QString	m_Hint;
        int		m_Block;
        int		m_WriteBlock;
        long	m_fpPin;
        bool	m_Pwidth5;

protected:
        int GetCardByteSerial(BYTE *pSerial);
        int ReadBLOCK(int blockIndex, BYTE *pData, unsigned char *key);
        int ReadBlock2(BYTE blockIndex, int mod, BYTE *pData, int blockLen);
        int WriteBLOCK(BYTE blockIndex, BYTE *pData, unsigned char *key);
        int WriteBlock2(BYTE blkIndex, int mod, BYTE *pData, int blockLen);
        QString GetSnr();
        QString GetCardStrSerial();

        void enableButton(bool bEnable);

        void paintEvent(QPaintEvent * event);

protected slots:
        void getFPImage();
        void test();

        void OnBTNInit();
        void OnBTNSave();
        void OnBTNReg();
        void OnBTNIdentify();
        void OnBTNVer();
        void OnClose();
        void OnSetRFIDReaderAddr();
        void OnGetCardSN();
        void OnReadcard();
        void OnWriteCard();
        void OnTemplateToCard();
        void OnButton6();
        void OnBnClickedBtnred();
        void OnBnClickedBtngreen();
        //void OnBnClickedButton9();
        void OnBnClickedBtnbeep();
        //void OnBnClickedBtnpwd();
        void OnBnClickedBtnwritepwd();
        void OnBnClickedBtnreadpwd();
        void OnBnClickedRadio9();
        void OnBnClickedRadio10();
        void OnBnClickedBtndisconnect();
        void OnBnClickedBtnregbyimage();
        void OnBnClickedBtnidenbyimage();

        //Для обработки событий от устройства
        void featureInfo(int);
        void imageReceived(bool&);
        void enroll(bool,QVariant);
        void capture(bool,QVariant);
        void captureToFile(bool);
        void enrollToFile(bool);

        void fingerTouching();
        void fingerLeaving();
};

#endif // MAINWINDOW_H

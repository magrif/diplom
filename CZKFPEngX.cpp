#include "CZKFPEngX.h"


void CZKFPEngX::slotException(int code, const QString & source, const QString & desc, const QString & help)
{
    qDebug() << "*****Exception*****";
    qDebug() << code;
    qDebug() << source;
    qDebug() << desc;
    qDebug() << help;
    qDebug() << "*******************";
}

void callback(int ev, const char* reader, VrBio_BiometricImage* data, const void* userData)
{
    qDebug() << "callback";
    Q_UNUSED(userData);
    void* consolidationContext = NULL;
    if(ev & VRBIO_CAPTURE_EVENT_PLUG)
    {
        qDebug() << "Fingerprint reader plugged: " << reader;
        VStartReader(reader);
    }
    else if(ev & VRBIO_CAPTURE_EVENT_UNPLUG)
    {
        qDebug() << "Fingerprint reader unplugged: " << reader;
    }
    else if(ev & VRBIO_CAPTURE_EVENT_PLACED)
    {
        qDebug() << "Finger placed on: " << reader;
    }
    else if(ev & VRBIO_CAPTURE_EVENT_REMOVED)
    {
        qDebug() << "Finger removed from: " << reader;
    }
    else if(ev & VRBIO_CAPTURE_EVENT_IMAGE_CAPTURED)
    {
        qDebug() << "Image captured on: " << reader;
        qDebug() << "Image witdh: " << data->width;
        qDebug() << "Image height: " << data->height;
        qDebug() << "Image resolution(DPI): " << data->resolution;

        if(consolidationContext != NULL)
        {
            int ret = VMergeImage(consolidationContext,(const VrBio_BiometricImage**)&data);
            if(ret != VRBIO_SUCCESS)
            {
                qDebug() << "Error VMergeImage: " << ret;
                return;
            }
            qDebug() << "Image merged";

        }
    }

}

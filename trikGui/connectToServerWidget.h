#pragma once

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include <QtGui/QWidget>
    #include <QtGui/QLabel>
    #include <QtGui/QVBoxLayout>
#else
    #include <QtWidgets/QWidget>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QVBoxLayout>
#endif

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QEventLoop>

#include "trikGuiDialog.h"

#include <QTcpSocket>
#include <QAbstractSocket>

#include <trikWeb/robotManager.h>
#include "controller.h"


namespace trikGui {

/// Widget showing parameters of wireless access point on the controller.
class ConnectToServerWidget : public TrikGuiDialog
{
    Q_OBJECT

public:
    /// Constructor
    /// @param parent - parent of this widget in Qt object hierarchy.
    explicit ConnectToServerWidget(Controller &controller, QWidget *parent = 0);

    void renewFocus() override;

    void doConnect();

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    Controller &mController;


    QVBoxLayout mMainLayout;
    QVBoxLayout mParametersLayout;
    QLabel mConnectionStatus;
    QLabel mMessage;
    QLabel mMessage2;

};

}

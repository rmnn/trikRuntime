#include "connectToServerWidget.h"


#include <QtCore/QList>

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>

#include "rcReader.h"
#include <trikWeb/robotJsonFactory.h>
#include <trikWeb/json.h>


using namespace trikGui;

ConnectToServerWidget::ConnectToServerWidget(QWidget *parent)
    : TrikGuiDialog(parent)
{

    mConnectionStatus.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mParametersLayout.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mMessage.setAlignment(Qt::AlignCenter);
    mMessage.setWordWrap(true);
    mConnectionStatus.setWordWrap(true);

    mConnectionStatus.setText(tr("Connection to server.."));

    mMainLayout.addWidget(&mConnectionStatus);
    mMainLayout.addLayout(&mParametersLayout);

    mParametersLayout.addWidget(&mMessage);
    setLayout(&mMainLayout);

    doConnect();
}

void ConnectToServerWidget::renewFocus()
{
    setFocus();
}


void ConnectToServerWidget::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    socket->connectToHost("192.168.1.20", 9002);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        mConnectionStatus.setText("Could not connected to server");
        mMessage.setText(socket->errorString());
    }
}

void ConnectToServerWidget::connected()
{
    trikWeb::RobotJsonFactory robotJsonFactory;

    mConnectionStatus.setText("Connected");
    mMessage.setText("Waiting for messages");

    socket->write(robotJsonFactory.createRobotJson() + "\n");
   // socket->write("{\"from\": \"Robot\", \"type\": \"connect\", \"robot\": {\"ssid\": \"ssid2\", \"modelConfig\": \"<config><initScript></initScript><E1><angularServomotor/></E1>    <E2><angularServomotor invert=\\\"false\\\"/></E2></config>\", \"systemConfig\": \"<config><deviceClasses><servoMotor period=\\\"20000000\\\" invert=\\\"false\\\"/></deviceClasses><devicePorts><servoMotor port=\\\"E1\\\"/><servoMotor port=\\\"E2\\\"/></devicePorts><deviceTypes><angularServomotor class=\\\"servoMotor\\\" min=\\\"600000\\\" max=\\\"2200000\\\" zero=\\\"1400000\\\" stop=\\\"0\\\" type=\\\"angular\\\"/><continuousRotationServomotor class=\\\"servoMotor\\\" min=\\\"600000\\\" max=\\\"2200000\\\" zero=\\\"1400000\\\" stop=\\\"0\\\" type=\\\"continuousRotation\\\"/></deviceTypes></config>\"}}\n");
}

void ConnectToServerWidget::disconnected()
{
    mConnectionStatus.setText("Disconnected");
}

void ConnectToServerWidget::bytesWritten(qint64 bytes)
{

}

void ConnectToServerWidget::readyRead()
{
    QString message = socket->readAll();
    if (message.contains("type")) {
        message.remove("HeartBeat");
        if (message.contains("sendDiagram")) {
            mMessage.setText("received diagram");
        } else {
            mMessage.setText("received model-config");
        }
//        bool ok = false;
//        QtJson::JsonObject result = QtJson::parse(message, ok).toMap();
//         if (!ok) {
//            qFatal("An error occurred during parsing");
//            return;
//         }
//         mMessage.setText(message);

    }
}

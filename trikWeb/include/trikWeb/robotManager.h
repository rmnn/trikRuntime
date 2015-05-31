#pragma once

#include "qbytearray.h"
#include "QObject"


namespace trikWeb {

class RobotManager: public QObject
{
    Q_OBJECT

public:
    RobotManager();

    QByteArray createJson();
    QString proccessMessage(QString const &message);
    QString getProgramm(QString const &message);

private:
    QString updateModelConfig(QString const &modelConfig);

};

}


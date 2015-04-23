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

private:
    void updateModelConfig(QString const &modelConfig);
    void runScript(QString const &script);

};

}


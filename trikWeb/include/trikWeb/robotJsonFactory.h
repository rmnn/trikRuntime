#pragma once

#include "qbytearray.h"
#include "QObject"


namespace trikWeb {

class RobotJsonFactory: public QObject
{
    Q_OBJECT

public:
    RobotJsonFactory();

    QByteArray createRobotJson();
};

}


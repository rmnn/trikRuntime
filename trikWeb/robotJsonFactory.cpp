#include <include/trikWeb/robotJsonFactory.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "json.h"
#include "rcReader.h"


using QtJson::JsonObject;
using QtJson::JsonArray;
using namespace trikWeb;

RobotJsonFactory::RobotJsonFactory()
{  
}

QByteArray RobotJsonFactory::createRobotJson()
{
    RcReader rcReader("/etc/trik/trikrc");
    RcReader modelConfigReader("/home/root/trik/model-config.xml");
    RcReader systemConfigReader("/home/root/trik/system-config.xml");


    QString ssid = rcReader.value("trik_wifi_ap_ssid");


    QtJson::JsonObject message;

    message["from"] = "Robot";
    message["type"] = "connect";

    QtJson::JsonObject robot;
    robot["ssid"] = ssid;
    robot["systemConfig"] = "";
    robot["modelConfig"] = modelConfigReader.readXmlFile();
    robot["systemConfig"] = systemConfigReader.readXmlFile();

    message["robot"] = robot;

    return QtJson::serialize(message);
}

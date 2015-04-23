#include <include/trikWeb/robotManager.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "json.h"
#include "rcReader.h"


using QtJson::JsonObject;
using QtJson::JsonArray;
using namespace trikWeb;

RobotManager::RobotManager()
{  
}

QString RobotManager::proccessMessage(QString const &message) {
    bool ok = false;
    JsonArray messages = QtJson::parse(message, ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        return "An error occurred during parsing";
    }

    QString result = "";
    foreach(QVariant msgJson, messages) {
        QtJson::JsonObject msg =  msgJson.toMap();
        if (msg["type"] == "sendDiagram") {
            runScript(msg["text"].toString());
            return "Received diagram. Running..";
        }
        if (msg["type"] == "sendModelConfig") {
            updateModelConfig(msg["text"].toString());
            return "Received new model-config. Updating..";
        }
        return "Unknown type of message.";
       }

    return result;
}


QByteArray RobotManager::createJson()
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

void RobotManager::runScript(QString const &script) {

}

void RobotManager::updateModelConfig(QString const &modelConfig) {

}


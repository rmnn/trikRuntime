#include <include/trikWeb/robotManager.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QIODevice>

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
            return "run";
        }
        if (msg["type"] == "sendModelConfig") {
           return updateModelConfig(msg["text"].toString());
           // return "Received new model-config. Updated";
        }
        return "Unknown type of message.";
       }

    return result;
}


QString RobotManager::getProgramm(QString const &message) {
    bool ok = false;
    JsonArray messages = QtJson::parse(message, ok).toList();
    if (!ok) {
        qFatal("An error occurred during parsing");
        return "An error QIODevice occurred during parsing";
    }

    QString result = "";
    foreach(QVariant msgJson, messages) {
        QtJson::JsonObject msg =  msgJson.toMap();
        if (msg["type"] == "sendDiagram") {
            return msg["text"].toString();
        }
        return "Unknown";
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

QString RobotManager::updateModelConfig(QString const &modelConfig) {
    QFile file ("/home/root/trik/model-config.xml");
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
       QTextStream stream(&file);
       stream << modelConfig << endl;
       return "Received new model-config. Reboot for apply.";
    } else {
        return "Failed to update model-config. Try again.";
    }
}


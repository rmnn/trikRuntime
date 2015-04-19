/* Copyright 2014 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "webClientWidget.h"

#include <QtGui/QKeyEvent>
#include <QtCore/QDebug>

#include "showSsidWidget.h"
#include "connectToServerWidget.h"
#include "wiFiClientWidget.h"
#include "wiFiAPWidget.h"
#include "wiFiInitWidget.h"

#include <QsLog.h>

using namespace trikGui;

WebClientWidget::WebClientWidget(QString const &configPath
        , QWidget *parent)
    : TrikGuiDialog(parent)
    , mConfigPath(configPath)
    , mRcReader("/etc/trik/trikrc")
    , mTitle(tr("Choose mode:"))
{
    mModes.addItem(tr("Show Robot SSID"));
    mModes.addItem(tr("Connect to server"));

    mLayout.addWidget(&mTitle);
    mLayout.addWidget(&mModes);
    setLayout(&mLayout);

    mModes.selectionModel()->select(
            mModes.model()->index(0, 0)
            , QItemSelectionModel::ClearAndSelect
            );
}

QString WebClientWidget::menuEntry()
{
    return tr("Robot Market");
}

void WebClientWidget::renewFocus()
{
    mModes.setFocus();
}

void WebClientWidget::keyPressEvent(QKeyEvent *event)
{

    int returnValue = 1;

    switch (event->key()) {
        case Qt::Key_Return: {
            if (mModes.currentItem()->text() == tr("Show Robot SSID")) {
                ShowSsidWidget showSsidWidget;
                emit newWidget(showSsidWidget);
                returnValue = showSsidWidget.exec();
                break;
            } else if (mModes.currentItem()->text() == tr("Connect to server")) {
                ConnectToServerWidget connectToServerWidget;
                emit newWidget(connectToServerWidget);
                returnValue = connectToServerWidget.exec();

            }
            break;
        }
        default: {
            TrikGuiDialog::keyPressEvent(event);
            break;
        }
    }

    if (returnValue == 1) {
        goHome();
    }

}

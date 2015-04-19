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

#include "showSsidWidget.h"

#include <QtCore/QDebug>
#include <QtCore/QList>



#include "rcReader.h"

using namespace trikGui;

ShowSsidWidget::ShowSsidWidget(QWidget *parent)
    : TrikGuiDialog(parent)
{
    mTitle.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mParametersLayout.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mNetworkLabel.setAlignment(Qt::AlignCenter);

    mTitle.setText(tr("Information"));
    mNetworkLabel.setText(tr("Name not found"));

    getParameters();

    mMainLayout.addWidget(&mTitle);
    mMainLayout.addLayout(&mParametersLayout);

    mParametersLayout.addWidget(&mNetworkLabel);
    setLayout(&mMainLayout);
}

void ShowSsidWidget::renewFocus()
{
    setFocus();
}

void ShowSsidWidget::getParameters()
{
    RcReader const rcReader("/etc/trik/trikrc");

    QString const ssid = rcReader.value("trik_wifi_ap_ssid");
    if (!ssid.isEmpty()) {
        mNetworkLabel.setText(tr("Robot SSID: ") + ssid);
    }
}

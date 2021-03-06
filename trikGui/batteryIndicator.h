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

#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QTimer>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QtGui/QLabel>
#else
	#include <QtWidgets/QLabel>
#endif

#include <trikControl/brickInterface.h>

namespace trikGui {

/// Label showing battery voltage.
class BatteryIndicator : public QLabel
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param brick - object that provides interface to a hardware.
	/// @param parent - parent of this widget in terms of Qt parent-child widget relations.
	explicit BatteryIndicator(trikControl::BrickInterface &brick, QWidget *parent = 0);

private slots:
	void renew();

private:
	trikControl::BrickInterface &mBrick;
	QTimer mRenewTimer;
	int const mRenewInterval = 1000;
};

}

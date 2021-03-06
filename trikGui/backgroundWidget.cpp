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

#include "backgroundWidget.h"

#include <QtCore/QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QtGui/QApplication>
#else
	#include <QtWidgets/QApplication>
#endif

using namespace trikGui;

BackgroundWidget::BackgroundWidget(QString const &configPath
		, QString const &startDirPath
		, QWidget *parent)
	: QWidget(parent)
	, mController(configPath, startDirPath)
	, mBatteryIndicator(mController.brick())
	, mStartWidget(mController, configPath)
	, mRunningWidget(mController)
{
	setWindowState(Qt::WindowFullScreen);

	mMainLayout.setSpacing(10);

	mBatteryIndicator.setStyleSheet("font: 12px");

	mStatusBarLayout.addWidget(&mBatteryIndicator);
	addMainWidget(mStartWidget);
	addLazyWidget(mController.brick().graphicsWidget());
	mMainWidgetsLayout.addWidget(&mRunningWidget);

	mMainLayout.addLayout(&mStatusBarLayout);
	mMainLayout.addLayout(&mMainWidgetsLayout);

	setLayout(&mMainLayout);

	connect(&mMainWidgetsLayout, SIGNAL(currentChanged(int)), this, SLOT(renewFocus()));
	connect(&mMainWidgetsLayout, SIGNAL(widgetRemoved(int)), this, SLOT(updateStack(int)));

	connect(&mController, SIGNAL(brickStopped()), this, SLOT(refresh()));
	connect(&mController, SIGNAL(showRunningWidget(QString, int)), this, SLOT(showRunningWidget(QString, int)));
	connect(&mController, SIGNAL(showError(QString, int)), this, SLOT(showError(QString, int)));
	connect(&mController, SIGNAL(hideRunningWidget(int)), this, SLOT(hideRunningWidget(int)));
	connect(&mController, SIGNAL(hideGraphicsWidget()), this, SLOT(hideGraphicsWidget()));
	connect(&mController, SIGNAL(hideScriptWidgets()), this, SLOT(hideScriptWidgets()));

	connect(&mRunningWidget, SIGNAL(hideMe(int)), this, SLOT(hideRunningWidget(int)));
}

void BackgroundWidget::resetWidgetLayout(trikKernel::MainWidget &widget)
{
	// If the widget has layout, remove its margins because main widgets layout has its own margins.
	QLayout *layout = widget.layout();
	if (layout != nullptr) {
		layout->setContentsMargins(0, 0, 0, 0);
	}
}

void BackgroundWidget::addMainWidget(trikKernel::MainWidget &widget)
{
	resetWidgetLayout(widget);

	mMainWidgetIndex.push(mMainWidgetsLayout.addWidget(&widget));
	mMainWidgetsLayout.setCurrentIndex(mMainWidgetIndex.top());

	connect(&widget, SIGNAL(newWidget(trikKernel::MainWidget &)), this, SLOT(addMainWidget(trikKernel::MainWidget &)));
}

void BackgroundWidget::addRunningWidget(trikKernel::MainWidget &widget)
{
	resetWidgetLayout(widget);
	mMainWidgetsLayout.addWidget(&widget);
}

void BackgroundWidget::addLazyWidget(trikKernel::LazyMainWidget &widget)
{
	resetWidgetLayout(widget);
	mMainWidgetsLayout.addWidget(&widget);

	connect(&widget, SIGNAL(showMe(trikKernel::MainWidget &)), this, SLOT(showMainWidget(trikKernel::MainWidget &)));
	connect(&widget, SIGNAL(hideMe()), this, SLOT(hideGraphicsWidget()));
}

void BackgroundWidget::showMainWidget(trikKernel::MainWidget &widget)
{
	int const index = mMainWidgetsLayout.indexOf(&widget);
	if (index >= 0) {
		mMainWidgetsLayout.setCurrentIndex(index);
	}
}

void BackgroundWidget::showRunningWidget(const QString &fileName, int scriptId)
{
	mRunningWidget.setProgram(fileName, scriptId);
	mMainWidgetsLayout.setCurrentWidget(&mRunningWidget);
	mRunningWidget.grabKeyboard();
}

void BackgroundWidget::hideRunningWidget(int scriptId)
{
	if (mRunningWidget.scriptId() == scriptId) {
		mMainWidgetsLayout.setCurrentIndex(mMainWidgetIndex.top());
		mRunningWidget.releaseKeyboard();
	}
}

void BackgroundWidget::showError(const QString &error, int scriptId)
{
	if (mRunningWidget.scriptId() == scriptId) {
		mRunningWidget.showError(error, scriptId);
		mMainWidgetsLayout.setCurrentWidget(&mRunningWidget);
	}
}

void BackgroundWidget::hideGraphicsWidget()
{
	if (mMainWidgetsLayout.currentWidget() == &mController.brick().graphicsWidget()) {
		mMainWidgetsLayout.setCurrentWidget(&mRunningWidget);
	}
}

void BackgroundWidget::hideScriptWidgets()
{
	mMainWidgetsLayout.setCurrentIndex(mMainWidgetIndex.top());
	mRunningWidget.releaseKeyboard();
}

void BackgroundWidget::renewFocus()
{
	// When current widget in main widgets layout changed, we should set focus properly.

	trikKernel::MainWidget *currentWidget = dynamic_cast<trikKernel::MainWidget *>(mMainWidgetsLayout.currentWidget());

	if (currentWidget != nullptr) {
		currentWidget->renewFocus();
	}
}

void BackgroundWidget::refresh()
{
	for (auto const widget : QApplication::allWidgets()) {
		widget->update();
	}
}

void BackgroundWidget::updateStack(int removedWidget)
{
	if (mMainWidgetIndex.top() == removedWidget) {
		mMainWidgetIndex.pop();
		mMainWidgetsLayout.setCurrentIndex(mMainWidgetIndex.top());
	}
}

#pragma once

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include <QtGui/QWidget>
    #include <QtGui/QLabel>
    #include <QtGui/QVBoxLayout>
#else
    #include <QtWidgets/QWidget>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QVBoxLayout>
#endif

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QEventLoop>

#include "trikGuiDialog.h"

namespace trikGui {

/// Widget showing parameters of wireless access point on the controller.
class ShowSsidWidget : public TrikGuiDialog
{
    Q_OBJECT

public:
    /// Constructor
    /// @param parent - parent of this widget in Qt object hierarchy.
    explicit ShowSsidWidget(QWidget *parent = 0);

    void renewFocus() override;

private:
    void getParameters();

    QVBoxLayout mMainLayout;
    QVBoxLayout mParametersLayout;
    QLabel mTitle;
    QLabel mNetworkLabel;
    QLabel mKeyLabel;
    QLabel mIpLabel;

};

}

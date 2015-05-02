#pragma once

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include <QtGui/QWidget>
    #include <QtGui/QLabel>
    #include <QtGui/QVBoxLayout>
    #include <QtGui/QListWidget>
#else
    #include <QtWidgets/QWidget>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QVBoxLayout>
    #include <QtWidgets/QListWidget>
#endif

#include <QtCore/QString>

#include "controller.h"
#include "rcReader.h"
#include "trikGuiDialog.h"

namespace trikGui {

/// Widget which allows to set wi-fi mode (client or access point) and then opens corresponding configuration widget.
class WebClientWidget : public TrikGuiDialog
{
    Q_OBJECT

public:

    /// Constructor
    /// @param configPath - full path to configuration files.
    /// @param parent - parent of this widget in Qt object hierarchy.
    WebClientWidget(Controller &controller, QString const &configPath, QWidget *parent = 0);

    /// Returns menu entry for this widget.
    static QString menuEntry();

    void renewFocus() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString const &mConfigPath;
    RcReader mRcReader;
    QEventLoop mEventLoop;
    QVBoxLayout mLayout;
    QLabel mTitle;
    Controller &mController;
    QListWidget mModes;
};

}

#pragma once

#include <QWidget>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include "ui_qtmonkey_gui.h"

class QtMonkeyAppCtrl
#ifndef Q_MOC_RUN
    final
#endif
    : public QObject
{
    Q_OBJECT
signals:
    void monkeyAppFinishedSignal(QString msg);
    void monkeyAppNewEvent(const QString &scriptLine);
    void monkeyUserAppError(const QString &errMsg);
    void monkeyScriptEnd();
    void monkeScriptLog(const QString &);
    void criticalError(const QString &);
public:
    explicit QtMonkeyAppCtrl(const QString &appPath, const QStringList &appArgs,
                             QObject *parent = nullptr);
    void runScript(const QString &script, const QString &scriptFilename = QString());
private slots:
    void monkeyAppError(QProcess::ProcessError);
    void monkeyAppFinished(int, QProcess::ExitStatus);
    void monkeyAppNewOutput();
    void monkeyAppNewErrOutput();

private:
    QProcess qtmonkeyApp_;
    QByteArray jsonFromMonkey_;
};

class QtMonkeyWindow
#ifndef Q_MOC_RUN
    final
#endif
    : public QWidget,
      private Ui::QtMonkeyMainWin
{
    Q_OBJECT
public:
    QtMonkeyWindow(QWidget *parent = nullptr);
private slots:
    //auto connection
    void on_pbStartRecording__pressed();
    void on_leTestApp__textEdited(const QString &text);
    void on_leTestAppArgs__textEdited(const QString &text);
    void on_pbBrowse__pressed();
    void on_pbRunScript__pressed();

    //manual connection
    void onMonkeyAppFinishedSignal(QString);
    void savePrefs();
    void onMonkeyAppNewEvent(const QString &scriptLine);
    void onMonkeyUserAppError(const QString &);
    void onMonkeyScriptEnd();
    void onMonkeScriptLog(const QString &);
    void showError(const QString &msg);
private:
    enum class State {
        DoNothing,
		RecordEvents,
		PlayingEvents,
    };

    QtMonkeyAppCtrl *monkeyCtrl_ = nullptr;
    QTimer savePrefsTimer_;
    State state_ = State::DoNothing;

    QtMonkeyAppCtrl *getMonkeyCtrl();    
    void loadPrefs();
    void scheduleSave();
    void changeState(State val);
    void logNewLine(QtMsgType, const QString &);
};
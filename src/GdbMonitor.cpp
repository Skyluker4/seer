#include "GdbMonitor.h"
#include <QtCore/QtCore>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>
#include <iostream>

GdbMonitor::GdbMonitor (QObject* parent) : QObject(parent) {
    _process = 0;
}

GdbMonitor::~GdbMonitor () {
}

void GdbMonitor::handleErrorOccurred (QProcess::ProcessError error) {

    Q_UNUSED(error);

    //qDebug() << __PRETTY_FUNCTION__ << ":" << error;

    qApp->exit();
}

void GdbMonitor::handleFinished (int exitCode, QProcess::ExitStatus exitStatus) {

    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    //qDebug() << __PRETTY_FUNCTION__ << ":" << exitCode << exitStatus;

    qApp->exit();
}

void GdbMonitor::handleReadyReadStandardError () {

    //qDebug() << __PRETTY_FUNCTION__ << ":" << "Ready to read stderr";

    QProcess* p = (QProcess*)sender();

    // Read a line at a time.
    while (p->canReadLine()) {

        QByteArray buf = p->readLine();

        // Chop off trailing RETURN character.
        if (buf[buf.size()-1] == '\n') {
            buf.chop(1);
        }

        // Convert to a string.
        QString text(buf);

        // Start broadcasting it around.
        emit allTextOutput(text);
    }
}

void GdbMonitor::handleReadyReadStandardOutput () {

    //qDebug() << __PRETTY_FUNCTION__ << ":" << "Ready to read stdout";

    QProcess* p = (QProcess*)sender();

    // Read a line at a time.
    while (p->canReadLine()) {

        QByteArray buf = p->readLine();

        // Chop off trailing RETURN character.
        if (buf[buf.size()-1] == '\n') {
            buf.chop(1);
        }

        // qDebug() << __PRETTY_FUNCTION__ << ":" << "Read buffer" << buf.size() << (int)buf[buf.size()-1];

        // Convert to a string.
        QString text(buf);

        // Start broadcasting it around.
        emit allTextOutput(text);

        if (text[0] == '~') {
            emit tildeTextOutput(text);
        }else if (text[0] == '=') {
            emit equalTextOutput(text);
        }else if (text[0] == '*') {
            emit astrixTextOutput(text);
        }else if (text[0] == '^') {
            emit caretTextOutput(text);
        }else if (text[0] == '&') {
            emit ampersandTextOutput(text);
        }else if (text.contains(QRegExp("^([0-9]+)\\~"))) {
            emit tildeTextOutput(text);
        }else if (text.contains(QRegExp("^([0-9]+)\\="))) {
            emit equalTextOutput(text);
        }else if (text.contains(QRegExp("^([0-9]+)\\*"))) {
            emit astrixTextOutput(text);
        }else if (text.contains(QRegExp("^([0-9]+)\\^"))) {
            emit caretTextOutput(text);
        }else if (text.contains(QRegExp("^([0-9]+)\\&"))) {
            emit ampersandTextOutput(text);
        }else{
            emit textOutput(text);
        }
    }

    //qDebug() << __PRETTY_FUNCTION__ << ":" << "Finished reading stdout";
}

void GdbMonitor::handleTextOutput (QString text) {

    emit allTextOutput(text);

    if (text[0] == '~') {
        emit tildeTextOutput(text);
    }else if (text[0] == '=') {
        emit equalTextOutput(text);
    }else if (text[0] == '*') {
        emit astrixTextOutput(text);
    }else if (text[0] == '^') {
        emit caretTextOutput(text);
    }else if (text[0] == '&') {
        emit ampersandTextOutput(text);
    }else if (text.contains(QRegExp("^([0-9]+)\\~"))) {
        emit tildeTextOutput(text);
    }else if (text.contains(QRegExp("^([0-9]+)\\="))) {
        emit equalTextOutput(text);
    }else if (text.contains(QRegExp("^([0-9]+)\\*"))) {
        emit astrixTextOutput(text);
    }else if (text.contains(QRegExp("^([0-9]+)\\^"))) {
        emit caretTextOutput(text);
    }else if (text.contains(QRegExp("^([0-9]+)\\&"))) {
        emit ampersandTextOutput(text);
    }else{
        emit textOutput(text);
    }
}

void GdbMonitor::handleStarted() {

    //qDebug() << __PRETTY_FUNCTION__ << ":";

    /*
    process()->write("-file-list-exec-source-files\n");
    process()->write("-exec-run --start\n");
    process()->write("-exec-step\n");
    process()->write("-exec-continue\n");
    process()->write("-exec-finish\n");
    process()->write("-gdb-exit\n");
    */
}

void GdbMonitor::handleStateChanged(QProcess::ProcessState newState) {

    Q_UNUSED(newState);

    //qDebug() << __PRETTY_FUNCTION__ << ":" << newState;
}

void GdbMonitor::setProcess (QProcess* process) {
    _process = process;
}

QProcess* GdbMonitor::process () {
    return _process;
}

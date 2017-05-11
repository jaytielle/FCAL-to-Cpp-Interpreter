#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>


//#define USE_QT_5_X //only use this if you are running Qt 5.1 or higher
#define USE_QT_4_X

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*Slot for makeButton_clicked() signal.
 *
 * Note: You will need to change the directory here to match your file structure.
 *
 * */
void MainWindow::on_makeButton_clicked()
{
    QProcess *cMake = new QProcess(this);
    QString file = QDir::homePath() + "//3081//group//repo-group-Zubat//project//"; //Your makefile path here!
    QStringList args;

    args.append("-C"); //-C because we are running make from outside of the directory with the makefile
    args.append(file);
    args.append("run-tests");

    cMake->setProcessChannelMode(QProcess::MergedChannels);
    cMake->setReadChannel(QProcess::StandardOutput);

#ifdef USE_QT_5_X
    cMake->QProcess::setProgram("//usr//bin//make");
    cMake->setArguments(args);
    cMake->startProgram();
#endif

//Because the more readable way to do it wasn't introduced til later versions of Qt...
#ifdef USE_QT_4_X
    cMake->QProcess::start(QString("//usr//bin//make"), args);
#endif

    if(!cMake->waitForFinished()) {
        qDebug() << "Failed to start make process: " << cMake->errorString();
    }
    else {
        QString output(cMake->readAll());
        this->ui->makeOutputDisplay->setPlainText(output);
    }

    delete cMake;
}

void MainWindow::on_pushButton_clicked()
{
    QProcess *cMake = new QProcess(this);
    QString file = QDir::homePath() + "//3081//group//repo-group-Zubat//project//"; //Your makefile path here!
    QStringList args;

    args.append("-C"); //-C because we are running make from outside of the directory with the makefile
    args.append(file);
    args.append("clean");

    cMake->setProcessChannelMode(QProcess::MergedChannels);
    cMake->setReadChannel(QProcess::StandardOutput);

#ifdef USE_QT_5_X
    cMake->QProcess::setProgram("//usr//bin//make");
    cMake->setArguments(args);
    cMake->startProgram();
#endif

//Because the more readable way to do it wasn't introduced til later versions of Qt...
#ifdef USE_QT_4_X
    cMake->QProcess::start(QString("//usr//bin//make"), args);
#endif

    if(!cMake->waitForFinished()) {
        qDebug() << "Failed to start make process: " << cMake->errorString();
    }
    else {
        QString output(cMake->readAll());
        this->ui->makeOutputDisplay->setPlainText(output);
    }

    delete cMake;

}

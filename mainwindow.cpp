#include "mainwindow.h"
#include "onevideo.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QScrollArea>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

#include <QResizeEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	resize(800, 600);
	setMinimumSize(OneVideo::WIDTH + 20, OneVideo::HEIGHT + 50);
	createMenu();
	setStatusBar(new QStatusBar(this));
	createContent();

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(timerSlot()));
	timer->start(1000);
}

MainWindow::~MainWindow()
{
	timer->deleteLater();
	removeWindowSlot();
}

void MainWindow::createMenu()
{
	QMenuBar *menuBar = new QMenuBar(this);

    QMenu *menuFile = new QMenu("Archivo (&A)", menuBar);
	menuBar->addAction(menuFile->menuAction());
	setMenuBar(menuBar);

    QAction *actionAdd = new QAction("Agregar ventana (&N)", menuFile);
	QAction *actionRemove = new QAction("Eliminar ventana (&D)", menuFile);
    QAction *actionExit = new QAction("Salir (&S)", menuFile);
	menuFile->addAction(actionAdd);
	menuFile->addAction(actionRemove);
	menuFile->addSeparator();
	menuFile->addAction(actionExit);

	connect(actionAdd, SIGNAL(triggered(bool)), SLOT(addWindowSlot()));
	connect(actionRemove, SIGNAL(triggered(bool)), SLOT(removeWindowSlot()));
	connect(actionExit, SIGNAL(triggered(bool)), SLOT(close()));
}

void MainWindow::createContent()
{
	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setFrameStyle(QFrame::NoFrame);

	mainContent = new QWidget(scrollArea);
	mainContent->resize(size());
	scrollArea->setWidget(mainContent);
	setCentralWidget(scrollArea);

	addBtn = new QPushButton("+", mainContent);
	addBtn->setGeometry(0, 0, OneVideo::WIDTH, OneVideo::HEIGHT);
    addBtn->setFont(QFont("Negrita", 200, 87));
	addBtn->setVisible(true);

	connect(addBtn, SIGNAL(clicked(bool)), SLOT(addWindowSlot()));
}

void MainWindow::layoutChild()
{
	int cols = (width() - 20) / OneVideo::WIDTH;
	int i;
	for(i = 0; i < showList.size(); ++i){
		showList.at(i)->move((i % cols) * OneVideo::WIDTH, (i / cols) * OneVideo::HEIGHT);
	}

	addBtn->move((i % cols) * OneVideo::WIDTH, (i / cols) * OneVideo::HEIGHT);

	int sw = width() - 20;
	int sh = ((i / cols) + 1) * OneVideo::HEIGHT;
	mainContent->resize(sw,  sh);
}

void MainWindow::addWindowSlot()
{
	OneVideo *one = new OneVideo(mainContent);
	connect(one, SIGNAL(closeSignal(OneVideo*)), SLOT(childClosed(OneVideo*)));
	one->show();
	showList.append(one);

	layoutChild();
}

void MainWindow::removeWindowSlot()
{
	for(OneVideo *one : showList){
		one->deleteLater();
	}
	showList.clear();
	layoutChild();
}

void MainWindow::aboutSlot()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::timerSlot()
{
    statusBar()->showMessage(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
}

void MainWindow::childClosed(OneVideo *who)
{
	showList.removeOne(who);
	layoutChild();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
	layoutChild();
}

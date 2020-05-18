#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class OneVideo;
class QPushButton;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void createMenu();

	void createContent();

	void layoutChild();

private slots:
	void addWindowSlot();

	void removeWindowSlot();

	void aboutSlot();

	void timerSlot();

	void childClosed(OneVideo *who);

private:
	QList<OneVideo*> showList;

	QWidget *mainContent;
	QPushButton *addBtn;

	QTimer *timer;
protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif

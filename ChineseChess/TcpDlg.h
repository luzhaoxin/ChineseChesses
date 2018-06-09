#pragma once
#include"ui_chatDlgBB.h"
#include<qdialog.h>

class MainWindow;
class TcpDlg :public QDialog
{
public:
	TcpDlg(MainWindow* mainWindow);

	void clientConn();
	void serverListen();
private:
	MainWindow* mainWindow;
	Ui::Dialog ui;
};
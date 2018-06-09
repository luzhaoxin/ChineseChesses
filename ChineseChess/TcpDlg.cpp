#include"TcpDlg.h"
#include"MainWindow.h"

TcpDlg::TcpDlg(MainWindow* mainWindow):
	mainWindow(mainWindow)
{
	ui.setupUi(this);
	connect(ui.pushButton_conn, &QPushButton::clicked, this, &TcpDlg::clientConn);
	connect(ui.pushButton_listen, &QPushButton::clicked, this, &TcpDlg::serverListen);
}

void TcpDlg::clientConn()
{
	if (ui.lineEdit_ip->text().isEmpty() || ui.lineEdit_port->text().isEmpty()) {
		return;
	}

	mainWindow->setClientConn(mainWindow, ui.lineEdit_ip->text(), ui.lineEdit_port->text().toUShort());
}

void TcpDlg::serverListen()
{
	if (ui.lineEdit_ip->text().isEmpty() || ui.lineEdit_port->text().isEmpty()) {
		return;
	}

	mainWindow->setServerListen(mainWindow, ui.lineEdit_ip->text(), ui.lineEdit_port->text().toUShort());
}
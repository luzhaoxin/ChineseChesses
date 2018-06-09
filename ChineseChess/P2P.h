#pragma once
#include<qdialog.h>
#include<qtcpserver.h>
#include<qtcpsocket.h>
#include"ui_chatDlgBB.h"

class MainWindow;
class chatServer:public QDialog
{
	Q_OBJECT

public:
	chatServer(MainWindow* mainWindow, QString ip = "192.168.137.122", quint16 port = 5344);
	void listen();
	void newConnection();
	void serverDisConn();

	void sendMsg(QString msg);
	void recvMsg();
private:
	MainWindow* mainWindow;
	QTcpServer* server;
	QTcpSocket* serverSocket;
	QString ip;
	quint16 port;
};


class chatClient :public QDialog
{
	Q_OBJECT

public:
	chatClient(MainWindow* mainWindow, QString ip = "192.168.137.122", quint16 port = 5344);
	void clientConnect();
	void clientDisConn();

	void sendMsg(QString msg);
	void recvMsg();

private:
	MainWindow* mainWindow;
	QTcpSocket* clientSocket;
	QString ip;
	quint16 port;
};
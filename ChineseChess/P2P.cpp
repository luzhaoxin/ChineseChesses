#include"P2P.h"
#include"MainWindow.h"
#include<qmessagebox.h>

static const int mvMsgSize = 5;
static const char mvMsgStartFlag = '{';

chatServer::chatServer(MainWindow* mainWindow, QString ip, quint16 port) :
	mainWindow(mainWindow),
	server(new QTcpServer(mainWindow)),
	serverSocket(new QTcpSocket(mainWindow)),
	ip(ip),
	port(port)
{
	connect(server, &QTcpServer::newConnection, this, &chatServer::newConnection);
}

void chatServer::newConnection()
{
	serverSocket = server->nextPendingConnection();
	QObject::connect(serverSocket, &QTcpSocket::readyRead, this, &chatServer::recvMsg);
	QObject::connect(serverSocket, &QTcpSocket::disconnected, this, &chatServer::serverDisConn);
}

void chatServer::serverDisConn()
{
	server->close();
}

void chatServer::listen()
{
	if (!server->listen(QHostAddress(ip), port)) {
		QMessageBox::critical(this, "Error£¡", server->errorString().toUtf8(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

void chatServer::sendMsg(QString msg)
{
	if (msg.isEmpty()) {
		return;
	}

	serverSocket->write(msg.toUtf8());
	if (!serverSocket->flush()) {
		QMessageBox::critical(this, "Error£¡", "serverSocket flushing data failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

void  chatServer::recvMsg()
{
	QString msg = QString(serverSocket->readAll());

	if (msg[0] == mvMsgStartFlag&& msg.size() == mvMsgSize) {
		mainWindow->recvMv(msg);
	}
	else {
		mainWindow->recvMsg(msg);
	}
}


/*=========================Client========================*/

chatClient::chatClient(MainWindow* mainWindow, QString ip, quint16 port) :
	mainWindow(mainWindow),
	clientSocket(new QTcpSocket(mainWindow)),
	ip(ip),
	port(port)
{
	connect(clientSocket, &QTcpSocket::readyRead, this, &chatClient::recvMsg);
	connect(clientSocket, &QTcpSocket::disconnected, this, &chatClient::clientDisConn);
}

void chatClient::clientConnect()
{
	clientSocket->connectToHost(ip, port);
	if (!clientSocket->waitForConnected(3000)) {
		QMessageBox msgBox;
		msgBox.setText(tr("Connect Failed!"));
		msgBox.resize(40, 30);
		msgBox.exec();
	}
}

void chatClient::clientDisConn()
{
	clientSocket->close();
}

void chatClient::sendMsg(QString msg)
{
	if (msg.isEmpty())
		return;

	clientSocket->write(msg.toUtf8());
	if (!clientSocket->flush()) {
		QMessageBox::critical(this, "Error£¡", "clientSocket flushing data failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

void chatClient::recvMsg()
{
	QString msg = QString(clientSocket->readAll());
	if (msg[0] == mvMsgStartFlag && msg.size() == mvMsgSize) {
		mainWindow->recvMv(msg);
	}
	else {
		mainWindow->recvMsg(msg);
	}
}
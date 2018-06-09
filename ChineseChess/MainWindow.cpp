#include "MainWindow.h"
#include"Chess.h"
#include"Controller.h"
#include"TcpDlg.h"
#include<qurl.h>
#include<qevent.h>
#include<qlabel.h>

const int MAXHISTORYMV = 4;

MainWindow::MainWindow(QWidget *parent)
	:	QMainWindow	(parent),
		connectType	(nullConnect),
		player				(BLACK),
		chess				(new Chess),
		chessBoard		(new ChessBoard(this)),
		chessFrame		(new QPixmap(":/MainWindow/Resources/ChessFrame.png"))
		//bgMusic			(new QSound("E:/vs2015project/C Projects/Clang++/Qt5.8_Project/ChineseChess/bgMusic.wav"))
{
	this->chess->initChesses();
	controller = new Controller(this);
	ui.setupUi(this);
	//this->playBgSound();占用内存过大

	connect(ui.cb_gamePattern, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::setGamePattern);
	connect(ui.pushButton_send, &QPushButton::clicked, this, &MainWindow::sendMsg);
	connect(ui.pushButton_undo, &QPushButton::clicked, this, &MainWindow::undoMv);
}

void MainWindow::initMainWindow()
{
	
}

void MainWindow::paintEvent(QPaintEvent* event)
{
	drawChessBoard();
	drawChesses();
	drawSelectedFlag();
}

//MainWindow与Controller的连接处
void MainWindow::mousePressEvent(QMouseEvent* event)
{
	/*if (player == RED && !controller->isAlreadySelected()) {
		controller->handlePcEvent();
	}
	else */if (event->button() == Qt::LeftButton && isContained_in_ChessBoard(event))
	{
		controller->handleEvent(event);
	 }
}

bool MainWindow::isContained_in_ChessBoard(QMouseEvent* event)
{
	if ( event->pos().x() >= 0  && event->pos().x() <= chessBoard->width() &&
		event->pos().y() >= 0 && event->pos().y() <= chessBoard->height())
		return true;
	else
		return false;
}

//-------------------------绘制函数-----------------------------------
void MainWindow::drawChessBoard()
{
	QPainter* painter = new QPainter(this);
	painter->drawPixmap(0, 0, *chessBoard->getBackground());
}

void MainWindow::drawChesses()
{
	QPainter* painter = new QPainter(this);
	
	for (int i = 0; i < chess->visibleChesses.size(); i++)
	{
		if (chess->visibleChesses[i].is_Visible())
		{
			painter->drawPixmap(
				chess->visibleChesses[i].getRealPos().x(),
				chess->visibleChesses[i].getRealPos().y(),
				*chess->visibleChesses[i].getPixmap());
		}
	}
}

void MainWindow::drawSelectedFlag()
{
	QPainter* painter = new QPainter(this);
	QRect targetRect;
	for (int i = 0; i < chess->visibleChesses.size(); i++)
	{
		Chess tmpChess = chess->visibleChesses[i];
		if (tmpChess.is_Selected() && tmpChess.is_Visible())
		{
			targetRect.setRect(tmpChess.getRealPos().x(), tmpChess.getRealPos().y(), CHESS_WIDTH, CHESS_WIDTH);
			painter->drawPixmap(targetRect, *this->chessFrame);
			return;
		}
	}
}

void MainWindow::clearChess(int index)
{
	if (index < 0)
		return;
	this->chess->visibleChesses[index].setVisibility(false);
	repaint();
}

void MainWindow::changeSide()
{
	if (this->player == BLACK)
		this->player = RED;
	else
		this->player = BLACK;
}

//=================游戏音效==================
void MainWindow::playBgSound()
{
	bgMusic->setLoops(QSound::Infinite);
	bgMusic->play();
}

void MainWindow::playEatSound()
{
	QSound::play(":/MainWindow/Resources/ChessEat.wav");
}

void MainWindow::playSelctedSound()
{
	QSound::play(":/MainWindow/Resources/ChessSelected.wav");
}

void MainWindow::playMoveSound()
{
	QSound::play(":/MainWindow/Resources/ChessMove.wav");
}

//=============处理着法信息================

void MainWindow::recordChessMove(CHESS_COLOR chessColor, CHESS_TYPE chessType, QPoint newPos, QPoint oldPos)
{
	if (chessColor == RED) {
		QString type = chessType_to_Chinese(RED, chessType);
		QString direction = moveDirection(RED, newPos, oldPos);
		ui.textEdit_rMv->append(tr("%1  %2  %3  %4").arg(type).arg(9 - oldPos.x()).arg(direction).arg(9 - newPos.x()));
	}
	else {
		QString type = chessType_to_Chinese(BLACK, chessType);
		QString direction = moveDirection(BLACK, newPos, oldPos);
		ui.textEdit_bMv->append(tr("%1  %2  %3  %4").arg(type).arg(9 - oldPos.x()).arg(direction).arg(9 - newPos.x()));
	}
}

QString MainWindow::chessType_to_Chinese(CHESS_COLOR chessColor, CHESS_TYPE chessType)
{
	if (chessColor == RED) {
		switch (chessType)
		{
		case CHESS_SHUAI:
			return tr("帅");
			break;
		case CHESS_SHI:
			return tr("仕");
			break;
		case CHESS_XIANG:
			return tr("相");
			break;
		case CHESS_MA:
			return tr("马");
			break;
		case CHESS_JV:
			return tr("车");
			break;
		case CHESS_PAO:
			return tr("炮");
			break;
		case CHESS_BING:
			return tr("兵");
			break;
		default:
			return tr(" ");
			break;
		}
	}
	if (chessColor == BLACK) {
		switch (chessType)
		{
		case CHESS_SHUAI:
			return tr("将");
			break;
		case CHESS_SHI:
			return tr("士");
			break;
		case CHESS_XIANG:
			return tr("象");
			break;
		case CHESS_MA:
			return tr("马");
			break;
		case CHESS_JV:
			return tr("车");
			break;
		case CHESS_PAO:
			return tr("炮");
			break;
		case CHESS_BING:
			return tr("卒");
			break;
		default:
			return tr(" ");
			break;
		}
	}
}

QString MainWindow::moveDirection(CHESS_COLOR chessColor, QPoint newPos, QPoint oldPos)
{
	if (chessColor == RED) {
		if (newPos.y() > oldPos.y()) {
			return tr("进");
		}
		if ((newPos.y() == oldPos.y()) && (newPos.x() != oldPos.x())) {
			return tr("平");
		}
		if (newPos.y() < oldPos.y()) {
			return tr("退");
		}
	}
	else
	{
		if (newPos.y() < oldPos.y()) {
			return tr("进");
		}
		if ((newPos.y() == oldPos.y()) && (newPos.x() != oldPos.x())) {
			return tr("平");
		}
		if (newPos.y() > oldPos.y()) {
			return tr("退");
		}
	}

	return tr(" ");
}

/*====================Game Pattern===================*/

void MainWindow::setGamePattern(int p)
{
	switch (p)
	{
	case 0:
		playWithMyself();
		break;
	case 1:
		playWithOther();
		break;
	case 2:
		playWithPc();
		break;
	default:
		break;
	}
}

void MainWindow::playWithMyself()
{
	chess->initChesses();
	ui.pushButton_undo->setEnabled(true);
}

void MainWindow::playWithOther()
{
	TcpDlg* dlg = new TcpDlg(this);
	dlg->show();
	chess->initChesses();

	ui.pushButton_undo->setEnabled(false);
}

void MainWindow::playWithPc()
{
	/*Uncomplete*/
}


/*====================tcp connect====================*/

void MainWindow::setClientConn(MainWindow* mw, QString ip, quint16 port)
{
	client = new chatClient(mw, ip, port);
	client->clientConnect();
	connectType = ConnectType::clientConnect;
}

void MainWindow::setServerListen(MainWindow* mw, QString ip, quint16 port)
{
	server = new chatServer(mw, ip, port);
	server->listen();
	connectType = ConnectType::serverConnect;
}

void MainWindow::sendMsg()
{
	QString msg = ui.textEdit_send->toPlainText().toUtf8();
	if (connectType == ConnectType::clientConnect) {
		this->client->sendMsg(msg);
	}
	else if (connectType == ConnectType::serverConnect) {
		this->server->sendMsg(msg);
	}

	ui.textEdit_msg->append(msg);
	ui.textEdit_send->clear();
}

void MainWindow::recvMsg(QString msg)
{
	if (connectType == ConnectType::clientConnect) {
		ui.textEdit_msg->append(msg);
	}
	else if (connectType == ConnectType::serverConnect) {
		ui.textEdit_msg->append(msg);
	}
}

void MainWindow::sendMv(int chessIndex, QPoint newPos)
{
	QString mv;
	if (chessIndex < 10) {
		mv = tr("{0%1%2%3").arg(chessIndex).arg(newPos.x()).arg(newPos.y());
	}
	else{
		mv = tr("{%1%2%3").arg(chessIndex).arg(newPos.x()).arg(newPos.y());
	}
	if (connectType == ConnectType::clientConnect) {
		this->client->sendMsg(mv);
	}
	else if (connectType == ConnectType::serverConnect) {
		this->server->sendMsg(mv);
	}
}

void MainWindow::recvMv(QString mv)
{
	QString tmpMv = mv;
	int number = tmpMv.remove(0, 1).toUInt();
	int chessIndex = number / 100;
	int point = number - chessIndex * 100;
	int x = point / 10;
	int y = point - x * 10;

	Chess tmpChess = chess->visibleChesses[chessIndex];
	recordChessMove(tmpChess.getColor(), tmpChess.getType(), QPoint(x, y), tmpChess.getPos());

	//判断是否出现吃子
	int index = chess->hasVisibleChess(QPoint(x, y));		
	if (index != -1) {
		this->chess->visibleChesses[index].setVisibility(false);
		playEatSound();
	}

	this->chess->visibleChesses[chessIndex].setPos(QPoint(x, y));
	this->repaint();
	this->changeSide();
	controller->checked();
}

void MainWindow::undoMv()
{
	if (historyMv.isEmpty()) {
		return;
	}
	MoveHistory tmp = historyMv.pop();
	chess->visibleChesses[tmp.index].setPos(tmp.chessPos);
	//处理出现吃子的情况
	if (tmp.capturedIndex > -1) {
		chess->visibleChesses[tmp.capturedIndex].setVisibility(true);
	}

	changeSide();

	repaint();
}

void MainWindow::saveMv(int chessIndex, QPoint chessPos, int capturedIndex)
{
	if (historyMv.size() >= MAXHISTORYMV) {
		historyMv.removeLast();
	}
	MoveHistory mv;
	mv.index = chessIndex;
	mv.chessPos = chessPos;
	mv.capturedIndex = capturedIndex;

	historyMv.push(mv);
}
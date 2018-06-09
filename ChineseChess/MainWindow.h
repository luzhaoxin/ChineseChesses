#pragma execution_character_set("utf-8")

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include<qstack.h>
#include<qtimer.h>
#include<qsound.h>
#include<qpainter.h>
#include<qmessagebox.h>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include"ChessBoard.h"
#include"P2P.h"

//enum GamePattern {
//	PlayWithMyself,		//自我博弈
//	PlayWithOther,			//真人对抗
//	PlayWithPc,				//人机对战
//	PlayWithEndGame	//挑战残局
//};

struct MoveHistory
{
	int		index;
	QPoint chessPos;
	int		capturedIndex;
};

enum ConnectType {
	clientConnect,
	serverConnect,
	nullConnect
};

enum CHESS_TYPE;
enum CHESS_COLOR;
class Chess;
class Controller;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	void initMainWindow();
	void recordChessMove(CHESS_COLOR chessColor, CHESS_TYPE chessType, QPoint newPos, QPoint oldPos);

	//设置局域网连接
	void setClientConn(MainWindow* mw, QString ip, quint16 port);
	void setServerListen(MainWindow* mw, QString ip, quint16 port);
private:
	bool isContained_in_ChessBoard(QMouseEvent* event);

	QString chessType_to_Chinese(CHESS_COLOR chessColor, CHESS_TYPE chessType);
	QString moveDirection(CHESS_COLOR chessColor, QPoint newPos, QPoint oldPos);

	void setGamePattern(int p);
	void playWithMyself();
	void playWithOther();
	void playWithPc();
public:
	void drawChesses();
	void drawChessBoard();
	void drawSelectedFlag();
	void clearChess(int index);

	void playBgSound();
	void playEatSound();
	void playSelctedSound();
	void playMoveSound();

	void sendMsg();
	void recvMsg(QString msg);
	void recvMv(QString mv);
	void sendMv(int chessIndex, QPoint newPos);

	void changeSide();

	void saveMv(int chessIndex, QPoint chessPos, int capturedIndex);			//存储历史走法
	void undoMv();
protected:
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);

private:
	Ui::MainWindowClass ui;

	QPixmap*			chessFrame;
	ChessBoard*	chessBoard;
	QSound*			bgMusic;

	chatClient*		client;
	chatServer*		server;

	QStack<MoveHistory> historyMv;
public:
	Controller*		controller;
	Chess*				chess;
	CHESS_COLOR	player;
	ConnectType	connectType;
};
#endif // !_MAINWINDOW_H
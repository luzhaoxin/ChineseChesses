#include"ChessBoard.h"
#include"MainWindow.h"
#include"Chess.h"

ChessBoard::ChessBoard(MainWindow* mainWindow) :
	QWidget(mainWindow),
	background(Q_NULLPTR)
{
	this->background = new QPixmap(":/MainWindow/Resources/chessBoard.jpg");
	this->setParent(mainWindow);
	this->resize(background->size());
}

QPixmap* ChessBoard::getBackground()
{
	return background;
}
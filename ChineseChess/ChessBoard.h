#pragma once
#include<qobject.h>
#include<qwidget.h>

class Chess;
class QPainter;
class QPixmap;
class MainWindow;

class ChessBoard:public QWidget
{
public:
	ChessBoard(MainWindow* mainWindow = Q_NULLPTR);

	QPixmap* getBackground();

private:
	QPixmap* background;
};
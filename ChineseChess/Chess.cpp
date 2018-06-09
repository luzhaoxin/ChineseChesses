#include"Chess.h"
#include<qpainter.h>

Chess::Chess()
{
}

Chess::Chess(int chessIndex, CHESS_TYPE chessType, CHESS_COLOR chessColor, QPoint chessPos, QPixmap* chessPixmap,
	bool isSelected, bool isVisible):
	chessIndex(chessIndex),
	chessType(chessType),
	chessColor(chessColor),
	chessPos(chessPos),
	chessPixmap(chessPixmap),
	isSelected(isSelected),
	isVisible(isVisible)
{
}

void Chess::initChesses()
{
	if(!visibleChesses.isEmpty())
		visibleChesses.clear();
	
	QPixmap* pixChesses	= new QPixmap(":/MainWindow/Resources/items.png");
	//初始化红方棋子
	QPixmap* r_pixShuai	= new QPixmap(pixChesses->copy(0, 0,								CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixShi		= new QPixmap(pixChesses->copy(0, CHESS_WIDTH,		CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixXiang	= new QPixmap(pixChesses->copy(0, CHESS_WIDTH *2,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixJv			= new QPixmap(pixChesses->copy(0, CHESS_WIDTH *3,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixMa		= new QPixmap(pixChesses->copy(0, CHESS_WIDTH *4,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixPao		= new QPixmap(pixChesses->copy(0, CHESS_WIDTH * 5,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* r_pixBing		= new QPixmap(pixChesses->copy(0, CHESS_WIDTH * 6,	CHESS_WIDTH, CHESS_WIDTH));
	
	Chess rChessPao		(9,	CHESS_PAO,			RED, QPoint(1, 2), r_pixPao);
	Chess rChessJv			(7,	CHESS_JV,			RED,	QPoint(0, 0),	 r_pixJv);
	Chess rChessMa		(5,	CHESS_MA,			RED,	QPoint(1, 0), r_pixMa);
	Chess rChessXiang	(3,	CHESS_XIANG,		RED,	QPoint(2, 0),	 r_pixXiang);
	Chess rChessShi		(1,	CHESS_SHI,			RED,	QPoint(3, 0),	 r_pixShi);
	Chess rChessShuai	(0,	CHESS_SHUAI,		RED,	QPoint(4, 0),	 r_pixShuai);
	Chess rChessShi2		(2,	CHESS_SHI,			RED,	QPoint(5, 0),	 r_pixShi);
	Chess rChessXiang2	(4,	CHESS_XIANG,		RED,	QPoint(6, 0),	 r_pixXiang);
	Chess rChessMa2		(6,	CHESS_MA,			RED,	QPoint(7, 0), r_pixMa);
	Chess rChessJv2		(8,	CHESS_JV,			RED,	QPoint(8, 0), r_pixJv);
	Chess rChessPao2	(10,	CHESS_PAO,			RED, QPoint(7, 2), r_pixPao);
	Chess rChessBing1	(11	,CHESS_BING,		RED, QPoint(0, 3), r_pixBing);
	Chess rChessBing2	(12,	CHESS_BING,		RED, QPoint(2, 3), r_pixBing);
	Chess rChessBing3	(13,	CHESS_BING,		RED, QPoint(4, 3), r_pixBing);
	Chess rChessBing4	(14,	CHESS_BING,		RED, QPoint(6, 3), r_pixBing);
	Chess rChessBing5	(15,	CHESS_BING,		RED, QPoint(8, 3), r_pixBing);

	visibleChesses.append(rChessShuai);	//0
	visibleChesses.append(rChessShi);		//1
	visibleChesses.append(rChessShi2);
	visibleChesses.append(rChessXiang);	//3
	visibleChesses.append(rChessXiang2);
	visibleChesses.append(rChessMa);		//5
	visibleChesses.append(rChessMa2);
	visibleChesses.append(rChessJv);			//7
	visibleChesses.append(rChessJv2);
	visibleChesses.append(rChessPao);		//9
	visibleChesses.append(rChessPao2);
	visibleChesses.append(rChessBing1);	//11
	visibleChesses.append(rChessBing2);
	visibleChesses.append(rChessBing3);
	visibleChesses.append(rChessBing4);
	visibleChesses.append(rChessBing5);	//15

	//初始化黑方棋子
	QPixmap* b_pixShuai		= new QPixmap(pixChesses->copy(CHESS_WIDTH, 0,							CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixShi			= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH,		CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixXiang		= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH * 2,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixJv			= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH * 3,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixMa			= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH * 4,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixPao			= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH * 5,	CHESS_WIDTH, CHESS_WIDTH));
	QPixmap* b_pixBing		= new QPixmap(pixChesses->copy(CHESS_WIDTH, CHESS_WIDTH * 6,	CHESS_WIDTH, CHESS_WIDTH));

	Chess bChessPao		(25,CHESS_PAO,		BLACK,		QPoint(1, 7), b_pixPao);
	Chess bChessJv		(23,CHESS_JV,			BLACK,		QPoint(0, 9), b_pixJv);
	Chess bChessMa		(21,CHESS_MA,		BLACK,		QPoint(1, 9), b_pixMa);
	Chess bChessXiang	(19,CHESS_XIANG,	BLACK,		QPoint(2, 9), b_pixXiang);
	Chess bChessShi		(17,CHESS_SHI,		BLACK,		QPoint(3, 9), b_pixShi);
	Chess bChessShuai	(16,CHESS_SHUAI,	BLACK,		QPoint(4, 9), b_pixShuai);
	Chess bChessShi2	(18,CHESS_SHI,		BLACK,		QPoint(5, 9), b_pixShi);
	Chess bChessXiang2(20,CHESS_XIANG,	BLACK,		QPoint(6, 9), b_pixXiang);
	Chess bChessMa2	(22,CHESS_MA,		BLACK,		QPoint(7, 9), b_pixMa);
	Chess bChessJv2		(24,CHESS_JV,			BLACK,		QPoint(8, 9), b_pixJv);
	Chess bChessPao2	(26,CHESS_PAO,		BLACK,		QPoint(7, 7), b_pixPao);
	Chess bChessBing1	(27,CHESS_BING,		BLACK,		QPoint(0, 6), b_pixBing);
	Chess bChessBing2	(28,CHESS_BING,		BLACK,		QPoint(2, 6), b_pixBing);
	Chess bChessBing3	(29,CHESS_BING,		BLACK,		QPoint(4, 6), b_pixBing);
	Chess bChessBing4	(30,CHESS_BING,		BLACK,		QPoint(6, 6), b_pixBing);
	Chess bChessBing5	(31,CHESS_BING,		BLACK,		QPoint(8, 6), b_pixBing);

	visibleChesses.append(bChessShuai);	//16
	visibleChesses.append(bChessShi);		//17
	visibleChesses.append(bChessShi2);
	visibleChesses.append(bChessXiang);	//19
	visibleChesses.append(bChessXiang2);
	visibleChesses.append(bChessMa);		//21
	visibleChesses.append(bChessMa2);
	visibleChesses.append(bChessJv);		//23
	visibleChesses.append(bChessJv2);
	visibleChesses.append(bChessPao);		//25
	visibleChesses.append(bChessPao2);
	visibleChesses.append(bChessBing1);	//27
	visibleChesses.append(bChessBing2);
	visibleChesses.append(bChessBing3);
	visibleChesses.append(bChessBing4);
	visibleChesses.append(bChessBing5);	//31
}

//有棋子返回索引，否则返回-1
int Chess::hasVisibleChess(QPoint chessPos)
{
	for (int i = 0; i < CHESS_SIZE; i++) {
		if (this->visibleChesses[i].is_Visible() && this->visibleChesses[i].getPos() == chessPos) {
			return i;
		}
	}

	return -1;
}

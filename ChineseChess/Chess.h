#ifndef CHESS_H
#define CHESS_H

#include<qobject.h>
#include<qwidget.h>
#include<qvector.h>
#include<qpoint.h>

static const int CHESS_SIZE		= 32;//象棋个数
static const int CHESS_WIDTH	= 62;//棋子宽度=高度
static const int BOARD_ROW	= 9;
static const int BOARD_COL		= 8;
static const int RECT_SIZE		= 97;	//棋盘每一格的长宽值(相等)
static const int START_X			= 39;	//棋盘绘制棋子的起始位置
static const int START_Y			= 44;

enum CHESS_TYPE {
	CHESS_SHUAI = 0,	//帅
	CHESS_SHI,				//士
	CHESS_XIANG,			//象
	CHESS_MA,				//马
	CHESS_JV,				//车
	CHESS_PAO,				//炮
	CHESS_BING,			//兵
};

enum CHESS_COLOR {
	RED = 0,	//红方(Pc)
	BLACK,		//黑方
};

class Chess
{
public:
	Chess();
	Chess(int chessIndex, CHESS_TYPE chessType, CHESS_COLOR chessColor, QPoint chessPos, QPixmap* chessPixmap, 
				bool isSelected = false, bool isVisible = true);

	void initChesses();
	
	void setPos			(QPoint chessPos)	{ this->chessPos = chessPos; }
	void setVisibility	(bool isVisible)			{ this->isVisible = isVisible; }
	void setSelected	(bool isSeleccted)	{ this->isSelected = isSeleccted; }
	void setPixmap	(QPixmap* pixmap) { this->chessPixmap = pixmap; }

	int						getIndex()				{ return chessIndex; }
	CHESS_TYPE			getType()					{ return chessType; }
	CHESS_COLOR		getColor()				{ return chessColor; }
	QPoint					getPos()					{ return chessPos; }
	QPoint					getRealPos()				{return QPoint(START_X + this->getPos().x()*RECT_SIZE ,
																						START_Y + this->getPos().y()*RECT_SIZE);}//左上角真实坐标
	QPixmap*				getPixmap()				{ return chessPixmap; }
	bool						is_Selected()				{ return isSelected; }
	bool						is_Visible()				{ return isVisible; }

	int						hasVisibleChess		(QPoint chessPos);
private:
	int						chessIndex;
	CHESS_TYPE			chessType;
	CHESS_COLOR		chessColor;
	QPoint					chessPos;
	bool						isSelected;
	bool						isVisible;
	QPixmap*				chessPixmap;

public:
	QVector<Chess> visibleChesses;
};

// 红方棋子位置价值数组 x+3,12-y
//黑方棋子位置价值数组 x+3,y+3
static const int CHESS_VALUE[7][9 * 10] = {
	{	// 帅（与兵合并）
		 9, 9, 9, 11, 13, 11, 9, 9, 9,
		19, 24, 34, 42, 44, 42, 34, 24, 19,
		19, 24, 32, 37, 37, 37, 32, 24, 19,
		19, 23, 27, 29, 30, 29, 27, 23, 19,
		14, 18, 20, 27, 29, 27, 20, 18, 14,
		7, 0, 13, 0, 16, 0, 13, 0, 7,
		7, 0, 7, 0, 15, 0, 7, 0, 7,
		0, 0, 0, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 2, 2, 2, 0, 0, 0,
		0, 0, 0, 1, 2, 1, 0, 0, 0//11，15，11
	},{	// 仕
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 20, 0, 0, 0, 20, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			18, 0, 0, 20, 23, 20, 0, 0, 18,
			0, 0, 0, 0, 23, 0, 0, 0, 0,
			0, 0, 20, 20, 0, 20, 20, 0, 0
	},{	// 相
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 20, 0, 0, 0, 20, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			18, 0, 0, 20, 23, 20, 0, 0, 18,
			0, 0, 0, 0, 23, 0, 0, 0, 0,
			0, 0, 20, 20, 0, 20, 20, 0, 0
	},{	// 马
			90, 90, 90, 96, 90, 96, 90, 90, 90,
			90, 96, 103, 97, 94, 97, 103, 96, 90,
			92, 98, 99, 103, 99, 103, 99, 98, 92,
			93, 108, 100, 107, 100, 107, 100, 108, 93,
			90, 100, 99, 103, 104, 103, 99, 100, 90,
			90, 98, 101, 102, 103, 102, 101, 98, 90,
			92, 94, 98, 95, 98, 95, 98, 94, 92,
			93, 92, 94, 95, 92, 95, 94, 92, 93,
			85, 90, 92, 93, 78, 93, 92, 90, 85,
			88, 85, 90, 88, 90, 88, 90, 85, 88
	},{	// 车
			206, 208, 207, 213, 214, 213, 207, 208, 206,
			206, 212, 209, 216, 233, 216, 209, 212, 206,
			206, 208, 207, 214, 216, 214, 207, 208, 206,
			206, 213, 213, 216, 216, 216, 213, 213, 206,
			208, 211, 211, 214, 215, 214, 211, 211, 208,
			208, 212, 212, 214, 215, 214, 212, 212, 208,
			204, 209, 204, 212, 214, 212, 204, 209, 204,
			198, 208, 204, 212, 212, 212, 204, 208, 198,
			200, 208, 206, 212, 200, 212, 206, 208, 200,
			194, 206, 204, 212, 200, 212, 204, 206, 194
	},{	// 炮
			100, 100, 96, 91, 90, 91, 96, 100, 100,
			98, 98, 96, 92, 89, 92, 96, 98, 98,
			97, 97, 96, 91, 92, 91, 96, 97, 97,
			96, 99, 99, 98, 100, 98, 99, 99, 96,
			96, 96, 96, 96, 100, 96, 96, 96, 96,
			95, 96, 99, 96, 100, 96, 99, 96, 95,
			96, 96, 96, 96, 96, 96, 96, 96, 96,
			97, 96, 100, 99, 101, 99, 100, 96, 97,
			96, 97, 98, 98, 98, 98, 98, 97, 96,
			96, 96, 97, 99, 99, 99, 97, 96, 96
	}, {	// 兵（与帅合并）
		9, 9, 9, 11, 13, 11, 9, 9, 9,
		19, 24, 34, 42, 44, 42, 34, 24, 19,
		19, 24, 32, 37, 37, 37, 32, 24, 19,
		19, 23, 27, 29, 30, 29, 27, 23, 19,
		14, 18, 20, 27, 29, 27, 20, 18, 14,
		7, 0, 13, 0, 16, 0, 13, 0, 7,
		7, 0, 7, 0, 15, 0, 7, 0, 7,
		0, 0, 0, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 2, 2, 2, 0, 0, 0,
		0, 0, 0, 1, 2, 1, 0, 0, 0//11，15，11
	}
};
#endif // !CHESS_H
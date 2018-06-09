#ifndef PCPLAYER_H
#define PCPLAYER_H

#include"Chess.h"
#include"Controller.h"
#include"MainWindow.h"
#include<time.h>


typedef unsigned long DWORD;
typedef unsigned char BYTE;

static const int MAX_MOVES			= 256;			//最大的历史走法数
static const int MAX_DEPTH				= 64;			//搜索最大深度
static const int ADVANCED_VALUE = 3;				//先着子一方的加权优势
static const int WIN_VALUE				= 9900;		//赢棋分值
static const int MATE_VALUE			= 10000;		//将死分值
static const int DRAW_VALUE			= 20;			//和棋分值
static const int NULL_MARGIN			= 400;			// 空步裁剪的子力边界
static const int NULL_DEPTH			= 2;				// 空步裁剪的裁剪深度

//========================RC4=================================

//class RC4
//{
//public:
//	BYTE s[256];
//	int x;
//	int y;
//
//public:
//	void InitZero();   // 用空密钥初始化密码流生成器
//	BYTE NextByte(); // 生成密码流的下一个字节
//	DWORD NextLong();
//};
//// 生成密码流的下一个字节
//BYTE RC4::NextByte()
//{
//	BYTE uc;
//	x = (x + 1) & 255;
//	y = (y + s[x]) & 255;
//	uc = s[x];
//	s[x] = s[y];
//	s[y] = uc;
//	return s[(s[x] + s[y]) & 255];
//}
//// 生成密码流的下四个字节
//DWORD RC4::NextLong()
//{
//	BYTE uc0, uc1, uc2, uc3;
//	uc0 = NextByte();
//	uc1 = NextByte();
//	uc2 = NextByte();
//	uc3 = NextByte();
//	return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
//}
//
//// 用空密钥初始化密码流生成器
//void RC4::InitZero()
//{
//	int i, j;
//	BYTE uc;
//
//	x = y = j = 0;
//	for (i = 0; i < 256; i++) {
//		s[i] = i;
//	}
//	for (i = 0; i < 256; i++) {
//		j = (j + s[i]) & 255;
//		uc = s[i];
//		s[i] = s[j];
//		s[j] = uc;
//	}
//}

//========================Zobrist==========================

// Zobrist结构
//struct ZobristStruct {
//	DWORD dwKey, dwLock0, dwLock1;
//
//	void InitZero(void) {                 // 用零填充Zobrist
//		dwKey = dwLock0 = dwLock1 = 0;
//	}
//	void InitRC4(RC4 &rc4) {        // 用密码流填充Zobrist
//		dwKey = rc4.NextLong();
//		dwLock0 = rc4.NextLong();
//		dwLock1 = rc4.NextLong();
//	}
//	void Xor(const ZobristStruct &zobr) { // 执行XOR操作
//		dwKey ^= zobr.dwKey;
//		dwLock0 ^= zobr.dwLock0;
//		dwLock1 ^= zobr.dwLock1;
//	}
//	void Xor(const ZobristStruct &zobr1, const ZobristStruct &zobr2) {
//		dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
//		dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
//		dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
//	}
//};
//
//// Zobrist表
//static struct {
//	ZobristStruct Player;	//暂时无用
//	ZobristStruct Table[CHESS_SIZE][BOARD_COL + 1][BOARD_ROW + 1];
//} Zobrist;
//
//// 初始化Zobrist表
//static void InitZobrist(void) {
//	RC4 rc4;
//	rc4.InitZero();
//	Zobrist.Player.InitRC4(rc4);
//	for (int i = 0; i < CHESS_SIZE; i++) {
//		for (int j = 0; j < BOARD_COL + 1; j++) {
//			for (int k = 0; k < BOARD_ROW + 1; k++) {
//				Zobrist.Table[i][j][k].InitRC4(rc4);
//			}
//		}
//	}
//}
//
//棋子着法存储结构
struct Move {
	int		index;
	QPoint chessPos;
};
//
////与搜索相关的全局变量
//static struct 
//{
//	Move pcMove;						//	电脑走的棋
//	int historyTable[CHESS_SIZE][BOARD_COL + 1][BOARD_ROW + 1];			//历史表：索引-X-Y
//}Search;
//
////历史走法存储结构
//struct HistoryMoves
//{
//	Move		move;
//	BYTE			capturedChess;
//	bool			isChecked;
//	DWORD	key;
//
//	void set(Move _move, int _capturedChess, bool _isChecked, DWORD _key) 
//	{
//		key					= _key;
//		move				= _move;
//		isChecked			= _isChecked;
//		capturedChess = _capturedChess;
//	}
//};

class PcPlayer:public QObject
{
	Q_OBJECT
public:
	PcPlayer(Controller* controller);

	void searchMain();

	//核心算法
	int AlphaBetaSearch		(int alpha, int beta, int depth);		//基本的alphaBeta剪枝算法
	int SearchFull				(int alpha, int beta, int depth);		//超出边界的alphaBeta算法
	int QuiescenceSearch	(int alpha, int beta);						//静态搜索

	//评估当前局面
	int		evaluate();
	Move	getBestMove() { return bestMove; }
private:
	//ZobristStruct			zobr;												//Zobrist
	//HistoryMoves			moveList[MAX_MOVES];					//历史走法信息列表
	int							moveNumbers;								//历史走法步数
	int							distance;											//当前搜索深度
	int							redValue;
	int							blackValue;
	int							player;
	QVector<Move>		theLastChessMove;							//存储上一次着法的棋子的信息
	Move						bestMove;										//记录最好的走法的信息
	void							initHistoryMoves();							//清空(初始化)历史走法信息
private:
	QVector<Move>		generateLegalMoves	(bool isGenCaptured = false);
	int							movePiece					(Move chessMove);
	void							undoMovePiece			(Move oldChess,		int capturedChess);
	bool							makeNextMove			(Move chessMove,	int &capturedChess);
	void							unmakeMove				(Move oldChess,		int capturedChess);
	void							nullMove						();
	void							undoNullMove				();
	bool							nullOkay						();					//是否允许空步走法
	int							eatChess						(QPoint chessPos);
	void							changePlayer();
	int							repStatus(int recur);							//检查重复局面
	//int							repValue(int repStatus);					//返回重复局面分值
	//！！！！！！可以将二者合并！！！！！！！全是重复代码
	//生成红方棋子的所有走法
	QVector<Move>		generateRShuaiMove	();
	QVector<Move>		generateRShiMove		();
	QVector<Move>		generateRXiangMove	();
	QVector<Move>		generateRMaMove		();
	QVector<Move>		generateRJvMove		();
	QVector<Move>		generateRPaoMove		();
	QVector<Move>		generateRBingMove	();
	//生成黑方棋子所有走法
	QVector<Move>		generateBShuaiMove	();
	QVector<Move>		generateBShiMove		();
	QVector<Move>		generateBXiangMove	();
	QVector<Move>		generateBMaMove		();
	QVector<Move>		generateBJvMove		();
	QVector<Move>		generateBPaoMove		();
	QVector<Move>		generateBBingMove	();

	//辅助函数
public:
	bool		isLeaglMove							(QPoint oldChessPos, QPoint newChessPos, CHESS_TYPE chessType, CHESS_COLOR chessColor);
	bool		checked();										//判断是否被将军
	bool		isMate();											//判断是否被杀
private:
	bool		isContained_in_ChessBoard	(QPoint newChessPos);
	bool		isContained_in_Sudoku		(QPoint newChessPos, CHESS_COLOR chessColor);//九宫格
	bool		isContained_in_Self				(QPoint newChessPos, CHESS_COLOR chessColor);
	bool		hasSelfChess						(QPoint newChessPos, CHESS_COLOR chessColor);
	bool		hasVisibleChess					(QPoint newChessPos);
	bool		isTrippedXiang						(QPoint oldChessPos, QPoint newChessPos);
	bool		isTrippedMa							(QPoint oldChessPos, QPoint newChessPos);
	bool		isLegalJvMove						(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor);
	bool		isLegalPaoMove					(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor);
	bool		isLegalBingMove					(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor);
	int		hasChessOnthePath				(QPoint oldChessPos, QPoint newChessPos);

	void		updateValue						(bool isDel, int chessIndex, QPoint newChessPos);
	int		getChessValue						(QPoint chessPos, CHESS_TYPE chessType, CHESS_COLOR chessColor);
	int		indexValueArray					(QPoint chessPos, CHESS_COLOR chessColor);
//private:
//	uint64_t				rand64();

private:
	Chess*					chess;
	Controller*			controller;
	QVector<Move> moves;		//所有合法着法
};

#endif 
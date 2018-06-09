#ifndef PCPLAYER_H
#define PCPLAYER_H

#include"Chess.h"
#include"Controller.h"
#include"MainWindow.h"
#include<time.h>


typedef unsigned long DWORD;
typedef unsigned char BYTE;

static const int MAX_MOVES			= 256;			//������ʷ�߷���
static const int MAX_DEPTH				= 64;			//����������
static const int ADVANCED_VALUE = 3;				//������һ���ļ�Ȩ����
static const int WIN_VALUE				= 9900;		//Ӯ���ֵ
static const int MATE_VALUE			= 10000;		//������ֵ
static const int DRAW_VALUE			= 20;			//�����ֵ
static const int NULL_MARGIN			= 400;			// �ղ��ü��������߽�
static const int NULL_DEPTH			= 2;				// �ղ��ü��Ĳü����

//========================RC4=================================

//class RC4
//{
//public:
//	BYTE s[256];
//	int x;
//	int y;
//
//public:
//	void InitZero();   // �ÿ���Կ��ʼ��������������
//	BYTE NextByte(); // ��������������һ���ֽ�
//	DWORD NextLong();
//};
//// ��������������һ���ֽ�
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
//// ���������������ĸ��ֽ�
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
//// �ÿ���Կ��ʼ��������������
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

// Zobrist�ṹ
//struct ZobristStruct {
//	DWORD dwKey, dwLock0, dwLock1;
//
//	void InitZero(void) {                 // �������Zobrist
//		dwKey = dwLock0 = dwLock1 = 0;
//	}
//	void InitRC4(RC4 &rc4) {        // �����������Zobrist
//		dwKey = rc4.NextLong();
//		dwLock0 = rc4.NextLong();
//		dwLock1 = rc4.NextLong();
//	}
//	void Xor(const ZobristStruct &zobr) { // ִ��XOR����
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
//// Zobrist��
//static struct {
//	ZobristStruct Player;	//��ʱ����
//	ZobristStruct Table[CHESS_SIZE][BOARD_COL + 1][BOARD_ROW + 1];
//} Zobrist;
//
//// ��ʼ��Zobrist��
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
//�����ŷ��洢�ṹ
struct Move {
	int		index;
	QPoint chessPos;
};
//
////��������ص�ȫ�ֱ���
//static struct 
//{
//	Move pcMove;						//	�����ߵ���
//	int historyTable[CHESS_SIZE][BOARD_COL + 1][BOARD_ROW + 1];			//��ʷ������-X-Y
//}Search;
//
////��ʷ�߷��洢�ṹ
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

	//�����㷨
	int AlphaBetaSearch		(int alpha, int beta, int depth);		//������alphaBeta��֦�㷨
	int SearchFull				(int alpha, int beta, int depth);		//�����߽��alphaBeta�㷨
	int QuiescenceSearch	(int alpha, int beta);						//��̬����

	//������ǰ����
	int		evaluate();
	Move	getBestMove() { return bestMove; }
private:
	//ZobristStruct			zobr;												//Zobrist
	//HistoryMoves			moveList[MAX_MOVES];					//��ʷ�߷���Ϣ�б�
	int							moveNumbers;								//��ʷ�߷�����
	int							distance;											//��ǰ�������
	int							redValue;
	int							blackValue;
	int							player;
	QVector<Move>		theLastChessMove;							//�洢��һ���ŷ������ӵ���Ϣ
	Move						bestMove;										//��¼��õ��߷�����Ϣ
	void							initHistoryMoves();							//���(��ʼ��)��ʷ�߷���Ϣ
private:
	QVector<Move>		generateLegalMoves	(bool isGenCaptured = false);
	int							movePiece					(Move chessMove);
	void							undoMovePiece			(Move oldChess,		int capturedChess);
	bool							makeNextMove			(Move chessMove,	int &capturedChess);
	void							unmakeMove				(Move oldChess,		int capturedChess);
	void							nullMove						();
	void							undoNullMove				();
	bool							nullOkay						();					//�Ƿ�����ղ��߷�
	int							eatChess						(QPoint chessPos);
	void							changePlayer();
	int							repStatus(int recur);							//����ظ�����
	//int							repValue(int repStatus);					//�����ظ������ֵ
	//���������������Խ����ߺϲ���������������ȫ���ظ�����
	//���ɺ췽���ӵ������߷�
	QVector<Move>		generateRShuaiMove	();
	QVector<Move>		generateRShiMove		();
	QVector<Move>		generateRXiangMove	();
	QVector<Move>		generateRMaMove		();
	QVector<Move>		generateRJvMove		();
	QVector<Move>		generateRPaoMove		();
	QVector<Move>		generateRBingMove	();
	//���ɺڷ����������߷�
	QVector<Move>		generateBShuaiMove	();
	QVector<Move>		generateBShiMove		();
	QVector<Move>		generateBXiangMove	();
	QVector<Move>		generateBMaMove		();
	QVector<Move>		generateBJvMove		();
	QVector<Move>		generateBPaoMove		();
	QVector<Move>		generateBBingMove	();

	//��������
public:
	bool		isLeaglMove							(QPoint oldChessPos, QPoint newChessPos, CHESS_TYPE chessType, CHESS_COLOR chessColor);
	bool		checked();										//�ж��Ƿ񱻽���
	bool		isMate();											//�ж��Ƿ�ɱ
private:
	bool		isContained_in_ChessBoard	(QPoint newChessPos);
	bool		isContained_in_Sudoku		(QPoint newChessPos, CHESS_COLOR chessColor);//�Ź���
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
	QVector<Move> moves;		//���кϷ��ŷ�
};

#endif 
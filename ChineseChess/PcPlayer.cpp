#include"PcPlayer.h"

PcPlayer::PcPlayer(Controller* controller) :
	distance(0),
	controller(controller),
	player(0),
	redValue(0),
	blackValue(0)
{
	this->bestMove.index = -1;//最优走法初始索引为-1
	this->chess = controller->mainWindow->chess;
}

void PcPlayer::searchMain()
{
	int val = 0;
	clock_t timer = clock();

	for (int depth = 1; depth < MAX_DEPTH; depth++) {
		val = SearchFull(-MATE_VALUE, MATE_VALUE, depth);
		if (val > WIN_VALUE || val < -WIN_VALUE) 
			break;
		if ((clock() - timer) >= CLOCKS_PER_SEC)
			break;
	}
}

int PcPlayer::QuiescenceSearch(int alpha, int beta)
{
	return 0;
}

// "qsort"按历史表排序的比较函数
//static int CompareHistory(const void *lpmv1, const void *lpmv2) {
//	return Search.historyTable[*(int *)lpmv2] - Search.historyTable[*(int *)lpmv1];
//}
//超出边界的AlphaBeta搜索
int PcPlayer::SearchFull(int alpha, int beta, int depth)
{
	int capturedChess = -1;
	if (depth <= 0) {
		return evaluate();
	}

	int bestValue = -MATE_VALUE;
	
	QVector<Move> tmpMoves = generateLegalMoves(false);
	/*将生成的着法写入历史表，对其进行qsort---自定义
		qsortHistory(HistoryTable)*/
	for (int i = 0; i < tmpMoves.size(); i++) {
		//===========================================
		Move oldMove;
		oldMove.index = tmpMoves[i].index;
		oldMove.chessPos = chess->visibleChesses[tmpMoves[i].index].getPos();
		//===========================================
		makeNextMove(tmpMoves[i], capturedChess);
		int val = -AlphaBetaSearch(-beta, -alpha, depth - 1);
		unmakeMove(oldMove, capturedChess);
		if (val > bestValue) {
			bestValue = val;
			if (val >= beta) {
				bestMove = tmpMoves[i];
				break;
			}
			if (val > alpha) {
				bestMove = tmpMoves[i];
				alpha = val;
			}
		}
	}

	if (bestValue == -MATE_VALUE) {//如果是杀棋，则根据杀棋步数给出评价
		return distance - MATE_VALUE;
	}
	if (bestMove.index != -1) {
		//Search.historyTable[bestMove.index][bestMove.chessPos.x()][bestMove.chessPos.y()] += depth*depth;
	}
	if (distance == 0) {
		//Search.pcMove = bestMove;
	}
	return bestValue;
}

int PcPlayer::AlphaBetaSearch(int alpha, int beta, int depth)
{
	int capturedChess = -1;
	if (depth <= 0) {
		return evaluate();
	}
	QVector<Move> tmpMoves = generateLegalMoves(false);
	for (int i = 0; i < tmpMoves.size(); i++) {
		//===========================================
		Move oldMove;
		oldMove.index = tmpMoves[i].index;
		oldMove.chessPos = chess->visibleChesses[tmpMoves[i].index].getPos();
		//===========================================
		makeNextMove(tmpMoves[i], capturedChess);
		int val = -AlphaBetaSearch(-beta, -alpha, depth - 1);
		unmakeMove(oldMove, capturedChess);
		if (val >= beta) {
			return beta;
		}
		if (val > alpha) {
			if(depth == 3)
				bestMove = tmpMoves[i];
			alpha = val;
		}
	}
	return alpha;
}

QVector<Move> PcPlayer::generateLegalMoves(bool isGenCaptured)
{
	if (!moves.isEmpty())
		moves.clear();

	if (player == 0) {
		moves  += generateRShuaiMove();
		moves += generateRShiMove();
		moves += generateRXiangMove();
		moves += generateRMaMove();
		moves += generateRJvMove();
		moves += generateRPaoMove();
		moves += generateRBingMove();
	}
	else
	{
		moves += generateBShuaiMove();
		moves += generateBShiMove();
		moves += generateBXiangMove();
		moves += generateBMaMove();
		moves += generateBJvMove();
		moves += generateBPaoMove();
		moves += generateBBingMove();
	}

	if (isGenCaptured) {
		for (int i = 0; i < moves.size(); i++) {
			if (!hasVisibleChess(moves[i].chessPos)) {
				moves.remove(i);
			}
		}
	}

	return moves;
}

int PcPlayer::movePiece(Move chessMove)
{
	int capturedChess = -1;
	Move lastMove;
	lastMove.index = chessMove.index;
	lastMove.chessPos = chess->visibleChesses[chessMove.index].getPos();
	if (hasVisibleChess(chessMove.chessPos) &&
		!hasSelfChess(chessMove.chessPos, chess->visibleChesses[chessMove.index].getColor())) {
		capturedChess = eatChess(chessMove.chessPos);
	}
	chess->visibleChesses[chessMove.index].setPos(chessMove.chessPos);
	updateValue(true, lastMove.index, lastMove.chessPos);
	updateValue(false, chessMove.index, chessMove.chessPos);

	return capturedChess;//返回被吃掉的棋子的索引
}

void PcPlayer::undoMovePiece(Move oldChess, int capturedChess)
{
	chess->visibleChesses[oldChess.index].setPos(oldChess.chessPos);
	if (capturedChess != -1)
		chess->visibleChesses[capturedChess].setVisibility(true);

	updateValue(true, oldChess.index, chess->visibleChesses[oldChess.index].getPos());
	updateValue(false, oldChess.index, oldChess.chessPos);
}

bool PcPlayer::makeNextMove(Move chessMove, int &capturedChess)
{
	capturedChess = movePiece(chessMove);
	distance++;

	if (checked()) {
		unmakeMove(chessMove, capturedChess);
		return false;
	}

	changePlayer();
	return true;
}

void PcPlayer::unmakeMove(Move oldChess, int capturedChess)
{
	distance--;
	changePlayer();
	undoMovePiece(oldChess, capturedChess);
}

//空步
void PcPlayer::nullMove()
{
	/*Move tmpMove;
	tmpMove.index = -1;

	DWORD dwKey;
	dwKey = zobr.dwKey;
	changePlayer();
	moveList[moveNumbers].set(tmpMove, 0, false, dwKey);
	moveNumbers++;
	distance++;*/
}

void PcPlayer::undoNullMove()
{
	distance--;
	moveNumbers--;
	changePlayer();
}

bool PcPlayer::nullOkay()
{
	return (player == 0 ? redValue : blackValue) > NULL_MARGIN;
}

int PcPlayer::eatChess(QPoint chessPos)
{
	//预测的着法位置的棋子可能被吃
	for (int i = 0; i < CHESS_SIZE; i++) {
		if (chess->visibleChesses[i].is_Visible() && chess->visibleChesses[i].getPos() == chessPos) {
			chess->visibleChesses[i].setVisibility(false);
			updateValue(true, i, chess->visibleChesses[i].getPos());
			return i;
		}
	}
	return -1;
}

int PcPlayer::evaluate()
{//player == 0时，轮到红方走棋，返回红方优势
	return player == 0 ?redValue - blackValue : blackValue - redValue + ADVANCED_VALUE;
}

void PcPlayer::changePlayer()
{
	player = 1 - player;
}
//判断是否被将军
bool PcPlayer::checked() 
{
	if (controller->mainWindow->player == RED) {
		player = 0;
	}
	else
		player = 1;
	QVector<Move> tmpMoves;
	QPoint kingPos = chess->visibleChesses[player * 16].getPos();

	if (player == 0) {
		tmpMoves += generateBBingMove();
		tmpMoves += generateBJvMove();
		tmpMoves += generateBMaMove();
		tmpMoves += generateBPaoMove();
		for (int i = 0; i < tmpMoves.size(); i++) {
			if (isLeaglMove(chess->visibleChesses[tmpMoves[i].index].getPos(), tmpMoves[i].chessPos, chess->visibleChesses[tmpMoves[i].index].getType(), BLACK))
				if (tmpMoves[i].chessPos == kingPos)
					return true;
		}
	}
	else {
		tmpMoves += generateRBingMove();
		tmpMoves += generateRJvMove();
		tmpMoves += generateRMaMove();
		tmpMoves += generateRPaoMove();
		for (int i = 0; i < tmpMoves.size(); i++) {
			if(isLeaglMove(chess->visibleChesses[tmpMoves[i].index].getPos(), tmpMoves[i].chessPos, chess->visibleChesses[tmpMoves[i].index].getType(), RED))
				if (tmpMoves[i].chessPos == kingPos)
					return true;
		}
	}
	if (!hasChessOnthePath(chess->visibleChesses[0].getPos(), chess->visibleChesses[16].getPos()))
		return true;

	return false;
}

bool PcPlayer::isMate()
{
	if (controller->mainWindow->player == RED) {
		player = 0;
	}
	else
		player = 1;
	QVector<Move> tmpMoves;
	Move oldMove;//存储原来帅的信息
	oldMove.index = 0;
	oldMove.chessPos = chess->visibleChesses[0].getPos();

	if (player == 0)
		tmpMoves = generateBShuaiMove();
	else
		tmpMoves = generateRShuaiMove();

	if (checked()) {//如果被将军后，再移动一次仍然被将军则判定将死
		for (int i = 0; i < tmpMoves.size(); i++) {
			if (isLeaglMove(chess->visibleChesses[tmpMoves[i].index].getPos(), tmpMoves[i].chessPos, chess->visibleChesses[tmpMoves[i].index].getType(), RED)) {
				int capturedChess = movePiece(tmpMoves[i]);
				if (checked()) {
					undoMovePiece(oldMove, capturedChess);
					return true;
				}
			}
		}
		return false;
	}
	else
	{//困毙判定将死
		for (int i = 0; i < tmpMoves.size(); i++) {
			if (isLeaglMove(chess->visibleChesses[tmpMoves[i].index].getPos(), tmpMoves[i].chessPos, chess->visibleChesses[tmpMoves[i].index].getType(), RED)) {
				int capturedChess = movePiece(tmpMoves[i]);
				if (!checked()) {
					undoMovePiece(oldMove, capturedChess);
					return false;
				}
			}
		}
		return true;
	}
}

int PcPlayer::repStatus(int recur)
{
	return 0;
}
//=================计算子力价值=================

void PcPlayer::updateValue(bool isDel, int chessIndex, QPoint ChessPos)
{
	Chess indexChess = chess->visibleChesses[chessIndex];
	if (indexChess.getColor() == RED) {
		if (isDel) {//isDel 为true时，newChessPos为任意值
			redValue	-= getChessValue(ChessPos, indexChess.getType(), RED);
		}
		else
		{
			redValue += getChessValue(ChessPos, indexChess.getType(), RED);
		}
	}
	else
	{
		if(isDel) {
			blackValue -= getChessValue(ChessPos, indexChess.getType(), BLACK);
		}
		else
		{
			blackValue += getChessValue(ChessPos, indexChess.getType(), BLACK);
		}
	}
}

int PcPlayer::getChessValue(QPoint chessPos, CHESS_TYPE chessType, CHESS_COLOR chessColor)
{
	return CHESS_VALUE[chessType][indexValueArray(chessPos, chessColor)];
}

int PcPlayer::indexValueArray(QPoint chessPos, CHESS_COLOR chessColor)
{
	if (chessColor == RED)
	{
		return (9 * (9 - chessPos.y()) + chessPos.x());
	}
	else if (chessColor == BLACK)
		return (9 * chessPos.y() + chessPos.x());
	else
		return 0;
}

//===================生成存在的棋子的合理走法================

QVector<Move> PcPlayer::generateRShuaiMove()
{
	QVector<Move> tmpMoves;
	Move shuai[4];
	Chess tmpChess = chess->visibleChesses[0];
	if (tmpChess.is_Visible())
	{
		shuai[0].chessPos = QPoint(tmpChess.getPos().x() + 1,	tmpChess.getPos().y());
		shuai[1].chessPos = QPoint(tmpChess.getPos().x() - 1,	tmpChess.getPos().y());
		shuai[2].chessPos = QPoint(tmpChess.getPos().x(),			tmpChess.getPos().y() + 1);
		shuai[3].chessPos = QPoint(tmpChess.getPos().x(),			tmpChess.getPos().y() - 1);

		for (int i = 0; i < 4; i++) {
			shuai[i].index = 0;
			if (isLeaglMove(tmpChess.getPos(), shuai[i].chessPos, CHESS_SHUAI, RED))
				tmpMoves.append(shuai[i]);
		}
	 }
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRShiMove()
{
	QVector<Move> tmpMoves;
	Move shi[8];
	Chess tmpChess1 = chess->visibleChesses[1];
	Chess tmpChess2 = chess->visibleChesses[2];
	if (tmpChess1.is_Visible()){
		shi[0].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() + 1);
		shi[1].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() + 1);
		shi[2].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() - 1);
		shi[3].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() - 1);

		for (int i = 0; i < 4; i++) {
			shi[i].index = 1;
			if (isLeaglMove(tmpChess1.getPos(), shi[i].chessPos, CHESS_SHI, RED))
				tmpMoves.append(shi[i]);
		}
	}
	if (tmpChess2.is_Visible()){
		shi[4].chessPos = QPoint(tmpChess2.getPos().x() + 1,		tmpChess2.getPos().y() + 1);
		shi[5].chessPos = QPoint(tmpChess2.getPos().x() - 1,		tmpChess2.getPos().y() + 1);
		shi[6].chessPos = QPoint(tmpChess2.getPos().x() + 1,		tmpChess2.getPos().y() - 1);
		shi[7].chessPos = QPoint(tmpChess2.getPos().x() - 1,		tmpChess2.getPos().y() - 1);

		for (int i = 4; i < 8; i++) {
			shi[i].index = 2;
			if (isLeaglMove(tmpChess2.getPos(), shi[i].chessPos, CHESS_SHI, RED))
				tmpMoves.append(shi[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRXiangMove()
{
	QVector<Move> tmpMoves;
	Move xiang[8];
	Chess tmpChess1 = chess->visibleChesses[3];
	Chess tmpChess2 = chess->visibleChesses[4];
	if (tmpChess1.is_Visible()){
		xiang[0].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() + 2);
		xiang[1].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() - 2);
		xiang[2].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() + 2);
		xiang[3].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() - 2);

		for (int i = 0; i < 4; i++) {
			xiang[i].index = 3;
			if (isLeaglMove(tmpChess1.getPos(), xiang[i].chessPos, CHESS_XIANG, RED))
				tmpMoves.append(xiang[i]);
		}
	}
	if (tmpChess2.is_Visible()){
		xiang[4].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() + 2);
		xiang[5].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() - 2);
		xiang[6].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() + 2);
		xiang[7].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() - 2);

		for (int i = 4; i < 8; i++) {
			xiang[i].index = 4;
			if (isLeaglMove(tmpChess2.getPos(), xiang[i].chessPos, CHESS_XIANG, RED))
				tmpMoves.append(xiang[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRMaMove()
{
	QVector<Move> tmpMoves;
	Move ma[16];
	Chess tmpChess1 = chess->visibleChesses[5];
	Chess tmpChess2 = chess->visibleChesses[6];
	if (tmpChess1.is_Visible()) {
		ma[0].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() + 2);
		ma[1].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() - 2);
		ma[2].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() + 2);
		ma[3].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() - 2);
		ma[4].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() + 1);
		ma[5].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() - 1);
		ma[6].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() + 1);
		ma[7].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() - 1);

		for (int i = 0; i < 8; i++) {
			ma[i].index = 5;
			if (isLeaglMove(tmpChess1.getPos(), ma[i].chessPos, CHESS_MA, RED))
				tmpMoves.append(ma[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		ma[8].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() + 2);
		ma[9].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() - 2);
		ma[10].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() + 2);
		ma[11].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() - 2);
		ma[12].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() + 1);
		ma[13].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() - 1);
		ma[14].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() + 1);
		ma[15].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() - 1);

		for (int i = 8; i < 16; i++) {
			ma[i].index = 6;
			if (isLeaglMove(tmpChess2.getPos(), ma[i].chessPos, CHESS_MA, RED))
				tmpMoves.append(ma[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRJvMove()
{
	QVector<Move> tmpMoves;
	Move jv[68];
	Chess tmpChess1 = chess->visibleChesses[7];
	Chess tmpChess2 = chess->visibleChesses[8];
	if (tmpChess1.is_Visible()){
		for (int i = 0; i < 9; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() + i + 1);
		for (int i = 9; i < 18; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() - i + 8);

		for(int i = 18; i < 26; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x() + i - 17, tmpChess1.getPos().y());
		for (int i = 26; i < 34; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x() - i + 25, tmpChess1.getPos().y());

		for (int i = 0; i < 34; i++) {
			jv[i].index = 7;
			if (isLeaglMove(tmpChess1.getPos(), jv[i].chessPos, CHESS_JV, RED))
				tmpMoves.append(jv[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		for (int i = 34; i < 43; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() + i - 33);
		for (int i = 43; i < 52; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() - i + 42);

		for (int i = 52; i < 60; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x() + i - 51, tmpChess2.getPos().y());
		for (int i = 60; i < 68; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x() - i + 59, tmpChess2.getPos().y());

		for (int i = 34; i < 68; i++) {
			jv[i].index = 8;
			if (isLeaglMove(tmpChess2.getPos(), jv[i].chessPos, CHESS_JV, RED))
				tmpMoves.append(jv[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRPaoMove()
{
	QVector<Move> tmpMoves;
	Move pao[68];
	Chess tmpChess1 = chess->visibleChesses[9];
	Chess tmpChess2 = chess->visibleChesses[10];
	if (tmpChess1.is_Visible()) {
		for (int i = 0; i < 9; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() + i + 1);
		for (int i = 9; i < 18; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() - i + 8);
		for (int i = 18; i < 26; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x() + i - 17, tmpChess1.getPos().y());
		for (int i = 26; i < 34; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x() - i + 25, tmpChess1.getPos().y());

		for (int i = 0; i < 34; i++) {
			pao[i].index = 9;
			if (isLeaglMove(tmpChess1.getPos(), pao[i].chessPos, CHESS_PAO, RED))
				tmpMoves.append(pao[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		for (int i = 34; i < 43; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() + i - 33);
		for (int i = 43; i < 52; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() - i + 42);
		for (int i = 52; i < 60; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x() + i - 51, tmpChess2.getPos().y());
		for (int i = 60; i < 68; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x() - i + 59, tmpChess2.getPos().y());

		for (int i = 34; i < 68; i++) {
			pao[i].index = 10;
			if (isLeaglMove(tmpChess2.getPos(), pao[i].chessPos, CHESS_PAO, RED))
				tmpMoves.append(pao[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateRBingMove()
{
	QVector<Move> tmpMoves;
	Move bing[15];
	Chess tmpChess[5];
	for (int i = 0; i < 5; i++)
		tmpChess[i] = chess->visibleChesses[i + 11];

	for (int i = 0; i < 5; i++) {
		if (tmpChess[i].is_Visible()) {
			bing[0 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x() + 1, tmpChess[i].getPos().y());
			bing[1 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x() - 1, tmpChess[i].getPos().y());
			bing[2 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x(), tmpChess[i].getPos().y() + 1);

			for (int j = i * 3; j < i * 3 + 3; j++) {
				bing[j].index = 11 + i;
				if (isLeaglMove(tmpChess[i].getPos(), bing[j].chessPos, CHESS_BING, RED))
					tmpMoves.append(bing[j]);
			}
		}
	}
	return tmpMoves;
}

//======================黑方生成着法===================

QVector<Move> PcPlayer::generateBShuaiMove()
{
	QVector<Move> tmpMoves;
	Move shuai[4];
	Chess tmpChess = chess->visibleChesses[16];
	if (tmpChess.is_Visible())
	{
		shuai[0].chessPos = QPoint(tmpChess.getPos().x() + 1,	tmpChess.getPos().y());
		shuai[1].chessPos = QPoint(tmpChess.getPos().x() - 1,	tmpChess.getPos().y());
		shuai[2].chessPos = QPoint(tmpChess.getPos().x(),			tmpChess.getPos().y() + 1);
		shuai[3].chessPos = QPoint(tmpChess.getPos().x(),			tmpChess.getPos().y() - 1);

		for (int i = 0; i < 4; i++) {
			shuai[i].index = 16;
			if (isLeaglMove(tmpChess.getPos(), shuai[i].chessPos, CHESS_SHUAI, BLACK))
				tmpMoves.append(shuai[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBShiMove()
{
	QVector<Move> tmpMoves;
	Move shi[8];
	Chess tmpChess1 = chess->visibleChesses[17];
	Chess tmpChess2 = chess->visibleChesses[18];
	if (tmpChess1.is_Visible()) {
		shi[0].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() + 1);
		shi[1].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() + 1);
		shi[2].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() - 1);
		shi[3].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() - 1);

		for (int i = 0; i < 4; i++) {
			shi[i].index = 17;
			if (isLeaglMove(tmpChess1.getPos(), shi[i].chessPos, CHESS_SHI, BLACK))
				tmpMoves.append(shi[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		shi[4].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() + 1);
		shi[5].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() + 1);
		shi[6].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() - 1);
		shi[7].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() - 1);

		for (int i = 4; i < 8; i++) {
			shi[i].index = 18;
			if (isLeaglMove(tmpChess2.getPos(), shi[i].chessPos, CHESS_SHI, BLACK))
				tmpMoves.append(shi[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBXiangMove()
{
	QVector<Move> tmpMoves;
	Move xiang[8];
	Chess tmpChess1 = chess->visibleChesses[19];
	Chess tmpChess2 = chess->visibleChesses[20];
	if (tmpChess1.is_Visible()) {
		xiang[0].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() + 2);
		xiang[1].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() - 2);
		xiang[2].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() + 2);
		xiang[3].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() - 2);

		for (int i = 0; i < 4; i++) {
			xiang[i].index = 19;
			if (isLeaglMove(tmpChess1.getPos(), xiang[i].chessPos, CHESS_XIANG, BLACK))
				tmpMoves.append(xiang[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		xiang[4].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() + 2);
		xiang[5].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() - 2);
		xiang[6].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() + 2);
		xiang[7].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() - 2);

		for (int i = 4; i < 8; i++) {
			xiang[i].index = 20;
			if (isLeaglMove(tmpChess2.getPos(), xiang[i].chessPos, CHESS_XIANG, BLACK))
				tmpMoves.append(xiang[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBMaMove()
{
	QVector<Move> tmpMoves;
	Move ma[16];
	Chess tmpChess1 = chess->visibleChesses[21];
	Chess tmpChess2 = chess->visibleChesses[22];
	if (tmpChess1.is_Visible()) {
		ma[0].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() + 2);
		ma[1].chessPos = QPoint(tmpChess1.getPos().x() + 1, tmpChess1.getPos().y() - 2);
		ma[2].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() + 2);
		ma[3].chessPos = QPoint(tmpChess1.getPos().x() - 1, tmpChess1.getPos().y() - 2);
		ma[4].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() + 1);
		ma[5].chessPos = QPoint(tmpChess1.getPos().x() + 2, tmpChess1.getPos().y() - 1);
		ma[6].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() + 1);
		ma[7].chessPos = QPoint(tmpChess1.getPos().x() - 2, tmpChess1.getPos().y() - 1);

		for (int i = 0; i < 8; i++) {
			ma[i].index = 21;
			if (isLeaglMove(tmpChess1.getPos(), ma[i].chessPos, CHESS_MA, BLACK))
				tmpMoves.append(ma[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		ma[8].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() + 2);
		ma[9].chessPos = QPoint(tmpChess2.getPos().x() + 1, tmpChess2.getPos().y() - 2);
		ma[10].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() + 2);
		ma[11].chessPos = QPoint(tmpChess2.getPos().x() - 1, tmpChess2.getPos().y() - 2);
		ma[12].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() + 1);
		ma[13].chessPos = QPoint(tmpChess2.getPos().x() + 2, tmpChess2.getPos().y() - 1);
		ma[14].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() + 1);
		ma[15].chessPos = QPoint(tmpChess2.getPos().x() - 2, tmpChess2.getPos().y() - 1);

		for (int i = 8; i < 16; i++) {
			ma[i].index = 22;
			if (isLeaglMove(tmpChess2.getPos(), ma[i].chessPos, CHESS_MA, BLACK))
				tmpMoves.append(ma[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBJvMove()
{
	QVector<Move> tmpMoves;
	Move jv[68];
	Chess tmpChess1 = chess->visibleChesses[23];
	Chess tmpChess2 = chess->visibleChesses[24];
	if (tmpChess1.is_Visible()) {
		for (int i = 0; i < 9; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() + i + 1);
		for (int i = 9; i < 18; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() - i + 8);

		for (int i = 18; i < 26; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x() + i - 17, tmpChess1.getPos().y());
		for (int i = 26; i < 34; i++)
			jv[i].chessPos = QPoint(tmpChess1.getPos().x() - i + 25, tmpChess1.getPos().y());

		for (int i = 0; i < 34; i++) {
			jv[i].index = 23;
			if (isLeaglMove(tmpChess1.getPos(), jv[i].chessPos, CHESS_JV, BLACK))
				tmpMoves.append(jv[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		for (int i = 34; i < 43; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() + i - 33);
		for (int i = 43; i < 52; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() - i + 42);

		for (int i = 52; i < 60; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x() + i - 51, tmpChess2.getPos().y());
		for (int i = 60; i < 68; i++)
			jv[i].chessPos = QPoint(tmpChess2.getPos().x() - i + 59, tmpChess2.getPos().y());

		for (int i = 34; i < 68; i++) {
			jv[i].index = 24;
			if (isLeaglMove(tmpChess2.getPos(), jv[i].chessPos, CHESS_JV, BLACK))
				tmpMoves.append(jv[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBPaoMove()
{
	QVector<Move> tmpMoves;
	Move pao[68];
	Chess tmpChess1 = chess->visibleChesses[25];
	Chess tmpChess2 = chess->visibleChesses[26];
	if (tmpChess1.is_Visible()) {
		for (int i = 0; i < 9; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() + i + 1);
		for (int i = 9; i < 18; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x(), tmpChess1.getPos().y() - i + 8);
		for (int i = 18; i < 26; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x() + i - 17, tmpChess1.getPos().y());
		for (int i = 26; i < 34; i++)
			pao[i].chessPos = QPoint(tmpChess1.getPos().x() - i + 25, tmpChess1.getPos().y());

		for (int i = 0; i < 34; i++) {
			pao[i].index = 25;
			if (isLeaglMove(tmpChess1.getPos(), pao[i].chessPos, CHESS_PAO, RED))
				tmpMoves.append(pao[i]);
		}
	}
	if (tmpChess2.is_Visible()) {
		for (int i = 34; i < 43; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() + i - 33);
		for (int i = 43; i < 52; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x(), tmpChess2.getPos().y() - i + 42);
		for (int i = 52; i < 60; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x() + i - 51, tmpChess2.getPos().y());
		for (int i = 60; i < 68; i++)
			pao[i].chessPos = QPoint(tmpChess2.getPos().x() - i + 59, tmpChess2.getPos().y());

		for (int i = 34; i < 68; i++) {
			pao[i].index = 26;
			if (isLeaglMove(tmpChess2.getPos(), pao[i].chessPos, CHESS_PAO, BLACK))
				tmpMoves.append(pao[i]);
		}
	}
	return tmpMoves;
}

QVector<Move> PcPlayer::generateBBingMove()
{
	QVector<Move> tmpMoves;
	Move bing[15];
	Chess tmpChess[5];
	for (int i = 0; i < 5; i++)
		tmpChess[i] = chess->visibleChesses[i + 27];

	for (int i = 0; i < 5; i++) {
		if (tmpChess[i].is_Visible()) {
			bing[0 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x() + 1, tmpChess[i].getPos().y());
			bing[1 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x() - 1, tmpChess[i].getPos().y());
			bing[2 + i * 3].chessPos = QPoint(tmpChess[i].getPos().x(), tmpChess[i].getPos().y() - 1);

			for (int j = i * 3; j < i * 3 + 3; j++) {
				bing[j].index = 27 + i;
				if (isLeaglMove(tmpChess[i].getPos(), bing[j].chessPos, CHESS_BING, RED))
					tmpMoves.append(bing[j]);
			}
		}
	}
	return tmpMoves;
}

//=======================辅助函数=========================

bool PcPlayer::isLeaglMove(QPoint oldChessPos, QPoint newChessPos, CHESS_TYPE chessType, CHESS_COLOR chessColor)
{
	if (isContained_in_ChessBoard(newChessPos))
	{
		switch (chessType)
		{
		case CHESS_SHUAI:
			return (isContained_in_Sudoku(newChessPos, chessColor) && !hasSelfChess(newChessPos, chessColor));
			break;
		case CHESS_SHI:
			return (isContained_in_Sudoku(newChessPos, chessColor) && !hasSelfChess(newChessPos, chessColor));
			break;
		case CHESS_XIANG:
			return (isContained_in_Self(newChessPos, chessColor) &&
				!isTrippedXiang(oldChessPos, newChessPos) &&
				!hasSelfChess(newChessPos, chessColor));
			break;
		case CHESS_MA:
			return (!isTrippedMa(oldChessPos, newChessPos) && !hasSelfChess(newChessPos, chessColor));
			break;
		case CHESS_JV:
			return isLegalJvMove(oldChessPos, newChessPos, chessColor);
			break;
		case CHESS_PAO:
			return isLegalPaoMove(oldChessPos, newChessPos, chessColor);
			break;
		case CHESS_BING:
			return isLegalBingMove(oldChessPos, newChessPos, chessColor);
			break;
		default:
			return false;
			break;
		}
	}
	else
		return false;
}

bool PcPlayer::isContained_in_Self(QPoint chessPos, CHESS_COLOR chessColor)
{
	if (chessColor == RED)
	{
		if (abs(chessPos.y() - 2) <= 2)
			return true;
		else
			return false;
	}
	else
	{
		if (abs(chessPos.y() - 7) <= 2)
			return true;
		else
			return false;
	}
}

bool PcPlayer::isContained_in_Sudoku(QPoint chessPos, CHESS_COLOR chessColor)
{
	if (chessColor == RED)
	{
		if (abs(chessPos.x() - BOARD_COL*0.5) <= 1 && abs(chessPos.y() - 1) <= 1)
			return true;
		else
			return false;
	}
	else
	{
		if (abs(chessPos.x() - BOARD_COL*0.5) <= 1 && abs(chessPos.y() - 8) <= 1)
			return true;
		else
			return false;
	}
}

bool PcPlayer::isContained_in_ChessBoard(QPoint chessPos)
{
	if (abs(chessPos.x() - BOARD_COL*0.5) <= BOARD_COL*0.5 &&
		abs(chessPos.y() - BOARD_ROW*0.5) <= BOARD_ROW*0.5)
		return true;
	else
		return false;
}

bool PcPlayer::hasSelfChess(QPoint chessPos, CHESS_COLOR chessColor)
{
	for (int i = 0; i < CHESS_SIZE; i++) {
		if (chess->visibleChesses[i].is_Visible()) {
			if (chess->visibleChesses[i].getPos() == chessPos) {
				if (chess->visibleChesses[i].getColor() == chessColor)
					return true;
				else
					return false;
			}
		}
	}
	return false;
}

bool PcPlayer::hasVisibleChess(QPoint chessPos)
{
	for (int i = 0; i < CHESS_SIZE; i++) {
		if (chess->visibleChesses[i].is_Visible()) {
			if (chess->visibleChesses[i].getPos() == chessPos)
				return true;
		}
	}
	return false;
}

bool PcPlayer::isTrippedXiang(QPoint oldChessPos, QPoint newChessPos)
{
	if (hasVisibleChess(QPoint(
		(oldChessPos.x() + newChessPos.x())*0.5,
		(oldChessPos.y() + newChessPos.y())*0.5)))
		return true;
	else
		return false;
}

bool PcPlayer::isTrippedMa(QPoint oldChessPos, QPoint newChessPos)
{
	if (abs(newChessPos.x() - oldChessPos.x()) == 1)
	{
		if (oldChessPos.y() - newChessPos.y() == 2)		//top
		{		//蹩脚马
			if (hasVisibleChess(QPoint(oldChessPos.x(), oldChessPos.y() - 1)))
				return true;
			else
				return false;
		}
		else if (newChessPos.y() - oldChessPos.y() == 2)	//bottom
		{
			if (hasVisibleChess(QPoint(oldChessPos.x(), oldChessPos.y() + 1)))
				return true;
			else
				return false;
		}
		else
			return true;//????
	}
	else if (abs(newChessPos.y() - oldChessPos.y()) == 1)
	{
		if (oldChessPos.x() - newChessPos.x() == 2)			//left
		{
			if (hasVisibleChess(QPoint(oldChessPos.x() - 1, oldChessPos.y())))
				return true;
			else
				return false;
		}
		else if (newChessPos.x() - oldChessPos.x() == 2)	//right
		{
			if (hasVisibleChess(QPoint(oldChessPos.x() + 1, oldChessPos.y())))
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return true;
}

bool PcPlayer::isLegalJvMove(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor)
{
	if (hasChessOnthePath(oldChessPos, newChessPos))
		return false;
	else if (hasVisibleChess(newChessPos)) {
		if (hasSelfChess(newChessPos, chessColor))
			return false;
		else
			return true;
	}
	else
		return true;
}

bool PcPlayer::isLegalPaoMove(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor) 
{
	if (hasChessOnthePath(oldChessPos, newChessPos) == 1) {
		if (hasVisibleChess(newChessPos)) {
			if (hasSelfChess(newChessPos, chessColor))
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else if (hasVisibleChess(newChessPos)) {
		return false;
	}
	else
		return true;
}

bool PcPlayer::isLegalBingMove(QPoint oldChessPos, QPoint newChessPos, CHESS_COLOR chessColor)
{
	if (isContained_in_Self(oldChessPos, chessColor)) {
		if (newChessPos.x() == oldChessPos.x())
			return true;
		else
			return false;
	}
	else
		return true;
}

int PcPlayer::hasChessOnthePath(QPoint oldChessPos, QPoint newChessPos)
{
	int number = 0;
	if (oldChessPos.x() == newChessPos.x()) {
		if (oldChessPos.y() > newChessPos.y()) {
			for (int i = newChessPos.y() + 1; i < oldChessPos.y(); i++) {
				if (hasVisibleChess(QPoint(newChessPos.x(), i)))
					number++;
			}
		}
		else if (oldChessPos.y() < newChessPos.y()) {
			for (int i = oldChessPos.y() + 1; i < newChessPos.y(); i++) {
				if (hasVisibleChess(QPoint(oldChessPos.x(), i)))
					number++;
			}
		}
	}
	else if (oldChessPos.y() == newChessPos.y()) {
		if (oldChessPos.x() > newChessPos.x()) {
			for (int i = newChessPos.x() + 1; i < oldChessPos.x(); i++) {
				if (hasVisibleChess(QPoint(i, oldChessPos.y())))
					number++;
			}
		}
		else if (oldChessPos.x() < newChessPos.x()) {
			for (int i = oldChessPos.x() + 1; i < newChessPos.x(); i++) {
				if (hasVisibleChess(QPoint(i, oldChessPos.y())))
					number++;
			}
		}
	}

	return number;
}


//=============================================

//uint64_t PcPlayer::rand64()
//{
//	return rand() ^ ((uint64_t)rand() << 15) ^ ((uint64_t)rand() << 30) ^ ((uint64_t)rand() << 45) ^ ((uint64_t)rand() << 60);
//}

void PcPlayer::initHistoryMoves()
{
	/*Move tmpMove;
	tmpMove.index = -1;
	moveList[0].set(tmpMove, -1, checked(), zobr.dwKey);
	moveNumbers = 1;*/
}
#include"Controller.h"
#include"PcPlayer.h"
#include"MainWindow.h"

Controller::Controller(MainWindow* mainWindow):
	mainWindow(mainWindow),
	pc(new PcPlayer(this))
{
}

void Controller::handleEvent(QMouseEvent* mouseEvent)
{
	//顺序不可随意更改
	this->mouseEvent				= mouseEvent;
	this->eventPoint					= trans_eventPos_to_chessPoint();
	this->clickedChessIndex		= getClickedChessIndex();
	this->selectedChessIndex	= getSelectedChessIndex();

	if (isAlreadySelected()){
		if (hasVisibleChess(eventPoint)){
			if(isSameColor()){ 
				mainWindow->chess->visibleChesses[selectedChessIndex].setSelected(false);
				makeChessBeSelected();
			}
			else if (isLeagalMove()) {
				eatChess();
			}
		}
		else if (isLeagalMove()){
			moveTo();
		}
	}
	else if (hasVisibleChess(eventPoint) && (theChess(clickedChessIndex).getColor() == mainWindow->player)){
		makeChessBeSelected();
		mainWindow->playSelctedSound();
	}
}

void Controller::handlePcEvent()
{
	pc->AlphaBetaSearch(-1000000, 1000000, 3);
	pcMoveTo();
}

bool Controller::isSameColor()
{
	return	theChess(selectedChessIndex).getColor() ==
				theChess(clickedChessIndex).getColor();
}

bool Controller::isLeagalMove()
{
	switch (getSelectedChess()->getType())
	{
	case CHESS_SHUAI:
		return moveShuai();
		break;
	case CHESS_SHI:
		return moveShi();
		break;
	case CHESS_XIANG:
		return moveXiang();
		break;
	case CHESS_MA:
		return moveMa();
		break;
	case CHESS_JV:
		return moveJv();
		break;
	case CHESS_PAO:
		return movePao();
		break;
	case CHESS_BING:
		return moveBing();
		break;
	default:
		return false;
		break;
	}
}

bool Controller::isVisible(int index)
{
	return theChess(index).is_Visible();
}

bool Controller::isSelected(int index)
{
	return theChess(index).is_Selected();
}

void Controller::eatChess()
{
	mainWindow->clearChess(getClickedChessIndex());
	moveTo();
	mainWindow->playEatSound();
}

void Controller::moveTo()
{
	CHESS_COLOR tmpColor = mainWindow->player;
	mainWindow->changeSide();
	mainWindow->recordChessMove(tmpColor, getSelectedChess()->getType(), eventPoint, getSelectedChess()->getPos());
	mainWindow->saveMv(selectedChessIndex, getSelectedChess()->getPos(), clickedChessIndex);

	mainWindow->chess->visibleChesses[selectedChessIndex].setPos(eventPoint);
	mainWindow->chess->visibleChesses[selectedChessIndex].setSelected(false);
	mainWindow->playMoveSound();
	mainWindow->repaint();

	checked();

	mainWindow->sendMv(selectedChessIndex, eventPoint);
}

void Controller::pcMoveTo()
{
	if (mainWindow->player == BLACK) {
		mainWindow->player = RED;
		return;
	}
	else
		mainWindow->player = BLACK;

	//将吃子动作放到PcPlayer的makeNextMove中，方便进行局面评估
	if (hasVisibleChess(pc->getBestMove().chessPos))
	{
		for (int i = 0; i < CHESS_SIZE; i++) {
			if (mainWindow->chess->visibleChesses[i].is_Visible()) {
				if (mainWindow->chess->visibleChesses[i].getPos() == pc->getBestMove().chessPos) {
					mainWindow->chess->visibleChesses[i].setVisibility(false);
				}
			}
		}
	}
	mainWindow->chess->visibleChesses[pc->getBestMove().index].setPos(pc->getBestMove().chessPos);
	mainWindow->playMoveSound();
	mainWindow->repaint();
}

bool Controller::isAlreadySelected()
{
	if (getSelectedChessIndex() == -1)
		return false;
	else
		return true;
}

void Controller::checked()
{
	if (pc->checked()) {
		QMessageBox::information(mainWindow, "Warning", "将军!");
	}
}

void Controller::makeChessBeSelected()
{
	for (int i = 0; i < CHESS_SIZE; i++)
	{
		if (isVisible(i) && theChess(i).getPos() == eventPoint)
		{
			mainWindow->chess->visibleChesses[i].setSelected(true);
			mainWindow->playSelctedSound();
			mainWindow->repaint();
			return;
		}
	}
}

int Controller::getClickedChessIndex()
{
	for (int i = 0; i < CHESS_SIZE; i++)
	{
		if (isVisible(i) && theChess(i).getPos() == eventPoint)
			return i;
	}
	return -1;
}

Chess* Controller::getSelectedChess()
{
	for (int i = 0; i < CHESS_SIZE; i++)
	{
		if (isVisible(i) && isSelected(i))
			return &mainWindow->chess->visibleChesses[i];
	}
	return Q_NULLPTR;
}

Chess Controller::theChess(int index)
{
	return mainWindow->chess->visibleChesses[index];
}

int Controller::getSelectedChessIndex()
{
	for (int i = 0; i < CHESS_SIZE; i++)
	{
		if (isVisible(i) && isSelected(i))
			return i;
	}
	return -1;
}

bool Controller::hasVisibleChess(QPoint point)
{
	for (int i = 0; i < CHESS_SIZE; i++)
	{
		if (isVisible(i) && theChess(i).getPos() == point)
			return true;
	}
	return false;
}

QPoint Controller::trans_eventPos_to_chessPoint()
{
	QPoint point(-1, -1);
	int transX = 0;
	int transY = 0;
	for (int i = 0; i <= BOARD_COL; i++)
	{
		transX = mouseEvent->pos().x() - START_X - RECT_SIZE*i;
		if (transX >0 && transX< CHESS_WIDTH)
		{
			point.setX(i);
			break;
		}
	}
	for (int i = 0; i <= BOARD_ROW; i++)
	{
		transY = mouseEvent->pos().y() - START_Y - RECT_SIZE*i;
		if (transY > 0 && transY < CHESS_WIDTH)
		{
			point.setY(i);
			break;
		}
	}

	return point;
}

/*================象棋移动判断==============*/

bool Controller::moveJv()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();
	//移动路径上是否有棋子
	if (eventPoint.x() == selectedChessPoint.x())
	{
		if (eventPoint.y() > selectedChessPoint.y())
		{
			for (int i = selectedChessPoint.y() + 1; i < eventPoint.y(); i++)
			{
				if (hasVisibleChess(QPoint(eventPoint.x(), i)))
					return false;
			}
			return true;
		}
		else if (eventPoint.y() < selectedChessPoint.y())
		{
			for (int i = selectedChessPoint.y() - 1; i > eventPoint.y(); i--)
			{
				if (hasVisibleChess(QPoint(eventPoint.x(), i)))
					return false;
			}
			return true;
		}
		return false;
	}
	else if (eventPoint.y() == selectedChessPoint.y())
	{
		if (eventPoint.x() > selectedChessPoint.x())
		{
			for (int i = selectedChessPoint.x() + 1; i < eventPoint.x(); i++)
			{
				if (hasVisibleChess(QPoint(i, eventPoint.y())))
					return false;
			}
			return true;
		}
		else if (eventPoint.x() < selectedChessPoint.x())
		{
			for (int i = selectedChessPoint.x() - 1; i > eventPoint.x(); i--)
			{
				if (hasVisibleChess(QPoint(i, eventPoint.y())))
					return false;
			}
			return true;
		}
		return false;
	}
	return false;
}

bool Controller::moveMa()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();
	if (abs(eventPoint.x() - selectedChessPoint.x()) == 1)
	{
		if (selectedChessPoint.y() - eventPoint.y() == 2)		//top
		{		//蹩脚马
			if (hasVisibleChess(QPoint(selectedChessPoint.x(), selectedChessPoint.y() - 1)))
				return false;
			else
				return true;
		}
		else if (eventPoint.y() - selectedChessPoint.y() == 2)	//bottom
		{
			if (hasVisibleChess(QPoint(selectedChessPoint.x(), selectedChessPoint.y() + 1)))
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else if (abs(eventPoint.y() - selectedChessPoint.y()) == 1)
	{
		if (selectedChessPoint.x() - eventPoint.x() == 2)			//left
		{
			if (hasVisibleChess(QPoint(selectedChessPoint.x() - 1, selectedChessPoint.y())))
				return false;
			else
				return true;
		}
		else if (eventPoint.x() - selectedChessPoint.x() == 2)	//right
		{
			if (hasVisibleChess(QPoint(selectedChessPoint.x() + 1, selectedChessPoint.y())))
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool Controller::moveXiang()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();
	//行走区域
	if (theChess(selectedChessIndex).getColor() == BLACK)
	{
		if (eventPoint.y() < 5)
			return false;
	}
	else if (eventPoint.y() >= 5)
		return false;
	//行走条件
	if (abs(eventPoint.x() - selectedChessPoint.x()) == 2 && abs(eventPoint.y() - selectedChessPoint.y()) == 2) {
		if (hasVisibleChess(QPoint( (eventPoint.x() + selectedChessPoint.x())*0.5, (eventPoint.y() + selectedChessPoint.y())*0.5))) {
			return false;
		}
		return true;
	}

	return false;
}

bool Controller::moveShi()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();

	if (theChess(selectedChessIndex).getColor() == BLACK)
	{
		if (abs(eventPoint.y() - 8) > 1 || abs(eventPoint.x() - 4) > 1)
			return false;
	}
	else if (abs(eventPoint.y() - 1) > 1 || abs(eventPoint.x() - 4) > 1)
		return false;

	if(abs(eventPoint.x() - selectedChessPoint.x()) == 1 && abs(eventPoint.y() - selectedChessPoint.y()) == 1)
		return true;
	else
		return false;
}

bool Controller::moveShuai()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();
	if (theChess(selectedChessIndex).getColor() == BLACK)
	{
		if (abs(eventPoint.y() - 8) > 1 || abs(eventPoint.x() - 4) > 1)
			return false;
	}
	else if (abs(eventPoint.y() - 1) > 1 || abs(eventPoint.x() - 4) > 1)
		return false;

	if ((abs(eventPoint.x() - selectedChessPoint.x()) == 1 && abs(eventPoint.y() - selectedChessPoint.y()) == 0) ||
		(abs(eventPoint.x() - selectedChessPoint.x()) == 0 && abs(eventPoint.y() - selectedChessPoint.y()) == 1))
		return true;
	else
		return false;
}

//与moveJv代码大量重复！！！！！！！
bool Controller::movePao()
{
	int num = 0;
	int target = 0;
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();
	
	if (eventPoint.x() == selectedChessPoint.x())
	{
		if (hasVisibleChess(QPoint(eventPoint.x(), eventPoint.y())))
			target++;
		if (eventPoint.y() > selectedChessPoint.y())
		{
			for (int i = selectedChessPoint.y() + 1; i < eventPoint.y(); i++)
			{
				if (hasVisibleChess(QPoint(eventPoint.x(), i)))
					num++;
			}
		}
		else if (eventPoint.y() < selectedChessPoint.y())
		{
			for (int i = selectedChessPoint.y() - 1; i > eventPoint.y(); i--)
			{
				if (hasVisibleChess(QPoint(eventPoint.x(), i)))
					num++;
			}
		}
	}
	else if (eventPoint.y() == selectedChessPoint.y())
	{
		if (hasVisibleChess(QPoint(eventPoint.x(), eventPoint.y())))
			target++;
		if (eventPoint.x() > selectedChessPoint.x())
		{
			for (int i = selectedChessPoint.x() + 1; i < eventPoint.x(); i++)
			{
				if (hasVisibleChess(QPoint(i, eventPoint.y())))
					num++;
			}
		}
		else if (eventPoint.x() < selectedChessPoint.x())
		{
			for (int i = selectedChessPoint.x() - 1; i > eventPoint.x(); i--)
			{
				if (hasVisibleChess(QPoint(i, eventPoint.y())))
					num++;
			}
		}
	}
	else
		return false;

	if ((num == 1 && target == 1) || (num == 0 && target == 0))
		return true;
	else
		return false;
}

bool Controller::moveBing()
{
	QPoint selectedChessPoint = theChess(selectedChessIndex).getPos();

	if (theChess(selectedChessIndex).getColor() == BLACK)
	{
		if (selectedChessPoint.y() >= 5)
		{
			if ((eventPoint.x() == selectedChessPoint.x()) && ((selectedChessPoint.y() - eventPoint.y()) == 1))
				return true;
			else
				return false;
		}
		else
		{
			if ((abs(eventPoint.x() - selectedChessPoint.x()) == 1 && ((selectedChessPoint.y() - eventPoint.y()) == 0)) || 
				((eventPoint.x() == selectedChessPoint.x()) && ((selectedChessPoint.y() - eventPoint.y()) == 1)))
				return true;
			else
				return false;
		}
	}
	else
	{
		if (selectedChessPoint.y() >= 5)
		{
			if ((abs(eventPoint.x() - selectedChessPoint.x()) == 1 && (eventPoint.y() - selectedChessPoint.y()) == 0) || 
				((eventPoint.x() == selectedChessPoint.x()) && ((eventPoint.y() - selectedChessPoint.y()) == 1)))
				return true;
			else
				return false;
		}
		else
		{
			if ((eventPoint.x() == selectedChessPoint.x()) && ((eventPoint.y() - selectedChessPoint.y()) == 1))
				return true;
			else
				return false;
		}
	}
}
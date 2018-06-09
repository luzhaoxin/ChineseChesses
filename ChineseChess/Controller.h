#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include<Windows.h>
#include<qobject.h>
#include<qevent.h>
#include<qvector.h>

class Chess;
class MainWindow;
class PcPlayer;
class Controller
{
public:
	Controller(MainWindow* mainWindow);

	void		handleEvent(QMouseEvent* mouseEvent);
	void		handlePcEvent();

	void		checked();										
	bool		isAlreadySelected();
	bool		hasVisibleChess(QPoint point);			//�жϸ�point���Ƿ���Visible����
private:
	bool		isLeagalMove();									//�ж������ڹ������Ƿ���ƶ������λ��
	bool		isVisible(int index);								//��������,���ӿɶ���
	bool		isSelected(int index);
	bool		isSameColor();
	Chess* getSelectedChess();
	Chess	theChess(int index);
	int		getSelectedChessIndex();
	int		getClickedChessIndex(); 
	void		makeChessBeSelected();
	void		pcMoveTo();
	void		moveTo();
	void		eatChess();
	QPoint trans_eventPos_to_chessPoint();

private:
	bool moveJv();
	bool moveMa();
	bool moveXiang();
	bool moveShi();
	bool moveShuai();
	bool movePao();
	bool moveBing();

private:
	QMouseEvent*		mouseEvent;
	QPoint					selectedChessPoint;
	QPoint					eventPoint;
	int						selectedChessIndex;
	int						clickedChessIndex;
public:
	MainWindow*		mainWindow;
	PcPlayer*				pc;
};

#endif // !_CONTROLLER_H
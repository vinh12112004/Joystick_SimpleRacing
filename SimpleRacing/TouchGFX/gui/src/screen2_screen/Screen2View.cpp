#include <gui/screen2_screen/Screen2View.hpp>
#include "cmsis_os.h"

extern osMessageQueueId_t Queue1Handle;
static int updateCounter = 0;
int16_t minX = 65;
int16_t maxX = 190;
int16_t minPos = 0;    // Sửa lại thành 0
int16_t maxPos = 200;
Screen2View::Screen2View()
{
	tickCount = 0;
}

void Screen2View::setupScreen()
{
	localImageX = presenter->GetImageX();
    Screen2ViewBase::setupScreen();
    image1.setX(localImageX);
    lamb.setX(14);
    lamb.setY(0);
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
    presenter->UpdateImageX(localImageX);
}

void Screen2View::handleTickEvent()
{
	Screen2ViewBase::handleTickEvent();

	tickCount++;
	switch (tickCount % 5)
	{
	case 0:
		track0.setVisible(true);
		track4.setVisible(false);
		break;
	case 1:
		track1.setVisible(true);
		track0.setVisible(false);
		break;
	case 2:
		track2.setVisible(true);
		track1.setVisible(false);
		break;
	case 3:
		track3.setVisible(true);
		track2.setVisible(false);
		break;
	case 4:
		track4.setVisible(true);
		track3.setVisible(false);
		break;
	default:
		break;
	}

	lamb.setY(tickCount*2%320);
	lamb.setX(tickCount*2/320%4*60+15);

//	int16_t res = 0;
//	if (osMessageQueueGet(Queue1Handle, &res, NULL, 10) == osOK) {
//	    image1.setX(res);
//	}

	updateCounter++;
	if (updateCounter >= 2) { // Chỉ cập nhật mỗi 3 lần tick
	    int16_t res = 0;
	    if (osMessageQueueGet(Queue1Handle, &res, NULL, 10) == osOK) {
	    	if (res < minX) res = minX;
	    	if (res > maxX) res = maxX;
	    	int16_t mappedX = (res - minX) * (maxPos - minPos) / (maxX - minX) + minPos;
	    	image1.setX(mappedX);
	    }
	    updateCounter = 0;
	}
	int image1X = image1.getX();
	int image1Y = image1.getY();
	int image1W = image1.getWidth();
	int image1H = image1.getHeight();

	int lambX = lamb.getX();
	int lambY = lamb.getY();
	int lambW = lamb.getWidth();
	int lambH = lamb.getHeight();

	bool isCollide = !(image1X + image1W < lambX ||
					   image1X > lambX + lambW ||
					   image1Y + image1H < lambY ||
					   image1Y > lambY + lambH);

	if (isCollide) {
		// Quay về màn hình 1
		application().gotoScreen1ScreenNoTransition();
		return;
	}

	invalidate();
}

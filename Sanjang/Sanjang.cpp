#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>

SceneID scene_start;
TimerID timer_start;
ObjectID object_start;

int start_cnt=1;
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown=true);
const char* countName(int num);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void keyboardCallback(KeyCode keycode, KeyState keystate);

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	scene_start = createScene("시작");

	object_start = createObject("resources/startscene/1.png", scene_start, 300, 300, false);


	startGame(scene_start);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{

}

void timerCallback(TimerID timer)
{
	if (timer == timer_start)
	{
		if (start_cnt < 14)
		{
			setObjectImage(object_start, countName(start_cnt));
			start_cnt++;
			setTimer(timer_start, 0.3f);
			startTimer(timer_start);
		}
		else if (start_cnt == 14)
		{
			hideObject(object_start);
		}
	}
}

void keyboardCallback(KeyCode keycode, KeyState keystate)
{
	if (keycode == KeyCode::KEY_SPACE)
	{
		showObject(object_start);
		timer_start = createTimer(0.3f);
		startTimer(timer_start);
	}
}

const char* countName(int num)
{
	static char buff[50];
	sprintf_s(buff, sizeof(buff), "resources/startscene/%d.png", num);
	return buff;
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown)
{
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	if (shown)
	{
		showObject(object);
	}
	return object;
}
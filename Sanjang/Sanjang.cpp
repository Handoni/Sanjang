#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>

SceneID scene_main, scene_start;
TimerID timer_start;
ObjectID object_start, object_textbox;

int start_cnt=1;
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown=true);
const char* countName(int num);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void keyboardCallback(KeyCode keycode, KeyState keystate);
void mainSetup();
void startSetup();

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	object_textbox = createObject("resources/common/textbox.png");

	scene_start = createScene("Start");

	object_start = createObject("resources/scene_start/1.png", scene_start, 300, 300, false);

	mainSetup();

	startGame(scene_main);
	
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
	sprintf_s(buff, sizeof(buff), "resources/scene_start/%d.png", num);
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

void mainSetup()
{
	scene_main = createScene("Main", "resources/scene_main/background.jpg");
	locateObject(object_textbox, scene_main, 200, 100);
}

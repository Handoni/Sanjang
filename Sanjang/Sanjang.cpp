#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>

#define LINE(n) 200-80*(n-1)

SceneID scene_main, scene_start, scene_1;
SceneID* scene_now;
TimerID timer_start, timer_fadein;
ObjectID object_arrow, object_start, object_textbox;
ObjectID text[5];
SoundID sound_rain;

int cnt=1, line_num, line_now;
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown=true);
const char* countName(int num);
void setObject(ObjectID object, const char* image, SceneID scene, int x, int y);
int main();
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
	object_arrow = createObject("resources/common/arrow.png");

	for (int i = 0; i < 5; i++)
	{
		text[i] = createObject("resources/common/text_idle.png");
	}
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
		if (cnt < 14)
		{
			setObjectImage(object_start, countName(cnt));
			cnt++;
			setTimer(timer_start, 0.3f);
			startTimer(timer_start);
		}
		else if (cnt == 14)
		{
			hideObject(object_start);
			scene_1 = createScene("1", "resources/scene_1/background.jpg");
			enterScene(scene_1);
			setSceneLight(scene_1, 0);
			scene_now = &scene_1;

			cnt = 1;
			timer_fadein = createTimer(0.2f);
			startTimer(timer_fadein);
		}
	}
	if (timer == timer_fadein)
	{
		if (cnt < 10)
		{
			setSceneLight(scene_1, cnt*0.1);
			cnt++;
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		else if (cnt == 10)
		{
			setSceneLight(scene_1, 1.0f);
			locateObject(object_textbox, *scene_now, 0, 0);
			showObject(object_textbox);
			setObject(text[0], "resources/scene_1/text1_line1.png", scene_1, 100, LINE(1));
			setObject(text[1], "resources/scene_1/text1_line2.png", scene_1, 100, LINE(2));
			locateObject(object_arrow, scene_1, 1230, 20);
		}
	}
}

void keyboardCallback(KeyCode keycode, KeyState keystate)
{
	if (scene_now == &scene_main)
	{
		if (keycode == KeyCode::KEY_UP_ARROW && keystate == KeyState::KEY_PRESSED)
		{
			if (line_now != 1)
			{
				line_now--;
				locateObject(object_arrow, scene_main, 50, LINE(line_now) + 25);
			}
		}
		if (keycode == KeyCode::KEY_DOWN_ARROW && keystate == KeyState::KEY_PRESSED)
		{
			if (line_now != line_num)
			{
				line_now++;
				locateObject(object_arrow, scene_main, 50, LINE(line_now) + 25);

			}
		}
		if (keycode == KeyCode::KEY_ENTER && line_now == 1)
		{
			startSetup();
			enterScene(scene_start);
			scene_now = &scene_start;
			showObject(object_start);
			playSound(sound_rain, true);
			cnt = 1;
			timer_start = createTimer(1.0f);
			startTimer(timer_start);
		}
	}
	
}

const char* countName(int num)
{
	static char buff[50];
	sprintf_s(buff, sizeof(buff), "resources/scene_start/%d.png", num);
	return buff;
}

void setObject(ObjectID object, const char* image, SceneID scene, int x=-100, int y=-100)
{
	setObjectImage(object, image);
	if(x!=0 && y!=0)
		locateObject(object, scene, x, y);
	showObject(object);
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
	scene_now = &scene_main;
	scene_main = createScene("Main", "resources/scene_main/background.jpg");
	locateObject(object_textbox, scene_main, 0, 0);
	showObject(object_textbox);
	
	line_num = 2;
	setObject(text[0], "resources/scene_main/text1_line1.png", scene_main, 100, LINE(1));
	setObject(text[1], "resources/scene_main/text1_line2.png", scene_main, 100, LINE(2));

	line_now = 1;
	locateObject(object_arrow, scene_main, 50, LINE(line_now)+25);
	
	showObject(object_arrow);
	
}

void startSetup()
{
	scene_start = createScene("Start");
	object_start = createObject("resources/scene_start/0.png", scene_start, 420, 300, false);
	sound_rain = createSound("resources/scene_start/rainsound.mp3");
}
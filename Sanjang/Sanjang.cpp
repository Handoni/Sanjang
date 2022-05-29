#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>

#define LINE(n) 200-60*(n-1)

SceneID scene_main, scene_start, scene[50];
SceneID* scene_now;
TimerID timer_start, timer_fadein;
ObjectID object_arrow, object_start, object_textbox;
ObjectID text[5];
ObjectID char_madongsuk;
SoundID sound_rain;

int cnt=1, line_num[15], line_now, script_num, script_now=1;
bool on_wait = false;
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown=true);
const char* countName(int num);
const char* scriptName(const char* scene_name, int num, int line);
void setObject(ObjectID object, const char* image, SceneID scene, int x, int y);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void keyboardCallback(KeyCode keycode, KeyState keystate);
void scriptSetup(const char* scene_name, SceneID scene, int num, int line_num);
void mainSetup();
void startSetup();
void sceneSetup(int n);

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
			scene[1] = createScene("1", "resources/scene_1/background.jpg");
			enterScene(scene[1]);
			setSceneLight(scene[1], 0);
			scene_now = &scene[1];

			cnt = 1;
			timer_fadein = createTimer(0.2f);
			startTimer(timer_fadein);
		}
	}
	if (timer == timer_fadein)
	{
		if (cnt < 10)
		{
			setSceneLight(*scene_now, cnt*0.1);
			cnt++;
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		else if (cnt == 10)
		{
			if (scene_now == &scene[1])
			{
				sceneSetup(1);
				
			}
			if (scene_now == &scene[2])
			{
				setSceneLight(scene[2], 1.f);
				locateObject(object_textbox, scene[2], 0, 0);
				locateObject(object_arrow, scene[2], 1230, 20);
				showObject(object_textbox);
				showObject(char_madongsuk);
				script_now = 1;
				scriptSetup("scene_2", scene[2], script_now, line_num[script_now]);
			}
		}
	}
}

void keyboardCallback(KeyCode keycode, KeyState keystate)
{
	if (keycode == KeyCode::KEY_ENTER && keystate == KeyState::KEY_PRESSED && on_wait)
	{
		if (scene_now == &scene_main)
		{
			if (line_now == 1)
			{
				on_wait = false;
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
		for (int i = 1;i<10; i++)
		{
			if (scene_now == &scene[i])
			{
				if (script_now < script_num)
				{
					char buff[20];
					sprintf_s(buff, sizeof(buff), "scene_%d", i);
					script_now++;
					scriptSetup(buff, scene[i], script_now, line_num[script_now]);
				}
				else if (script_now == script_num)
				{
					on_wait = false;
					sceneSetup(i+1);
				}
				break;
			}
		}
	}

	if (keycode == KeyCode::KEY_UP_ARROW && keystate == KeyState::KEY_PRESSED)
	{
		if (scene_now == &scene_main)
		{
			if (line_now != 1)
			{
				line_now--;
				locateObject(object_arrow, scene_main, 50, LINE(line_now) + 25);
			}
		}
	}
	if (keycode == KeyCode::KEY_DOWN_ARROW && keystate == KeyState::KEY_PRESSED)
	{
		if (scene_now == &scene_main)
		{
			if (line_now != line_num[script_now])
			{
				line_now++;
				locateObject(object_arrow, scene_main, 50, LINE(line_now) + 25);

			}
		}
	}
}

const char* countName(int num)
{
	static char buff[50];
	sprintf_s(buff, sizeof(buff), "resources/scene_start/%d.png", num);
	return buff;
}

const char* scriptName(const char* scene_name, int num, int line)
{
	static char buff[100];
	sprintf_s(buff, sizeof(buff), "resources/%s/text%d_line%d.png", scene_name, num, line);
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

void scriptSetup(const char* scene_name, SceneID scene, int num, int line_num)
{
	for (int i = 1; i <= line_num; i++)
	{
		setObject(text[i], scriptName(scene_name, num, i), scene, 100, LINE(i));
	}
	on_wait = true;
}

void mainSetup()
{
	scene_now = &scene_main;
	scene_main = createScene("Main", "resources/scene_main/background.jpg");
	locateObject(object_textbox, scene_main, 0, 0);
	showObject(object_textbox);
	
	script_num = 1;
	script_now = 1;
	line_num[1] = 2;
	scriptSetup("scene_main", scene_main, script_now, line_num[script_now]);

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

void sceneSetup(int n)
{
	switch (n)
	{
	case 1:
		setSceneLight(scene[1], 1.0f);
		locateObject(object_textbox, *scene_now, 0, 0);
		showObject(object_textbox);
		script_num = 3;
		script_now = 1;
		line_num[1] = 3;
		line_num[2] = 4;
		line_num[3] = 4;
		scriptSetup("scene_1", scene[1], script_now, line_num[script_now]);
		on_wait = true;
		locateObject(object_arrow, scene[1], 1230, 20);
		break;
	case 2:
		scene[2] = createScene("2", "resources/scene_2/background.jpg");
		setSceneLight(scene[2], 0);
		enterScene(scene[2]);
		scene_now = &scene[2];
		
		script_num = 5;
		script_now = 1;
		
		line_num[1] = 3;
		line_num[2] = 3;
		line_num[3] = 3;
		line_num[4] = 3;
		line_num[5] = 4;
		cnt = 1;

		char_madongsuk = createObject("resources/common/char_madongsuk.png", scene[2], 100, 300, false);
		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		break;
	case 3:
		scene[3] = createScene("3", "resources/scene_3/background.jpg");
		setSceneLight(scene[3], 0);
		enterScene(scene[3]);
		scene_now = &scene[3];

		script_num = 15;
		script_now = 1;

		line_num[1] = 3; line_num[2] = 3; line_num[3] = 3; line_num[4] = 3; line_num[5] = 4;
		line_num[6] = 3; line_num[7] = 3; line_num[8] = 3; line_num[9] = 3; line_num[10] = 4;
		line_num[11] = 3; line_num[12] = 3; line_num[13] = 3; line_num[14] = 3; line_num[15] = 4;

		cnt = 1;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		break;
	default:
		break;
	}
}
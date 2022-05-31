#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>

#define LINE(n) 200-60*(n-1)

SceneID scene_main, scene_start, scene[50];
SceneID* scene_now;
TimerID timer_start, timer_fadein, timer_fadeout;
ObjectID object_arrow, object_start, object_textbox;
ObjectID text[5];
ObjectID char_madongsuk, char_hansohyee, char_parkboyeong, char_leedohyeon, char_kimjongkuk, char_ohdalsoo, char_husungtae;
SoundID sound_rain, sound_knock, sound_open, sound_close, sound_click, sound_beep, sound_type;

int cnt=1, line_num[65], line_now, script_num, script_now=1, scene_num;
bool on_wait = false, on_select = false;
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown=true);
const char* countName(int num);
const char* scriptName(const char* scene_name, int num, int line);
void setObject(ObjectID object, const char* image, SceneID scene, int x, int y);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void keyboardCallback(KeyCode keycode, KeyState keystate);
void soundCallback(SoundID sound);
void scriptSetup(const char* scene_name, SceneID scene, int num, int line_num, bool select = false);
void mainSetup();
void startSetup();
void sceneSetup(int n);
void nextScript(bool select=false);

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);
	setSoundCallback(soundCallback);

	object_textbox = createObject("resources/common/textbox.png");
	object_arrow = createObject("resources/common/arrow.png");

	sound_knock = createSound("resources/common/door_knock.mp3");
	sound_open = createSound("resources/common/door_open.mp3");
	sound_close = createSound("resources/common/door_close.mp3");
	sound_click = createSound("resources/common/click.mp3");
	sound_beep = createSound("resources/common/beep.mp3");
	sound_type = createSound("resources/common/type.mp3");

	timer_fadein = createTimer(0.2f);
	timer_fadeout = createTimer(0.2f);

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
			if (cnt >= 1 && cnt <= 5 || cnt >= 7 && cnt <= 10)
			{
				stopSound(sound_type);
				playSound(sound_type);
			}
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
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
	}
	else if (timer == timer_fadein)
	{
		if (cnt < 10)
		{
			setSceneLight(*scene_now, cnt*0.1f);
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
				script_now = 1;
				scriptSetup("scene_2", scene[2], script_now, line_num[script_now]);

				playSound(sound_knock);
			}
			if (scene_now == &scene[3])
			{
				setSceneLight(scene[3], 1.f);
				locateObject(object_textbox, scene[3], 0, 0);
				locateObject(object_arrow, scene[3], 1230, 20);
				showObject(object_textbox);
				showObject(char_madongsuk);
				script_now = 1;
				scriptSetup("scene_3", scene[3], script_now, line_num[script_now]);
			}
		}
	}
	else if (timer == timer_fadeout)
	{
		if (cnt > 0)
		{
			setSceneLight(*scene_now, cnt * 0.1f);
			cnt--;
			setTimer(timer_fadeout, 0.2f);
			startTimer(timer_fadeout);
		}
		else if (cnt == 0)
		{
			sceneSetup(scene_num + 1);
		}
	}
}

void keyboardCallback(KeyCode keycode, KeyState keystate)
{
	if (keycode == KeyCode::KEY_ENTER && keystate == KeyState::KEY_PRESSED && on_wait)
	{
		if (on_wait)
		{
			if (on_select)
			{
				if (scene_now == &scene_main)
				{
					if (line_now == 1)
					{
						stopSound(sound_click);
						playSound(sound_click);
						on_wait = false;
						on_select = false;
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
				else if (scene_num == 2 && script_now == 3)
				{
					nextScript();
				}
			}
			else if (scene_num >= 1)
			{
				if (scene_num == 2 && script_now + 1 == 2)
				{
					playSound(sound_open);
					nextScript();
					showObject(char_madongsuk);
				}

				else if (scene_num == 2 && script_now + 1 == 3)
					nextScript(true);

				else if (scene_num == 3 && script_now + 1 == 3)
				{
					locateObject(char_madongsuk, scene[3], 100, 300);
					nextScript();
				}


				else
				{
					if (script_now < script_num)
						nextScript();

					else if (script_now == script_num)
					{
						stopSound(sound_click);
						playSound(sound_click);

						on_wait = false;
						setTimer(timer_fadeout, 0.2f);
						startTimer(timer_fadeout);
					}
				}
			}
		}
	}

	else if (keycode == KeyCode::KEY_UP_ARROW && keystate == KeyState::KEY_PRESSED)
	{
		if (on_select)
		{
			if (line_now != 1)
			{
				stopSound(sound_beep);
				playSound(sound_beep);

				line_now--;
				locateObject(object_arrow, *scene_now, 50, LINE(line_now) + 25);
			}
		}
	}
	else if (keycode == KeyCode::KEY_DOWN_ARROW && keystate == KeyState::KEY_PRESSED)
	{
		if (on_select)
		{
			if (line_now != line_num[script_now])
			{
				stopSound(sound_beep);
				playSound(sound_beep);

				line_now++;
				locateObject(object_arrow, *scene_now, 50, LINE(line_now) + 25);
			}
		}
	}
}

void soundCallback(SoundID sound)
{
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
	if (on_select)
		sprintf_s(buff, sizeof(buff), "resources/%s/select%d_line%d.png", scene_name, num, line);

	else
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

void nextScript(bool select)
{
	stopSound(sound_click);
	playSound(sound_click);
	script_now++;
	char buff[20];
	sprintf_s(buff, sizeof(buff), "scene_%d", scene_num);

	if (select)
	{
		on_select = true;
		scriptSetup(buff, scene[scene_num], script_now, line_num[script_now], true);
	}
	else
	{
		on_select = false;
		scriptSetup(buff, scene[scene_num], script_now, line_num[script_now]);
	}

}

void scriptSetup(const char* scene_name, SceneID scene, int num, int line_num, bool select)
{
	if (select)
	{
		on_select = true;
		for (int i = 1; i <= line_num; i++)
			setObject(text[i], scriptName(scene_name, num, i), scene, 100, LINE(i));

		for (int i = line_num+1; i <= 4; i++)
			hideObject(text[i]);

		line_now = 1;
		locateObject(object_arrow, scene, 50, LINE(line_now) + 25);
	}
	else
	{
		for (int i = 1; i <= line_num; i++)
			setObject(text[i], scriptName(scene_name, num, i), scene, 100, LINE(i));

		for (int i = line_num + 1; i <= 4; i++)
			hideObject(text[i]);

		locateObject(object_arrow, scene, 1230, 20);
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
	scriptSetup("scene_main", scene_main, script_now, line_num[script_now], true);

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
		scene_num = 1;
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
		scene_num = 2;
		scene[2] = createScene("2", "resources/scene_2/background.jpg");
		setSceneLight(scene[2], 0);
		enterScene(scene[2]);
		scene_now = &scene[2];
		
		script_num = 5;
		script_now = 1;
		
		line_num[1] = 2;
		line_num[2] = 3;
		line_num[3] = 2;
		line_num[4] = 3;
		line_num[5] = 4;

		char_madongsuk = createObject("resources/common/char_madongsuk.png", scene[2], 100, 300, false);

		cnt = 1;
		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		break;
	case 3:
		scene_num = 3;
		scene[3] = createScene("3", "resources/scene_3/background.jpg");
		setSceneLight(scene[3], 0);
		enterScene(scene[3]);
		scene_now = &scene[3];

		script_num = 4;
		script_now = 1;

		line_num[1] = 4; line_num[2] = 3; line_num[3] = 3; line_num[4] = 4; 

		cnt = 1;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		break;
	case 4:
		scene_num = 4;
		scene[4] = createScene("4", "resources/scene_4/background.png");


		line_num[1] = 4; line_num[2] = 2; line_num[3] = 4; line_num[4] = 4; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4; line_num[9] = 3; line_num[10] = 3; line_num[11] = 3;

		char_hansohyee = createObject("resources/common/char_hansohyee.png");
		char_parkboyeong = createObject("resources/common/char_parkboyeong.png");
		char_leedohyeon = createObject("resources/common/char_leedohyeon.png");
		char_kimjongkuk = createObject("resources/common/char_kimjongkuk.png");
		char_ohdalsoo = createObject("resources/common/char_ohdalsoo.png"); 
	default:
		break;
	}
}
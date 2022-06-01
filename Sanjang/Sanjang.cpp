#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>
#include <string.h>

#define LINE(n) 200-60*(n-1)

struct dataset {
	int scene_num;
} file_data;

SceneID scene_main, scene_start, scene[50];
SceneID saving;
SceneID* scene_now;
TimerID timer_start, timer_fadein, timer_fadeout, timer_saving;
ObjectID object_arrow, object_start, object_textbox, save_text;
ObjectID text[5];
ObjectID char_madongsuk, char_hansohyee, char_parkboyeong, char_leedohyeon, char_kimjongkuk, char_husungtae, char_kimyoonsuk;
SoundID sound_rain, sound_knock, sound_open, sound_close, sound_click, sound_beep, sound_type;

int cnt=1, line_num[65], line_now, script_num, script_now=1, scene_num;
bool on_wait = false, on_select = false, on_load = false;
FILE* savedata;


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
void nextScript(bool select = false);
void sceneStart();
void saveGame();
void loadGame();
int loadData();

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

	sound_rain = createSound("resources/scene_start/rainsound.mp3");

	char_madongsuk = createObject("resources/common/char_madongsuk.png");
	char_hansohyee = createObject("resources/common/char_hansohyee.png");
	char_parkboyeong = createObject("resources/common/char_parkboyeong.png");
	char_leedohyeon = createObject("resources/common/char_leedohyeon.png");
	char_kimjongkuk = createObject("resources/common/char_kimjongkuk.png");
	char_kimyoonsuk = createObject("resources/common/char_kimyoonsuk.png");
	char_husungtae = createObject("resources/common/char_husungtae.png");

	timer_fadein = createTimer(0.2f);
	timer_fadeout = createTimer(0.2f);

	saving = createScene("saving");

	for (int i = 0; i < 5; i++)
	{
		text[i] = createObject("resources/common/text_idle.png");
	}

	if(loadData()==1)
		return 1;

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
			if (scene_num >= 2 && scene_num <= 10)
			{
				sceneStart();
				if (scene_now == &scene[2])
				{
					playSound(sound_knock);
				}
				else if (scene_now == &scene[3])
				{
				}
				else if (scene_now == &scene[4])
				{
				}
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
			if (scene_num == 4)
				saveGame();

			else
				sceneSetup(scene_num + 1);
		}
	}

	else if (timer == timer_saving)
	{
		sceneSetup(scene_num + 1);
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
						scene_num = 0;
						saveGame();

						playSound(sound_rain, true);
						cnt = 1;
						timer_start = createTimer(1.0f);
						startTimer(timer_start);
					}
					if (line_now == 2)
					{
						stopSound(sound_click);
						playSound(sound_click);
						on_wait = false;
						on_select = false;
						loadGame();
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
					locateObject(char_madongsuk, scene[2], 100, 300);
					showObject(char_madongsuk);
				}

				else if (scene_num == 2 && script_now + 1 == 3)
					nextScript(true);

				else if (scene_num == 3 && script_now + 1 == 3)
				{
					locateObject(char_madongsuk, scene[3], 100, 300);
					nextScript();
				}

				else if (scene_num == 4 && script_now + 1 >= 2 && script_now <= 10)
				{
					nextScript();
					if (script_now == 2)
					{
						locateObject(char_madongsuk, scene[4], 100, 300);
					}
					else if (script_now == 3)
						hideObject(char_madongsuk);
					else if (script_now == 4)
					{
						hideObject(char_madongsuk);
						locateObject(char_hansohyee, scene[4], 100, 300);
						showObject(char_hansohyee);
					}
					else if (script_now == 5)
					{
						hideObject(char_hansohyee);
						locateObject(char_parkboyeong, scene[4], 100, 300);
						showObject(char_parkboyeong);
					}
					else if (script_now == 6)
					{
						hideObject(char_parkboyeong);
						locateObject(char_leedohyeon, scene[4], 100, 300);
						showObject(char_leedohyeon);
					}
					else if (script_now == 7)
					{
						hideObject(char_leedohyeon);
						locateObject(char_kimjongkuk, scene[4], 100, 300);
						showObject(char_kimjongkuk);
					}
					else if (script_now == 8)
					{
						hideObject(char_kimjongkuk);
						locateObject(char_kimyoonsuk, scene[4], 100, 300);
						showObject(char_kimyoonsuk);
					}
					else if (script_now == 9)
					{
						hideObject(char_kimyoonsuk);
						locateObject(char_husungtae, scene[4], 100, 300);
						showObject(char_husungtae);
					}
					else if (script_now == 10)
					{
						hideObject(char_husungtae);
						locateObject(char_madongsuk, scene[4], 100, 300);
						showObject(char_madongsuk);
					}
				}
				else if (scene_num == 5 && script_now + 1 >= 3 && script_now + 1 <= 8)
				{
					nextScript();
					if (script_now == 3)
					{
						locateObject(char_hansohyee, scene[5], 100, 300);
						showObject(char_hansohyee);
					}
					else if (script_now == 4)
					{
						hideObject(char_hansohyee);
						locateObject(char_madongsuk, scene[5], 100, 300);
						showObject(char_madongsuk);
					}
					else if (script_now == 5)
					{
						hideObject(char_madongsuk);
						locateObject(char_leedohyeon, scene[5], 100, 300);
						showObject(char_leedohyeon);
					}
					else if (script_now == 6)
					{
						hideObject(char_leedohyeon);
						locateObject(char_madongsuk, scene[5], 100, 300);
						showObject(char_madongsuk);
					}
					else if (script_now == 7)
					{
						hideObject(char_madongsuk);
						locateObject(char_parkboyeong, scene[5], 100, 300);
						showObject(char_parkboyeong);
					}
					else if (script_now == 8)
					{
						hideObject(char_parkboyeong);
						locateObject(char_madongsuk, scene[5], 100, 300);
						showObject(char_madongsuk);
					}
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
	
	if (file_data.scene_num == 0)
	{
		setObjectImage(text[2], "resources/scene_main/select1_line2_nodata.png");
		line_num[1] = 1;
	}

	showObject(object_arrow);
}

void startSetup()
{
	scene_start = createScene("Start");
	object_start = createObject("resources/scene_start/0.png", scene_start, 420, 300, false);
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


		cnt = 1;

		if (!on_load) 
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}

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
		if (!on_load)
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		break;
	case 4:
		scene_num = 4;
		scene[4] = createScene("4", "resources/scene_4/background.jpg");
		setSceneLight(scene[4], 0);
		enterScene(scene[4]);
		scene_now = &scene[4];

		script_num = 11;
		script_now = 1;


		line_num[1] = 4; line_num[2] = 2; line_num[3] = 4; line_num[4] = 4; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4; line_num[9] = 3; line_num[10] = 3; line_num[11] = 3;


		cnt = 1;
		if (!on_load)
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		break;
	case 5:
		scene_num = 5;
		scene[5] = createScene("5", "resources/scene_5/background.jpg");
		setSceneLight(scene[5], 0);
		enterScene(scene[5]);
		scene_now = &scene[5];

		script_num = 9;
		script_now = 1;


		line_num[1] = 2; line_num[2] = 4; line_num[3] = 2; line_num[4] = 3; line_num[5] = 2; line_num[6] = 3;
		line_num[7] = 3; line_num[8] = 4; line_num[9] = 2;

		cnt = 1;
		if (!on_load)
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		break;
	case 6:
		scene_num = 6;
		scene[6] = createScene("6", "resources/scene_6/background.jpg");
		setSceneLight(scene[6], 0);
		enterScene(scene[6]);
		scene_now = &scene[6];

		script_num = 8;
		script_now = 1;


		line_num[1] = 2; line_num[2] = 2; line_num[3] = 2; line_num[4] = 3; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4;

		if (!on_load)
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		cnt = 1;
	case 7:
		scene_num = 7;
		scene[7] = createScene("7", "resources/scene_7/background.jpg");
		setSceneLight(scene[7], 0);
		enterScene(scene[7]);
		scene_now = &scene[7];

		//line 정보 수정 필요

		script_num = 8;
		script_now = 1;

		line_num[1] = 2; line_num[2] = 2; line_num[3] = 2; line_num[4] = 3; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4;

		if (!on_load)
		{
			setTimer(timer_fadein, 0.2f);
			startTimer(timer_fadein);
		}
		cnt = 1;
	default:
		break;
	}
}

void sceneStart()
{
	char buff[20];
	setSceneLight(scene[scene_num], 1.f);
	locateObject(object_textbox, scene[scene_num], 0, 0);
	locateObject(object_arrow, scene[scene_num], 1230, 20);
	showObject(object_textbox);
	script_now = 1;
	sprintf_s(buff, sizeof(buff), "scene_%d", scene_num);
	scriptSetup(buff, scene[scene_num], script_now, line_num[script_now]);
}

void saveGame()
{
	if(scene_num>=1)
	{
		enterScene(saving);

		save_text = createObject("resources/saving/saving.png", saving, 100, LINE(1), true);
		locateObject(object_textbox, saving, 0, 0);

		timer_saving = createTimer(2.f);
		startTimer(timer_saving);
	}

	file_data.scene_num = scene_num;

	FILE* temp;
	fopen_s(&temp, "resources/tempdata.dat", "wt");

	fprintf(temp, "scene_num=%d\n", file_data.scene_num);
	fclose(savedata);
	fclose(temp);
	remove("resources/data.dat");
	rename("resources/tempdata.dat", "resources/data.dat");
	loadData();


}

int loadData()
{
	
	int file_state = fopen_s(&savedata, "resources/data.dat", "r+t");
	if (file_state != 0)
	{
		return 1;
	}

	fscanf_s(savedata, "%*[^=]%*c%d\n", &file_data.scene_num);

	return 0;
}

void loadGame()
{
	scene_num = file_data.scene_num + 1;
	sceneSetup(scene_num);
	on_select = false;
	if (scene_num == 5)
	{
		playSound(sound_rain, true);
	}
}
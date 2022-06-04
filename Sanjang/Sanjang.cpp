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
ObjectID object_arrow, object_start, object_textbox, save_text, object_title;
ObjectID object_detectfile,icon_file;
ObjectID object_deadbody, object_namecard, object_carrier, object_knives;
ObjectID object_surgerytool, object_drawer, object_wallet, object_knife, object_gloves;
ObjectID object_backpack, object_notebook, object_notebookpad, object_student, object_card, object_gps, object_dadpicture;
ObjectID object_bigbackpack, object_note, object_ring;
ObjectID object_tool, object_file, object_note_detective, object_exercisetool;
ObjectID object_girlclothes;
ObjectID object_picture[6];
ObjectID text[5];
ObjectID char_madongsuk, char_hansohyee, char_parkboyeong, char_leedohyeon, char_kimjongkuk, char_husungtae, char_kimyoonsuk;
ObjectID icons[20];
ObjectID* char_now;
SoundID sound_rain, sound_detecting, sound_knock, sound_open, sound_close, sound_click, sound_beep, sound_type, sound_doong, sound_screaming;

int cnt = 1, line_num[65], line_now, script_num, script_now = 1, scene_num;
bool on_wait = false, on_select = false, on_load = false, on_click = false;
bool checked[5] = { 0, }, selected[5] = { 0, };
bool showIcons[20] = { 0, };
bool showing_Icons = false;
FILE* savedata;


ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true);
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
bool judgeChecked(const char* scene_name, int objectnum, int scriptnum);
void setClick();
bool judgeSelected(const char* scene_name, int selectnum, int scriptnum);
void setSelect();
void selectedObject();
void setIcons();

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);
	setSoundCallback(soundCallback);

	object_title = createObject("resources/scene_main/title.png");
	object_textbox = createObject("resources/common/textbox.png");
	object_arrow = createObject("resources/common/arrow.png");

	icon_file = createObject("resources/icons/icon_file.png");
	object_detectfile = createObject("resources/common/detectfile.png");

	sound_knock = createSound("resources/common/door_knock.mp3");
	sound_open = createSound("resources/common/door_open.mp3");
	sound_close = createSound("resources/common/door_close.mp3");
	sound_click = createSound("resources/common/click.mp3");
	sound_beep = createSound("resources/common/beep.mp3");
	sound_type = createSound("resources/common/type.mp3");

	sound_rain = createSound("resources/scene_start/rainsound.mp3");
	sound_detecting = createSound("resources/common/detecting.mp3");

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

	setIcons();

	for (int i = 0; i < 5; i++)
	{
		text[i] = createObject("resources/common/text_idle.png");
	}

	for (int i = 1; i <= 5; i++)
	{
		char buff[50];
		sprintf_s(buff, sizeof(buff), "resources/scene_%d/object_picture_%d.png", i + 7, i);
		object_picture[i] = createObject(buff);
	}

	if (loadData() == 1)
		return 1;

	mainSetup();

	startGame(scene_main);

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
			setSceneLight(*scene_now, cnt * 0.1f);
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
			if (scene_num >= 2 && scene_num <= 12)
			{
				sceneStart();
				if (scene_now == &scene[2])
				{
					playSound(sound_knock);
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
			if (scene_num == 4 || scene_num >= 6 && scene_num <= 11)
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
		if (on_wait && !showing_Icons)
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
				else if (scene_num == 8)
				{
					if (selected[line_now] == false)
					{
						stopSound(sound_click);
						playSound(sound_click);

						selected[line_now] = true;
						if (line_now == 1)	script_now = 7;
						else if (line_now == 2)	script_now = 11;
						else if (line_now == 3)	script_now = 13;
						else if (line_now == 4)	script_now = 17;
						on_select = false;
						scriptSetup("scene_8", scene[8], script_now, line_num[script_now], false);
					}
				}
				else if (scene_num == 9)
				{
					if (selected[line_now] == false)
					{
						stopSound(sound_click);
						playSound(sound_click);

						selected[line_now] = true;
						if (line_now == 1)	script_now = 8;
						else if (line_now == 2)	script_now = 11;
						else if (line_now == 3)	script_now = 14;
						else if (line_now == 4)	script_now = 16;
						on_select = false;
						scriptSetup("scene_9", scene[9], script_now, line_num[script_now], false);
					}
				}
				else if (scene_num == 10)
				{
					if (selected[line_now] == false)
					{
						stopSound(sound_click);
						playSound(sound_click);

						selected[line_now] = true;
						if (line_now == 1)	script_now = 8;
						else if (line_now == 2)	script_now = 12;
						else if (line_now == 3)	script_now = 16;
						else if (line_now == 4)	script_now = 20;
						on_select = false;
						scriptSetup("scene_10", scene[10], script_now, line_num[script_now], false);
					}
				}
				else if (scene_num == 11)
				{
					if (selected[line_now] == false)
					{
						stopSound(sound_click);
						playSound(sound_click);

						selected[line_now] = true;
						if (line_now == 1)	script_now = 7;
						else if (line_now == 2)	script_now = 9;
						else if (line_now == 3)	script_now = 12;
						else if (line_now == 4)	script_now = 14;
						on_select = false;
						scriptSetup("scene_11", scene[11], script_now, line_num[script_now], false);
					}
				}
				else if (scene_num == 12)
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
				{
					nextScript(true);
				}

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

				else if (scene_num == 6 && script_now + 1 == 2)
				{
					playSound(sound_open);
					on_wait = false;


					setSceneImage(scene[6], "resources/scene_6/background_black.png");
					hideObject(object_textbox);
					hideObject(object_arrow);
					for (int i = 1; i <= 4; i++)
					{
						setObjectImage(text[i], "resources/common/text_idle.png");
					}

				}

				else if (scene_num == 7 && script_now + 1 == 3)
				{
					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[7], 100, LINE(4));
				}

				else if (scene_num == 7 && (script_now == 5 || script_now == 6 || script_now == 7 || script_now == 8))
				{
					stopSound(sound_click);
					playSound(sound_click);

					setClick();
				}

				else if (scene_num == 7 && script_now == -1)
				{
					stopSound(sound_click);
					playSound(sound_click);
					setClick();
				}

				else if (scene_num == 8 && script_now + 1 == 2)
				{
					stopSound(sound_click);
					playSound(sound_click);

					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
				}
				else if (scene_num == 8 && (script_now == 2 || script_now == 3 || script_now == 4))
				{
					hideObject(object_knife);
					hideObject(object_gloves);
					hideObject(object_picture[1]);

					stopSound(sound_click);
					playSound(sound_click);

					setClick();
				}
				else if (scene_num == 8 && script_now + 1 == 6)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(*char_now, scene[scene_num], 100, 300);
					showObject(*char_now);
					nextScript(true);
				}
				else if (scene_num == 8 && (script_now == 10 || script_now == 12 || script_now == 14 || script_now == 18))
				{
					stopSound(sound_click);
					playSound(sound_click);

					setSelect();
				}
				else if (scene_num == 8 && script_now == -1)
				{
					stopSound(sound_click);
					playSound(sound_click);
					setClick();
				}

				else if (scene_num == 9 && script_now + 1 == 2)
				{
					stopSound(sound_click);
					playSound(sound_click);

					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
				}
				else if (scene_num == 9 && (script_now == 2 || script_now == 3 || script_now == 4 || script_now == 5))
				{
					hideObject(object_card);
					hideObject(object_gps);
					hideObject(object_dadpicture);
					hideObject(object_student);
					hideObject(object_picture[2]);

					stopSound(sound_click);
					playSound(sound_click);

					setClick();
				}
				else if (scene_num == 9 && script_now + 1 == 7)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(*char_now, scene[scene_num], 100, 300);
					showObject(*char_now);
					nextScript(true);
				}
				else if (scene_num == 9 && (script_now == 10 || script_now == 13 || script_now == 15 || script_now == 17))
				{
				
					stopSound(sound_click);
					playSound(sound_click);

					setSelect();
				}
				else if (scene_num == 9 && script_now == -1)
				{
					stopSound(sound_click);
					playSound(sound_click);
					setClick();
				}

				else if (scene_num == 10 && script_now + 1 == 2)
				{
					stopSound(sound_click);
					playSound(sound_click);

					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
				}
				else if (scene_num == 10 && (script_now == 2 || script_now == 3 || script_now == 4 || script_now == 5))
				{
					hideObject(object_ring);
					hideObject(object_picture[3]);

					stopSound(sound_click);
					playSound(sound_click);

					setClick();
				}
				else if (scene_num == 10 && script_now + 1 == 7)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(*char_now, scene[scene_num], 100, 300);
					showObject(*char_now);
					nextScript(true);
				}
				else if (scene_num == 10 && (script_now == 11 || script_now == 15 || script_now == 19 || script_now == 21))
				{
					stopSound(sound_click);
					playSound(sound_click);

					setSelect();
				}
				else if (scene_num == 10 && script_now == -1)
				{
					stopSound(sound_click);
					playSound(sound_click);
					setClick();
				}

				else if (scene_num == 11 && script_now + 1 == 2)
				{
					stopSound(sound_click);
					playSound(sound_click);

					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
				}
				else if (scene_num == 11 && (script_now == 2 || script_now == 3 || script_now == 4))
				{
				hideObject(object_exercisetool);
				hideObject(object_picture[4]);
					stopSound(sound_click);
					playSound(sound_click);

					setClick();
				}
				else if (scene_num == 11 && script_now + 1 == 6)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(*char_now, scene[scene_num], 100, 300);
					showObject(*char_now);
					nextScript(true);
				}
				else if (scene_num == 11 && (script_now == 8 || script_now == 11 || script_now == 13 || script_now == 15))
				{
					stopSound(sound_click);
					playSound(sound_click);

					setSelect();
				}
				else if (scene_num == 11 && script_now == -1)
				{
					stopSound(sound_click);
					playSound(sound_click);
					setClick();
				}

				else if (scene_num == 12 && script_now + 1 == 3)
				{

					stopSound(sound_click);
					playSound(sound_click);

					on_click = true;

					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					hideObject(object_arrow);
					setObjectImage(object_textbox, "resources/common/textbox_half.png");
					setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
				}
				else if (scene_num == 12 && script_now + 1 == 5)
				{
					stopSound(sound_click);
					playSound(sound_click);

					hideObject(object_picture[4]);

					locateObject(*char_now, scene[scene_num], 100, 300);
					showObject(*char_now);
					nextScript(true);
				}
				else if (scene_num == 12 && script_now + 1 == 7)
				{
					for (int i = 1; i <= 4; i++)
					{
						hideObject(text[i]);
					}
					script_now = -1;
					hideObject(char_parkboyeong);
					hideObject(object_arrow);
					stopSound(sound_detecting);
					playSound(sound_screaming);
					setObject(text[2], "resources/scene_12/text7_line1.png", scene[scene_num], 100, LINE(2));
				}
				else if (scene_num == 12 && script_now == -1) {}

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

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (!showing_Icons)
	{
		if (scene_num == 7 && on_click)
		{
			if (object == object_deadbody)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					checked[0] = true;
					script_now = 3;
					on_click = false;
					showIcons[0] = true;
					scriptSetup("scene_7", scene[7], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
			else if (object == object_namecard)
			{
				if (checked[1] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[2] = true;
					checked[1] = true;
					script_now = 6;
					on_click = false;
					scriptSetup("scene_7", scene[7], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}

			}
			else if (object == object_carrier)
			{
				if (checked[2] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[3] = true;
					checked[2] = true;
					script_now = 7;
					on_click = false;
					scriptSetup("scene_7", scene[7], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}

			else if (object == object_knives)
			{
				if (checked[3] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[1] = true;
					checked[3] = true;
					script_now = 8;
					on_click = false;
					scriptSetup("scene_7", scene[7], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}

		}
		else if (scene_num == 8 && on_click)
		{
			if (object == object_surgerytool)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[4] = true;
					checked[0] = true;
					script_now = 2;
					on_click = false;
					scriptSetup("scene_8", scene[8], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
			else if (object == object_drawer)
			{
				if (checked[1] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					showObject(object_knife);
					showObject(object_gloves);
					showIcons[5] = true;
					checked[1] = true;
					script_now = 3;
					on_click = false;
					scriptSetup("scene_8", scene[8], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
			else if (object == object_wallet)
			{
				if (checked[2] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(object_picture[1], scene[8], 500, 300);
					showObject(object_picture[1]);


					checked[2] = true;
					script_now = 4;
					on_click = false;


					scriptSetup("scene_8", scene[8], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
		}
		else if (scene_num == 9 && on_click)
		{
			if (object == object_backpack)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					showObject(object_student);

					checked[0] = true;
					script_now = 2;
					on_click = false;
					scriptSetup("scene_9", scene[9], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}

			}
			else if (object == object_notebook)
			{
				if (checked[1] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					showObject(object_card);
					showIcons[6] = true;
					checked[1] = true;
					script_now = 3;
					on_click = false;
					scriptSetup("scene_9", scene[9], script_now, line_num[script_now], false);
				}
				else if (checked[2] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					showObject(object_gps);
					checked[2] = true;
					script_now = 4;
					on_click = false;
					scriptSetup("scene_9", scene[9], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
			else if (object == object_notebookpad)
			{
				if (checked[3] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[7] = true;
					locateObject(object_picture[2], scene[9], 400, 300);
					showObject(object_picture[2]);
					showObject(object_dadpicture);

					checked[3] = true;
					script_now = 5;
					on_click = false;
					scriptSetup("scene_9", scene[9], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
		}
		else if (scene_num == 10 && on_click)
		{
			if (object == object_bigbackpack)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[8] = true;
					showIcons[9] = true;
					showObject(object_ring);
					checked[0] = true;
					script_now = 2;
					on_click = false;
					scriptSetup("scene_10", scene[10], script_now, line_num[script_now], false);
				}
				else if (checked[1] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					checked[1] = true;
					script_now = 3;
					on_click = false;
					scriptSetup("scene_10", scene[10], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}

			}
			else if (object == object_note)
			{
				if (checked[2] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[10] = true;
					checked[2] = true;
					script_now = 4;
					on_click = false;
					scriptSetup("scene_10", scene[10], script_now, line_num[script_now], false);
				}
				else if (checked[3] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(object_picture[3], scene[10], 500, 300);
					showObject(object_picture[3]);

					checked[3] = true;
					script_now = 5;
					on_click = false;
					scriptSetup("scene_10", scene[10], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
		}
		else if (scene_num == 11 && on_click)
		{
			if (object == object_tool)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					showObject(object_exercisetool);
					showIcons[11] = true;
					checked[0] = true;
					script_now = 2;
					on_click = false;
					scriptSetup("scene_11", scene[11], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}

			}
			else if (object == object_note_detective)
			{
				if (checked[1] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[12] = true;
					checked[1] = true;
					script_now = 3;
					on_click = false;
					scriptSetup("scene_11", scene[11], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
			else if (object == object_file)
			{
				if (checked[2] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);

					locateObject(object_picture[4], scene[11], 550, 300);
					showObject(object_picture[4]);
					showIcons[13] = true;
					checked[2] = true;
					script_now = 4;
					on_click = false;
					scriptSetup("scene_11", scene[11], script_now, line_num[script_now], false);
				}
				else
				{
					selectedObject();
				}
			}
		}
		else if (scene_num == 12 && on_click)
		{
			if (object == object_carrier)
			{
				if (checked[0] == false)
				{
					stopSound(sound_click);
					playSound(sound_click);
					showIcons[14] = true;
					checked[0] = true;
					script_now = 3;
					on_click = false;
					scriptSetup("scene_12", scene[12], script_now, line_num[script_now], false);

					locateObject(object_picture[5], scene[12], 550, 300);
					showObject(object_picture[5]);
				}
			}
		}
	}
	if (object == icon_file)
	{
		if (!showing_Icons)
		{
			locateObject(object_detectfile, scene[scene_num], 400, 300);
			showObject(object_detectfile);
			showing_Icons = true;
			for (int i = 0; i <= 14; i++)
			{
				if (showIcons[i])
				{
					locateObject(icons[i], scene[scene_num], 500 + (i % 4) * 100, 600 - (i / 4) * 100);
					showObject(icons[i]);
				}
			}
		}
		else
		{
			hideObject(object_detectfile);
			for (int i = 0; i <= 14; i++)
			{
				locateObject(icons[i], scene[scene_num], 0, 0);
				if (showIcons[i])
					hideObject(icons[i]);
			}
			showing_Icons = false;
			
		}
	}
}

void soundCallback(SoundID sound)
{
	if (sound == sound_open && scene_num == 6)
	{
		playSound(sound_doong);

		locateObject(object_deadbody, scene[6], 400, 200);
		showObject(object_deadbody);
	}
	if (sound == sound_doong)
	{
		stopSound(sound_rain);
		playSound(sound_detecting, true);
		locateObject(object_textbox, scene[6], 0, 0);
		showObject(object_textbox);
		showObject(object_arrow);


		setSceneImage(scene[6], "resources/scene_6/background2.jpg");
		nextScript();
		locateObject(char_madongsuk, scene[6], 100, 300);
		showObject(char_madongsuk);
	}
	if (sound == sound_screaming)
	{
		startTimer(timer_fadeout);
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
	if (on_select)
		sprintf_s(buff, sizeof(buff), "resources/%s/select%d_line%d.png", scene_name, num, line);

	else
		sprintf_s(buff, sizeof(buff), "resources/%s/text%d_line%d.png", scene_name, num, line);
	return buff;
}

void setObject(ObjectID object, const char* image, SceneID scene, int x, int y)
{
	setObjectImage(object, image);
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
	setObjectImage(object_textbox, "resources/common/textbox.png");
	if (select)
	{
		on_select = true;
		for (int i = 1; i <= line_num; i++)
		{
			setObject(text[i], scriptName(scene_name, num, i), scene, 100, LINE(i));
			showObject(text[i]);
		}

		for (int i = line_num + 1; i <= 4; i++)
			hideObject(text[i]);

		line_now = 1;
		locateObject(object_arrow, scene, 50, LINE(line_now) + 25);
		showObject(object_arrow);
	}
	else
	{
		for (int i = 1; i <= line_num; i++)
			setObject(text[i], scriptName(scene_name, num, i), scene, 100, LINE(i));

		for (int i = line_num + 1; i <= 4; i++)
			hideObject(text[i]);

		locateObject(object_arrow, scene, 1230, 20);
		showObject(object_arrow);
	}
	on_wait = true;

}

void mainSetup()
{
	scene_now = &scene_main;
	scene_main = createScene("Main", "resources/scene_main/background.jpg");
	locateObject(object_textbox, scene_main, 0, 0);
	showObject(object_textbox);
	locateObject(object_title, scene_main, 240, 350);
	showObject(object_title);

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

		line_num[1] = 2; line_num[2] = 3; line_num[3] = 2; line_num[4] = 3; line_num[5] = 4;

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
		scene[4] = createScene("4", "resources/scene_4/background.jpg");
		setSceneLight(scene[4], 0);
		enterScene(scene[4]);
		scene_now = &scene[4];

		script_num = 11;
		script_now = 1;


		line_num[1] = 4; line_num[2] = 2; line_num[3] = 4; line_num[4] = 4; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4; line_num[9] = 3; line_num[10] = 3; line_num[11] = 3;


		cnt = 1;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

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

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		break;
	case 6:
		scene_num = 6;
		scene[6] = createScene("6", "resources/scene_6/background.jpg");
		setSceneLight(scene[6], 0);
		enterScene(scene[6]);
		scene_now = &scene[6];
		object_deadbody = createObject("resources/scene_6/object_deadbody.png");
		sound_doong = createSound("resources/scene_6/sound_doong.mp3");

		script_num = 8;
		script_now = 1;


		line_num[1] = 2; line_num[2] = 2; line_num[3] = 2; line_num[4] = 3; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 4;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		cnt = 1;
		break;
	case 7:
		scene_num = 7;
		scene[7] = createScene("7", "resources/scene_7/background.jpg");
		setSceneLight(scene[7], 0);
		enterScene(scene[7]);
		scene_now = &scene[7];

		locateObject(icon_file, scene[7], 30, 630);
		showObject(icon_file);

		object_namecard = createObject("resources/scene_7/object_namecard.png");
		locateObject(object_namecard, scene[7], 1000, 300);
		showObject(object_namecard);


		object_deadbody = createObject("resources/scene_6/object_deadbody.png");
		setObject(object_deadbody, "resources/scene_7/object_deadbody_onclick.png", scene[7], 0, 0);

		object_carrier = createObject("resources/scene_7/object_carrier.png");
		locateObject(object_carrier, scene[7], 650, 200);
		showObject(object_carrier);

		object_knives = createObject("resources/scene_7/object_knives.png");
		locateObject(object_knives, scene[7], 450, 200);
		showObject(object_knives);

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
		}
		script_num = 10;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 4; line_num[3] = 2; line_num[4] = 2; line_num[5] = 4; line_num[6] = 4;
		line_num[7] = 4; line_num[8] = 3; line_num[9] = 3, line_num[10] = 3;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		cnt = 1;
		break;
	case 8:
		scene_num = 8;
		scene[8] = createScene("8", "resources/scene_8/background.jpg");
		setSceneLight(scene[8], 0);
		enterScene(scene[8]);
		scene_now = &scene[8];
		char_now = &char_hansohyee;

		locateObject(icon_file, scene[8], 30, 630);
		showObject(icon_file);

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
			selected[i] = false;
		}
		for (int i = 0; i <= 3; i++)
		{
			showIcons[i] = true;
		}
		object_surgerytool = createObject("resources/scene_8/object_surgerytool.jpg");
		locateObject(object_surgerytool, scene[8], 1000, 300);
		showObject(object_surgerytool);

		object_drawer = createObject("resources/scene_8/object_drawer.png");
		locateObject(object_drawer, scene[8], 600, 200);
		showObject(object_drawer);

		object_wallet = createObject("resources/scene_8/object_wallet.png");
		locateObject(object_wallet, scene[8], 300, 200);
		showObject(object_wallet);

		object_knife = createObject("resources/scene_8/object_knife.png");
		locateObject(object_knife, scene[8], 300, 300);

		object_gloves = createObject("resources/scene_8/object_gloves.png");
		locateObject(object_gloves, scene[8], 600, 300);


		script_num = 19;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 3; line_num[3] = 3; line_num[4] = 3; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 2; line_num[8] = 2; line_num[9] = 2; line_num[10] = 2; line_num[11] = 2; line_num[12] = 2;
		line_num[13] = 2; line_num[14] = 3; line_num[15] = 2; line_num[16] = 3; line_num[17] = 2; line_num[18] = 2; line_num[19] = 2;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);

		cnt = 1;
		break;
	case 9:
		scene_num = 9;
		scene[9] = createScene("9", "resources/scene_9/background.jpg");
		setSceneLight(scene[9], 0);
		enterScene(scene[9]);
		scene_now = &scene[9];
		char_now = &char_leedohyeon;

		locateObject(icon_file, scene[9], 30, 630);
		showObject(icon_file);

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
			selected[i] = false;
		}
		for (int i = 0; i <= 5; i++)
		{
			showIcons[i] = true;
		}

		object_backpack = createObject("resources/scene_9/object_backpack.png");
		locateObject(object_backpack, scene[9], 650, 200);
		showObject(object_backpack);

		object_notebook = createObject("resources/scene_9/object_notebook.png");
		locateObject(object_notebook, scene[9], 200, 200);
		showObject(object_notebook);

		object_notebookpad = createObject("resources/scene_9/object_notebookpad.png");
		locateObject(object_notebookpad, scene[9], 1000, 300);
		showObject(object_notebookpad);

		object_student = createObject("resources/scene_9/object_student.png");
		locateObject(object_student, scene[9], 550, 300);

		object_gps = createObject("resources/scene_9/object_gps.jpg");
		locateObject(object_gps, scene[9], 360, 300);

		object_card = createObject("resources/scene_9/object_card.jpg");
		locateObject(object_card, scene[9], 360, 300);

		object_dadpicture = createObject("resources/scene_9/object_dadpicture.png");
		locateObject(object_dadpicture, scene[9], 700, 300);

		script_num = 18;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 3; line_num[3] = 4; line_num[4] = 3; line_num[5] = 4; line_num[6] = 3;
		line_num[7] = 4; line_num[8] = 2; line_num[9] = 2; line_num[10] = 2; line_num[11] = 2; line_num[12] = 2;
		line_num[13] = 2; line_num[14] = 2; line_num[15] = 2; line_num[16] = 2; line_num[17] = 2; line_num[18] = 2;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		cnt = 1;
		break;
	case 10:
		scene_num = 10;
		scene[10] = createScene("10", "resources/scene_10/background.jpg");
		setSceneLight(scene[10], 0);
		enterScene(scene[10]);
		scene_now = &scene[10];
		char_now = &char_kimyoonsuk;

		locateObject(icon_file, scene[10], 30, 630);
		showObject(icon_file);

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
			selected[i] = false;
		}

		for (int i = 0; i <= 7; i++)
		{
			showIcons[i] = true;
		}

		object_bigbackpack = createObject("resources/scene_10/object_bigbackpack.png");
		locateObject(object_bigbackpack, scene[10], 650, 200);
		showObject(object_bigbackpack);

		object_note = createObject("resources/scene_10/object_note.png");
		locateObject(object_note, scene[10], 1000, 300);
		showObject(object_note);

		object_ring = createObject("resources/scene_10/object_ring.png");
		locateObject(object_ring, scene[10], 550, 300);

		script_num = 22;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 4; line_num[3] = 4; line_num[4] = 4; line_num[5] = 4; line_num[6] = 3;
		line_num[7] = 4; line_num[8] = 2; line_num[9] = 2; line_num[10] = 2; line_num[11] = 3; line_num[12] = 2;
		line_num[13] = 3; line_num[14] = 2; line_num[15] = 2; line_num[16] = 2; line_num[17] = 2; line_num[18] = 2;
		line_num[19] = 2; line_num[20] = 2; line_num[21] = 3; line_num[22] = 2;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		cnt = 1;
		break;
	case 11:
		scene_num = 11;
		scene[11] = createScene("11", "resources/scene_11/background.jpg");
		setSceneLight(scene[11], 0);
		enterScene(scene[11]);
		scene_now = &scene[11];
		char_now = &char_kimjongkuk;

		locateObject(icon_file, scene[11], 30, 630);
		showObject(icon_file);

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
			selected[i] = false;
		}

		for (int i = 0; i <= 10; i++)
		{
			showIcons[i] = true;
		}

		object_tool = createObject("resources/scene_11/object_tool.png");
		locateObject(object_tool, scene[11], 650, 200);
		showObject(object_tool);

		object_file = createObject("resources/scene_11/object_file.png");
		locateObject(object_file, scene[11], 200, 300);
		showObject(object_file);

		object_note_detective = createObject("resources/scene_11/object_note_detective.png");
		locateObject(object_note_detective, scene[11], 1000, 300);
		showObject(object_note_detective);

		object_exercisetool = createObject("resources/scene_11/object_exercisetool.png");
		locateObject(object_exercisetool, scene[11], 550, 300);

		script_num = 16;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 4; line_num[3] = 3; line_num[4] = 4; line_num[5] = 3; line_num[6] = 4;
		line_num[7] = 3; line_num[8] = 2; line_num[9] = 3; line_num[10] = 4; line_num[11] = 3; line_num[12] = 2;
		line_num[13] = 2; line_num[14] = 2; line_num[15] = 3; line_num[16] = 2;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		cnt = 1;
		break;
	case 12:
		scene_num = 12;
		scene[12] = createScene("12", "resources/scene_12/background.jpg");
		setSceneLight(scene[12], 0);
		enterScene(scene[12]);
		scene_now = &scene[12];
		char_now = &char_parkboyeong;

		locateObject(icon_file, scene[12], 30, 630);
		showObject(icon_file);


		for (int i = 0; i <= 13; i++)
		{
			showIcons[i] = true;
		}

		for (int i = 0; i < 5; i++)
		{
			checked[i] = false;
			selected[i] = false;
		}

		object_carrier = createObject("resources/scene_7/object_carrier.png");
		locateObject(object_carrier, scene[12], 650, 200);
		showObject(object_carrier);

		object_girlclothes = createObject("resources/scene_12/object_girlclothes.png");
		locateObject(object_carrier, scene[12], 650, 200);

		sound_screaming = createSound("resources/scene_12/screaming.mp3");

		script_num = 7;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 4; line_num[3] = 3; line_num[4] = 4;
		line_num[5] = 3; line_num[6] = 2; line_num[7] = 1;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		cnt = 1;
		break;
	case 13:
		scene_num = 13;
		scene[13] = createScene("13", "resources/scene_13/background.jpg");
		setSceneLight(scene[13], 0);
		enterScene(scene[13]);
		scene_now = &scene[13]; 
		script_num = 62;
		script_now = 1;

		line_num[1] = 1; line_num[2] = 4; line_num[3] = 3; line_num[4] = 4;
		line_num[5] = 3; line_num[6] = 2; line_num[7] = 1;

		setTimer(timer_fadein, 0.2f);
		startTimer(timer_fadein);
		cnt = 1;
		break;
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
	if (scene_num >= 1)
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
	else
	{
		playSound(sound_detecting, true);
	}
}

bool judgeChecked(const char* scene_name, int objectnum, int scriptnum)
{
	int i = 0;
	for (i = 0; i < objectnum; i++)
	{
		if (checked[i] == false)
			break;
	}
	if (i == objectnum)
	{
		script_now = scriptnum;
		scriptSetup(scene_name, scene[scene_num], script_now, line_num[script_now], false);
		return true;
	}
	else
	{
		return false;
	}
}

void setClick()
{
	char scene_name[50];
	sprintf_s(scene_name, sizeof(scene_name), "scene_%d", scene_num);
	bool judged = false;
	if (scene_num == 7)
	{
		judged = judgeChecked(scene_name, 4, 9);
	}
	else if (scene_num == 8)
	{
		judged = judgeChecked(scene_name, 3, 5);
	}
	else if (scene_num == 9)
	{
		judged = judgeChecked(scene_name, 4, 6);
	}
	else if (scene_num == 10)
	{
		judged = judgeChecked(scene_name, 4, 6);
	}
	else if (scene_num == 11)
	{
		judged = judgeChecked(scene_name, 3, 5);
	}
	else if (scene_num == 12)
	{
		judged = judgeChecked(scene_name, 1, 4);
	}

	if (judged == false)
	{
		on_click = true;
		on_wait = false;
		script_now = 2;
		for (int i = 1; i <= 4; i++)
		{
			hideObject(text[i]);
		}
		hideObject(object_arrow);
		setObjectImage(object_textbox, "resources/common/textbox_half.png");
		setObject(text[3], "resources/common/text_info.png", scene[scene_num], 100, LINE(4));
	}
}

bool judgeSelected(const char* scene_name, int selectnum, int scriptnum)
{
	int i;
	for (i = 1; i <= selectnum; i++)
	{
		if (selected[i] == false)
			break;
	}
	if (i == selectnum + 1)
	{
		script_now = scriptnum;
		on_select = false;
		scriptSetup(scene_name, scene[scene_num], script_now, line_num[script_now], false);
		hideObject(*char_now);
		return true;
	}
	else
	{
		return false;
	}
}

void setSelect()
{
	char scene_name[50];
	sprintf_s(scene_name, sizeof(scene_name), "scene_%d", scene_num);
	bool judged = false;

	if (scene_num == 8)
	{
		judged = judgeSelected(scene_name, 4, 19);
	}
	else if (scene_num == 9)
	{
		judged = judgeSelected(scene_name, 4, 18);
	}
	else if (scene_num == 10)
	{
		judged = judgeSelected(scene_name, 4, 22);
	}
	else if (scene_num == 11)
	{
		judged = judgeSelected(scene_name, 4, 16);
	}
	if (judged == false)
	{
		on_select = true;
		on_wait = false;

		if (scene_num == 8)
			script_now = 6;
		else if (scene_num == 9)
			script_now = 7;
		else if (scene_num == 10)
			script_now = 7;
		else if (scene_num == 11)
			script_now = 6;

		for (int i = 1; i <= 4; i++)
		{
			hideObject(text[i]);
		}
		scriptSetup(scene_name, *scene_now, script_now, line_num[script_now], true);
		for (int i = 1; i < 5; i++)
		{
			if (selected[i] == true)
			{
				char buff[100];
				if (scene_num == 8)
				{
					sprintf_s(buff, sizeof(buff), "resources/scene_8/select6_line%d_selected.png", i);
				}
				else if (scene_num == 9)
				{
					sprintf_s(buff, sizeof(buff), "resources/scene_9/select7_line%d_selected.png", i);
				}
				else if (scene_num == 10)
				{
					sprintf_s(buff, sizeof(buff), "resources/scene_10/select7_line%d_selected.png", i);
				}
				else if (scene_num == 11)
				{
					sprintf_s(buff, sizeof(buff), "resources/scene_11/select6_line%d_selected.png", i);
				}
				setObjectImage(text[i], buff);
			}
		}
	}
}

void selectedObject()
{
	stopSound(sound_click);
	playSound(sound_click);
	script_now = -1;
	for (int i = 1; i <= 4; i++)
	{
		hideObject(text[i]);
	}
	showObject(object_arrow);
	setObject(text[3], "resources/common/text_selected.png", scene[scene_num], 100, LINE(4));
	on_wait = true;
}

void setIcons()
{
	icons[0] = createObject("resources/icons/deadbody.png");
	icons[1] = createObject("resources/icons/knives.png");
	icons[2] = createObject("resources/icons/namecard.png");
	icons[3] = createObject("resources/icons/carrier.png");
	icons[4] = createObject("resources/icons/surgerytool.png");
	icons[5] = createObject("resources/icons/gloves.png");
	icons[6] = createObject("resources/icons/notebook.png");
	icons[7] = createObject("resources/icons/dadpicture.png");
	icons[8] = createObject("resources/icons/ring.png");
	icons[9] = createObject("resources/icons/bigbackpack.png");
	icons[10] = createObject("resources/icons/note.png");
	icons[11] = createObject("resources/icons/tool.png");
	icons[12] = createObject("resources/icons/note_detective.png");
	icons[13] = createObject("resources/icons/file.png");
	icons[14] = createObject("resources/icons/girlclothes.png");
}
#include "Arkanoid_includes.h"
//Klasy i struktury

class Highscore_list_t{
public:
	unsigned result;
	char player_name[200];
	Highscore_list_t();
};
class GameStates_t{
public:
	bool Start;
	bool Pause;
	bool GameOver;
	bool Reset;
	bool quit;
	GameStates_t();
	void Default();
};
struct Bitmaps_t{
	SDL_Surface *background;
	SDL_Surface *ball;
	SDL_Surface *wall;
	SDL_Surface *ceiling;
	SDL_Surface *block[7];
	SDL_Surface *bonus[7];
	SDL_Surface *paddle[8];
	SDL_Surface *bullet;
};
class Times_t
{
public:
	double delta;
	double TotalTime;
	double StageTime;
	Times_t();
};
class laser_gun_t{
public:
	bool module;
	int TimeToShoot;
	laser_gun_t();
};
class Module_t{
public:
	laser_gun_t laser_gun;
	bool glue;
	bool fast_ball;
	bool slow_ball;
	Module_t();
};
class ElementsOfGame{
public:
	int Points;
	int NumberOfBlocks;
	int Stage;
	char BlocksMap[MAP_HEIGHT][MAP_WIDTH];
	int Lifes;
	short NumberOfBalls;
	short NumberOfGameOvers;
	short NumberOfBonuses;
	double Speeder;
	ElementsOfGame();
	void Default();
};
struct Ball_t{
	double position_x;
	double position_y;
	double direction_x;
	double direction_y;
	double angle;
};
class Paddle_t{
public:
	double position_x;
	double position_y;
	double direction_x;
	unsigned value;
	unsigned int width;
	unsigned short bmp;
	Paddle_t();
};
class Bonus_t{
public:
	double position_x;
	double position_y;
	short value;
	Bonus_t();
	void DeleteBonus();
};
class Bullet_t{
public:
	double position_x;
	double position_y;
	Bullet_t();
	void DeleteBullet();
};
class TimeBonus_t{
public:
	int extended_paddle;
	int slow_ball;
	int fast_ball;
	int laser_gun;
	TimeBonus_t();
};
struct ElementsOfScreen_t{
	SDL_Surface *screen;
	SDL_Surface *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

};
class Objects_t{
public:
	Ball_t Ball[MAX_BALLS];
	Bullet_t Bullet[MAX_BULLETS];
	Paddle_t Paddle;
	Bonus_t Bonus[MAX_BONUSES];
	Objects_t();
	void Default();
};
class ModuleSet_t{
public:
	TimeBonus_t TimeBonus;
	Module_t Module;
	void Default();
};
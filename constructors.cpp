#include "Arkanoid_functions.h"
Highscore_list_t::Highscore_list_t(){
	result = 0;
	for (int i = 0; i < MAX_NAME_LEN; i++)
	player_name[i] = '\0';
}
GameStates_t::GameStates_t(){
	Start = false;
	Pause = false;
	GameOver = false;
	Reset = false;
	quit = false;
}
Times_t::Times_t(){
	StageTime = 0;
	TotalTime = 0;
}
laser_gun_t::laser_gun_t(){
	module = OFF;
	TimeToShoot = 0;
}
Module_t::Module_t(){
	glue = OFF;
	fast_ball = OFF;
	slow_ball = OFF;
}
ElementsOfGame::ElementsOfGame(){
	Points = 0;
	Stage = 1;
	for (int x = 0; x < MAP_WIDTH; x++)
		for (int y = 0; y < MAP_HEIGHT; y++)
			BlocksMap[y][x] = 0;
	Lifes = LIFES;
	NumberOfBalls = 1;
	NumberOfGameOvers = 0;
	NumberOfBonuses = 0;
	Speeder = 1;
}
Paddle_t::Paddle_t(){
	position_x = SCREEN_WIDTH / 2;
	position_y = SCREEN_HEIGHT - 40;
	direction_x = 0;
	value = 0;
	width = PADDLE_WIDTH;
	bmp = NORMAL;
}
Bonus_t::Bonus_t(){
	position_x = 0;
	position_y = 0;
}
Bullet_t::Bullet_t(){
	position_x = 0;
	position_y = 0;
}
TimeBonus_t::TimeBonus_t(){
	extended_paddle = 0;
	slow_ball = 0;
	fast_ball = 0;
	laser_gun = 0;
}
Objects_t::Objects_t(){
	Ball[0].direction_x = 0;
	Ball[0].direction_y = 0;
	Ball[0].position_x = SCREEN_WIDTH / 2;
	Ball[0].position_y = Paddle.position_y - BALL_SIZE / 2;
	Ball[0].angle = 1;
}

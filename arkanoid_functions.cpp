#include "Arkanoid_functions.h"
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {
// narysowanie napisu txt na powierzchni screen, zaczynaj�c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj�ca znaki
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt �rodka obrazka sprite na ekranie
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {// rysowanie pojedynczego pixela
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	*(Uint32 *)p = color;
};
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
// rysowanie linii o d�ugo�ci l w pionie (gdy dx = 0, dy = 1) 
// b�d� poziomie (gdy dx = 1, dy = 0)
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,	Uint32 outlineColor, Uint32 fillColor) {// rysowanie prostok�ta o d�ugo�ci bok�w l i k
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};
void DeleteBall(Ball_t &Ball){
	Ball.position_y = 0;
	Ball.position_x = 0;
}
void LoadStage(ElementsOfGame &Game){												 /*Wczytanie planszy z pliku*/
	Game.NumberOfBlocks = 0;
	char FileName[] = "Stages/Stage##.txt";
	FileName[12] = (char)(Game.Stage / 10) + '0';
	FileName[13] = (char)(Game.Stage % 10) + '0';
	SDL_RWops* file;
	file = SDL_RWFromFile(FileName, "r");
	if (file == NULL)
	{
		printf("Warning: Unable to open file! %s", FileName);
		Game.NumberOfBlocks = -1;
		return;
	}
	for (int i = 0; i < MAP_HEIGHT*MAP_WIDTH; ++i)
	{
		SDL_RWread(file, &Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH], sizeof(char), 1);
		if (Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH] == 13 || Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH] == 10) i--;
		else
			if (Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH] >= ORANGE_BLOCK &&Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH] <= WHITE_BLOCK) {
				Game.NumberOfBlocks++;
			}
			else
				if (Game.BlocksMap[i / MAP_WIDTH][i % MAP_WIDTH] == GRAY_BLOCK)
					Game.NumberOfBlocks+=2;
	}
	SDL_RWclose(file);
}
bool CheckHit(ElementsOfGame &Game, int position_x, int position_y, Bonus_t *Bonus, Bitmaps_t Bitmaps){
	if (Game.BlocksMap[position_y][position_x] >= 'A' &&	Game.BlocksMap[position_y][position_x] <= 'G') {
		switch (Game.BlocksMap[position_y][position_x]){
		case ORANGE_BLOCK: Game.Points += ORANGE_BLOCK_POINTS;
			break;
		case RED_BLOCK: Game.Points += RED_BLOCK_POINTS;
			break;
		case YELLOW_BLOCK: Game.Points += YELLOW_BLOCK_POINTS;
			break;
		case GREEN_BLOCK: Game.Points += GREEN_BLOCK_POINTS;
			break;
		case GRAY_BLOCK: Game.Points += GRAY_BLOCK_POINTS;
			break;
		case BLUE_BLOCK: Game.Points += BLUE_BLOCK_POINTS;
			break;
		case WHITE_BLOCK: Game.Points += WHITE_BLOCK_POINTS;
			break;
		default: Game.Points = 0;
		}
		if (Game.BlocksMap[position_y][position_x] == GRAY_BLOCK)
			Game.BlocksMap[position_y][position_x] = WHITE_BLOCK;
		else
			Game.BlocksMap[position_y][position_x] = 0;
		Game.NumberOfBlocks--;
		if (rand() % 100 < BONUS_CHANCE) {
			MakeBonus(Bitmaps.wall->w + Bitmaps.block[0]->w / 2 + Bitmaps.block[0]->w*position_x, 40 + Bitmaps.block[0]->h / 2 + Bitmaps.block[0]->h*position_y, Bonus, Game.NumberOfBonuses);
			
		}return true;
	}
	return false;
}
void Collision(Ball_t &Ball, ElementsOfGame &Game, Bitmaps_t Bitmaps, Bonus_t *Bonus){	/*odbicie pileczki od bloczka*/
	int margin_y = 40 + Bitmaps.ceiling->h;
	int margin_x = Bitmaps.wall->w;
	double radius = sqrt(2.0)*(Bitmaps.ball->w / 2) / 2;
	/*Sprawdzenie kolizji gornego kranca pilki o dolna krawedz bloczka*/
	if (((int)Ball.position_y - margin_y - Bitmaps.ball->w / 2) / Bitmaps.block[0]->h < MAP_HEIGHT &&
		((int)Ball.position_x - margin_x) / Bitmaps.block[0]->w < MAP_WIDTH &&
		(CheckHit(Game, ((int)Ball.position_x - margin_x) / Bitmaps.block[0]->w,
		((int)(Ball.position_y - margin_y) - Bitmaps.ball->w / 2) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true))
		Ball.direction_y = 1;
	else
		/*Sprawdzenie kolizji dolnego kranca pilki*/
		if (((int)Ball.position_y - margin_y + Bitmaps.ball->w / 2) / Bitmaps.block[0]->h < MAP_HEIGHT &&
			((int)Ball.position_x - margin_x) / Bitmaps.block[0]->w < MAP_WIDTH &&
			(CheckHit(Game, ((int)Ball.position_x - margin_x) / Bitmaps.block[0]->w,
			((int)(Ball.position_y - margin_y) + Bitmaps.ball->w / 2) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true))
			Ball.direction_y = -1;
		else
			/*Sprawdzenie kolizji lewego kranca pilki*/
			if (((int)Ball.position_y - margin_y) / Bitmaps.block[0]->h < MAP_HEIGHT &&
				((int)Ball.position_x - margin_x - Bitmaps.ball->w / 2) / Bitmaps.block[0]->w < MAP_WIDTH &&
				(CheckHit(Game, ((int)Ball.position_x - margin_x - Bitmaps.ball->w / 2) / Bitmaps.block[0]->w,
				((int)(Ball.position_y - margin_y)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true))
				Ball.direction_x = 1;
			else
				/*Sprawdzenie kolizji prawego kranca pilki*/
				if (((int)Ball.position_y - margin_y) / Bitmaps.block[0]->h < MAP_HEIGHT &&
					((int)Ball.position_x - margin_x + Bitmaps.ball->w / 2) / Bitmaps.block[0]->w < MAP_WIDTH &&
					(CheckHit(Game, ((int)Ball.position_x - margin_x + Bitmaps.ball->w / 2) / Bitmaps.block[0]->w,
						((int)(Ball.position_y - margin_y)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true))
						Ball.direction_x = -1;
				else
					/*Sprawdzenie kolizji prawego gornego kranca pilki*/
					if (((int)Ball.position_y - margin_y - radius) / Bitmaps.block[0]->h < MAP_HEIGHT &&
						((int)Ball.position_x - margin_x + radius) / Bitmaps.block[0]->w < MAP_WIDTH &&
						(CheckHit(Game, ((int)Ball.position_x - margin_x + radius) / Bitmaps.block[0]->w,
							(((int)(Ball.position_y - radius) - margin_y)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true)){
							Ball.direction_x = -1;
							Ball.direction_y = 1;
						}
					else
						/*Sprawdzenie kolizji lewego gornego kranca pilki*/
						if (((int)Ball.position_y - margin_y - radius) / Bitmaps.block[0]->h < MAP_HEIGHT &&
							((int)Ball.position_x - margin_x - radius) / Bitmaps.block[0]->w < MAP_WIDTH &&
							(CheckHit(Game, ((int)Ball.position_x - margin_x - radius) / Bitmaps.block[0] -> w,
								((int)(Ball.position_y - margin_y - radius)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true)){
								Ball.direction_x = 1;
								Ball.direction_y = 1;
							}
						else
							/*Sprawdzenie kolizji prawego dolnego kranca pilki*/
							if (((int)Ball.position_y - margin_y + radius) / Bitmaps.block[0]->h < MAP_HEIGHT &&
								((int)Ball.position_x - margin_x + radius) / Bitmaps.block[0]->w < MAP_WIDTH &&
								(CheckHit(Game, ((int)Ball.position_x - margin_x + radius) / Bitmaps.block[0] -> w,
									((int)(Ball.position_y - margin_y + radius)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true)){
									Ball.direction_x = -1;
									Ball.direction_y = -1;
								}
							else
								/*Sprawdzenie kolizji lewego dolnego kranca pilki*/
								if (((int)Ball.position_y - margin_y + radius) / Bitmaps.block[0]->h < MAP_HEIGHT &&
									((int)Ball.position_x - margin_x - radius) / Bitmaps.block[0]->w < MAP_WIDTH &&
									(CheckHit(Game, ((int)Ball.position_x - margin_x - radius) / Bitmaps.block[0] -> w,
										((int)(Ball.position_y - margin_y + radius)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true)){
										Ball.direction_x = 1;
										Ball.direction_y = -1;
									}
}
bool Detector(Paddle_t Paddle, double position_x, double position_y){
	if (position_y >= Paddle.position_y -3- BALL_SIZE / 2){
		if (position_x > Paddle.position_x - Paddle.width / 2 - BALL_SIZE / 2 && position_x < Paddle.position_x + Paddle.width / 2 + BALL_SIZE / 2)
			return true;
		
	}return false;
}
void AngleChanger(Paddle_t Paddle, Ball_t &Ball){
	Ball.direction_y = -1;
	//Zalezne od poprzedniego kata
	if (Ball.direction_x > 0){
		Ball.angle *= 0.5 + (Ball.position_x - Paddle.position_x + Paddle.width / 2) / Paddle.width;
		Ball.angle += (Ball.position_x - Paddle.position_x + Paddle.width / 2) / Paddle.width - 0.5;
		if (Ball.angle < 0) {
			Ball.angle *= -1;
			Ball.direction_x *= -1;
		}

	}
	else if (Ball.direction_x < 0){
		Ball.angle *= 0.5 + (Paddle.width - (Ball.position_x - Paddle.position_x + Paddle.width / 2)) / Paddle.width;

		Ball.angle += (Paddle.width - (Ball.position_x - Paddle.position_x + Paddle.width / 2)) / Paddle.width - 0.5;
		if (Ball.angle < 0) {
			Ball.angle *= -1;
			Ball.direction_x *= -1;
		}
	}
}
void PaddleDetector(Objects_t &Objects, ElementsOfGame &Game, short Ball_ID, bool &Reset){
	if (Objects.Ball[Ball_ID].position_y >= Objects.Paddle.position_y - BALL_SIZE / 2){
		if (Detector(Objects.Paddle, Objects.Ball[Ball_ID].position_x, Objects.Ball[Ball_ID].position_y) == true){
			AngleChanger(Objects.Paddle, Objects.Ball[Ball_ID]);
		}
		else if (Game.NumberOfBalls == 1){
			Game.Lifes--;
			Game.NumberOfGameOvers++;
			if (Game.Lifes > 0)
			{
				Reset = true;
			}
			return;
		}
		else{
			Game.NumberOfBalls--;
			DeleteBall(Objects.Ball[Ball_ID]);
			SwapBalls(Objects.Ball[Ball_ID], Objects.Ball[Game.NumberOfBalls]);
		}
	}
	for (short Bonus_ID = 0; Bonus_ID < MAX_BONUSES; Bonus_ID++){
		if (Detector(Objects.Paddle, Objects.Bonus[Bonus_ID].position_x, Objects.Bonus[Bonus_ID].position_y) == true){
			{
				Objects.Paddle.value = Objects.Bonus[Bonus_ID].value;
				Objects.Bonus[Bonus_ID].position_x = 0;
				Objects.Bonus[Bonus_ID].position_y = 0;
				Game.NumberOfBonuses--;
			}
		}
	}
}
void SetBonusValue(Bonus_t &Bonus){
	int SetValue;
	SetValue = 100 * rand() / RAND_MAX;
	SetValue = 100 * rand() / RAND_MAX;		//Za pierwszym losowaniem nie ma losowosci
	if (SetValue < GLUE_CHANCE) Bonus.value = GLUE;
	else
		if (SetValue < GLUE_CHANCE + MULTIBALL_CHANCE) Bonus.value = MULTIBALL;
		else
			if (SetValue < GLUE_CHANCE + MULTIBALL_CHANCE + EXTENDED_CHANCE) Bonus.value = EXTENDED_PADDLE;
			else
				if (SetValue < GLUE_CHANCE + MULTIBALL_CHANCE + EXTENDED_CHANCE + LASER_GUN_CHANCE) Bonus.value = LASER_GUN;
				else
					if (SetValue < GLUE_CHANCE + MULTIBALL_CHANCE + EXTENDED_CHANCE + LASER_GUN_CHANCE + FAST_BALL_CHANCE) Bonus.value = FAST_BALL;
					else
						if (SetValue < GLUE_CHANCE + MULTIBALL_CHANCE + EXTENDED_CHANCE + LASER_GUN_CHANCE + FAST_BALL_CHANCE+SLOW_BALL_CHANCE) Bonus.value = SLOW_BALL;
						else Bonus.value = EXTRA_LIFE;

}
void MakeBonus(int position_x, int position_y, Bonus_t *Bonus, short &NumberOfBonuses){
	if (NumberOfBonuses < MAX_BONUSES){
		for (short Bonus_ID = 0; Bonus_ID < MAX_BONUSES; Bonus_ID++){
			if (Bonus[Bonus_ID].position_x == 0) {
				Bonus[Bonus_ID].position_x = position_x;
				Bonus[Bonus_ID].position_y = position_y;
				SetBonusValue(Bonus[Bonus_ID]);
				NumberOfBonuses++;
				return;
			}
		}
	}
}
void Destructor(ElementsOfScreen_t ElementsOfScreen){
	SDL_FreeSurface(ElementsOfScreen.charset);
	SDL_FreeSurface(ElementsOfScreen.screen);
	SDL_DestroyTexture(ElementsOfScreen.scrtex);
	SDL_DestroyRenderer(ElementsOfScreen.renderer);
	SDL_DestroyWindow(ElementsOfScreen.window);
	SDL_Quit();
}
void CreateNewBall(Ball_t *Ball, short &NumberOfBalls){
	if (NumberOfBalls < MAX_BALLS){
		Ball[NumberOfBalls] = Ball[0];
		Ball[NumberOfBalls].angle += (rand() / RAND_MAX) - 0.5;
		Ball[NumberOfBalls].angle += (rand() / RAND_MAX) - 0.5;
		if (Ball[NumberOfBalls].angle < 0){
			Ball[NumberOfBalls].angle *= -1;
			Ball[NumberOfBalls].direction_x *= -1;
		}
		NumberOfBalls++;
	}
	if (NumberOfBalls < MAX_BALLS){
		Ball[NumberOfBalls] = Ball[0];
		Ball[NumberOfBalls].angle += (rand() / RAND_MAX) - 0.5;
		if (Ball[NumberOfBalls].angle < 0){
			Ball[NumberOfBalls].angle *= -1;
			Ball[NumberOfBalls].direction_x *= -1;
		}
		NumberOfBalls++;
	}
}
void SwapBalls(Ball_t &Ball1, Ball_t &Ball2){
	Ball_t NewBall = Ball1;
	Ball1 = Ball2;
	Ball2 = NewBall;
}
void AddBonus(Paddle_t &Paddle, unsigned value){
	switch (value){
	case GLUED:
		if (Paddle.bmp == EXTENDED) Paddle.bmp = EXTENDED_GLUED;
		else
			if (Paddle.bmp == EXTENDED_GUN) Paddle.bmp = EXTENDED_GLUED_GUN;
			else
				if (Paddle.bmp == NORMAL) Paddle.bmp = GLUED;
				else
					if (Paddle.bmp == GUN) Paddle.bmp = GLUED_GUN;
		break;
	case EXTENDED:
		if (Paddle.bmp == GLUED) Paddle.bmp = EXTENDED_GLUED;
		else
			if (Paddle.bmp == GLUED_GUN) Paddle.bmp = EXTENDED_GLUED_GUN;
			else
				if (Paddle.bmp == NORMAL) Paddle.bmp = EXTENDED;
				else
					if (Paddle.bmp == GUN) Paddle.bmp = EXTENDED_GUN;
		break;
	case GUN:
		if (Paddle.bmp == GLUED) Paddle.bmp = GLUED_GUN;
		else
			if (Paddle.bmp == EXTENDED_GLUED) Paddle.bmp = EXTENDED_GLUED_GUN;
			else
				if (Paddle.bmp == NORMAL) Paddle.bmp = GUN;
				else
					if (Paddle.bmp == EXTENDED) Paddle.bmp = EXTENDED_GUN;
		break;

	}

}
void RemoveBonus(Paddle_t &Paddle, unsigned value){
	switch (value){
	
	case EXTENDED:
		if (Paddle.bmp == EXTENDED_GLUED) Paddle.bmp = GLUED;
		else
			if (Paddle.bmp == EXTENDED_GLUED_GUN) Paddle.bmp = GLUED_GUN;
			else
				if (Paddle.bmp == EXTENDED) Paddle.bmp = NORMAL;
				else
					if (Paddle.bmp == EXTENDED_GUN) Paddle.bmp = GUN;
		Paddle.width = PADDLE_WIDTH;
		break;
	case GUN:
		if (Paddle.bmp == GLUED_GUN) Paddle.bmp = GLUED;
		else
			if (Paddle.bmp == EXTENDED_GLUED_GUN) Paddle.bmp = EXTENDED_GLUED;
			else
				if (Paddle.bmp == GUN) Paddle.bmp = NORMAL;
				else
					if (Paddle.bmp == EXTENDED_GUN) Paddle.bmp = EXTENDED;
		break;

	}

}
void LookPaddleValue(Paddle_t &Paddle, ElementsOfGame &Game, ModuleSet_t &ModuleSet,Ball_t *Ball){
	switch (Paddle.value)
	{
	case MULTIBALL:
		CreateNewBall(Ball, Game.NumberOfBalls);
		Game.Points += MULTIBALL_POINTS;
		break;
	case GLUE:
		AddBonus(Paddle, GLUED);
		ModuleSet.Module.glue = ON;
		Game.Points += GLUE_POINTS;
		break;
	case FAST_BALL:
		ModuleSet.Module.fast_ball = ON;
		ModuleSet.TimeBonus.fast_ball = SDL_GetTicks() + SPEED_TIME * 1000;
		Game.Points += FAST_BALL_POINTS;
		break;
	case SLOW_BALL:
		ModuleSet.Module.slow_ball = ON;
		ModuleSet.TimeBonus.slow_ball = SDL_GetTicks() + SPEED_TIME * 1000;
		Game.Points += SLOW_BALL_POINTS;
		break;
	case EXTENDED_PADDLE:
		Paddle.width = PADDLE_WIDTH2;
		AddBonus(Paddle, EXTENDED);
		Game.Points += EXTENDED_POINTS;
		ModuleSet.TimeBonus.extended_paddle = SDL_GetTicks() + EXTENDED_TIME * 1000;
		break;
	case EXTRA_LIFE:
		Game.Lifes++;
		Game.Points += EXTRA_LIFE_POINTS;
		break;
	case LASER_GUN:
		AddBonus(Paddle, GUN);
		ModuleSet.Module.laser_gun.module = ON;
		Game.Points += LASER_GUN_POINTS;
		ModuleSet.TimeBonus.laser_gun = SDL_GetTicks() + LASER_GUN_TIME * 1000;
		break;
	}
	Paddle.value = 0;
}
void MakeBullet(Paddle_t Paddle, Bullet_t *Bullet){
	for (unsigned Bullet_ID = 0; Bullet_ID < MAX_BULLETS; Bullet_ID++)
		if (Bullet[Bullet_ID].position_x == 0){
			Bullet[Bullet_ID].position_x = Paddle.position_x - Paddle.width/2 + 2;
			Bullet[Bullet_ID].position_y = Paddle.position_y;
			break;
		}
	for (unsigned Bullet_ID = 0; Bullet_ID < MAX_BULLETS; Bullet_ID++)
		if (Bullet[Bullet_ID].position_x == 0){
			Bullet[Bullet_ID].position_x = Paddle.position_x + Paddle.width/2 - 2;
			Bullet[Bullet_ID].position_y = Paddle.position_y;
			break;
		}
}
void UsingBullets(Bullet_t *Bullet, SDL_Surface *screen, Bitmaps_t Bitmaps, ElementsOfGame &Game, Bonus_t *Bonus){
	int margin_y = 40 + Bitmaps.ceiling->h;
	int margin_x = Bitmaps.wall->w;
	for (unsigned Bullet_ID = 0; Bullet_ID < MAX_BULLETS; Bullet_ID++)
		if (Bullet[Bullet_ID].position_x != 0){
			if (Bullet[Bullet_ID].position_y < 40 + Bitmaps.ceiling->h + Bitmaps.bullet->h / 2)
				Bullet[Bullet_ID].DeleteBullet();
			if (((int)(Bullet[Bullet_ID].position_y - margin_y)) / Bitmaps.block[0]->h<MAP_HEIGHT)
			if (CheckHit(Game, ((int)Bullet[Bullet_ID].position_x - margin_x) / Bitmaps.block[0]->w,
				((int)(Bullet[Bullet_ID].position_y - margin_y)) / Bitmaps.block[0]->h, Bonus, Bitmaps) == true){
				Bullet[Bullet_ID].DeleteBullet();
			}
			Bullet[Bullet_ID].position_y--;
		}
}
void DeleteBullets(Bullet_t *Bullet){
	for (unsigned Bullet_ID = 0; Bullet_ID < MAX_BULLETS; Bullet_ID++){
		Bullet[Bullet_ID].DeleteBullet();
	}
}
void LookBonusTime(Objects_t &Objects, ModuleSet_t &ModuleSet, double &Speeder){

	//Wylaczenie starszego bonusu sposrod przyspieszenie i spowolnienia
	if (ModuleSet.TimeBonus.slow_ball > 0 && ModuleSet.TimeBonus.slow_ball > ModuleSet.TimeBonus.fast_ball) {
		ModuleSet.Module.fast_ball = OFF;
		Speeder = 0.5;
	}
	if (ModuleSet.TimeBonus.fast_ball > 0 && ModuleSet.TimeBonus.fast_ball > ModuleSet.TimeBonus.slow_ball) {
		ModuleSet.Module.slow_ball = OFF;
		Speeder = 4;
	}
	if (ModuleSet.TimeBonus.slow_ball < SDL_GetTicks()){
		ModuleSet.TimeBonus.slow_ball = 0;
		ModuleSet.Module.slow_ball = OFF;
	}
	if (ModuleSet.TimeBonus.fast_ball < SDL_GetTicks()){
		ModuleSet.TimeBonus.fast_ball = 0;
		ModuleSet.Module.fast_ball = OFF;
	}
	if (ModuleSet.TimeBonus.laser_gun>0)
		if (ModuleSet.TimeBonus.laser_gun < SDL_GetTicks()){
			ModuleSet.TimeBonus.laser_gun = 0;
			ModuleSet.Module.laser_gun.module = OFF;
		RemoveBonus(Objects.Paddle, GUN);
	}
	if (ModuleSet.TimeBonus.extended_paddle < SDL_GetTicks()){
		ModuleSet.TimeBonus.extended_paddle = 0;
		RemoveBonus(Objects.Paddle, EXTENDED);
	}
	if (ModuleSet.Module.slow_ball == OFF &&ModuleSet.Module.fast_ball == OFF) Speeder = 1;
}
void LoadHighscoreList(Highscore_list_t *Highscore_list){
	SDL_RWops* file;
	file = SDL_RWFromFile("highscore.txt", "r");
	if (file == NULL) return;
	SDL_RWseek(file, SEEK_SET, 0);
	for (unsigned ID = 0; ID < HIGHSCORE_LIST_LEN; ID++){
		Highscore_list[ID].player_name[0] = '\0';
		SDL_RWread(file, &Highscore_list[ID].result, sizeof(int), 1);
		SDL_RWread(file, &Highscore_list[ID].player_name, MAX_NAME_LEN, 1);
	}
	SDL_RWclose(file);
}
void SaveScore(Highscore_list_t *Highscore_list, int Points, char PlayerName[MAX_NAME_LEN]){
	SDL_RWops* file;
	file = SDL_RWFromFile("highscore.txt", "w+");
	char name[MAX_NAME_LEN] = {};
	for (int i = 0; PlayerName[i] != '\0'; i++)
		name[i] = PlayerName[i];
	for (unsigned ID = 0; ID < HIGHSCORE_LIST_LEN; ID++){
		if (Highscore_list[ID].result < Points){
			SDL_RWwrite(file, &Points, sizeof(int), 1);
			SDL_RWwrite(file, &name, MAX_NAME_LEN, 1);
			ID--;
			Points = 0;
		}
		else{
			SDL_RWwrite(file, &Highscore_list[ID].result, sizeof(int), 1);
			SDL_RWwrite(file, &Highscore_list[ID].player_name, MAX_NAME_LEN, 1);
		}
	}
	SDL_RWclose(file);
	LoadHighscoreList(Highscore_list); 
}
void GetPlayerName(ElementsOfScreen_t ElementsOfScreen, char PlayerName[MAX_NAME_LEN]){
	SDL_Event event;
	char text[128];
	int czarny = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0xFF, 0x00);
	bool quit = false;
	int i = 0;
	SDL_StartTextInput();
	while (!quit)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ENTER) {
					quit = 1; break;
				}
				else if (event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z' && i < MAX_NAME_LEN - 1){
						if (SDL_GetModState() & KMOD_CAPS && SDL_GetModState() & KMOD_SHIFT)
							PlayerName[i] = (event.key.keysym.sym);
						else
							if (SDL_GetModState() & KMOD_CAPS || SDL_GetModState() & KMOD_SHIFT)
								PlayerName[i] = (event.key.keysym.sym) - 'a' + 'A';
							else
								PlayerName[i] = (event.key.keysym.sym);
						i++;
					}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && i>0) {
					PlayerName[i - 1] = '\0';
					i--;
				}
				else if (event.key.keysym.sym == SDLK_SPACE && i < MAX_NAME_LEN - 1) {
					PlayerName[i] = ' ';
					i++;
				}
			}
		}
		Render(ElementsOfScreen);
		sprintf(text, "Podaj swoje imie");
		DrawRectangle(ElementsOfScreen.screen, (SCREEN_WIDTH - 200) / 2, SCREEN_HEIGHT / 2 - 40, 200, 60, zielony, czarny);
		DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(PlayerName) * 8 / 2, SCREEN_HEIGHT / 2, PlayerName, ElementsOfScreen.charset);

		DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2-30, text, ElementsOfScreen.charset);

	}
	PlayerName[i] = '\0';
}
void ShowHighscores(Highscore_list_t *Highscore_list, ElementsOfScreen_t ElementsOfScreen, int Points){
	char text[128];
	int czarny = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(ElementsOfScreen.screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(ElementsOfScreen.screen->format, 0x11, 0x11, 0xCC);
	DrawRectangle(ElementsOfScreen.screen, 2, 40, SCREEN_WIDTH - 4, 40 * HIGHSCORE_LIST_LEN + 40, czerwony, czarny);
	for (int ID = 0; ID < HIGHSCORE_LIST_LEN; ID++){
		sprintf(text, "%d\t%s\t%d",ID+1, Highscore_list[ID].player_name, Highscore_list[ID].result);
		DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, 60 + 40*ID, text, ElementsOfScreen.charset);
	}
	sprintf(text, "Twoj wynik: %d", Points);
	DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, 60 + 40 * HIGHSCORE_LIST_LEN, text, ElementsOfScreen.charset);
	DrawRectangle(ElementsOfScreen.screen, 4, SCREEN_HEIGHT-36, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
	sprintf(text, "GAME OVER! Aby kontynuowac wcisnij y");
	DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2,SCREEN_HEIGHT - 23, text, ElementsOfScreen.charset);
}
void KeyboardOperations(GameStates_t &GameStates, Bitmaps_t &Bitmaps, Objects_t &Objects, ModuleSet_t &ModuleSet, ElementsOfGame &Game, Times_t &Times ){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) GameStates.quit = 1;
			else if (event.key.keysym.sym == SDLK_p) {
				if (GameStates.Pause == true) 
					GameStates.Pause = false;
				else 
					GameStates.Pause = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
					Objects.Paddle.direction_x = 1;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				Objects.Paddle.direction_x = -1;
			}
			else if (event.key.keysym.sym == SDLK_s  && GameStates.GameOver == false) {
				if (GameStates.Start == false){
					for (int Ball_ID = 0; Ball_ID < Game.NumberOfBalls; Ball_ID++){
						Objects.Ball[Ball_ID].direction_x = 1;
						Objects.Ball[Ball_ID].direction_y = -1;
					}
				}
				GameStates.Start = true;
				if (ModuleSet.Module.glue == ON){
					for (int Ball_ID = 0; Ball_ID < Game.NumberOfBalls; Ball_ID++)
						if (Detector(Objects.Paddle, Objects.Ball[Ball_ID].position_x, Objects.Ball[Ball_ID].position_y) == true){
							AngleChanger(Objects.Paddle, Objects.Ball[Ball_ID]); 
							MoveObject(Objects, Bitmaps, BALL, Ball_ID, Times.delta * (Times.StageTime + 50) / 50 * Game.Speeder);
						}
				}
			}
			else if (event.key.keysym.sym == SDLK_f && ModuleSet.Module.laser_gun.module == ON && GameStates.Start==true) {
				if (ModuleSet.Module.laser_gun.TimeToShoot < SDL_GetTicks()){
					MakeBullet(Objects.Paddle, Objects.Bullet);
					ModuleSet.Module.laser_gun.TimeToShoot = SDL_GetTicks() + SHOOT_DELAY;
				}
			}
			else if (event.key.keysym.sym == SDLK_y && GameStates.GameOver == true) {
				GameStates.GameOver = false;
				GameStates.Reset = true;
			}
			break;
		case SDL_KEYUP:
			Objects.Paddle.direction_x = 0;
			break;
		case SDL_QUIT:
			GameStates.quit = 1;
			break;
		};
	};
}
void Render(ElementsOfScreen_t ElementsOfScreen){
	SDL_UpdateTexture(ElementsOfScreen.scrtex, NULL, ElementsOfScreen.screen->pixels, ElementsOfScreen.screen->pitch);
	SDL_RenderClear(ElementsOfScreen.renderer);
	SDL_RenderCopy(ElementsOfScreen.renderer, ElementsOfScreen.scrtex, NULL, NULL);
	SDL_RenderPresent(ElementsOfScreen.renderer);
}
void ShowInformations(ElementsOfScreen_t ElementsOfScreen, ElementsOfGame Game, int Highscore, double TotalTime){
	char text[128]; 
	if (Game.Points > Highscore) 
Highscore = Game.Points;
	int czarny = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(ElementsOfScreen.screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(ElementsOfScreen.screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(ElementsOfScreen.screen->format, 0x11, 0x11, 0xCC);
	DrawRectangle(ElementsOfScreen.screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
	sprintf(text, "Highscore %d\t czas trwania = %.1lf s", Highscore, TotalTime);
	DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, 10, text, ElementsOfScreen.charset);
	sprintf(text, "Poziom %d\tWynik: %d pkt\tStracone zycia: %d", Game.Stage,  Game.Points, Game.NumberOfGameOvers);
	DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, 26, text, ElementsOfScreen.charset);
}
void MoveObject(Objects_t &Objects, Bitmaps_t Bitmaps, short Name, short ID, double accelerator){
	switch (Name){
	case PADDLE:
		if (Objects.Paddle.position_x > Bitmaps.wall->w + Objects.Paddle.width / 2 && Objects.Paddle.direction_x < 0 ||
			Objects.Paddle.position_x < SCREEN_WIDTH - Bitmaps.wall->w - Objects.Paddle.width / 2 && Objects.Paddle.direction_x>0)
			Objects.Paddle.position_x += PADDLE_SPEED * Objects.Paddle.direction_x;		//porszuszanie paddle
		if (Objects.Paddle.position_x < Bitmaps.wall->w + Objects.Paddle.width / 2) 
			Objects.Paddle.position_x = Bitmaps.wall->w + Objects.Paddle.width / 2;
		if (Objects.Paddle.position_x > SCREEN_WIDTH - Bitmaps.wall->w - Objects.Paddle.width / 2)
			Objects.Paddle.position_x = SCREEN_WIDTH - Bitmaps.wall->w - Objects.Paddle.width / 2;


		break;
	case BALL:
		Objects.Ball[ID].position_y += BALL_SPEED / 80 * 1 / sqrt((1 + Objects.Ball[ID].angle*Objects.Ball[ID].angle)) 
			*Objects.Ball[ID].direction_y * 100 * accelerator;
		Objects.Ball[ID].position_x += BALL_SPEED / 80 * Objects.Ball[ID].angle / sqrt((1 + Objects.Ball[ID].angle*Objects.Ball[ID].angle)) 
			*Objects.Ball[ID].direction_x  * 100 * accelerator;
		break;

	case GLUED_BALL:
		if (Objects.Paddle.position_x > Bitmaps.wall->w + Objects.Paddle.width / 2 && Objects.Paddle.direction_x <0 ||
		Objects.Paddle.position_x < SCREEN_WIDTH - Bitmaps.wall->w - Objects.Paddle.width / 2 && Objects.Paddle.direction_x>0)
		Objects.Ball[ID].position_x += PADDLE_SPEED * Objects.Paddle.direction_x;
	}
}
void AnalyzeBalls(Objects_t &Objects, ElementsOfGame &Game, ModuleSet_t &ModuleSet, Bitmaps_t Bitmaps, GameStates_t &GameStates, double acccelerator){
	for (int Ball_ID = 0; Ball_ID < Game.NumberOfBalls; Ball_ID++){
		/*Klejenie pilki*/
			if (GameStates.Pause == false){
				if (ModuleSet.Module.glue == ON && Detector(Objects.Paddle, Objects.Ball[Ball_ID].position_x, Objects.Ball[Ball_ID].position_y) == true
					&& Objects.Ball[Ball_ID].position_y <= Objects.Paddle.position_y + 3 + Bitmaps.ball->w / 2 || GameStates.Start == false){
					MoveObject(Objects, Bitmaps, GLUED_BALL, Ball_ID, 0);
					Objects.Ball[Ball_ID].position_y = Objects.Paddle.position_y - 3 - Bitmaps.ball->w / 2;
				}
				else
				{
					MoveObject(Objects, Bitmaps, BALL, Ball_ID, acccelerator);
				}
			}
			/*Odbicie pileczki od sciany*/
			if (Objects.Ball[Ball_ID].position_x < Bitmaps.wall->w + Bitmaps.ball->w / 2) Objects.Ball[Ball_ID].direction_x = 1;
			if (Objects.Ball[Ball_ID].position_x > SCREEN_WIDTH - Bitmaps.wall->w - Bitmaps.ball->w / 2) Objects.Ball[Ball_ID].direction_x = -1;
			/*odbicie pileczki od sufitu*/
			if (Objects.Ball[Ball_ID].position_y < 40 + Bitmaps.ceiling->h + Bitmaps.ball->h / 2) Objects.Ball[Ball_ID].direction_y = 1;
			/*wykrycie kolizji*/
			Collision(Objects.Ball[Ball_ID], Game, Bitmaps, Objects.Bonus);

			/*odbicie od paletki*/
			PaddleDetector(Objects, Game, Ball_ID, GameStates.Reset);
			/*Rozpoznanie bonusu*/
			LookPaddleValue(Objects.Paddle, Game, ModuleSet, Objects.Ball);
		}

	
}
void BonusOperations(ElementsOfGame &Game, Objects_t &Objects, Bitmaps_t Bitmaps, SDL_Surface *Screen){
	/*Opadanie bonusu*/
	if (Game.NumberOfBonuses > 0){
		for (int Bonus_ID = 0; Bonus_ID < MAX_BONUSES; Bonus_ID++){
			if (Objects.Bonus[Bonus_ID].position_y>0 && Objects.Bonus[Bonus_ID].position_y < Objects.Paddle.position_y){
				Objects.Bonus[Bonus_ID].position_y += 0.5;
			}
			else if (Objects.Bonus[Bonus_ID].position_y >= Objects.Paddle.position_y && Objects.Bonus[Bonus_ID].position_y < SCREEN_HEIGHT){
				Objects.Bonus[Bonus_ID].position_x = 0;
				Objects.Bonus[Bonus_ID].position_y = 0;
				Game.NumberOfBonuses--;

			}
		}
	}
}
void Drawing(ElementsOfGame &Game, Objects_t Objects, Bitmaps_t Bitmaps, SDL_Surface *Screen,  bool Pause){
	char text[128];

	SDL_Surface *charset = SDL_LoadBMP("./cs8x8.bmp");

	SDL_SetColorKey(charset, true, 0x000000);
	int czarny = SDL_MapRGB(Screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(Screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(Screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(Screen->format, 0x11, 0x11, 0xCC);
	/*Rysowanie tla*/
	DrawSurface(Screen, Bitmaps.background, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	/*Rysowanie sufitu*/
	for (int i = 0; i <= SCREEN_WIDTH / Bitmaps.ceiling->w; i++)
		DrawSurface(Screen, Bitmaps.ceiling, Bitmaps.ceiling->w / 2 + Bitmaps.wall->w + Bitmaps.ceiling->w * i, 40 + Bitmaps.ceiling->h / 2);

	//Rysowanie scian
	for (int i = 0; i < (SCREEN_HEIGHT - 43) / Bitmaps.wall->h - 1; i++)
		DrawSurface(Screen, Bitmaps.wall, Bitmaps.wall->w / 2, 50 + Bitmaps.wall->h * i);
	for (int i = 0; i < (SCREEN_HEIGHT - 43) / Bitmaps.wall->h - 1; i++)
		DrawSurface(Screen, Bitmaps.wall, SCREEN_WIDTH - Bitmaps.wall->w / 2, 50 + Bitmaps.wall->h * i);

	/*Rysowanie pileczki*/
	for (int Ball_ID = 0; Ball_ID < Game.NumberOfBalls; Ball_ID++){
		DrawSurface(Screen, Bitmaps.ball, (int)Objects.Ball[Ball_ID].position_x, (int)Objects.Ball[Ball_ID].position_y);
	}
	/*Rysowanie bloczkow*/
	for (int j = 0; j < MAP_HEIGHT; j++)
		for (int i = 0; i < MAP_WIDTH; i++)
			if (Game.BlocksMap[j][i] >= 'A')
				DrawSurface(Screen, Bitmaps.block[Game.BlocksMap[j][i] - 'A'], Bitmaps.wall->w + Bitmaps.block[0]->w/2 + i * Bitmaps.block[Game.BlocksMap[j][i] - 'A']->w,
				Bitmaps.block[Game.BlocksMap[j][i] - 'A']->h * j + 40 + Bitmaps.block[0]->h / 2 + Bitmaps.ceiling->h);

	/*Rysowanie paletki*/
	DrawSurface(Screen, Bitmaps.paddle[Objects.Paddle.bmp], Objects.Paddle.position_x, Objects.Paddle.position_y);

	/*Rysowanie bonusow*/
	for (int Bonus_ID = 0; Bonus_ID < MAX_BONUSES; Bonus_ID++)
		if (Objects.Bonus[Bonus_ID].position_y>0 && Objects.Bonus[Bonus_ID].position_y < Objects.Paddle.position_y){
			DrawSurface(Screen, Bitmaps.bonus[Objects.Bonus[Bonus_ID].value - 1], Objects.Bonus[Bonus_ID].position_x, Objects.Bonus[Bonus_ID].position_y);
		}
	/*Rysowanie pociskow*/
	for (unsigned Bullet_ID = 0; Bullet_ID < SCREEN_HEIGHT; Bullet_ID++)
		if (Objects.Bullet[Bullet_ID].position_x != 0)
			DrawSurface(Screen, Bitmaps.bullet, Objects.Bullet[Bullet_ID].position_x, Objects.Bullet[Bullet_ID].position_y);
	DrawRectangle(Screen, 4, Objects.Paddle.position_y + 4, SCREEN_WIDTH - 8, 36, czarny, czarny);
	/*Rysowanie pozostalych zyc*/
	for (int Life_ID = 1; Life_ID < Game.Lifes; Life_ID++)
		DrawSurface(Screen, Bitmaps.ball, Life_ID*(Bitmaps.ball->w+2), SCREEN_HEIGHT - Bitmaps.ball->h);

	if (Pause == true){
		DrawRectangle(Screen, 4, Objects.Paddle.position_y + 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "PAUSE");
		DrawString(Screen, Screen->w / 2 - strlen(text) * 8 / 2, Objects.Paddle.position_y + 17, text, charset);
	}

}
void HighscoreListOperating(ElementsOfGame &Game, GameStates_t &GameStates, Highscore_list_t *Highscore_list, ElementsOfScreen_t ElementsOfScreen){
	if (Game.Lifes <= 0 && GameStates.GameOver == false || Game.NumberOfBlocks < 0){
		GameStates.Reset = true;
		if (Game.NumberOfBlocks < 0) {
			Game.NumberOfBlocks = 0;
		}
		GameStates.Start = false;
		GameStates.GameOver = true;
		if (Highscore_list[HIGHSCORE_LIST_LEN - 1].result < Game.Points){
			char PlayerName[MAX_NAME_LEN] = {};
			GetPlayerName(ElementsOfScreen, PlayerName);
			SaveScore(Highscore_list, Game.Points, PlayerName);
		}
		ShowHighscores(Highscore_list, ElementsOfScreen, Game.Points);
	}
	
	if (GameStates.GameOver == true)
		ShowHighscores(Highscore_list, ElementsOfScreen,Game.Points);
}
void Bullet_t::DeleteBullet(){
	position_x = 0;
	position_y = 0;
}
bool SDL_Start(ElementsOfScreen_t &ElementsOfScreen){
	int rc;
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// tryb pe�noekranowy
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &ElementsOfScreen.window, &ElementsOfScreen.renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&ElementsOfScreen.window, &ElementsOfScreen.renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(ElementsOfScreen.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(ElementsOfScreen.renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(ElementsOfScreen.window, "Arkanoid 2014");


	ElementsOfScreen.screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	ElementsOfScreen.scrtex = SDL_CreateTexture(ElementsOfScreen.renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy��czenie widoczno�ci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	ElementsOfScreen.charset = SDL_LoadBMP("./cs8x8.bmp");
	if (ElementsOfScreen.charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
		return false;
	};
	SDL_SetColorKey(ElementsOfScreen.charset, true, 0x000000);
	return true;
}
void TimeControl(Times_t &Times, GameStates_t GameStates, int &PreviousTime){
	int NewTime = SDL_GetTicks();
	if (GameStates.Start == true && GameStates.Pause == false) Times.delta = (NewTime - PreviousTime) * 0.001;
	else Times.delta = 0;
	Times.TotalTime += Times.delta;
	if (Times.StageTime < ACCELERATING_TIME) Times.StageTime += Times.delta;
	PreviousTime = NewTime;
}
void EndMap(ElementsOfGame &Game, Objects_t &Objects, GameStates_t &GameStates, ElementsOfScreen_t ElementsOfScreen){
	if (Game.NumberOfBlocks <= 0)
	{
		SDL_Delay(1000);
		GameStates.Reset = true;
		DeleteBullets(Objects.Bullet);
		Game.Stage++;
		Objects.Default();
		DeleteBullets(Objects.Bullet);
		Game.Points += COMPLETE_MAP_POINTS;
		LoadStage(Game);
		if (Game.NumberOfBlocks < 0){
			char text[128];
			int czerwony = SDL_MapRGB(ElementsOfScreen.screen->format, 0xFF, 0x00, 0x00);
			int niebieski = SDL_MapRGB(ElementsOfScreen.screen->format, 0x11, 0x11, 0xCC);
			DrawRectangle(ElementsOfScreen.screen, 4, (int)Objects.Paddle.position_y + 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
			sprintf_s(text, "Winner");
			GameStates.GameOver = true;
			DrawString(ElementsOfScreen.screen, ElementsOfScreen.screen->w / 2 - strlen(text) * 8 / 2, (int)Objects.Paddle.position_y + 17, text, ElementsOfScreen.charset);
			SDL_Delay(3000);
		}
	}

}
void GameStates_t::Default(){
	
	Start = false;
	Pause = false;
	Reset = false;
	quit = false;
}
void ElementsOfGame::Default(){
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
void Objects_t::Default(){
	Paddle.position_x = SCREEN_WIDTH / 2;
	Paddle.position_y = SCREEN_HEIGHT - 40;
	Paddle.direction_x = 0;
	Paddle.value = 0;
	Paddle.width = PADDLE_WIDTH;
	Paddle.bmp = NORMAL;
	Ball[0].direction_x = 0;
	Ball[0].direction_y = 0;
	Ball[0].position_x = SCREEN_WIDTH / 2;
	Ball[0].position_y = Paddle.position_y - BALL_SIZE / 2;
	Ball[0].angle = 1;
	for (int Bonus_ID = 0; Bonus_ID < MAX_BONUSES; Bonus_ID++)
		Bonus[Bonus_ID].DeleteBonus();
}
void ModuleSet_t::Default(){
	TimeBonus.extended_paddle = 0;
	TimeBonus.slow_ball = 0;
	TimeBonus.fast_ball = 0;
	TimeBonus.laser_gun = 0;
	Module.fast_ball = OFF;
	Module.glue = OFF;
	Module.laser_gun.module = OFF;
	Module.laser_gun.TimeToShoot = 0;
	Module.slow_ball = OFF;	
}
void Reset(GameStates_t &GameStates, ElementsOfGame &Game, Objects_t &Objects, ModuleSet_t &ModuleSet,double & StageTime){
	if (GameStates.GameOver == true)
	{
		Game.Default();
		LoadStage(Game);

	}
	GameStates.Default();
	Game.NumberOfBalls = 1;
	Game.NumberOfBonuses = 0;
	Objects.Default();
	ModuleSet.Default();
	StageTime = 0;
	Game.Speeder = 1;
}
void Bonus_t::DeleteBonus(){
	position_x = 0;
	position_y = 0;
}
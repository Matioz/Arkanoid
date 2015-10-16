#include "Arkanoid_functions.h"
void LoadBitmaps(Bitmaps_t &Bitmaps, ElementsOfScreen_t ElementsOfScreen){
	/*Wczytanie bitmap*/
	Bitmaps.ball = SDL_LoadBMP("./ball.bmp");
	if (Bitmaps.ball == NULL) {
		printf("SDL_LoadBMP(ball.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
	};

	SDL_SetColorKey(Bitmaps.ball, true, 0x000000);
	Bitmaps.wall = SDL_LoadBMP("./wall.bmp");
	if (Bitmaps.wall == NULL) {
		printf("SDL_LoadBMP(wall.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
	};
	Bitmaps.ceiling = SDL_LoadBMP("./ceiling.bmp");
	if (Bitmaps.ceiling == NULL) {
		printf("SDL_LoadBMP(sufit.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
	};
	Bitmaps.bullet = SDL_LoadBMP("./bullet.bmp");
	if (Bitmaps.bullet == NULL) {
		printf("SDL_LoadBMP(bullet.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
	};
	for (int BonusNumber = 0; BonusNumber < 7; BonusNumber++){
		char  BonusFileName[] = "./bonus1.bmp";
		BonusFileName[7] = char(BonusNumber + '1');
		Bitmaps.bonus[BonusNumber] = SDL_LoadBMP(BonusFileName);
		if (Bitmaps.bonus[BonusNumber] == NULL) {
			printf("SDL_LoadBMP(%s) error: %s\n", BonusFileName, SDL_GetError());
			Destructor(ElementsOfScreen);
		};

		SDL_SetColorKey(Bitmaps.bonus[BonusNumber], true, 0x000000);
	}
	for (int BlockNumber = 0; BlockNumber < 7; BlockNumber++){
		char  BlockFileName[] = "./block1.bmp";
		BlockFileName[7] = char(BlockNumber + '1');
		Bitmaps.block[BlockNumber] = SDL_LoadBMP(BlockFileName);
		if (Bitmaps.block[BlockNumber] == NULL) {
			printf("SDL_LoadBMP(%s) error: %s\n", BlockFileName, SDL_GetError());
			Destructor(ElementsOfScreen);
		};

	}

	for (int PaddleNumber = 0; PaddleNumber < 8; PaddleNumber++){
		char  PaddleFileName[] = "./Paddle1.bmp";
		PaddleFileName[8] = char(PaddleNumber + '1');
		Bitmaps.paddle[PaddleNumber] = SDL_LoadBMP(PaddleFileName);
		if (Bitmaps.paddle[PaddleNumber] == NULL) {
			printf("SDL_LoadBMP(%s) error: %s\n", PaddleFileName, SDL_GetError());
			Destructor(ElementsOfScreen);
		};

		SDL_SetColorKey(Bitmaps.paddle[PaddleNumber], true, 0x000080);
	}
	Bitmaps.background = SDL_LoadBMP("./background.bmp");
	if (Bitmaps.background == NULL) {
		printf("SDL_LoadBMP(background.bmp) error: %s\n", SDL_GetError());
		Destructor(ElementsOfScreen);
	};


}
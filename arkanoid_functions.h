#include "Arkanoid_includes.h"
#include "Arkanoid_StructuresAndClasses.h"

//Funkcje
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,	Uint32 outlineColor, Uint32 fillColor);
void LoadStage(ElementsOfGame &Game);
void Collision(Ball_t &Ball, ElementsOfGame &Game, Bitmaps_t Bitmaps, Bonus_t *Bonus);
void PaddleDetector(Objects_t &Objects, ElementsOfGame &Game,  short Ball_ID , bool &Reset);
void MakeBonus(int position_x, int position_y, Bonus_t *Bonus, short &NumberOfBonuses);
void Destructor(ElementsOfScreen_t ElementsOfScreen);
void CreateNewBall(Ball_t *Ball, short &NumberOfBalls);
void SwapBalls(Ball_t &Ball1, Ball_t &Ball2);
bool Detector(Paddle_t Paddle, double position_x, double position_y);
void AddBonus(Paddle_t &Paddle, unsigned value);
void LookPaddleValue(Paddle_t &Paddle, ElementsOfGame &Game, ModuleSet_t &ModuleSet, Ball_t *Ball);
void MakeBullet(Paddle_t Paddle, Bullet_t *Bullet);
void UsingBullets(Bullet_t *Bullet, SDL_Surface *screen, Bitmaps_t Bitmaps, ElementsOfGame &Game, Bonus_t *Bonus);
void DeleteBullets(Bullet_t *Bullet);
void AngleChanger(Paddle_t Paddle, Ball_t &Ball);
void LoadBitmaps(Bitmaps_t &Bitmaps, ElementsOfScreen_t ElementsOfScreen);
void LookBonusTime(Objects_t &Objects, ModuleSet_t &ModuleSet, double &Speeder);
void LoadHighscoreList(Highscore_list_t *Highscore_list);
void SaveScore(Highscore_list_t *Highscore_list, int Points, char* PlayerName); 
void GetPlayerName(ElementsOfScreen_t ElementsOfScreen, char PlayerName[MAX_NAME_LEN]);
void ShowHighscores(Highscore_list_t *Highscore_list,ElementsOfScreen_t ElementsOfScreen, int Points);
void KeyboardOperations(GameStates_t &GameStates, Bitmaps_t &Bitmaps, Objects_t &Objects, ModuleSet_t &ModuleSet, ElementsOfGame &Game, Times_t &Times);
void Render(ElementsOfScreen_t ElementsOfScreen);
void ShowInformations(ElementsOfScreen_t ElementsOfScreen, ElementsOfGame Game, int Highscore, double TotalTime);
void MoveObject(Objects_t &Objects, Bitmaps_t Bitmaps, short Name, short ID, double accelerator);
void AnalyzeBalls(Objects_t &Objects, ElementsOfGame &Game, ModuleSet_t &ModuleSet, Bitmaps_t Bitmaps, GameStates_t &GameStates, double acccelerator);
void BonusOperations(ElementsOfGame &Game, Objects_t &Objects, Bitmaps_t Bitmaps, SDL_Surface *Screen);
void Drawing(ElementsOfGame &Game, Objects_t Objects, Bitmaps_t Bitmaps, SDL_Surface *Screen, bool Pause);
void HighscoreListOperating(ElementsOfGame &Game, GameStates_t &GameStates, Highscore_list_t *Highscore_list, ElementsOfScreen_t ElementsOfScreen);
bool SDL_Start(ElementsOfScreen_t &ElementsOfScreen);
void TimeControl(Times_t &Times, GameStates_t GameStates, int &PreviousTime);
void EndMap(ElementsOfGame &Game, Objects_t &Objects, GameStates_t &GameStates, ElementsOfScreen_t ElementsOfScreen);
void Reset(GameStates_t &GameStates, ElementsOfGame &Game, Objects_t &Objects, ModuleSet_t &ModuleSet, double & StageTime);
#include "Arkanoid_functions.h"
int main(int argc, char **argv) {

	srand(time(NULL));
	int time;
	double accelerator;
	Times_t Times;
	ElementsOfGame Game;
	GameStates_t GameStates;
	Highscore_list_t Highscore_list[HIGHSCORE_LIST_LEN];
	Objects_t Objects;
	ModuleSet_t ModuleSet;
	ElementsOfScreen_t ElementsOfScreen;
	Bitmaps_t Bitmaps;
	LoadHighscoreList(Highscore_list);
	LoadStage(Game);
	if(SDL_Start(ElementsOfScreen)==false) //Zaladowanie charseta, screena itp.
		return 1; 
	/*	Wczytanie bitmap	*/
	LoadBitmaps(Bitmaps, ElementsOfScreen);
	//Koniec wczytywania tekstur
	time = SDL_GetTicks();
	while(!GameStates.quit) {
		TimeControl(Times, GameStates, time);
		LookBonusTime(Objects, ModuleSet, Game.Speeder);
		if (ModuleSet.Module.slow_ball == ON || ModuleSet.Module.fast_ball == ON)
			Times.StageTime = 0;																						
		accelerator = Times.delta * (Times.StageTime + ACCELERATING_TIME) / (ACCELERATING_TIME) * Game.Speeder;			//wzgledne przyspieszenie

		/*Rysowanie*/
		Drawing(Game, Objects, Bitmaps, ElementsOfScreen.screen, GameStates.Pause);
		if (GameStates.GameOver == false && GameStates.Pause == false){
			if (GameStates.Reset == true){						//Resetowanie gry
				SDL_Delay(500);
				if (Game.Lifes <= 0 || Game.NumberOfBlocks ==0)						//Jezeli wznawiamy po przegranej grze
					GameStates.GameOver = true;
					Reset(GameStates, Game, Objects, ModuleSet, Times.StageTime);
					GameStates.GameOver = false;
			}
			//poruszanie sie paletki
			MoveObject(Objects, Bitmaps, PADDLE, 0, 0);
			/*Bonusy*/
			BonusOperations(Game, Objects, Bitmaps, ElementsOfScreen.screen);
			/*Ruch pociskow*/
			UsingBullets(Objects.Bullet, ElementsOfScreen.screen, Bitmaps, Game, Objects.Bonus);
			/*Analiza kazdej pileczki */
			AnalyzeBalls(Objects, Game, ModuleSet, Bitmaps, GameStates, accelerator);
			/*zmiana planszy*/
			EndMap(Game, Objects, GameStates, ElementsOfScreen);
		}
		// tekst informacyjny po ukonczeniu gry
		HighscoreListOperating(Game, GameStates, Highscore_list, ElementsOfScreen);
		//Wyswietlanie informacji o grze
		ShowInformations(ElementsOfScreen, Game, Highscore_list[0].result, Times.TotalTime);
		
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		KeyboardOperations(GameStates,Bitmaps,Objects, ModuleSet, Game, Times);
		//renderowanie
		Render(ElementsOfScreen);
		};
	// zwolnienie powierzchni
	Destructor(ElementsOfScreen);
	return 0;
	};

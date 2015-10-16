//Tryby do funkcji od poruszania obiektami
#define BALL				0
#define PADDLE				1
#define GLUED_BALL			2

//Definicja klawisza Enter, ktorej brak w SDL
#define SDLK_ENTER			13

//Maksymalna dlugosc nazwy
#define MAX_NAME_LEN		20
#define HIGHSCORE_LIST_LEN	5

//Czas w milisekundach pomiedzy strzalami
#define SHOOT_DELAY			300

//Czas w sekundach ile ma trwac przyspieszanie pilki
#define ACCELERATING_TIME	20

//Prêdkoœci
#define BALL_SPEED			80
#define PADDLE_SPEED		3


//Rozmiary planszy
#define MAP_HEIGHT			14
#define MAP_WIDTH			13

//Czasy trwania bonusow
#define SPEED_TIME			20
#define EXTENDED_TIME		20
#define LASER_GUN_TIME		20

//Ilosc zyc
#define LIFES				3

//Ograniczenia pilek i bonusow
#define MAX_BALLS			40
#define MAX_BONUSES			5
#define MAX_BULLETS			1000

//Wymiary planszy
#define SCREEN_HEIGHT		480
#define SCREEN_WIDTH		640

//Szerokosci paletki
#define PADDLE_WIDTH		50
#define PADDLE_WIDTH2		70

//Rozmiar pilki
#define BALL_SIZE			10

//Bonusy
#define GLUE				1
#define MULTIBALL			2
#define EXTENDED_PADDLE		3
#define LASER_GUN			4
#define FAST_BALL			5
#define SLOW_BALL			6
#define EXTRA_LIFE			7


//Szansa na wypadnienie bonusu
#define BONUS_CHANCE		20


//Szanse na bonusy
#define GLUE_CHANCE			14
#define MULTIBALL_CHANCE	14
#define EXTENDED_CHANCE		14
#define LASER_GUN_CHANCE	14
#define FAST_BALL_CHANCE	14
#define SLOW_BALL_CHANCE	14
#define EXTRA_LIFE_CHANCE	100-GLUE_CHANCE-MULTIBALL_CHANCE-EXTENDED_CHANCE-LASER_GUN_CHANCE-FAST_BALL_CHANCE-SLOW_BALL_CHANCE

//Punkty za bonusy
#define GLUE_POINTS			100
#define MULTIBALL_POINTS	200
#define EXTENDED_POINTS		150
#define LASER_GUN_POINTS	300
#define FAST_BALL_POINTS	400
#define SLOW_BALL_POINTS	20
#define EXTRA_LIFE_POINTS	400

//Paletki
#define NORMAL				0
#define EXTENDED			1
#define EXTENDED_GLUED		2
#define	EXTENDED_GUN		3
#define	GLUED				4
#define GUN					5
#define	GLUED_GUN			6
#define EXTENDED_GLUED_GUN	7

//Wartosci modulow
#define ON					true
#define OFF					false

//Klocki
#define ORANGE_BLOCK		'A'
#define RED_BLOCK			'B'
#define YELLOW_BLOCK		'C'
#define GREEN_BLOCK			'D'
#define BLUE_BLOCK			'E'
#define	WHITE_BLOCK			'F'
#define GRAY_BLOCK			'G'

//Punkty za klocki

#define ORANGE_BLOCK_POINTS 50
#define RED_BLOCK_POINTS	100
#define YELLOW_BLOCK_POINTS	125
#define GREEN_BLOCK_POINTS	150
#define BLUE_BLOCK_POINTS	175
#define	WHITE_BLOCK_POINTS	200
#define GRAY_BLOCK_POINTS	400


//Punkty za ukonczenie planszy
#define COMPLETE_MAP_POINTS	1000
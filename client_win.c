/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"
#include <joyconlib.h>

static SDL_Window *gMainWindow;
static SDL_Renderer *gMainRenderer;
static SDL_Rect gCharaBackRect;
static SDL_Rect gCharaEarthARect;
static SDL_Rect gCharaEarthERect;
static SDL_Rect gCharaLoadRect;
static SDL_Rect gCharaAllyRect[MAX_CLIENTS];
static SDL_Rect gCharaEnemyRect[MAX_CLIENTS];
static SDL_Rect gCharaBeam0Rect[BEAMNUM];
static SDL_Rect gCharaBeam1Rect[BEAMNUM];
static SDL_Rect gCharaBeam2Rect[BEAMNUM];
static SDL_Rect gCharaBeam3Rect[BEAMNUM];
static int HP[4] = {3, 3, 3, 3};

static SDL_Texture *textureBack;
static SDL_Texture *textureEarthA;
static SDL_Texture *textureEarthE;
static SDL_Texture *textureItem;
static SDL_Texture *textureLoad0;
static SDL_Texture *textureLoad1;
static SDL_Texture *textureLoad2;
static SDL_Texture *textureLoad3;
static SDL_Texture *textureMeteo;
static SDL_Texture *textureAlly;
static SDL_Texture *textureEnemy;
static SDL_Texture *textureBeam0;
static SDL_Texture *textureBeam1;
static SDL_Texture *textureBeam2;
static SDL_Texture *textureBeam3;
static SDL_Surface *imageBack;
static SDL_Surface *imageEarthA;
static SDL_Surface *imageEarthE;
static SDL_Surface *imageItem;
static SDL_Surface *imageLoad0;
static SDL_Surface *imageLoad1;
static SDL_Surface *imageLoad2;
static SDL_Surface *imageLoad3;
static SDL_Surface *imageMeteo;
static SDL_Surface *imageAlly;
static SDL_Surface *imageEnemy;
static SDL_Surface *imageBeam0;
static SDL_Surface *imageBeam1;
static SDL_Surface *imageBeam2;
static SDL_Surface *imageBeam3;
static SDL_Rect srcBack_rect;
static SDL_Rect srcEarthA_rect;
static SDL_Rect srcEarthE_rect;
static SDL_Rect srcItem_rect;
static SDL_Rect srcLoad0_rect;
static SDL_Rect srcLoad1_rect;
static SDL_Rect srcLoad2_rect;
static SDL_Rect srcLoad3_rect;
static SDL_Rect srcMeteo_rect;
static SDL_Rect srcAlly_rect;
static SDL_Rect srcEnemy_rect;
static SDL_Rect srcBeam0_rect;
static SDL_Rect srcBeam1_rect;
static SDL_Rect srcBeam2_rect;
static SDL_Rect srcBeam3_rect;
static SDL_Rect dstBack_rect;
static SDL_Rect dstEarthA_rect;
static SDL_Rect dstEarthE_rect;
static SDL_Rect dstItem_rect;
static SDL_Rect dstLoad0_rect;
static SDL_Rect dstLoad1_rect;
static SDL_Rect dstLoad2_rect;
static SDL_Rect dstLoad3_rect;
static SDL_Rect dstMeteo_rect;
static SDL_Rect dstHp_rect;
static SDL_Rect dstAlly_rect;
static SDL_Rect dstEnemy_rect;
static SDL_Rect dstBeam0_rect;
static SDL_Rect dstBeam1_rect;
static SDL_Rect dstBeam2_rect;
static SDL_Rect dstBeam3_rect;
static char charaAlly[] = "chara_ally.png";
static char charaEnemy[] = "chara_enemy.png";
static char charaBeam0[] = "sozai.png";
static char charaBeam1[] = "sozai.png";
static char charaBeam2[] = "sozai.png";
static char charaBeam3[] = "sozai.png";

static SDL_Surface *strings;
static SDL_Texture *texture;
static TTF_Font *font;
static SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
static SDL_Rect src_rect;
static SDL_Rect dst_rect;
static Mix_Chunk *chunk1;
static Mix_Chunk *chunk2;
static Mix_Chunk *chunk3;
static Mix_Chunk *chunk4;
static Mix_Chunk *chunk5;
static Mix_Music* music;
static void Collision(int num);

static int timer0cnt;
static int timer1cnt;
static int timer2cnt;
static int timer3cnt;
static int timerIcnt;
static int timerMcnt;
static int beam0cnt;
static int beam1cnt;
static int beam2cnt;
static int beam3cnt;
static int tntn;
static int load[4] = {0, 0, 0, 0};
static Uint32 BeamAnimation0(Uint32 interval, void *param);
static Uint32 BeamAnimation1(Uint32 interval, void *param);
static Uint32 BeamAnimation2(Uint32 interval, void *param);
static Uint32 BeamAnimation3(Uint32 interval, void *param);
static Uint32 ItemAnimation(Uint32 interval, void *param);
static Uint32 MeteoAnimation(Uint32 interval, void *param);

/* ジョイコンオープン
joyconlib_t jc; */

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	char *s,title[10];

    	/* 引き数チェック */
    	assert(0<num && num<=MAX_CLIENTS);
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	// SDL_mixerの初期化（MP3ファイルを使用）
    	Mix_Init(MIX_INIT_MP3); // Mix_Init(フラグ値);
    	
    	// オーディオデバイスの初期化
    	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        	printf("failed to initialize SDL_mixer.\n");
        	return -1;
   	 }
   	 
   	 // BGMと効果音のサウンドファイルの読み込み
    	if((music = Mix_LoadMUS("boss.mp3")) == NULL || (chunk1 = Mix_LoadWAV("tap.mp3")) == NULL || (chunk2 = Mix_LoadWAV("damage.mp3")) == NULL || (chunk3 = Mix_LoadWAV("heal.mp3")) == NULL || (chunk4 = Mix_LoadWAV("death.mp3")) == NULL || (chunk5 = Mix_LoadWAV("gameset.mp3")) == NULL) {
        	printf("failed to load music and chunk.\n");
        	Mix_CloseAudio(); // オーディオデバイスの終了
        	return -1;
    	}
	
	/* メインのウインドウを作成する */
	if((gMainWindow = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}

	gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_SOFTWARE);

	/* ウインドウのタイトルをセット */
	sprintf(title,"%dP",clientID+1);
	SDL_SetWindowTitle(gMainWindow, title);
	
	/* 背景を白にする */
	SDL_SetRenderDrawColor(gMainRenderer, 255, 255, 255, 255);
  	SDL_RenderClear(gMainRenderer);
  	
  	/* 背景の作成 */
  	imageBack = IMG_Load("back.png");
	textureBack = SDL_CreateTextureFromSurface(gMainRenderer, imageBack);
	srcBack_rect = (SDL_Rect){0, 0, imageBack->w, imageBack->h};
	SDL_RenderCopy(gMainRenderer, textureBack, NULL, NULL);
	SDL_FreeSurface(imageBack);
	
	/* 地球の作成 */
  	imageEarthA = IMG_Load("earthA.png");
	textureEarthA = SDL_CreateTextureFromSurface(gMainRenderer, imageEarthA);
	srcEarthA_rect = (SDL_Rect){0, 0, imageEarthA->w, imageEarthA->h};
	SDL_RenderCopy(gMainRenderer, textureEarthA, NULL, NULL);
	SDL_FreeSurface(imageEarthA);
	
	imageEarthE = IMG_Load("earthE.png");
	textureEarthE = SDL_CreateTextureFromSurface(gMainRenderer, imageEarthE);
	srcEarthE_rect = (SDL_Rect){0, 0, imageEarthE->w, imageEarthE->h};
	SDL_RenderCopy(gMainRenderer, textureEarthE, NULL, NULL);
	SDL_FreeSurface(imageEarthE);
	
	/* HPの作成 */
  	imageItem = IMG_Load("item.png");
	textureItem = SDL_CreateTextureFromSurface(gMainRenderer, imageItem);
	srcItem_rect = (SDL_Rect){0, 0, imageItem->w, imageItem->h};
	dstItem_rect = (SDL_Rect){0, 0, 0, 0};
	SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstItem_rect);
	SDL_FreeSurface(imageItem);

	/* 弾数の作成 */
  	imageLoad0 = IMG_Load("sozai.png");
  	imageLoad1 = IMG_Load("sozai.png");
  	imageLoad2 = IMG_Load("sozai.png");
  	imageLoad3 = IMG_Load("sozai.png");

	textureLoad0 = SDL_CreateTextureFromSurface(gMainRenderer, imageLoad0);
	textureLoad1 = SDL_CreateTextureFromSurface(gMainRenderer, imageLoad1);
	textureLoad2 = SDL_CreateTextureFromSurface(gMainRenderer, imageLoad2);
	textureLoad3 = SDL_CreateTextureFromSurface(gMainRenderer, imageLoad3);

	srcLoad0_rect = (SDL_Rect){8, 88, 12, 12};
	srcLoad1_rect = (SDL_Rect){8, 88, 12, 12};
	srcLoad2_rect = (SDL_Rect){8, 108, 12, 12};
	srcLoad3_rect = (SDL_Rect){8, 108, 12, 12};

	dstLoad0_rect = (SDL_Rect){0, 0, 0, 0};
	dstLoad1_rect = (SDL_Rect){0, 0, 0, 0};
	dstLoad2_rect = (SDL_Rect){0, 0, 0, 0};
	dstLoad3_rect = (SDL_Rect){0, 0, 0, 0};

	SDL_RenderCopy(gMainRenderer, textureLoad0, &srcLoad0_rect, &dstLoad0_rect);
	SDL_RenderCopy(gMainRenderer, textureLoad1, &srcLoad1_rect, &dstLoad1_rect);
	SDL_RenderCopy(gMainRenderer, textureLoad2, &srcLoad2_rect, &dstLoad2_rect);
	SDL_RenderCopy(gMainRenderer, textureLoad3, &srcLoad3_rect, &dstLoad3_rect);

	SDL_FreeSurface(imageLoad0);
	SDL_FreeSurface(imageLoad1);
	SDL_FreeSurface(imageLoad2);
	SDL_FreeSurface(imageLoad3);

	/* 隕石の作成 */
  	imageMeteo = IMG_Load("meteo.png");
	textureMeteo = SDL_CreateTextureFromSurface(gMainRenderer, imageMeteo);
	srcMeteo_rect = (SDL_Rect){0, 0, imageMeteo->w, imageMeteo->h};
	dstMeteo_rect = (SDL_Rect){0, 0, 0, 0};
	SDL_RenderCopy(gMainRenderer, textureMeteo, &srcMeteo_rect, &dstMeteo_rect);
	SDL_FreeSurface(imageMeteo);
	
	/* 終了の作成 */
	TTF_Init();
    	font = TTF_OpenFont("FreeMono.ttf", 480);
    	strings = TTF_RenderUTF8_Blended(font, "GAME SET", white);
   	texture = SDL_CreateTextureFromSurface(gMainRenderer, strings);
    	src_rect = (SDL_Rect){0, 0, strings->w, strings->h};
    	dst_rect = (SDL_Rect){0, 0, 0, 0};
    	SDL_RenderCopy(gMainRenderer, texture, &src_rect, &dst_rect);
    	SDL_FreeSurface(strings);
  	
  	/* 機体と弾の作成 */
	for(i=0;i<num;i++){
		if (i < num/2){
			gCharaAllyRect[i].x = 160+240*i;
			gCharaAllyRect[i].y=80;
			gCharaAllyRect[i].w=80;
			gCharaAllyRect[i].h=80;

			s=charaAlly;
			imageAlly = IMG_Load(s);
			textureAlly = SDL_CreateTextureFromSurface(gMainRenderer, imageAlly);
			srcAlly_rect = (SDL_Rect){0, 0, imageAlly->w, imageAlly->h};
			SDL_RenderCopy(gMainRenderer, textureAlly, &srcAlly_rect, (&gCharaAllyRect[i]));
			SDL_FreeSurface(imageAlly);			
		}
		else if(num == 4) {
			gCharaEnemyRect[i].x = 160+240*(i-2);
			gCharaEnemyRect[i].y=800;
			gCharaEnemyRect[i].w=80;
			gCharaEnemyRect[i].h=80;
	
			s=charaEnemy;
			imageEnemy = IMG_Load(s);
			textureEnemy = SDL_CreateTextureFromSurface(gMainRenderer, imageEnemy);
			srcEnemy_rect = (SDL_Rect){0, 0, imageEnemy->w, imageEnemy->h};
			SDL_RenderCopy(gMainRenderer, textureEnemy, &srcEnemy_rect, (&gCharaEnemyRect[i]));
			SDL_FreeSurface(imageEnemy);
					
		}
		else {
			gCharaEnemyRect[i].x = 160+240*i;
			gCharaEnemyRect[i].y=800;
			gCharaEnemyRect[i].w=80;
			gCharaEnemyRect[i].h=80;
	
			s=charaEnemy;
			imageEnemy = IMG_Load(s);
			textureEnemy = SDL_CreateTextureFromSurface(gMainRenderer, imageEnemy);
			srcEnemy_rect = (SDL_Rect){0, 0, imageEnemy->w, imageEnemy->h};
			SDL_RenderCopy(gMainRenderer, textureEnemy, &srcEnemy_rect, (&gCharaEnemyRect[i]));
			SDL_FreeSurface(imageEnemy);
		}
	}
		
	for (i = 0; i < BEAMNUM; i++) {
		gCharaBeam0Rect[i].x=1000;
		gCharaBeam0Rect[i].y=1000;
		gCharaBeam0Rect[i].w=0;
		gCharaBeam0Rect[i].h=0;

		s=charaBeam0;
		imageBeam0 = IMG_Load(s);
		textureBeam0 = SDL_CreateTextureFromSurface(gMainRenderer, imageBeam0);
		srcBeam0_rect = (SDL_Rect){8, 88, 12, 12};
		SDL_RenderCopy(gMainRenderer, textureBeam0, &srcBeam0_rect, (&gCharaBeam0Rect[i]));
		SDL_FreeSurface(imageBeam0);
			
		gCharaBeam1Rect[i].x=1000;
		gCharaBeam1Rect[i].y=1000;
		gCharaBeam1Rect[i].w=0;
		gCharaBeam1Rect[i].h=0;

		s=charaBeam1;
		imageBeam1 = IMG_Load(s);
		textureBeam1 = SDL_CreateTextureFromSurface(gMainRenderer, imageBeam1);
		srcBeam1_rect = (SDL_Rect){8, 88, 12, 12};
		SDL_RenderCopy(gMainRenderer, textureBeam1, &srcBeam1_rect, (&gCharaBeam1Rect[i]));
		SDL_FreeSurface(imageBeam1);
			
		gCharaBeam2Rect[i].x=1000;
		gCharaBeam2Rect[i].y=1000;
		gCharaBeam2Rect[i].w=0;
		gCharaBeam2Rect[i].h=0;

		s=charaBeam2;
		imageBeam2 = IMG_Load(s);
		textureBeam2 = SDL_CreateTextureFromSurface(gMainRenderer, imageBeam2);
		srcBeam2_rect = (SDL_Rect){8, 108, 12, 12};
		SDL_RenderCopy(gMainRenderer, textureBeam2, &srcBeam2_rect, (&gCharaBeam2Rect[i]));
		SDL_FreeSurface(imageBeam2);
			
		gCharaBeam3Rect[i].x=1000;
		gCharaBeam3Rect[i].y=1000;
		gCharaBeam3Rect[i].w=0;
		gCharaBeam3Rect[i].h=0;

		s=charaBeam3;
		imageBeam3 = IMG_Load(s);
		textureBeam3 = SDL_CreateTextureFromSurface(gMainRenderer, imageBeam3);
		srcBeam3_rect = (SDL_Rect){8, 108, 12, 12};
		SDL_RenderCopy(gMainRenderer, textureBeam3, &srcBeam3_rect, (&gCharaBeam3Rect[i]));
		SDL_FreeSurface(imageBeam3);
	}
	
	//joycon_open(&jc, JOYCON_R);
	Mix_PlayMusic(music, -1);
}

/* 画像更新 */
void Update (int num, int clientID)
{
	int i;
	SDL_RenderClear(gMainRenderer);
	
	SDL_RenderCopy(gMainRenderer, textureBack, NULL, NULL);
	SDL_RenderCopy(gMainRenderer, textureEarthA, NULL, NULL);
	SDL_RenderCopy(gMainRenderer, textureEarthE, NULL, NULL);
	SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstItem_rect);
	SDL_RenderCopy(gMainRenderer, textureMeteo, &srcMeteo_rect, &dstMeteo_rect);
	
	for (i = 0; i < num; i++) {
		if (i < num/2)
			SDL_RenderCopy(gMainRenderer, textureAlly, &srcAlly_rect, (&gCharaAllyRect[i]));
		else 
			SDL_RenderCopy(gMainRenderer, textureEnemy, &srcEnemy_rect, (&gCharaEnemyRect[i]));
	}
	
	for (i = 0; i < BEAMNUM; i++) {
		SDL_RenderCopy(gMainRenderer, textureBeam0, &srcBeam0_rect, (&gCharaBeam0Rect[i]));
		SDL_RenderCopy(gMainRenderer, textureBeam1, &srcBeam1_rect, (&gCharaBeam1Rect[i]));
		SDL_RenderCopy(gMainRenderer, textureBeam2, &srcBeam2_rect, (&gCharaBeam2Rect[i]));
		SDL_RenderCopy(gMainRenderer, textureBeam3, &srcBeam3_rect, (&gCharaBeam3Rect[i]));
	}

	boxColor(gMainRenderer, 0, 475, 640, 485, 0xff00ff00);

	if(clientID==0){
		for (int i = 0;i < HP[clientID]; i++) {
		dstHp_rect = (SDL_Rect){30*i, 440, 30, 30};
		SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstHp_rect);
		}
	}

	if(clientID==1){
		for (int i = 0;i < HP[clientID]; i++) {
		dstHp_rect = (SDL_Rect){30*i, 440, 30, 30};
		SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstHp_rect);
		}
	}

	if(clientID==2){
		for (int i = 0;i < HP[clientID]; i++) {
		dstHp_rect = (SDL_Rect){30*i, 490, 30, 30};
		SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstHp_rect);
		}
	}

	if(clientID==3){
		for (int i = 0;i < HP[clientID]; i++) {
		dstHp_rect = (SDL_Rect){30*i, 490, 30, 30};
		SDL_RenderCopy(gMainRenderer, textureItem, &srcItem_rect, &dstHp_rect);
		}
	}

	if(clientID==0){
		for (int i = 0;i < load[0] - 1; i++) {
			dstLoad0_rect = (SDL_Rect){30*(i/2), 410, 30, 30};
			SDL_RenderCopy(gMainRenderer, textureLoad0, &srcLoad0_rect, &dstLoad0_rect);
		}
	}

	if(clientID==1){
		for (int i = 0;i < load[1] - 1; i++) {
			if (load[1]) {
				dstLoad1_rect = (SDL_Rect){30*(i/2), 410, 30, 30};
				SDL_RenderCopy(gMainRenderer, textureLoad1, &srcLoad1_rect, &dstLoad1_rect);
			}
		}
	}

	if(clientID==2){
		for (int i = 0;i < load[2] - 1; i++) {
			if (load[2]) {
				dstLoad2_rect = (SDL_Rect){30*(i/2), 520, 30, 30};
				SDL_RenderCopy(gMainRenderer, textureLoad2, &srcLoad2_rect, &dstLoad2_rect);
			}
		}
	}

	if(clientID==3){
		for (int i = 0;i < load[3] - 1; i++) {
			if (load[3]) {
				dstLoad3_rect = (SDL_Rect){30*(i/2), 520, 30, 30};
				SDL_RenderCopy(gMainRenderer, textureLoad3, &srcLoad3_rect, &dstLoad3_rect);
			}
		}
	}

	SDL_RenderCopy(gMainRenderer, texture, &src_rect, &dst_rect);
	SDL_RenderPresent(gMainRenderer);
}

/*****************************************************************
関数名	: DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow(void)
{
	//joycon_close(&jc);
	SDL_Quit();
}

/*****************************************************************
関数名	: WindowEvent
機能	: メインウインドウに対するイベント処理を行う
引数	: int		num		: 全クライアント数
出力	: なし
*****************************************************************/
void WindowEvent(int num, int clientID)
{
	SDL_Event event;
	int buttonNO;

	/* 引き数チェック */
	assert(0<num && num<=MAX_CLIENTS);

	Collision(num); 
	Update(num, clientID);

	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				SendEndCommand();
				break;
			case SDL_KEYDOWN:
				if (event.key.repeat)
                			break;
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					SendEndCommand();
					break;
				case SDLK_UP:
					SendUpCommand();
					break;
				case SDLK_DOWN:
					SendDownCommand();
					break;
				case SDLK_LEFT:
					SendLeftCommand();
					break;
				case SDLK_RIGHT:
					SendRightCommand();
					break;
				case SDLK_SPACE:
					SendBeamCommand();
					if (load[clientID] > 0)
					Mix_PlayChannel(1, chunk1, 0);
					break;
				case SDLK_TAB:
					SendLoadCommand();
					break;
			}
			break;
		}
	}
	
	/* joycon_get_state(&jc);

	if (jc.button.btn.Home) {
		SendEndCommand();
	}

	if (&jc.stick) {
		if (jc.stick.y > 0.4 && jc.stick.y < 0.8) {
			Chara.boost		= 0;
			SendLeftCommand();
        } 
        else if (jc.stick.y >= 0.8) {
			Chara.boost		= 1;
			SendLeftCommand();
        }
        else if (jc.stick.y < -0.4 && jc.stick.y > -0.8) {
			Chara.boost		= 0;
			SendRightCommand();
        } 
        else if (jc.stick.y <= -0.8) {
            Chara.boost		= 1;
            SendRightCommand();
        } 
        else if (jc.stick.x > 0.4 && jc.stick.x < 0.8) {
			Chara.boost		= 0;
            SendDownCommand();
        }  
        else if (jc.stick.x >= 0.8) {
			Chara.boost		= 1;
            SendDownCommand();
        } 
        else if (jc.stick.x < -0.4 && jc.stick.x > -0.8) {
			Chara.boost		= 0;
            SendUpCommand();
        } 
        else if (jc.stick.x <= -0.8) {
            Chara.boost		= 1;
            SendUpCommand();
        }
		else if ((jc.stick.x >= -0.4 && jc.stick.x <= 0.4) && (jc.stick.y >= -0.4 && jc.stick.y <= 0.4)) {
			SendNoneCommand();
		}
	} */
}

/*****************************************************************
関数名	: MoveUp
機能	: キャラクターを上に動かす。
引数	: int		y		: y座標の変化量
		  int		clientID: クライアント番号
出力	: なし
*****************************************************************/
void MoveUp(int y, int clientID)
{
	if (clientID < 2) {
		if (gCharaAllyRect[clientID].y > 0) {
			gCharaAllyRect[clientID].y -= y;
		}
	}
	else {
		if (gCharaEnemyRect[clientID].y > 485) {
			gCharaEnemyRect[clientID].y -= y;
		}
	}
}

/*****************************************************************
関数名	: MoveDown
機能	: キャラクターを下に動かす。
引数	: int		y		: y座標の変化量
		  int		clientID: クライアント番号
出力	: なし
*****************************************************************/
void MoveDown(int y, int clientID)
{
	if (clientID < 2) {
		if (gCharaAllyRect[clientID].y + 80 < WINDOW_HEIGHT / 2) {
			gCharaAllyRect[clientID].y += y;
		}
	}
	else {
		if (gCharaEnemyRect[clientID].y + 80 < WINDOW_HEIGHT) {
			gCharaEnemyRect[clientID].y += y;
		}
	}
}

/*****************************************************************
関数名	: MoveLeft
機能	: キャラクターを左に動かす。
引数	: int		x		: x座標の変化量
		  int		clientID: クライアント番号
出力	: なし
*****************************************************************/
void MoveLeft(int x, int clientID)
{
	if (clientID < 2) {
		if (gCharaAllyRect[clientID].x > 0) {
			gCharaAllyRect[clientID].x -= x;
		}
	}
	else {
		if (gCharaEnemyRect[clientID].x > 0) {
			gCharaEnemyRect[clientID].x -= x;
		}
	}
}

/*****************************************************************
関数名	: MoveRight
機能	: キャラクターを右に動かす。
引数	: int		x		: x座標の変化量
		  int		clientID: クライアント番号
出力	: なし
*****************************************************************/
void MoveRight(int x, int clientID)
{
	if (clientID < 2) {
		if (gCharaAllyRect[clientID].x + 80 < WINDOW_WIDTH) {
			gCharaAllyRect[clientID].x += x;
		}
	}
	else {
		if (gCharaEnemyRect[clientID].x + 80 < WINDOW_WIDTH) {
			gCharaEnemyRect[clientID].x += x;
		}
	}
}

/*****************************************************************
関数名	: MoveItem
機能	: アイテムを動かす。
引数	: int		x		: x座標の変化量
出力	: なし
*****************************************************************/
void MoveItem(int y)
{
	int i;
	timerIcnt++;
	if (timerIcnt == 1)
		SDL_AddTimer(20, ItemAnimation, NULL);
	
	if (gCharaBeam0Rect[0].h == 0) {
		dstItem_rect.x = 700;
		dstItem_rect.y = y;
		dstItem_rect.w = ITEMSIZE;
		dstItem_rect.h = ITEMSIZE;
	}
}

/*****************************************************************
関数名	: MoveMeteo
機能	: 隕石を動かす。
引数	: int		x		: x座標の変化量
出力	: なし
*****************************************************************/
void MoveMeteo(int y)
{
	int i;
	timerMcnt++;
	if (timerMcnt == 1)
		SDL_AddTimer(5, MeteoAnimation, NULL);
	
	dstMeteo_rect.x = -50;
	dstMeteo_rect.y = y;
	dstMeteo_rect.w = METEOSIZE;
	dstMeteo_rect.h = METEOSIZE;
}

/*****************************************************************
関数名	: MoveBeam
機能	: ビームを発射する。
引数	: int		clientID: クライアント番号 
出力	: なし
*****************************************************************/
void MoveBeam(int clientID)
{
	if (beam0cnt == BEAMNUM)
		beam0cnt = 0;
	
	if (clientID == 0 && load[0] > 1) {
		timer0cnt++;
		if (timer0cnt == 1)
			SDL_AddTimer(BEAMSPEED, BeamAnimation0, NULL);
		
		gCharaBeam0Rect[beam0cnt].x = gCharaAllyRect[0].x + 20;
		gCharaBeam0Rect[beam0cnt].y = gCharaAllyRect[0].y + 80;
		gCharaBeam0Rect[beam0cnt].w = BEAMSIZE;
		gCharaBeam0Rect[beam0cnt].h = BEAMSIZE;
		
		beam0cnt++;
		if (load[0] % 2 == 1 && load[0] != 1)
			load[0] -= 3;
		else if (load[0] == 1)
			load[0] -= 1;
		else
			load[0] -= 2;
	}
	else if (clientID == 1 && load[1] > 1) {
		timer1cnt++;
		if (timer1cnt == 1)
			SDL_AddTimer(BEAMSPEED, BeamAnimation1, NULL);
	
		gCharaBeam1Rect[beam1cnt].x = gCharaAllyRect[1].x + 20;
		gCharaBeam1Rect[beam1cnt].y = gCharaAllyRect[1].y + 80;
		gCharaBeam1Rect[beam1cnt].w = BEAMSIZE;
		gCharaBeam1Rect[beam1cnt].h = BEAMSIZE;
		
		beam1cnt++;
		if (load[1] % 2 == 1 && load[1] != 1)
			load[1] -= 3;
		else if (load[1] == 1)
			load[1] -= 1;
		else
			load[1] -= 2;
	}
	else if (clientID == 2 && load[2] > 1) {
		timer2cnt++;
		if (timer2cnt == 1)
			SDL_AddTimer(BEAMSPEED, BeamAnimation2, NULL);
	
		gCharaBeam2Rect[beam2cnt].x = gCharaEnemyRect[2].x + 20;
		gCharaBeam2Rect[beam2cnt].y = gCharaEnemyRect[2].y - 40;
		gCharaBeam2Rect[beam2cnt].w = BEAMSIZE;
		gCharaBeam2Rect[beam2cnt].h = BEAMSIZE;
		
		beam2cnt++;
		if (load[2] % 2 == 1 && load[2] != 1)
			load[2] -= 3;
		else if (load[2] == 1)
			load[2] -= 1;
		else
			load[2] -= 2;
	}
	else if (clientID == 3 && load[3] > 1) {
		timer3cnt++;
		if (timer3cnt == 1)
			SDL_AddTimer(BEAMSPEED, BeamAnimation3, NULL);
	
		gCharaBeam3Rect[beam3cnt].x = gCharaEnemyRect[3].x + 20;
		gCharaBeam3Rect[beam3cnt].y = gCharaEnemyRect[3].y - 40;
		gCharaBeam3Rect[beam3cnt].w = BEAMSIZE;
		gCharaBeam3Rect[beam3cnt].h = BEAMSIZE;
		
		beam3cnt++;
		if (load[3] % 2 == 1 && load[3] != 1)
			load[3] -= 3;
		else if (load[3] == 1)
			load[3] -= 1;
		else
			load[3] -= 2;
	}
}

/*****************************************************************
関数名	: MoveLoad
機能	: 弾を装填する。
引数	: int		clientID: クライアント番号
出力	: なし
*****************************************************************/
void MoveLoad(int clientID)
{
	if (clientID == 0 && load[0] < 10)
		load[0]++;
	if (clientID == 1 && load[1] < 10)
		load[1]++;
	if (clientID == 2 && load[2] < 10)
		load[2]++;
	if (clientID == 3 && load[3] < 10)
		load[3]++;
}

void Collision(int num)
{
	int i, j;
	
	for (i = 0; i < BEAMNUM; i++) {
		for (j = 2; j < num; j++) {
			if (gCharaBeam0Rect[i].x + BEAMSIZE  - 10 > gCharaEnemyRect[j].x && gCharaBeam0Rect[i].x < gCharaEnemyRect[j].x + 65 && gCharaBeam0Rect[i].y + BEAMSIZE  - 37 > gCharaEnemyRect[j].y && gCharaBeam0Rect[i].y < gCharaEnemyRect[j].y + 65) {
				gCharaBeam0Rect[i].x = 1000;
				gCharaBeam0Rect[i].y = 1000;
				gCharaBeam0Rect[i].w = 0;
				gCharaBeam0Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam1Rect[i].x + BEAMSIZE - 10 > gCharaEnemyRect[j].x && gCharaBeam1Rect[i].x < gCharaEnemyRect[j].x + 65 && gCharaBeam1Rect[i].y + BEAMSIZE - 37 > gCharaEnemyRect[j].y && gCharaBeam1Rect[i].y < gCharaEnemyRect[j].y + 65) {
				gCharaBeam1Rect[i].x = 1000;
				gCharaBeam1Rect[i].y = 1000;
				gCharaBeam1Rect[i].w = 0;
				gCharaBeam1Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam0Rect[i].x + BEAMSIZE - 30 > gCharaEnemyRect[j].x && gCharaBeam0Rect[i].x < gCharaEnemyRect[j].x + 45 && gCharaBeam0Rect[i].y + BEAMSIZE  - 10 > gCharaEnemyRect[j].y && gCharaBeam0Rect[i].y < gCharaEnemyRect[j].y + 65) {
				gCharaBeam0Rect[i].x = 1000;
				gCharaBeam0Rect[i].y = 1000;
				gCharaBeam0Rect[i].w = 0;
				gCharaBeam0Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam1Rect[1].x + BEAMSIZE - 30 > gCharaEnemyRect[j].x && gCharaBeam1Rect[i].x < gCharaEnemyRect[j].x + 45 && gCharaBeam1Rect[i].y + BEAMSIZE - 10 > gCharaEnemyRect[j].y && gCharaBeam1Rect[i].y < gCharaEnemyRect[j].y + 65) {
				gCharaBeam1Rect[i].x = 1000;
				gCharaBeam1Rect[i].y = 1000;
				gCharaBeam1Rect[i].w = 0;
				gCharaBeam1Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (HP[j] == 0) {
				gCharaEnemyRect[j].x = -1000;
				gCharaEnemyRect[j].y = -1000;
				gCharaEnemyRect[j].w = 0;
				gCharaEnemyRect[j].h = 0;
			}
		}
	}
	
	for (i = 0; i < BEAMNUM; i++) {
		for (j = 0; j < num / 2; j++) {
			if (gCharaBeam2Rect[i].x + BEAMSIZE - 10 > gCharaAllyRect[j].x && gCharaBeam2Rect[i].x < gCharaAllyRect[j].x + 65 && gCharaBeam2Rect[i].y + BEAMSIZE - 10 > gCharaAllyRect[j].y && gCharaBeam2Rect[i].y < gCharaAllyRect[j].y + 46) {
				gCharaBeam2Rect[i].x = 1000;
				gCharaBeam2Rect[i].y = 1000;
				gCharaBeam2Rect[i].w = 0;
				gCharaBeam2Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam3Rect[i].x + BEAMSIZE - 10 > gCharaAllyRect[j].x && gCharaBeam3Rect[i].x < gCharaAllyRect[j].x + 65 && gCharaBeam3Rect[i].y + BEAMSIZE - 10 > gCharaAllyRect[j].y && gCharaBeam3Rect[i].y < gCharaAllyRect[j].y + 46) {
				gCharaBeam3Rect[i].x = 1000;
				gCharaBeam3Rect[i].y = 1000;
				gCharaBeam3Rect[i].w = 0;
				gCharaBeam3Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam2Rect[i].x + BEAMSIZE - 30 > gCharaAllyRect[j].x && gCharaBeam2Rect[i].x < gCharaAllyRect[j].x + 45 && gCharaBeam2Rect[i].y + BEAMSIZE - 10 > gCharaAllyRect[j].y && gCharaBeam2Rect[i].y < gCharaAllyRect[j].y + 65) {
				gCharaBeam2Rect[i].x = 1000;
				gCharaBeam2Rect[i].y = 1000;
				gCharaBeam2Rect[i].w = 0;
				gCharaBeam2Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (gCharaBeam3Rect[i].x + BEAMSIZE - 30 > gCharaAllyRect[j].x && gCharaBeam3Rect[i].x < gCharaAllyRect[j].x + 45 && gCharaBeam3Rect[i].y + BEAMSIZE - 10 > gCharaAllyRect[j].y && gCharaBeam3Rect[i].y < gCharaAllyRect[j].y + 65) {
				gCharaBeam3Rect[i].x = 1000;
				gCharaBeam3Rect[i].y = 1000;
				gCharaBeam3Rect[i].w = 0;
				gCharaBeam3Rect[i].h = 0;
				if (HP[j] == 1) {
					HP[j]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
				else if (HP[j] > 0 || HP[j] != 1) {
					HP[j]--;
					Mix_PlayChannel(2, chunk2, 0);
				}
			}
			if (HP[j] == 0) {
				gCharaAllyRect[j].x = -1000;
				gCharaAllyRect[j].y = -1000;
				gCharaAllyRect[j].w = 0;
				gCharaAllyRect[j].h = 0;
			}
		}
	}
	
	for (i = 0; i < num / 2; i++) {
		if (dstItem_rect.x + ITEMSIZE - 10 > gCharaAllyRect[i].x && dstItem_rect.x < gCharaAllyRect[i].x + 65 && dstItem_rect.y + ITEMSIZE - 10 > gCharaAllyRect[i].y && dstItem_rect.y < gCharaAllyRect[i].y + 46) {
			dstItem_rect.x = 1000;
			dstItem_rect.y = -1000;
			dstItem_rect.w = 0;
			dstItem_rect.h = 0;
			if(HP[i] < 6) {
				HP[i]++;
				Mix_PlayChannel(3, chunk3, 0);
			}
		}
		if (dstItem_rect.x + ITEMSIZE - 30 > gCharaAllyRect[i].x && dstItem_rect.x < gCharaAllyRect[i].x + 45 && dstItem_rect.y + ITEMSIZE - 10 > gCharaAllyRect[i].y && dstItem_rect.y < gCharaAllyRect[i].y + 65) {
			dstItem_rect.x = 1000;
			dstItem_rect.y = -1000;
			dstItem_rect.w = 0;
			dstItem_rect.h = 0;
			if(HP[i] < 6) {
				HP[i]++;
				Mix_PlayChannel(3, chunk3, 0);
			}
		}
	}
	for (i = 2; i < num; i++) {
		if (dstItem_rect.x + ITEMSIZE - 10 > gCharaEnemyRect[i].x && dstItem_rect.x < gCharaEnemyRect[i].x + 65 && dstItem_rect.y + ITEMSIZE - 37 > gCharaEnemyRect[i].y && dstItem_rect.y < gCharaEnemyRect[i].y + 65) {
			dstItem_rect.x = 1000;
			dstItem_rect.y = -1000;
			dstItem_rect.w = 0;
			dstItem_rect.h = 0;
			if(HP[i] < 6) {
				HP[i]++;
				Mix_PlayChannel(3, chunk3, 0);
			}
		}
		if (dstItem_rect.x + ITEMSIZE - 30 > gCharaEnemyRect[i].x && dstItem_rect.x < gCharaEnemyRect[i].x + 45 && dstItem_rect.y + ITEMSIZE - 10 > gCharaEnemyRect[i].y && dstItem_rect.y < gCharaEnemyRect[i].y + 65) {
			dstItem_rect.x = 1000;
			dstItem_rect.y = -1000;
			dstItem_rect.w = 0;
			dstItem_rect.h = 0;
			if(HP[i] < 6) {
				HP[i]++;
				Mix_PlayChannel(3, chunk3, 0);
			}
		}
	}

	for (i = 0; i < num / 2; i++) {
		if (dstMeteo_rect.x + ITEMSIZE - 10 > gCharaAllyRect[i].x && dstMeteo_rect.x < gCharaAllyRect[i].x + 65 && dstMeteo_rect.y + ITEMSIZE - 10 > gCharaAllyRect[i].y && dstMeteo_rect.y < gCharaAllyRect[i].y + 46) {
			dstMeteo_rect.x = -1000;
			dstMeteo_rect.y = 1000;
			dstMeteo_rect.w = 0;
			dstMeteo_rect.h = 0;
			if (HP[i] == 1) {
					HP[i]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
			else if (HP[i] > 0 || HP[i] != 1) {
				HP[i]--;
				Mix_PlayChannel(2, chunk2, 0);
			}
		}
		if (dstMeteo_rect.x + ITEMSIZE - 30 > gCharaAllyRect[i].x && dstMeteo_rect.x < gCharaAllyRect[i].x + 45 && dstMeteo_rect.y + ITEMSIZE - 10 > gCharaAllyRect[i].y && dstMeteo_rect.y < gCharaAllyRect[i].y + 65) {
			dstMeteo_rect.x = -1000;
			dstMeteo_rect.y = 1000;
			dstMeteo_rect.w = 0;
			dstMeteo_rect.h = 0;
			if (HP[i] == 1) {
					HP[i]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
			else if (HP[i] > 0 || HP[i] != 1) {
				HP[i]--;
				Mix_PlayChannel(2, chunk2, 0);
			}
		}
	}
	for (i = 2; i < num; i++) {
		if (dstMeteo_rect.x + ITEMSIZE - 10 > gCharaEnemyRect[i].x && dstMeteo_rect.x < gCharaEnemyRect[i].x + 65 && dstMeteo_rect.y + ITEMSIZE - 37 > gCharaEnemyRect[i].y && dstMeteo_rect.y < gCharaEnemyRect[i].y + 65) {
			dstMeteo_rect.x = -1000;
			dstMeteo_rect.y = 1000;
			dstMeteo_rect.w = 0;
			dstMeteo_rect.h = 0;
			if (HP[i] == 1) {
					HP[i]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
			else if (HP[i] > 0 || HP[i] != 1) {
				HP[i]--;
				Mix_PlayChannel(2, chunk2, 0);
			}
		}
		if (dstMeteo_rect.x + ITEMSIZE - 30 > gCharaEnemyRect[i].x && dstMeteo_rect.x < gCharaEnemyRect[i].x + 45 && dstMeteo_rect.y + ITEMSIZE - 10 > gCharaEnemyRect[i].y && dstMeteo_rect.y < gCharaEnemyRect[i].y + 65) {
			dstMeteo_rect.x = -1000;
			dstMeteo_rect.y = 1000;
			dstMeteo_rect.w = 0;
			dstMeteo_rect.h = 0;
			if (HP[i] == 1) {
					HP[i]--;
					Mix_PlayChannel(4, chunk4, 0);
				}
			else if (HP[i] > 0 || HP[i] != 1) {
				HP[i]--;
				Mix_PlayChannel(2, chunk2, 0);
			}
		}
	}
	
	if (((gCharaAllyRect[0].h == 0 && gCharaAllyRect[1].h == 0) || (gCharaEnemyRect[2].h == 0 && gCharaEnemyRect[3].h == 0)) && tntn == 0) {
		dst_rect.x = 120;
		dst_rect.y = 355;
		dst_rect.w = 400;
		dst_rect.h = 250;
		Mix_HaltChannel(1);
		Mix_HaltChannel(2);
		Mix_HaltChannel(3);
		Mix_HaltChannel(4);
		Mix_HaltMusic();
		Mix_PlayChannel(5, chunk5, 0);

		tntn++;
	}
	
	SDL_RenderPresent(gMainRenderer);
}


static Uint32 ItemAnimation(Uint32 interval, void *param)
{
	dstItem_rect.x -= 1;
	
	if (dstItem_rect.x + ITEMSIZE < 0) {
		dstItem_rect.x = 1000;
		dstItem_rect.y = -1000;
		dstItem_rect.w = 0;
		dstItem_rect.h = 0;
	}

	return interval;
}


static Uint32 MeteoAnimation(Uint32 interval, void *param)
{
	dstMeteo_rect.x += 1;
	
	if (dstMeteo_rect.x > 650) {
		dstMeteo_rect.x = -1000;
		dstMeteo_rect.y = 1000;
		dstMeteo_rect.w = 0;
		dstMeteo_rect.h = 0;
	}

	return interval;
}


static Uint32 BeamAnimation0(Uint32 interval, void *param)
{	
	int i;
	
	for (i = 0; i < beam0cnt + 1; i++) {
		gCharaBeam0Rect[i].y += 1;
		
		if (gCharaBeam0Rect[i].y > WINDOW_HEIGHT) {
			gCharaBeam0Rect[i].x = 1000;
			gCharaBeam0Rect[i].y = 1000;
			gCharaBeam0Rect[i].w = 0;
			gCharaBeam0Rect[i].h = 0;
		}
	}
	
	return interval;
}


static Uint32 BeamAnimation1(Uint32 interval, void *param)
{	
	int i;
	
	for (i = 0; i < beam1cnt + 1; i++) {
		gCharaBeam1Rect[i].y += 1;
		
		if (gCharaBeam1Rect[i].y > WINDOW_HEIGHT) {
			gCharaBeam1Rect[i].x = 1000;
			gCharaBeam1Rect[i].y = 1000;
			gCharaBeam1Rect[i].w = 0;
			gCharaBeam1Rect[i].h = 0;
		}
	}
	
	return interval;
}


static Uint32 BeamAnimation2(Uint32 interval, void *param)
{	
	int i;
	
	for (i = 0; i < beam2cnt + 1; i++) {
		gCharaBeam2Rect[i].y -= 1;
		
		if (gCharaBeam2Rect[i].y + 40 < 0) {
			gCharaBeam2Rect[i].x = 1000;
			gCharaBeam2Rect[i].y = 1000;
			gCharaBeam2Rect[i].w = 0;
			gCharaBeam2Rect[i].h = 0;
		}
	}
	
	return interval;
}

static Uint32 BeamAnimation3(Uint32 interval, void *param)
{	
	int i;
	
	for (i = 0; i < beam3cnt + 1; i++) {
		gCharaBeam3Rect[i].y -= 1;
		
		if (gCharaBeam3Rect[i].y + 40 < 0) {
			gCharaBeam3Rect[i].x = 1000;
			gCharaBeam3Rect[i].y = 1000;
			gCharaBeam3Rect[i].w = 0;
			gCharaBeam3Rect[i].h = 0;
		}
	}
	
	return interval;
}

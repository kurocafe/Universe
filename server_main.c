/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/

#include<SDL2/SDL.h>
#include"server_common.h"

static Uint32 SignalHandler1(Uint32 interval, void *param);
static Uint32 SignalHandler2(Uint32 interval, void *param);

int main(int argc,char *argv[])
{
	int	num;
	int	endFlag = 1;

	/* 引き数チェック */
	if(argc != 2){
		fprintf(stderr,"Usage: number of clients\n");
		exit(-1);
	}
	if((num = atoi(argv[1])) < 0 ||  num > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* クライアントとの接続 */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	
	/* 割り込み処理のセット */
	SDL_AddTimer(15000,SignalHandler1,NULL);
	SDL_AddTimer(5000,SignalHandler2,NULL);
	
	/* メインイベントループ */
	while(endFlag){
		endFlag = SendRecvManager();
		SDL_Delay(10);
	};

	/* 終了処理 */
	Ending();

	return 0;
}

/*****************************************************************
関数名  : SignalHandler
機能    : 割り込み用関数 
引数    : Uint32	interval	: タイマー
		  void		*param		: 割り込み処理の引数
出力    : タイマーの次の間隔
*****************************************************************/
static Uint32 SignalHandler1(Uint32 interval, void *param)
{
	SendItemCommand();

	return interval;
}

static Uint32 SignalHandler2(Uint32 interval, void *param)
{
	SendMeteoCommand();

	return interval;
}

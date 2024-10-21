/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static int GetRandomInt(int n);
static int RecvDataInfo = 0;

/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos, int clientID, int charaID)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

    switch(command){
	    case END_COMMAND:
			dataSize = 0;

			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);

			/* 全ユーザーに送る */
			SendData(ALL_CLIENTS,data,dataSize);
			
			break;
	    case UP_COMMAND:
	    case DOWN_COMMAND:
	    case LEFT_COMMAND:
	    case RIGHT_COMMAND:
			dataSize = 0;

			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);
			/* 座標 */
			SetIntData2DataBlock(data,20,&dataSize);
			/* ID */
			SetIntData2DataBlock(data, pos, &dataSize);
			
			/* 全ユーザーに送る */
			SendData(ALL_CLIENTS,data,dataSize);

			break;
	    case SPACE_COMMAND:
	    case LOAD_COMMAND:
			dataSize = 0;

			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);

			/* ID */
			SetIntData2DataBlock(data, pos, &dataSize);
			
			/* 全ユーザーに送る */
			SendData(ALL_CLIENTS,data,dataSize);

			break;
	    default:
			/* 未知のコマンドが送られてきた */
			fprintf(stderr,"0x%02x is absence!\n",command);
    }
    return endFlag;
}


void SendItemCommand(void)
{
	unsigned char data[MAX_DATA];
    	int           dataSize;

    	dataSize = 0;
    	/* コマンドのセット */
   	 SetCharData2DataBlock(data,ITEM_COMMAND,&dataSize);
   	 /* アイテムの y 座標 */
    	SetIntData2DataBlock(data,GetRandomInt(960),&dataSize);

   	 /* 全ユーザーに送る */
    	SendData(ALL_CLIENTS,data,dataSize);
}

void SendMeteoCommand(void)
{
    	unsigned char data[MAX_DATA];
    	int           dataSize;

    	dataSize = 0;
    	/* コマンドのセット */
    	SetCharData2DataBlock(data,METEO_COMMAND,&dataSize);
    	/* アイテムの y 座標 */
    	SetIntData2DataBlock(data,GetRandomInt(960),&dataSize);
    
   	/* 全ユーザーに送る */
	SendData(ALL_CLIENTS,data,dataSize);
}

/*****
static
*****/
/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}


/*****************************************************************
関数名	: GetRandomInt
機能	: 整数の乱数を得る
引数	: int		n	: 乱数の最大値
出力	: 乱数値
*****************************************************************/
static int GetRandomInt(int n)
{
    return rand()%n;
}

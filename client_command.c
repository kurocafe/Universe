/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void RecvUpData(void);
static void RecvDownData(void);
static void RecvLeftData(void);
static void RecvRightData(void);
static void RecvItemData(void);
static void RecvMeteoData(void);
static void RecvBeamData(void);
static void RecvLoadData(void);

/*****************************************************************
関数名	: ExecuteCommand
機能	: サーバーから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;

    switch(command){
		case END_COMMAND:
			endFlag = 0;
			break;
		case UP_COMMAND:
			RecvUpData();
			break;
		case DOWN_COMMAND:
			RecvDownData();
			break;
		case LEFT_COMMAND:
			RecvLeftData();
			break;
		case RIGHT_COMMAND:
			RecvRightData();
			break;
		case ITEM_COMMAND:
			RecvItemData();
			break;
        	case METEO_COMMAND:
			RecvMeteoData();
			break;
		case SPACE_COMMAND:
			RecvBeamData();
			break;
		case LOAD_COMMAND:
			RecvLoadData();
			break;
    }
    return endFlag;
}

/*****************************************************************
関数名	: SendUpCommand
機能	: 機体の上移動を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendUpCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,UP_COMMAND,&dataSize);
    
    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendDownCommand
機能	: 機体の下移動を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendDownCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,DOWN_COMMAND,&dataSize);
    
    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendLeftCommand
機能	: 機体の左移動を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendLeftCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,LEFT_COMMAND,&dataSize);

    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendRightCommand
機能	: 機体の右移動を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendRightCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;
    
    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,RIGHT_COMMAND,&dataSize);

    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendBeamCommand
機能	: ビームの発射を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendBeamCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,SPACE_COMMAND,&dataSize);

    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendLoadCommand
機能	: ビームの装填を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendLoadCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,LOAD_COMMAND,&dataSize);

    SendData(data,dataSize);
}

/*****************************************************************
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    SendData(data,dataSize);
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

    /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
関数名	: RecvUpData
機能	: サーバーからキャラクターを上に動かすためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvUpData(void)
{
    int	y;
    int charaID;

    RecvIntData(&y);
    RecvIntData(&charaID);
    
    MoveUp(y, charaID);
}

/*****************************************************************
関数名	: RecvDownData
機能	: サーバーからキャラクターを下に動かすためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvDownData(void)
{
    int	y;
    int charaID;

    RecvIntData(&y);
    RecvIntData(&charaID);

    MoveDown(y, charaID);
}

/*****************************************************************
関数名	: RecvLeftData
機能	: サーバーからキャラクターを左に動かすためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvLeftData(void)
{
    int	x;
    int charaID;

    RecvIntData(&x);
    RecvIntData(&charaID);

    MoveLeft(x, charaID);
}

/*****************************************************************
関数名	: RecvRightData
機能	: サーバーからキャラクターを右に動かすためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvRightData(void)
{
    int	x;
    int charaID;

    RecvIntData(&x);
    RecvIntData(&charaID);

    MoveRight(x, charaID);  
}

/*****************************************************************
関数名	: RecvItemData
機能	: サーバーからアイテムを出すためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvItemData(void)
{
    int	y;

    RecvIntData(&y);

    MoveItem(y);
}

/*****************************************************************
関数名	: RecvMeteoData
機能	: サーバーから隕石を出すためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvMeteoData(void)
{
    int	y;

    RecvIntData(&y);

    MoveMeteo(y); 
}

/*****************************************************************
関数名	: RecvBeamData
機能	: サーバーからキャラクターのビームを出すためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvBeamData(void)
{
    int charaID;

    RecvIntData(&charaID);

    MoveBeam(charaID);  
}

/*****************************************************************
関数名	: RecvLoadData
機能	: サーバーからキャラクターのビームを出すためのデータを受け取る。
引数	: なし
出力	: なし
*****************************************************************/
static void RecvLoadData(void)
{
    int charaID;

    RecvIntData(&charaID);

    MoveLoad(charaID);  
}

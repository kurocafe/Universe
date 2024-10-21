/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#define PORT			(u_short)8888			/* ポート番号 */

#define MAX_CLIENTS		4				/* クライアント数の最大値 */
#define MAX_NAME_SIZE		10 				/* ユーザー名の最大値*/

#define MAX_DATA		300				/* 送受信するデータの最大値 */

#define WINDOW_WIDTH		640				/* ウィンドウの幅 */
#define WINDOW_HEIGHT		960				/* ウィンドウの高さ */
#define ITEMSIZE		30				/* アイテムの大きさ */
#define METEOSIZE		45				/* 隕石の大きさ */
#define BEAMSIZE		40				/* ビームの大きさ */
#define BEAMSPEED		2				/* ビームの速さ */
#define BEAMNUM		1000				/* ビームの数 */

#define END_COMMAND		    'E'		/* プログラム終了コマンド */
#define UP_COMMAND		    'U'		/* 機体上移動コマンド */
#define DOWN_COMMAND		'D'		/* 機体下移動コマンド */
#define LEFT_COMMAND		'L'		/* 機体左移動コマンド */
#define RIGHT_COMMAND		'R'		/* 機体右移動コマンド */
#define ITEM_COMMAND		'I'		/* アイテムコマンド */
#define METEO_COMMAND		'M'		/* 隕石コマンド */
#define SPACE_COMMAND		'S'		/* ビーム発射コマンド */
#define LOAD_COMMAND		'X'		/* 装填コマンド */

#endif

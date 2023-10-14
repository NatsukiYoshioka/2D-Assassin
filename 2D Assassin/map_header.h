#pragma once

#include"DxLib.h"
#include"collision.h"

#define P_WALK      0x00        //プレイヤー歩行状態
#define P_CLIMB     0x01        //プレイヤー登り状態
#define P_HANGING   0x02        //プレイヤーぶら下がり状態

#define BLOCK1_WIDTH 3
#define BLOCK1_HEIGHT 3
typedef struct moveblock
{
    int x;
    int y;
}moveblock;

///<summary>
///マップ初期化
void mapInit();
///</summary>
///<summary>
///マップ描画
void mapDraw();
///</summary>
///<summary>
///マップ後始末
void mapFinalize();
///</summary> 
/// <param name ="checkRect">マップと当たっているか調査したい矩形</param>
/// <returns>マップと衝突したかをtrue/falseで返す</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// マップのインデックス位置からブロック番号を返す
/// </summary>
/// <param name ="ix">マップのインデックス番号x</param>
/// <param name ="iy">マップのインデックス番号y</param>
/// <returns>マップのブロック番号を返す</returns>
unsigned char mapGetBlockType(int ix, int iy);

/// <summary>
/// 壁情報取得 あるブロックの壁の有無を調べ BlockInfoを返す
/// </summary>
/// <param name="x">調査したいブロックの横インデックス</param>
/// <param name="y">調査したいブロックの縦インデックス</param>
/// <returns>壁の有無情報 BLOCK_LEFT～ BLOCK_DOWNまでのビット値が重ねあわされて返却される</returns>
BlockInfo mapGetBlockInfo(int ix, int iy);

/// <summary>
/// マップの15マス目の値をゲット
/// </summary>
/// <returns>mapChipSize * 15</returns>
float getMapXNum();

/// <summary>
/// scrollXの値をゲット
/// </summary>
/// <returns>scrollX</returns>
float getScrollXNum();

/// <summary>
/// climbside変数をゲット 
/// </summary>
/// <returns>登っているブロックの場所</returns>
BlockInfo getClimbSide();

int getPlayerStatus();

bool getPlayerSurvive();

bool getClear();

void moveblockPlayerHit(sHitRect playerRect);

bool getMoveBlockMapHit();
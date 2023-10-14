#include "player.h"
#include "collision.h"
#include "map_header.h"

const int animPatternNum = 4;     // アニメーションのパターン数
const int animLeft = 1;     // 左方向
const int animRight = 0;     // 右方向
const int animLJump = 3;		//左ジャンプ
const int animRJump = 2;		//右ジャンプ
const int animUp = 0;			//最初の壁登りパターン
const int animDown = 2;		//最初の壁下りパターン
const int animReverse = 6;	//逆さの右向きパターン
const int reverseRight = 0;	//逆さの左向きパターン
const int reverseLeft = 2;
const int animTypeNum = 7;     // アニメーションの種類
const float animationFPS = 6.0f;  // 秒間 6コマのアニメーション
const float moveSpeed = 300.0f;// 移動速度 (ここで指定した値分のピクセル/秒進む) 

const int   imageSizeX = 48;    // 画像横サイズ
const int   imageSizeY = 48;    // 画像縦サイズ
const int   hitSizeX = 48;    // 当たり判定サイズ幅
const int   hitSizeY = 48;    // 当たり判定サイズ高さ

const float jumpInitalVelocity = 8.5f;
const float gravity = 20.0f;
const float maxFallSpeed = 30.0f;
const float hitHeadBrakeRatio = 0.1f;
const int colliderOffset = 10;
bool isJump;
bool onGround;
bool hitHead;
float screenCenterNum = getMapXNum();	//マップX座標555.0fの値
float px, pxtmp, py;                     // プレイヤーの座標
float vx, vy;                     // プレイヤーの速度
float fy;
float animTimer;                  // アニメーションタイマー
int   animUpDown;
int   animReverseSide;
int   animNowType;                // 現在のアニメ種類
int   animNowPattern;            // 現在のアニメパターン
int   animNowIndex;               // アニメーション画像配列の添え字 0～11までの数値
int   drawOffsetX;                // 当たり判定からの横方向ずらし量
int   drawOffsetY;                // 当たり判定からの縦方向ずらし量

int playerImg[animPatternNum * animTypeNum]; // プレイヤーの画像ハンドル

sHitRect playerHit;
sHitRect playerFootCollider;
sHitRect playerHeadCollider;

void playerInit()
{
	px = 37.0f;
	py = 500.0f;
	vx = 0.0f;
	vy = 0.0f;
	animTimer = 0.0f;
	isJump = false;

	animNowType = animRight;     // 正面向きアニメーション種類
	animNowPattern = 0;          

	animNowIndex = 0;

	// 当たり判定初期化
	initRect(playerHit, hitSizeX, hitSizeY);
	initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
	initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

	// 横7 縦4 計28枚の画像ファイルの読み込み
	LoadDivGraph("image/assassinCharchip.png", animPatternNum * animTypeNum, animTypeNum, animPatternNum, imageSizeX, imageSizeY, playerImg);

	// 描画位置のオフセット値を計算
	// 左右から見てセンター、上下方向は底辺基準となるように計算
	drawOffsetX = (hitSizeX - imageSizeX) / 2;
	drawOffsetY = (hitSizeY - imageSizeY);

	onGround = false;
	hitHead = false;
}

void playerUpdate(float deltaTime)
{
	float scrollX = getScrollXNum();
	bool isMove = false;
	int ps;
	ps = getPlayerStatus();

	fy += gravity * deltaTime;
	if (fy * deltaTime > maxFallSpeed)
	{
		fy = maxFallSpeed;
	}

	if (ps == 0)
	{
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			vx = -moveSpeed;
			animNowPattern = animLeft;
			isMove = true;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			vx = +moveSpeed;
			animNowPattern = animRight;
			isMove = true;
		}
		if (!isJump && CheckHitKey(KEY_INPUT_SPACE))
		{
			vy -= jumpInitalVelocity;
			isJump = true;
			onGround = false;
		}

		if (onGround && !CheckHitKey(KEY_INPUT_SPACE))
		{
			isJump = false;
			vy = 0.0f;
		}
		else
		{
			isJump = true;
		}

		if (hitHead)
		{
			vy *= hitHeadBrakeRatio;
		}

		// キー入力より移動速度求める
		if (isJump)
		{
			vy += gravity * deltaTime;
		}
		if (vy * deltaTime > maxFallSpeed)
		{
			vy = maxFallSpeed;
		}

		if (isMove)
		{
			vx *= deltaTime;
		}
		else
		{
			vx = 0.0f;
		}

		// 位置更新
		if (px <= 0)
		{
			px = 0;
		}

		if (px <= 555.0f)
		{
			px += vx;
		}
		else
		{
			pxtmp = px;
			pxtmp += vx;
			screenCenterNum += vx;
			if (screenCenterNum <= 555.0f)
			{
				px += vx;
			}
		}
		py += vy;
	}
	else if (ps == 2)
	{
		onGround = true;
		if (!CheckHitKey(KEY_INPUT_SPACE))
		{
			isJump = false;
		}
		BlockInfo cs = getClimbSide();
		if (cs & BLOCK_RIGHT)
		{
			animNowPattern = 5;
		}
		else if (cs & BLOCK_LEFT)
		{
			animNowPattern = 4;
		}
		if (CheckHitKey(KEY_INPUT_UP))
		{
			vy = -moveSpeed;
			isMove = true;
			animUpDown = animUp;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			vy = +moveSpeed;
			isMove = true;
			animUpDown = animDown;
		}
		if (!isJump && CheckHitKey(KEY_INPUT_SPACE))
		{
			if (cs & BLOCK_RIGHT)
			{
				vx = 1.0f;
				animNowPattern = animLJump;
			}
			else if (cs & BLOCK_LEFT)
			{
				vx = -1.0f;
				animNowPattern = animRJump;
			}
			isJump = true;
			onGround = false;
		}

		if (isJump)
		{
			vy = 0.0f;
			vy -= jumpInitalVelocity;
			vy += gravity * deltaTime;
		}
		else if (isMove)
		{
			vy *= deltaTime;
		}
		else
		{
			vx = 0.0f;
			vy = 0.0f;
		}
		py += vy;
	}
	if (ps == 3)
	{
		vy = 0.0f;
		animNowPattern = animReverse;
		onGround = true;
		isJump = false;
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			vx = -moveSpeed;
			animReverseSide = reverseLeft;
			isMove = true;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			vx = +moveSpeed;
			animReverseSide = reverseRight;
			isMove = true;
		}
		if (isMove)
		{
			vx *= deltaTime;
		}
		else
		{
			vx = 0.0f;
		}
		py += vy;
	}
	else if (ps == 1)
	{
		if (vy <= 0)
		{
			vx = +moveSpeed;
		}
		if (vx > 0)
		{
			vx -= 250;
		}
		else
		{
			vx = 0.0;
		}
		if (!isJump)
		{
			vy = 0.0f;
			vy -= jumpInitalVelocity;
		}
		isJump = true;
		if (isJump)
		{
			vy += gravity * deltaTime;
		}
		py += vy;
		vx *= deltaTime;
	}

	// 当たり判定位置更新
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);

	////////////////////////////
	// アニメーション計算
	////////////////////////////
	if (isJump)
	{
		if (animNowPattern == 0)
		{
			animNowPattern = 2;
		}
		else if (animNowPattern == 1)
		{
			animNowPattern = 3;
		}
		if (vy < 0)
		{
			animNowType = 0;
		}
		else
		{
			animNowType = 1;
		}
	}
	else if (isMove)
	{
		// animTimer で時間経過を測り、 1.0 / animationFPS →次のアニメーションパターンに進んでよい秒数に
		// なったら次のコマに進める
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
			animTimer = 0.0f;
			if (ps == 0)
			{
				animNowType++;
				animNowType %= 2; // 余りなので 0,1,2のいずれかになる
			}
			else if (ps == 2)
			{
				if (animUpDown == animUp)
				{
					animNowType++;
					animNowType %= 2;
				}
				else if (animUpDown == animDown)
				{
					animNowType++;
					animNowType %= 2;
					animNowType += animDown;
				}
				if (animNowPattern == animLJump || animNowPattern == animRJump)
				{
					animNowType = 1;
				}
			}
			else if (ps == 3)
			{
				if (animReverseSide == reverseRight)
				{
					animNowType++;
					animNowType %= 2;
				}
				else if(animReverseSide == reverseLeft)
				{
					animNowType++;
					animNowType %= 2;
					animNowType += reverseLeft;
				}
				if (animNowPattern == animLJump || animNowPattern == animRJump)
				{
					animNowType = 1;
				}
			}
		}
	}
	else
	{
		if (animNowPattern == 2)
		{
			animNowPattern = 0;
		}
		else if (animNowPattern == 3)
		{
			animNowPattern = 1;
		}
		if (ps == 0)
		{
			animNowType = 0; // 静止時は足をそろえたパターンに強制する
		}
	}

	// 最終的な添え字を計算 
	// 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
	animNowIndex = animNowPattern + animNowType * animTypeNum;
}

void playerDraw()
{
	// 描画位置は
	DrawGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY, playerImg[animNowIndex], TRUE);
}


void playerFinalize()
{
	if (playerImg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(playerImg[i]);
		}
	}
}

sHitRect getPlayerHitRect()
{
	return playerHit;
}

void playerfixColPosition(sHitRect& hitRect)
{
	px = hitRect.worldLX;
	py = hitRect.worldLY;

	playerHit = hitRect;
}

void playerSetGroundFlg(bool groundFlg)
{
	onGround = groundFlg;
}

void playerSetHeadHitFlg(bool headHitFlg)
{
	hitHead = headHitFlg;
}

sHitRect playerGetGroundCollider()
{
	return playerFootCollider;
}

sHitRect playerGetHeadCollider()
{
	return playerHeadCollider;
}

float getPlayerMoveSpeed()
{
	return vx;
}

float getPlayerXPoisition()
{
	return px;
}

float getPlayerYPosition()
{
	return py;
}

float getFallSpeed()
{
	return fy;
}
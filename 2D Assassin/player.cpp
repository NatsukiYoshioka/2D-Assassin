#include "player.h"
#include "collision.h"
#include "map_header.h"

const int animPatternNum = 4;     // �A�j���[�V�����̃p�^�[����
const int animLeft = 1;     // ������
const int animRight = 0;     // �E����
const int animLJump = 3;		//���W�����v
const int animRJump = 2;		//�E�W�����v
const int animUp = 0;			//�ŏ��̕Ǔo��p�^�[��
const int animDown = 2;		//�ŏ��̕ǉ���p�^�[��
const int animReverse = 6;	//�t���̉E�����p�^�[��
const int reverseRight = 0;	//�t���̍������p�^�[��
const int reverseLeft = 2;
const int animTypeNum = 7;     // �A�j���[�V�����̎��
const float animationFPS = 6.0f;  // �b�� 6�R�}�̃A�j���[�V����
const float moveSpeed = 300.0f;// �ړ����x (�����Ŏw�肵���l���̃s�N�Z��/�b�i��) 

const int   imageSizeX = 48;    // �摜���T�C�Y
const int   imageSizeY = 48;    // �摜�c�T�C�Y
const int   hitSizeX = 48;    // �����蔻��T�C�Y��
const int   hitSizeY = 48;    // �����蔻��T�C�Y����

const float jumpInitalVelocity = 8.5f;
const float gravity = 20.0f;
const float maxFallSpeed = 30.0f;
const float hitHeadBrakeRatio = 0.1f;
const int colliderOffset = 10;
bool isJump;
bool onGround;
bool hitHead;
float screenCenterNum = getMapXNum();	//�}�b�vX���W555.0f�̒l
float px, pxtmp, py;                     // �v���C���[�̍��W
float vx, vy;                     // �v���C���[�̑��x
float fy;
float animTimer;                  // �A�j���[�V�����^�C�}�[
int   animUpDown;
int   animReverseSide;
int   animNowType;                // ���݂̃A�j�����
int   animNowPattern;            // ���݂̃A�j���p�^�[��
int   animNowIndex;               // �A�j���[�V�����摜�z��̓Y���� 0�`11�܂ł̐��l
int   drawOffsetX;                // �����蔻�肩��̉��������炵��
int   drawOffsetY;                // �����蔻�肩��̏c�������炵��

int playerImg[animPatternNum * animTypeNum]; // �v���C���[�̉摜�n���h��

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

	animNowType = animRight;     // ���ʌ����A�j���[�V�������
	animNowPattern = 0;          

	animNowIndex = 0;

	// �����蔻�菉����
	initRect(playerHit, hitSizeX, hitSizeY);
	initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
	initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

	// ��7 �c4 �v28���̉摜�t�@�C���̓ǂݍ���
	LoadDivGraph("image/assassinCharchip.png", animPatternNum * animTypeNum, animTypeNum, animPatternNum, imageSizeX, imageSizeY, playerImg);

	// �`��ʒu�̃I�t�Z�b�g�l���v�Z
	// ���E���猩�ăZ���^�[�A�㉺�����͒�ӊ�ƂȂ�悤�Ɍv�Z
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

		// �L�[���͂��ړ����x���߂�
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

		// �ʒu�X�V
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

	// �����蔻��ʒu�X�V
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);

	////////////////////////////
	// �A�j���[�V�����v�Z
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
		// animTimer �Ŏ��Ԍo�߂𑪂�A 1.0 / animationFPS �����̃A�j���[�V�����p�^�[���ɐi��ł悢�b����
		// �Ȃ����玟�̃R�}�ɐi�߂�
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
			animTimer = 0.0f;
			if (ps == 0)
			{
				animNowType++;
				animNowType %= 2; // �]��Ȃ̂� 0,1,2�̂����ꂩ�ɂȂ�
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
			animNowType = 0; // �Î~���͑������낦���p�^�[���ɋ�������
		}
	}

	// �ŏI�I�ȓY�������v�Z 
	// �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
	animNowIndex = animNowPattern + animNowType * animTypeNum;
}

void playerDraw()
{
	// �`��ʒu��
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
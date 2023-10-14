#pragma once

#include"DxLib.h"
#include"collision.h"

#define P_WALK      0x00        //�v���C���[���s���
#define P_CLIMB     0x01        //�v���C���[�o����
#define P_HANGING   0x02        //�v���C���[�Ԃ牺������

#define BLOCK1_WIDTH 3
#define BLOCK1_HEIGHT 3
typedef struct moveblock
{
    int x;
    int y;
}moveblock;

///<summary>
///�}�b�v������
void mapInit();
///</summary>
///<summary>
///�}�b�v�`��
void mapDraw();
///</summary>
///<summary>
///�}�b�v��n��
void mapFinalize();
///</summary> 
/// <param name ="checkRect">�}�b�v�Ɠ������Ă��邩������������`</param>
/// <returns>�}�b�v�ƏՓ˂�������true/false�ŕԂ�</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// �}�b�v�̃C���f�b�N�X�ʒu����u���b�N�ԍ���Ԃ�
/// </summary>
/// <param name ="ix">�}�b�v�̃C���f�b�N�X�ԍ�x</param>
/// <param name ="iy">�}�b�v�̃C���f�b�N�X�ԍ�y</param>
/// <returns>�}�b�v�̃u���b�N�ԍ���Ԃ�</returns>
unsigned char mapGetBlockType(int ix, int iy);

/// <summary>
/// �Ǐ��擾 ����u���b�N�̕ǂ̗L���𒲂� BlockInfo��Ԃ�
/// </summary>
/// <param name="x">�����������u���b�N�̉��C���f�b�N�X</param>
/// <param name="y">�����������u���b�N�̏c�C���f�b�N�X</param>
/// <returns>�ǂ̗L����� BLOCK_LEFT�` BLOCK_DOWN�܂ł̃r�b�g�l���d�˂��킳��ĕԋp�����</returns>
BlockInfo mapGetBlockInfo(int ix, int iy);

/// <summary>
/// �}�b�v��15�}�X�ڂ̒l���Q�b�g
/// </summary>
/// <returns>mapChipSize * 15</returns>
float getMapXNum();

/// <summary>
/// scrollX�̒l���Q�b�g
/// </summary>
/// <returns>scrollX</returns>
float getScrollXNum();

/// <summary>
/// climbside�ϐ����Q�b�g 
/// </summary>
/// <returns>�o���Ă���u���b�N�̏ꏊ</returns>
BlockInfo getClimbSide();

int getPlayerStatus();

bool getPlayerSurvive();

bool getClear();

void moveblockPlayerHit(sHitRect playerRect);

bool getMoveBlockMapHit();
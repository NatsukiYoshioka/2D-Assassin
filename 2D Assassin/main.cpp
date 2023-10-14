#include"DxLib.h"
#include<Windows.h>
#include"map_header.h"
#include"collision.h"
#include"player.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//----------------------------------// 
	//��{�ݒ�
	//----------------------------------// 

	ChangeWindowMode(FALSE);				//�E�B���h�E���[�h�ɂ���
	SetGraphMode(1184, 666, 16);			//�E�B���h�E�̃T�C�Y�ƃJ���[���[�h�����߂�

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	int titleImg = LoadGraph("image/eagle.png");
	int clearImg = LoadGraph("image/clearEagle.png");
	int overImg = LoadGraph("image/fukuro.png");

	int startBGM = LoadSoundMem("sound/kyuutai.mp3");
	int playBGM = LoadSoundMem("sound/shukumei.mp3");
	int clearBGM = LoadSoundMem("sound/tanoshiibouken.mp3");
	int overBGM = LoadSoundMem("sound/amenoprelude.mp3");

	SetFontSize(50);
	bool start = false;
	PlaySoundMem(startBGM, DX_PLAYTYPE_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		DrawExtendGraph(279, 0, 905, 666, titleImg, FALSE);
		DrawString(320, 550, "Enter�������ăX�^�[�g", GetColor(0, 0, 0), FALSE);
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			start = true;
			break;
		}
		ScreenFlip();
	}
	StopSoundMem(startBGM);

	bool survive = true;
	bool clear = false;

	PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP);
	//�Q�[���J�n
	playerInit();
	mapInit();

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && start)
	{
		float py;
		py = getPlayerYPosition();
		if (!survive)
		{
			//�Q�[���J�n
			playerInit();
			mapInit();
			PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP);
		}
		survive = getPlayerSurvive();
		clear = getClear();
		// deltaTime�v��
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;

		//�X�V����
		playerUpdate(deltaTime);

		// �v���C���[�̓����蔻���`
		sHitRect playerRect;
		playerRect = getPlayerHitRect();

		// �}�b�v�Ɠ������Ă��邩��hit�Ɋi�[
		bool hit;
		hit = mapHitCalc(playerRect);

		// �����Ԃ������Ȃ瓖���蔻��{�b�N�X����v���C���[�ʒu���C������
		if (hit)
		{
			playerfixColPosition(playerRect);
		}

		// �����`�F�b�N
		playerRect = playerGetGroundCollider();
		playerSetGroundFlg(mapHitCalc(playerRect));
		// ����`�F�b�N
		playerRect = playerGetHeadCollider();
		playerSetHeadHitFlg(mapHitCalc(playerRect));

		ClearDrawScreen();

		moveblockPlayerHit(playerRect);
		mapDraw();
		playerDraw();

		ScreenFlip();

		if (clear && py >= 666)
		{
			StopSoundMem(playBGM);
			PlaySoundMem(clearBGM, DX_PLAYTYPE_BACK);
			while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				ClearDrawScreen;
				SetBackgroundColor(255, 255, 255);
				DrawExtendGraph(0, 0, 1184, 666, clearImg, FALSE);
				DrawString(500, 600, "GAME CLEAR Enter�ŏI��", GetColor(0, 0, 0), FALSE);
				if (CheckHitKey(KEY_INPUT_RETURN))
				{
					//�Q�[���J�n
					playerInit();
					mapInit();
					break;
				}
				ScreenFlip();
			}
			StopSoundMem(clearBGM);
		}
		if (!survive)
		{
			StopSoundMem(playBGM);
			PlaySoundMem(overBGM, DX_PLAYTYPE_BACK);
			while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				ClearDrawScreen;
				DrawExtendGraph(0, 0, 1184, 666, overImg, FALSE);
				DrawString(500, 600, "GAME OVER Enter�ŏI��", GetColor(0, 0, 0), FALSE);
				if (CheckHitKey(KEY_INPUT_RETURN))
				{
					break;
				}
				ScreenFlip();
			}
			StopSoundMem(overBGM);
		}

		prevCount = nowCount;
	}
	playerFinalize();
	mapFinalize();

	DxLib_End();
	return 0;
}
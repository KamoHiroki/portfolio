//	
//	�Q�[���^�C�g���uBallAction�v
//  ����ҁF���Α�P
//	������ԁF2021/2/21�`2021/3/11
//�@
#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // ��ʂ̉���
#define SCREEN_HEIGHT    (480)                          // ��ʂ̏c��
#define CHIP_SIZE        (32)                           // ��̃`�b�v�̃T�C�Y
#define BALL_SIZE		 (30)							// �{�[���̃T�C�Y�i�摜1�ӂ̃s�N�Z�����j
#define EASY_MAP_WIDTH   (SCREEN_WIDTH  / CHIP_SIZE)	// Eazy�}�b�v�̉���
#define HARD_MAP_WIDTH   (SCREEN_WIDTH*2 / CHIP_SIZE)	// Hard�}�b�v�̉���
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // �}�b�v�̏c��

#define EASY_MODE		 (0)							// EASY���[�h
#define HARD_MODE		 (1)						    // HARD���[�h

#define FLOAR_POWER      (2)							// ����

// �{�[���̏��
enum BallState {
	WOOD,		// �O	��
	METAL,		// �P	����
	STATE_END	// �Q	�I���
};

// �ǁi�w�i�j�̏��
enum WallState {
	// �����蔻��Ȃ�
	BACKGROUND,	// �O�@�w�i�i�����Ȃ��j
	WATER,		// �P�@���i�؃{�[���͕����āA�����{�[���͒��ށj

	// �����蔻�肠��
	// �i�R�ȍ~�́A�d�l�Ƃ��Ă�1�X�e�[�W��1�j
	BRICK,		// �Q�@�����K�i�����Ȃ��ǁj
	ROCK,		// �R�@��(�X�C�b�`��������邱�Ƃɂ���ď����� -> BACKGROUND�ɂȂ�)
	SWITCH_OFF,	// �S�@�I�t�̃X�C�b�`
	SWITCH_ON,	// �T�@�I���̃X�C�b�`
	GOAL		// �U�@�S�[��
};


// �Q�[�����Ɉ����ω�����}�b�v�̃f�[�^
int MapData[MAP_HEIGHT][HARD_MAP_WIDTH];

// EASY�}�b�v�̃f�[�^
int OriginEasyMapData[MAP_HEIGHT][EASY_MAP_WIDTH] =
{
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 ,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2 } ,
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 3, 0, 2 } ,
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 3, 0, 2 } ,
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 2, 1, 1, 2, 2, 2, 2 ,  2, 2, 2, 2, 2, 0, 0, 2, 0, 2 } ,

	{ 2, 0, 0, 2, 1, 1, 2, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 2, 1, 2, 2, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 2, 1, 1, 2, 0, 0, 2 ,  2, 2, 2, 2, 2, 2, 2, 2, 0, 2 } ,
	{ 2, 0, 0, 2, 2, 1, 2, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 2, 1, 1, 2, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,

	{ 2, 0, 0, 2, 2, 4, 2, 2, 2, 2 ,  2, 2, 2, 2, 2, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 0, 2 } ,
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 2, 6, 2 } ,
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 ,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2 } ,
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 ,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2 } ,
};

// HARD�}�b�v�̃f�[�^
int OriginHardMapData[MAP_HEIGHT][HARD_MAP_WIDTH] =
{
	{ 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 } ,
	{ 2,0,0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0,2,0 , 0,0,0,0,0,0,2,0,0,0 , 0,0,3,0,0,0,0,0,0,2 } ,
	{ 2,2,2,2,2,2,2,2,2,2 , 0,0,2,2,0,0,2,2,2,0 , 0,0,0,0,0,0,2,0,0,0 , 0,0,3,0,0,0,0,0,0,2 } ,
	{ 2,0,0,0,2,0,0,0,0,0 , 0,0,2,0,0,0,0,0,2,0 , 2,2,2,0,0,2,2,0,2,2 , 2,2,2,2,0,0,2,2,2,2 } ,
	{ 2,0,0,0,2,2,2,2,0,0 , 2,2,2,0,2,2,2,0,2,0 , 2,0,0,0,0,0,2,0,0,0 , 2,2,1,2,0,0,0,0,0,2 } ,

	{ 2,0,2,4,2,0,0,0,2,0 , 0,0,0,0,2,0,0,0,2,0 , 2,0,2,2,0,0,2,0,0,0 , 2,1,1,2,2,2,2,2,0,2 } ,
	{ 2,0,0,2,2,0,2,2,2,0 , 0,0,0,2,2,0,2,2,2,0 , 2,0,0,2,0,0,2,2,2,0 , 2,1,2,2,0,0,0,0,0,2 },
	{ 2,2,0,2,2,0,0,2,0,0 , 2,0,0,0,2,1,1,1,2,1 , 2,2,1,2,2,1,1,1,1,1 , 2,1,1,2,0,0,2,0,0,2 },
	{ 2,0,0,2,0,2,0,0,0,2 , 2,2,0,0,2,2,2,1,2,1 , 2,1,1,1,2,1,1,1,1,1 , 2,1,1,2,0,2,2,2,2,2 },
	{ 2,0,0,2,0,2,2,2,2,2 , 0,0,0,0,2,1,1,1,2,1 , 2,1,1,1,1,1,2,2,2,2 , 2,2,1,2,0,0,0,0,0,2 },

	{ 2,1,2,2,0,0,0,0,0,0 , 0,0,2,0,0,2,2,2,2,1 , 2,2,2,1,1,1,2,1,1,1 , 2,1,1,2,2,2,2,2,0,2 },
	{ 2,1,1,2,2,2,2,2,1,1 , 2,1,1,1,1,1,1,1,2,1 , 1,1,2,2,1,1,2,1,2,1 , 1,1,1,1,1,1,1,2,0,2 },
	{ 2,1,1,1,2,1,1,1,2,1 , 1,2,2,2,2,2,1,1,2,2 , 2,1,1,2,2,2,2,2,2,2 , 2,2,2,2,2,2,1,2,0,2 },
	{ 2,2,1,1,1,1,2,1,1,1 , 1,1,1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,2,6,2 } ,
	{ 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 , 2,2,2,2,2,2,2,2,2,2 } ,
};

// Str ������N���X�̒�`
class Str {
public:
	int x;				// x���W
	int y;				// y���W
	const TCHAR *Str;	// ������
	int StrLen;			// ������̒���
	int StrWidth;		// ������̉���
	int Color = GetColor(255, 255, 255);	// �����̐F�i���j

	// �N���X�̕ϐ���ݒ肷��
	void set(int y, const CHAR*str);

	// �������`�悷��
	void Draw();
};

// �N���X�̕ϐ���ݒ肷��
void Str::set(int y, const CHAR*str) {

	this->Str = str;
	this->StrLen = strlen(Str);
	this->StrWidth = GetDrawStringWidth(Str, StrLen);

	// �����񂪐^�񒆂ɗ���悤��x���W���v�Z����
	this->x = (SCREEN_WIDTH - this->StrWidth) / 2;
	this->y = y;
}

// �������`�悷��
void Str::Draw() {
	DrawString(this->x, this->y, this->Str, this->Color);
}


int BallX, BallY;		// �{�[���̍��W
int ScrollX;			// ��ʂ̃X�N���[����
int ScrollXMax;			// ��ʂ̃X�N���[���ʂ̍ő�l

int Speed;              // �{�[���̈ړ����x
int BallDown;			// �{�[���̗������x
int G;				 	// ���������x
int JumpPower;			// �W�����v��
bool JumpFlag;			// �W�����v�ł��邩�̃t���O�i�ڒn��TRUE�j

int Input, EdgeInput;	// ���͏��
int LastKey;			// �O��̃L�[���͒l
bool GoalFlag;			// �S�[���t���O

BallState StateB;		// �{�[���̏��

// �摜�t�@�C�������[�h����ϐ�
int GraphBallWood, GraphBallMetal; // �{�[��
int GraphBrick, GraphRock, GraphSwitchOff, GraphSwitchOn, GraphGoal; // ��
int GraphDark; // �È�


//************ �v���g�^�C�v�錾 ************//
//*** �X�^�[�g��ʗp ****//

// �X�^�[�g��ʂ̏������i�w�i�ƃ��j���[��\������j�֐�
void InitStartScreen(Str Title, Str Easy, Str Hard, Str HowToPlay, Str GameQuit);
// �X�^�[�g��ʂ̕`��֐�
void DrawStart(int* SelectMenuNum);

//*** �Q�[����ʗp ****//

// �Q�[����ʂ̃��C���֐�
void BallGameMain(int *SelectMenuNum);
// �}�b�v�ƃ{�[���̕`��֐�
void DrawGraph(int MapWidth);
// �����蔻����l�������{�[���̃A�N�V����
void BallAct(int Key, int MapWidth);
// �ړ���̓����蔻��̃`�F�b�N�����Ĉړ��ʂ��C������
int FixMove(int x, int y, int* MoveX, int* MoveY);
// �X�N���[����̓����蔻��̃`�F�b�N�����Ĉړ��ʂ��C������
void FixScroll(int x, int y, int* ScrollSubX);
// �}�b�v�`�b�v�̒l���擾����֐�
int GetChipValue(int X, int Y);
// �X�C�b�`�������ꂽ���Ɋ�������֐�
void DeleteRock(int MapWidth);
// �{�[���̏�ԕω��֐�
void ChangeBallState(void);
// �S�[���\������
void DrawGoal(void);

// �v�����l�������֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// �^�C�g���� BallAction �ɕύX
	SetMainWindowText("BallAction");

	// �A�C�R����ݒ肷��
	SetWindowIconID(111);

	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	// �`����ʂ𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �v���C���[���I�𒆂̃��j���[
	// 0->EASY�A 1->HARD �A�@2->�V�ѕ��A 3->��߂�
	int SelectMenuNum = 0;

	// Escape�L�[��������邩
	// "��߂�"���I�������܂Ń��[�v�𑱂���
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && SelectMenuNum != 3) {

		// �X�^�[�g��ʂ̃��j���[�t���[���̈ʒu��������
		SelectMenuNum = 0;

		// �X�^�[�g��ʂ�`�悷��
		DrawStart(&SelectMenuNum);

		// �S�[���t���O��������
		GoalFlag = FALSE;

		// �Q�[���X�^�[�g�Ȃ�Q�[�����J�n
		if (SelectMenuNum == 0 || SelectMenuNum == 1) {
			BallGameMain(&SelectMenuNum);		// �Q�[����ʂ̃��C���֐�
		}

		// �S�[�����Ă�����S�[����\������
		if (GoalFlag) DrawGoal();

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}


// �X�^�[�g��ʂ̏������i�w�i�ƃ��j���[��\������j�֐�
void InitStartScreen(Str Title, Str Easy, Str Hard, Str HowToPlay, Str GameQuit) {

	// �w�i�\��
	LoadGraphScreen(0, 0, "WallPaper.png", TRUE);

	// �^�C�g����`��
	SetFontSize(64);
	Title.Draw();

	// ���j���[��`��
	SetFontSize(32);
	Easy.Draw();
	Hard.Draw();
	HowToPlay.Draw();
	GameQuit.Draw();

}

// �X�^�[�g��ʂ̕`��֐�
void DrawStart(int* SelectMenuNum) {

	// �X�^�[�g��ʂł́A�^�C�g���ƃ��j���[�̕�����̂��ꂼ���y���W
	int TitleY = SCREEN_HEIGHT / 5;
	int EasyY = 5 * SCREEN_HEIGHT / 12;
	int HardY = 13 * SCREEN_HEIGHT / 24;
	int HowToPlayY = 4 * SCREEN_HEIGHT / 6;
	int GameQuitY = 19 * SCREEN_HEIGHT / 24;

	// �X�^�[�g��ʂɕ\�����镶����
	Str Title, Easy, Hard, HowToPlay, GameQuit;

	// �N���X�̏�����
	SetFontSize(64);
	Title.set(TitleY, "BallAction!");
	SetFontSize(32);
	Easy.set(EasyY, "EASY");
	Hard.set(HardY, "HARD");
	HowToPlay.set(HowToPlayY, "�V�ѕ�");
	GameQuit.set(GameQuitY, "��߂�");;

	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// ��ʂ�������
		InitStartScreen(Title, Easy, Hard, HowToPlay, GameQuit);

		// �I��g�̕`��
		LoadGraphScreen(SCREEN_WIDTH / 4, 5 * SCREEN_HEIGHT / 12 + *SelectMenuNum * SCREEN_HEIGHT / 8 - 15, "SelectFrame.png", TRUE);

		// ����ʂ̓��e��\��ʂɉf��
		ScreenFlip();

		// ���͏�Ԃ��X�V
		{
			int i;

			// �p�b�h�P�ƃL�[�{�[�h������͂𓾂�
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// �G�b�W����������͂��Z�b�g
			EdgeInput = i & ~Input;

			// ���͏�Ԃ̕ۑ�
			Input = i;
		}


		// �O��̃��[�v�ŉ����ꂽ�{�^������{�^���łȂ��āA
		// ��{�^���������ꂽ���A�I��g�����ɂ�����
		if ((Input != LastKey) && (Input & PAD_INPUT_UP)) *SelectMenuNum = (*SelectMenuNum + 7) % 4;

		// �O��̃��[�v�ŉ����ꂽ�{�^�������{�^���łȂ��āA
		// ���{�^���������ꂽ���A�I��g������ɂ�����
		if ((Input != LastKey) && (Input & PAD_INPUT_DOWN)) *SelectMenuNum = (*SelectMenuNum + 1) % 4;

		// �G���^�[�L�[�������ꂽ���̏���
		if (CheckHitKey(KEY_INPUT_RETURN)) {

			// �u�V�ѕ��v���I�����ꂽ���A�V�ѕ���\������
			if (*SelectMenuNum == 2) {

				// �܂��A�w�i�\������
				LoadGraphScreen(0, 0, "WallPaper.png", TRUE);

				// ���ɁA�V�ѕ���\������
				LoadGraphScreen(140, 0, "HowToPlay.png", TRUE);

				// ����ʂ̓��e��\��ʂɉf��
				ScreenFlip();

				// �G���^�[�L�[����w�������܂ő҂�
				while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;

				// �L�[���͑҂�
				WaitKey();

				// �L�[���͑҂��ŉ����ꂽ�L�[��
				// �G���^�[�L�[�@���@��L�[�@���@���L�[�Ȃ�
				// �L�[����w�������܂ő҂�
				while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;
				while (CheckHitKey(KEY_INPUT_UP)) if (CheckHitKey(KEY_INPUT_UP) == 0)break;
				while (CheckHitKey(KEY_INPUT_DOWN)) if (CheckHitKey(KEY_INPUT_DOWN) == 0)break;

			}

			// �uEASY�v�uHARD�v�u��߂�v���I�����ꂽ��
			// ���[�v�𔲂���
			else {
				break;
			}

		}

		// �G�X�P�[�v�L�[�������ꂽ��
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			// "��߂�"��I���������Ƃɂ��Ăɂ��ă��[�v�𔲂���
			*SelectMenuNum = 3;
			break;
		}

		// ���̃��[�v�̎��A��r�Ɏg�����߂�
		// ���݂̓��̓L�[��ۑ����Ă���
		LastKey = Input;

	}

}

// �Q�[����ʂ̃��C���֐�
void BallGameMain(int *SelectMenuNum) {

	BallX = CHIP_SIZE;	// ��ʏ�ł̃{�[����x���W��������
	BallY = CHIP_SIZE;	// ��ʏ�ł̃{�[����y���W��������

	ScrollX = 0;		// ��ʂ̃X�N���[���ʂ�������

	BallDown = 0;		// �v���C���[�̗������x��������

	// �{�[���Ƃ��̑��������Z�Ɏg���ϐ��̏�����Ԃ�
	// �؃{�[���ɐݒ�
	StateB = WOOD;		// �{�[���̏��
	G = 1;				// ���������x
	Speed = 10;			// �ړ��X�s�[�h
	JumpPower = 16;		// �W�����v��


	// �}�b�v�̉�����ۑ�
	int MapWidth;
	if (*SelectMenuNum == EASY_MODE) {
		MapWidth = EASY_MAP_WIDTH;
	}
	else if (*SelectMenuNum == HARD_MODE) {
		MapWidth = HARD_MAP_WIDTH;
	}

	// �X�N���[���ʂ̍ő�l�i�}�b�v�̉E�[-��ʂ̕��j
	// ������g���ĉ�ʂ��X�N���[���������Ȃ��悤�ɂ���
	ScrollXMax = MapWidth * CHIP_SIZE - SCREEN_WIDTH;

	// �}�b�v��������
	for (int height = 0; height < MAP_HEIGHT; height++) {
		for (int width = 0; width < MapWidth; width++) {
			// EASY���[�h��������EASY�p�̃}�b�v������
			if (*SelectMenuNum == EASY_MODE) MapData[height][width] = OriginEasyMapData[height][width];
			// HARD���[�h��������HARD�p�̃}�b�v������
			if (*SelectMenuNum == HARD_MODE) MapData[height][width] = OriginHardMapData[height][width];
		}
	}

	// �摜�̔��F�𓧉߂�����ݒ�ɂ��Ă���
	SetTransColor(255, 255, 255);

	// �摜�t�@�C�������[�h����
	GraphBallWood = LoadGraph("WoodBall.png");
	GraphBallMetal = LoadGraph("MetalBall.png");
	GraphBrick = LoadGraph("Brick.png");
	GraphRock = LoadGraph("Rock.png");
	GraphSwitchOff = LoadGraph("SwitchOff.png");
	GraphSwitchOn = LoadGraph("SwitchOn.png");
	GraphGoal = LoadGraph("Goal.png");
	GraphDark = LoadGraph("Dark.png");

	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// ��ʂ�������
		ClearDrawScreen();

		// ���͏�Ԃ��X�V
		{
			int i;

			// �p�b�h�P�ƃL�[�{�[�h������͂𓾂�
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// �G�b�W����������͂��Z�b�g
			EdgeInput = i & ~Input;

			// ���͏�Ԃ̕ۑ�
			Input = i;
		}

		// �����蔻����l�������A�N�V����
		// (�L�[���͂ɏ]���ă{�[�����ړ���������A��ʂ��X�N���[��������)
		BallAct(Input, MapWidth);

		// �}�b�v�ƃ{�[����`��
		DrawGraph(MapWidth);

		// ����ʂ̓��e��\��ʂɉf��
		ScreenFlip();

		// �E�G�C�g
		WaitTimer(30);

		// �S�[�����Ă����烋�[�v�𔲂���
		if (GoalFlag)break;
	}
}

// �}�b�v�ƃ{�[���̕`��֐�
void DrawGraph(int MapWidth) {

	// �}�b�v�̕`��
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MapWidth; x++) {

			// �`�悷��`�b�v���W
			int DrawX = x * CHIP_SIZE - ScrollX;
			int DrawY = y * CHIP_SIZE;

			// �`�悷��`�b�v�̏�Ԃ��擾
			int StateW = MapData[y][x];

			// �}�b�v�f�[�^��`�悷��
			switch (StateW) {

				// �w�i�i�^�����j
				case BACKGROUND:
					DrawBox(DrawX, DrawY, DrawX + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE, GetColor(0, 0, 0), TRUE);
					break;
				// �w�i�i���j
				case WATER:
					DrawBox(DrawX, DrawY, DrawX + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE, GetColor(0, 0, 255), TRUE);
					break;

				// �����K
				case BRICK:
					DrawGraph(DrawX, DrawY, GraphBrick, FALSE);
					break;
				// ��
				case ROCK:
					DrawGraph(DrawX, DrawY, GraphRock, FALSE);
					break;
				// �������O�̃X�C�b�`
				case SWITCH_OFF:
					DrawGraph(DrawX, DrawY, GraphSwitchOff, FALSE);
					break;
				// �����ꂽ��̃X�C�b�`
				case SWITCH_ON:
					DrawGraph(DrawX, DrawY, GraphSwitchOn, FALSE);
					break;
				// �S�[��
				case GOAL:
					DrawGraph(DrawX, DrawY, GraphGoal, FALSE);
					break;
			}
		}
	}


	// �{�[���̕`��
	{
		// �{�[���̏�Ԃɂ���ēǂݍ��މ摜��ς���
		switch (StateB) {
			// �؃{�[��
		case WOOD:
			DrawGraph(BallX, BallY, GraphBallWood, TRUE);
			break;
			// �����{�[��
		case METAL:
			DrawGraph(BallX, BallY, GraphBallMetal, TRUE);
			break;
		}
	}

	// �{�[���̈ʒu�����邭�Ȃ�悤�ɍ��W���C������
	// �Èŉ摜��`��
	int DarkX = BallX - SCREEN_WIDTH + CHIP_SIZE / 2;
	int DarkY = BallY - SCREEN_HEIGHT + CHIP_SIZE / 2;
	DrawGraph(DarkX, DarkY, GraphDark, TRUE);
	
}

// �����蔻����l�������{�[���̃A�N�V����
void BallAct(int Key, int MapWidth) {

	// �{�[���̈ړ���
	int MoveX = 0;
	int MoveY = 0;

	// �C���O�̈ړ���
	int ScrollSubX = 0;

	// z�{�^���������ꂽ��{�[���̏�ԕω�������
	if ((Key != LastKey) && (Key & PAD_INPUT_A)) ChangeBallState();

	// ���{�^���������ꂽ���̏���
	if (Key & PAD_INPUT_LEFT) {
		
		// �{�[���̈ʒu�ƃX�N���[���ʂ𓮂����Ă݂�
		MoveX -= Speed;
		ScrollSubX -= Speed;

		//** �����ɂ���ă{�[���̈ʒu�ƃX�N���[���ʂ��C������ **//

		if (ScrollX == 0 && (BallX < SCREEN_WIDTH / 2)) {
			// ��ʂ��}�b�v�̍��[�ɂ�����
			// �{�[������ʂ̍������ɂ���Ƃ�
			// �{�[���͎��R�ɓ�����i�X�N���[�����Ȃ��悤�ɂ���j
			ScrollSubX = 0;
		}
		else if (ScrollX == ScrollXMax && (BallX > SCREEN_WIDTH / 2)) {
			// ��ʂ��}�b�v�̉E�[�ɂ�����
			// �{�[������ʂ̉E�����ɂ���Ƃ�
			// �{�[���͎��R�ɓ�����i�X�N���[�����Ȃ��悤�ɂ���j
			ScrollSubX = 0;
		}
		else{
			// ��̏����ȊO�̓X�N���[�����Ȃ̂�
			// �{�[������ʂ̐^�񒆂ɌŒ肵�ē����Ȃ��悤�ɂ���
			BallX = SCREEN_WIDTH / 2;
			MoveX = 0;
		}
		
		// �X�N���[����(MoveX == 0)��
		// �{�[�������R�ɓ�����͈͂ɓ��낤�Ƃ��Ă���
		// �{�[�����ړ��ł���悤��MoveX�Ɉړ��ʂ𔽉f������
		if (MoveX == 0) {
			// ���{�^���������ꂽ���̏����Ȃ̂�
			// �������ł�if���̏����́A�h��ʂ��}�b�v�̍��[�ɂ����ă{�[������ʂ̍������ɓ��낤�Ƃ����Ƃ��h
			if ((ScrollX == 0) && (BallX - Speed < SCREEN_WIDTH / 2)) {
				MoveX -= Speed;
			}
		}
	}

	// �E�{�^���������ꂽ���̏���
	if (Key & PAD_INPUT_RIGHT) {

		// �{�[���̈ʒu�ƃX�N���[���ʂ𓮂����Ă݂�
		MoveX += Speed;
		ScrollSubX += Speed;

		//** �����ɂ���ă{�[���̈ʒu�ƃX�N���[���ʂ��C������ **//

		if (ScrollX == 0 && BallX < SCREEN_WIDTH / 2) {
			// ��ʂ��}�b�v�̍��[�ɂ�����
			// �{�[������ʂ̍������ɂ���Ƃ�
			// �{�[���͎��R�ɓ�����i�X�N���[�����Ȃ��悤�ɂ���j
			ScrollSubX = 0;
		}
		else if ((ScrollX == ScrollXMax) && (BallX > SCREEN_WIDTH / 2)) {
			// ��ʂ��}�b�v�̉E�[�ɂ�����
			// �{�[������ʂ̉E�����ɂ���Ƃ�
			// �{�[���͎��R�ɓ�����i�X�N���[�����Ȃ��悤�ɂ���j
			ScrollSubX = 0;
		}
		else {
			// ��̏����ȊO�̓X�N���[�����Ȃ̂�
			// �{�[������ʂ̐^�񒆂ɌŒ肵�ē����Ȃ��悤�ɂ���
			BallX = SCREEN_WIDTH / 2;
			MoveX = 0;
		}

		// �X�N���[����(MoveX == 0)��
		// �{�[�������R�ɓ�����͈͂ɓ��낤�Ƃ��Ă���
		// �{�[�����ړ��ł���悤��MoveX�Ɉړ��ʂ𔽉f������
		if (MoveX == 0) {
			// �E�{�^���������ꂽ���̏����Ȃ̂�
			// �������ł�if���̏����́A�h��ʂ��}�b�v�̉E�[�ɂ����ă{�[������ʂ̉E�����ɓ��낤�Ƃ����Ƃ��h
			if ((ScrollX == ScrollXMax) && (BallX + Speed > SCREEN_WIDTH / 2)) {
				MoveX += Speed;
			}
		}
	}

	// �W�����v����
	{
		// �{�[���̉����n�ʂɂ��Ă�
		// ��{�^���i�W�����v�{�^���j�������ꂽ���̏���
		if (JumpFlag == TRUE && (EdgeInput & PAD_INPUT_UP)) {

			// �{�[�����W�����v�����āA�W�����v�t���O�����낷�i�W�����v�ł��Ȃ��悤�ɂ���j
			BallDown -= JumpPower;
			JumpFlag = FALSE;

		}
	}

	// ���������x(��������)��������
	BallDown += G;

	// �������x���ړ��ʂɉ�����
	MoveY += BallDown;

	// �ړ���0�̃_�~�[
	int Dummy = 0;

	// �㉺�̈ړ��ɂ��ē����蔻��𒲂ׂďc�̈ړ��� MoveY ���C������
	{

		// �����̓_�����̃u���b�N�ɓ������Ă������A�������x���O�ɂ���
		if (FixMove(BallX, BallY + BALL_SIZE, &Dummy, &MoveY) == 1) BallDown = 0;

		// �E���̓_�����̃u���b�N�ɓ������Ă������A�������x���O�ɂ���
		if (FixMove(BallX + BALL_SIZE, BallY + BALL_SIZE, &Dummy, &MoveY) == 1) BallDown = 0;

		// ����̓_����̃u���b�N�ɓ������Ă������A�������x���O�ɂ���
		if (FixMove(BallX, BallY, &Dummy, &MoveY) == 2) BallDown = 0;

		// �E��̓_����̃u���b�N�ɓ������Ă������A�������x���O�ɂ���
		if (FixMove(BallX + BALL_SIZE, BallY, &Dummy, &MoveY) == 2) BallDown = 0;

		// �㉺�̈ړ������Z
		BallY += MoveY;

	}

	// ���E�̈ړ��ɂ��ē����蔻��𒲂ׂĉ��̈ړ��� MoveX �� ScrollX ���C������
	{
		
		// �����̓_�����̃u���b�N�ɓ����鎞�A�ړ��ʂ��C������
		FixMove(BallX, BallY + BALL_SIZE, &MoveX, &Dummy);

		// �E���̓_���E�̃u���b�N�ɓ����鎞�A�ړ��ʂ��C������
		FixMove(BallX + BALL_SIZE, BallY + BALL_SIZE, &MoveX, &Dummy);

		// ����̓_�����̃u���b�N�ɓ����鎞�A�ړ��ʂ��C������
		FixMove(BallX, BallY, &MoveX, &Dummy);

		// �E��̓_���E�̃u���b�N�ɓ����鎞�A�ړ��ʂ��C������
		FixMove(BallX + BALL_SIZE, BallY, &MoveX, &Dummy);

		// ���E�̈ړ������Z
		BallX += MoveX;

		// �X�N���[�����̓����蔻����`�F�b�N
		if (ScrollSubX != 0) {
			// �����̓_�����̃u���b�N�ɓ����鎞�A�X�N���[���ʂ��C������
			FixScroll(BallX, BallY + BALL_SIZE, &ScrollSubX);

			// �E���̓_���E�̃u���b�N�ɓ����鎞�A�X�N���[���ʂ��C������
			FixScroll(BallX + BALL_SIZE, BallY + BALL_SIZE, &ScrollSubX);

			// ����̓_�����̃u���b�N�ɓ����鎞�A�X�N���[���ʂ��C������
			FixScroll(BallX, BallY, &ScrollSubX);

			// �E��̓_���E�̃u���b�N�ɓ����鎞�A�X�N���[���ʂ��C������
			FixScroll(BallX + BALL_SIZE, BallY, &ScrollSubX);

			// ���E�̃X�N���[���ʂ����Z
			ScrollX += ScrollSubX;
		}

	}

	// �ڒn����
	{

		// �{�[���̍����ƉE���̓_�̉���
		// �����蔻��̂���u���b�N�����ׂ�
		if (GetChipValue(BallX, BallY + BALL_SIZE + 1) >= 2 ||
			GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) >= 2) {

			// ���ꂪ��������W�����v�\�ɂ���
			JumpFlag = TRUE;

			// ���ꂪoff�X�C�b�`��������on�ɂ��āA�������
			// �����̃`�F�b�N
			if (GetChipValue(BallX, BallY + BALL_SIZE + 1) == SWITCH_OFF) {

				MapData[(int)(BallY + BALL_SIZE + 1) / CHIP_SIZE][(int)(BallX + ScrollX) / CHIP_SIZE] = SWITCH_ON;
				DeleteRock(MapWidth);

			}
			// �E���̃`�F�b�N
			if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == SWITCH_OFF) {

				MapData[(int)(BallY + BALL_SIZE + 1) / CHIP_SIZE][(int)(BallX + CHIP_SIZE + ScrollX) / CHIP_SIZE] = SWITCH_ON;
				DeleteRock(MapWidth);

			}


		}
		else {

			// ���ꂪ����������W�����v�s�ɂ���
			JumpFlag = FALSE;

		}
	}

	// ��������
	{
		// �{�[���̍����ƉE���̓_�̉���
		// ���ɂ����Ă��邩���ׂ�
		if (GetChipValue(BallX, BallY + BALL_SIZE) == WATER ||
			GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE) == WATER) {

			// ���͂�������
			BallDown -= FLOAR_POWER;

		}
	}

	// �S�[������
	{
		// �S�[���ɓ���������S�[���t���O�����Ă�

		// ����̓_�ɂ��ă`�F�b�N
		if (GetChipValue(BallX, BallY - 1) == GOAL) GoalFlag = TRUE;
		if (GetChipValue(BallX - 1, BallY) == GOAL) GoalFlag = TRUE;
		// �E��̓_�ɂ��ă`�F�b�N
		if (GetChipValue(BallX + BALL_SIZE, BallY - 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE + 1, BallY) == GOAL) GoalFlag = TRUE;
		// �����̓_�ɂ��ă`�F�b�N
		if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE - 1, BallY + BALL_SIZE) == GOAL) GoalFlag = TRUE;
		// �E���̓_�ɂ��ă`�F�b�N
		if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE + 1, BallY + BALL_SIZE) == GOAL) GoalFlag = TRUE;

	}

	// �����ꂽ�L�[��ۑ�����
	LastKey = Key;

}

// �ړ���̓����蔻��̃`�F�b�N�����Ĉړ��ʂ��C������
// ��ʏ�̃{�[���̎l����4�_�̂����ǂꂩ�P�̍��W(x, y) �� �ړ��� MoveX,MoveY�i�ǂ��炩���O�łȂ��Ƃ����Ɠ����Ȃ��j
int FixMove(int x, int y, int* MoveX, int* MoveY) {

	// �ړ���̂���_�ix,y�j�̍��W�iafterX,afterY�j
	int afX = x + *MoveX;
	int afY = y + *MoveY;

	// �����蔻��̂���u���b�N(�l��2�ȏ�)�ɓ������Ă��邩�`�F�b�N
	if (GetChipValue(afX, afY) >= 2) {

		// �u���b�N��4�̍��W�l
		// BlockLeftX, BlockRightX
		// BlockTopY, BlockBottomY
		int blx, brx, bty, bby;

		// �u���b�N�̏㉺���E�̍��W
		blx = int(afX / CHIP_SIZE) * CHIP_SIZE;        // ���ӂ� X ���W
		brx = int(afX / CHIP_SIZE + 1) * CHIP_SIZE;    // �E�ӂ� X ���W

		bty = int(afY / CHIP_SIZE) * CHIP_SIZE;        // ��ӂ� Y ���W
		bby = int(afY / CHIP_SIZE + 1) * CHIP_SIZE;    // ���ӂ� Y ���W

		// �u���b�N�Ƃ̋���
		int dx, dy;


		// �������Ă�����ǂ��痣���������s���i�ړ��ʂ��C������j

		// ���Ɉړ����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̉����ɂ���u���b�N�̏�ӂƓ��������j
		if (*MoveY > 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̏�Ӂj-�i�{�[���̉����̓_�j
			dy = bty - y;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��-1���ď�Ɏ����グ��j
			*MoveY = dy - 1;

			return 1;
		}

		// ��Ɉړ����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̏㑤�ɂ���u���b�N�̉��ӂƓ��������j
		if (*MoveY < 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̉��Ӂj-�i�{�[���̏㑤�̓_�j
			// �㑤�ւ̈ړ��Ȃ̂ŕ��̒l�ƂȂ�悤�Ɍv�Z����
			dy = bby - y;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��+1���ĉ��ɉ�����j
			*MoveY = dy + 1;

			return 2;
		}

		// �E�Ɉړ����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̉E���ɂ���u���b�N�̍��ӂƓ��������j
		if (*MoveX > 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̍��Ӂj-�i�{�[���̉E���̓_�j
			dx = blx - x;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��-1���ĉE�Ɉړ�������j
			*MoveX = dx - 1;

			return 3;
		}

		// ���Ɉړ����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̍����ɂ���u���b�N�̉E�ӂƓ��������j
		if (*MoveX < 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̉E�Ӂj-�i�{�[���̍����̓_�j
			// �����ւ̈ړ��Ȃ̂ŕ��̒l�ƂȂ�悤�Ɍv�Z����
			dx = brx - x;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��+1���č��Ɉړ�������j
			*MoveX = dx + 1;

			return 4;
		}
		return 2;
	}

	// �ǂ��ɂ�������Ȃ������ƕԂ�
	return 0;
}

// �X�N���[����̓����蔻��̃`�F�b�N�����Ĉړ��ʂ��C������
// ��ʏ�̍��W(x, y) �Ɓ@�X�N���[���� ScrollX
void FixScroll(int x, int y, int* ScrollSubX) {

	// �X�N���[�����Ă݂�
	// �����蔻��̂���u���b�N(�l��2�ȏ�)�ɓ������Ă��邩�`�F�b�N
	if (GetChipValue(x + *ScrollSubX, y) >= 2) {

		// �u���b�N�̍����ƉE����x���W
		// BlockLeftX, BlockRightX
		int blx, brx;

		// �u���b�N�̍��E�̍��W
		blx = int((ScrollX + x + *ScrollSubX) / CHIP_SIZE) * CHIP_SIZE - ScrollX;        // ���ӂ� X ���W
		brx = int((ScrollX + x + *ScrollSubX) / CHIP_SIZE + 1) * CHIP_SIZE - ScrollX;    // �E�ӂ� X ���W

		// �u���b�N�Ƃ̋���
		int dx;


		// �������Ă�����ǂ��痣���������s���i�ړ��ʂ��C������j

		// �E�ɃX�N���[�����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̉E���ɂ���u���b�N�̍��ӂƓ��������j
		if (*ScrollSubX > 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̍��Ӂj-�i�{�[���̉E���̓_�j
			dx = blx - x;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��-1����j
			*ScrollSubX = dx - 1;

		}

		// ���ɃX�N���[�����ău���b�N�ɓ��������ꍇ
		// �i�{�[���̍����ɂ���u���b�N�̉E�ӂƓ��������j
		if (*ScrollSubX < 0) {

			// �u���b�N�Ƃ̋������v�Z
			// �i�u���b�N�̉E�Ӂj-�i�{�[���̍����̓_�j
			// �����ւ̈ړ��Ȃ̂ŕ��̒l�ƂȂ�悤�Ɍv�Z����
			dx = brx - x;

			// �ړ��ʂ��C������i�߂荞�܂Ȃ��悤��+1����j
			*ScrollSubX = dx + 1;

		}
	}
	// �X�N���[�����Ă݂�
	// ��ʂ��}�b�v�̍��[�ɗ��Ă��Ȃ����`�F�b�N
	else if (ScrollX + *ScrollSubX < 0) {

		// ���[�ɗ��Ă�����A����ȏ�X�N���[�����Ȃ��悤�ɂ���
		*ScrollSubX = 0;
		ScrollX = 0;

	}
	// �X�N���[�����Ă݂�
	// ��ʂ��}�b�v�̉E�[�ɗ��Ă��Ȃ����`�F�b�N
	else if (ScrollX + *ScrollSubX > ScrollXMax) {

		// �E�[�ɗ��Ă�����A����ȏ�X�N���[�����Ȃ��悤�ɂ���
		*ScrollSubX = 0;
		ScrollX = ScrollXMax;

	}

}

// ���W�l����}�b�v�`�b�v�̒l���擾����֐�
int GetChipValue(int X, int Y) {
	
	// �擾�������}�b�v�`�b�v�̒l�̈ʒu�iMapData�̓Y�����j
	int x, y;

	// �����l�֕ϊ�
	x = (int)(X + ScrollX) / CHIP_SIZE;
	y = (int)Y / CHIP_SIZE;

	// �w��̍��W�ɊY������}�b�v�̏���Ԃ�
	return MapData[y][x];
}

// �X�C�b�`�������ꂽ���Ɋ�������֐�
void DeleteRock(int MapWidth) {
	int x, y;

	// �}�b�v�ƃ{�[����`��
	DrawGraph(MapWidth);

	// ����ʂ̓��e��\��ʂɉf��
	ScreenFlip();

	// ��̃u���b�N��T��
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MapWidth; x++) {
			if (MapData[y][x] == ROCK) {

				// ��������i�w�i�ɂ���j
				MapData[y][x] = BACKGROUND;

				// �}�b�v�ƃ{�[����`��
				DrawGraph(MapWidth);

				// ����ʂ̓��e��\��ʂɉf��
				ScreenFlip();

			}
		}
	}
}

// �{�[���̏�ԕω��֐�
void ChangeBallState(void) {

	// ��Ԃ�1�i�߂�
	int i = static_cast<int>(StateB);
	int numState = static_cast<int>(BallState::STATE_END);
	StateB = static_cast<BallState>((i + 1) % numState);

	// ��Ԃɂ���āA���������x�A���������x�A�W�����v�͂�ς���
	//						�؁@����
	// ���������x G			 1	  3		(���͂�2�Ȃ̂ŁA�؂͕����悤�ɁA�����͒��ނ悤�ɂ��Ă���)
	// �ړ����x	�@Speed�@	10	  3
	// �W�����v�� JumpPower	10	  5
	G = (int)((static_cast<int>(StateB) + 1) * 1.5);
	Speed = (int)10 / G ;
	JumpPower = (int)16 / G;
}

// �S�[����\������
void DrawGoal(void) {

	// �\�����镶����̏�����
	Str Goal, AnyTapKey;

	// �S�[������ʒ����ɑ傫���`�悷��
	SetFontSize(128);
	Goal.set(SCREEN_HEIGHT / 4, "GOAL!!");
	Goal.Draw();

	// �L�[���͑҂��p�������`�悷��
	SetFontSize(32);
	AnyTapKey.set(2 * SCREEN_HEIGHT / 3, "- Any Tap Key -");
	AnyTapKey.Draw();

	// ����ʂ̓��e��\��ʂɉf��
	ScreenFlip();

	// �L�[���͑҂�
	WaitKey();

	// �����ꂽ�L�[��
	// �G���^�[�L�[�@���@��L�[�@���@���L�[�Ȃ�
	// �L�[����w�������܂ő҂�
	while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;
	while (CheckHitKey(KEY_INPUT_UP)) if (CheckHitKey(KEY_INPUT_UP) == 0)break;
	while (CheckHitKey(KEY_INPUT_DOWN)) if (CheckHitKey(KEY_INPUT_DOWN) == 0)break;
}
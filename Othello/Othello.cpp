//	
//	�Q�[���^�C�g���uOthello�v
//  ����ҁF���Α�P
//	������ԁF2021/3/12�`2021/3/18
//�@
// �����ݒ�ł́ACOM�ƃv���C����悤�ɍ���Ă��܂��B
// 228�s�ڂ̃R�����g�A�E�g���O���āA230�`328�s�ڂ��R�����g�A�E�g����ƃv���C���[��l�ŗV�ׂ܂��B
// 

#include "DxLib.h"

#define BOARD_WIDTH		8	// �{�[�h�̉��̃}�X�̐�
#define BOARD_HEIGHT	8	// �{�[�h�̏c�̃}�X�̐�
#define CELL_SIZE		52	// �}�X�̂P�ӂ̃s�N�Z����

#define STONE_START_NUM  4	// ������Ԃ̐΂̐�
#define STONE_END_NUM   64	// �u����΂̍ő吔

#define VERTICAL_OFFSET 100	// ��ʏ�ŔՖʂ����ɂ��炷�ׂ̃I�t�Z�b�g
#define STONE_OFFSET      2	// �}�X���ɏ������炵�Đ΂�`�悷��ׂ̃I�t�Z�b�g

#define SCREEN_WIDTH	CELL_SIZE*BOARD_WIDTH					// �Q�[����ʂ̉���
#define SCREEN_HEIGHT	CELL_SIZE*BOARD_HEIGHT+VERTICAL_OFFSET	// �Q�[����ʂ̏c��


// �Q�[���Ŏg���{�[�h
int BoardMain[BOARD_HEIGHT][BOARD_WIDTH];

// �΂�������ʒu
bool BoardCanPut[BOARD_HEIGHT][BOARD_WIDTH];

// �Ֆʂ̏������
int BoardInit[BOARD_HEIGHT][BOARD_WIDTH] = {
	{ -1, -1, -1, -1, -1, -1, -1, -1 } ,
	{ -1, -1, -1, -1, -1, -1, -1, -1 } ,
	{ -1, -1, -1, -1, -1, -1, -1, -1 } ,
	{ -1, -1, -1,  1,  0, -1, -1, -1 } ,
	{ -1, -1, -1,  0,  1, -1, -1, -1 } ,
	{ -1, -1, -1, -1, -1, -1, -1, -1 } ,
	{ -1, -1, -1, -1, -1, -1, -1, -1 } ,
	{ -1, -1, -1, -1, -1, -1, -1, -1 }
};

// ���Ղ̕]���l�\
int BoardValueFirst[BOARD_HEIGHT][BOARD_WIDTH] = {
	{  30, -12,  0, -1, -1,  0, -12,  30 } ,
	{ -12, -15, -3, -3, -3, -3, -15, -12 } ,
	{   0,  -3,  0, -1, -1,  0,  -3,   0 } ,
	{  -1,  -3, -1, -1, -1, -1,  -3,  -1 } ,
	{  -1,  -3, -1, -1, -1, -1,  -3,  -1 } ,
	{   0,  -3,  0, -1, -1,  0,  -3,   0 } ,
	{ -12, -15, -3, -3, -3, -3, -15, -12 } ,
	{  30, -12,  0, -1, -1,  0, -12,  30 }
};

// �㔼�̕]���֐�
int BoardValueLast[BOARD_HEIGHT][BOARD_WIDTH] = {
	{  45, -11,  4, -1, -1,  4, -11,  45 } ,
	{ -11, -16, -1, -3, -3,  2, -16, -11 } ,
	{   4,  -1,  2, -1, -1,  2,  -1,   4 } ,
	{  -1,  -3, -1,  0,  0, -1,  -3,  -1 } ,
	{  -1,  -3, -1,  0,  0, -1,  -3,  -1 } ,
	{   4,  -1,  2, -1, -1,  2,  -1,   4 } ,
	{ -11, -16, -1, -3, -3, -1, -16, -11 } ,
	{  45, -11,  4, -1, -1,  4, -11,  45 }
};

// ���W�ix,y�j�̃N���X
class Vec2 {
public:
	int X;		// x���W
	int Y;		// y���W
	void Set(int x, int y); // ���W�̒l��������֐�
	int GetX(); // X���W�l��Ԃ��֐�
	int GetY(); // Y���W�l��Ԃ��֐�
};

// ���W�̒l��������֐�
void Vec2::Set(int x, int y) {
	X = x;
	Y = y;
}

// ���W�̒l��������֐�
int Vec2::GetX() { return X; }
int Vec2::GetY() { return Y; }

// �}�X�̏��
enum StoneState{
	NONE = -1,	// �����u����Ă��Ȃ�
	BLACK = 0,	// ��
	WHITE = 1,	// ��
	STATE_MAX	// �ő�l
};

// �Q�[���̏��
enum GameState {
	BLACK_TURN,		// ���̃^�[��
	WHITE_TURN,		// ���̃^�[��
	PASS,			// �p�X
	BLACK_WIN,		// ���̏���
	WHITE_WIN,		// ���̉��l
	DRAW,			// ��������
	GAME_STATE_MAX	// �ő�l
};

// �Q�[���̏�Ԃ̕�����
TCHAR GameStates[6][21] = {
	"���Ȃ��i���j�̃^�[��",
	"COM�i���j�̃^�[��",
	"�p�X",
	"���Ȃ��i���j�̏���",
	"COM�i���j�̏���",
	"��������"
};

// �΂̌��@[0]->���΂̐� ,[1]->���΂̐�
int StonesNum[STATE_MAX] = { 2,2 };


int MouseX, MouseY;		// �}�E�X�̍��W
int PassCounter;		// �p�X�̉񐔁i2��ɂȂ�����Q�[���I���j

bool GameEndFlag;		// �Q�[�����I���t���O

// �摜��ǂݍ���ł���������
int GraphCell, GraphBlack, GraphWhite, GraphFrame, GraphPuttable;


// �v���g�^�C�v�錾

// �Q�[�����s���郁�C���֐�
void GameMain(void);				
// �Ֆʂ̏�����
void InitBoard(void);				
// �Ֆʂ�`�悷��
void DrawGame(int GameState);					
// �v���C���[���v���C����i���̃^�[���j
void PlayPlayer(int Turn, int GameState);		
// �u����ʒu��T���Ĕz��BoardCanPut�ɔ��f����
int SearchCanPutBlack(int Turn);				
// �Տ�́ix,y�j�ɐ΂��u���邩�̔���
bool IsPuttable(int x, int y, int Turn, int BoardSub[8][8]);		
// �΂��Ђ�����Ԃ�
void TurnOverStones(int x, int y, int Turn, int BoardSub[8][8]);	

// �u����ʒu��T���āA�q�̍��W��z��ɔ��f���A�u����ꏊ�̌���Ԃ��iCOM�p�j
int SearchCanPutWhite(int Turn, Vec2 Children[STONE_END_NUM], int BoardSub[8][8]);
// �A���t�@�x�[�^�@��p���čőP��Ő΂�u���i���̃^�[���j
Vec2 AlphaBeta(int Turn, int BoardSub[8][8]);	
// ���i�v���C���[�j�̃x�X�g�Ȏ��T������֐�
int MinValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth);
// ���iCOM�j�̃x�X�g�Ȏ��T������֐�
int MaxValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth);
// �]���l���v�Z����֐�
int CalcValue(int Turn, int BoardSub[8][8]);	
// �m��΂̐����v�Z���� 
int CalcFS(int Turn, int BoardSub[8][8]);		



// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);// �E�B���h�E���[�h�̐ݒ�
	
	ChangeWindowMode(TRUE);		// �E�C���h�E���[�h�ɕύX

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK); // �`����ʂ𗠉�ʂɂ���

	GameMain();					// �Q�[�����s���郁�C���֐�

	WaitKey();					// �L�[���͑҂�

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I�� 
}

// �Q�[�����s���郁�C���֐�
void GameMain(void) {

	// �Ֆʂ�������
	InitBoard();

	// �Տ�̐΂̐����������i��2��2��4�j
	int StoneNum = STONE_START_NUM;

	// �摜��ǂݍ���ł���
	GraphCell = LoadGraph("Cell.png");
	GraphBlack = LoadGraph("Black.png");
	GraphWhite = LoadGraph("White.png");
	GraphFrame = LoadGraph("Frame.png");
	GraphPuttable = LoadGraph("Puttable.png");

	// ��(�v���C���[)�̃^�[��
	int Turn = BLACK;					// �ǂ���̃^�[�����i0->��,1->���j
	int GameState = BLACK;				// �Q�[���̏��

	// �p�X�̉񐔂�0��ɂ���
	PassCounter = 0;

	// �Q�[�����I����Ă��Ȃ����Ƃɂ���
	GameEndFlag = FALSE;

	// ���[�v�i�΂̐���64�ȉ��̊ԁ@���@�Q�[�����I����Ă��Ȃ��� ���@�p�X�̉񐔂�2��ȉ��̊ԁj
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && 
		StonesNum[BLACK]+StonesNum[WHITE] < STONE_END_NUM && GameEndFlag == FALSE && PassCounter<2) {

		// �^�[���ƃQ�[���̏�Ԃ����̃^�[���ɂ���
		Turn = BLACK;	GameState = BLACK;
		
		// �v���C���[�����΂�u��
		PlayPlayer(Turn, GameState);

		// �����������烋�[�v�𔲂���
		if (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM && GameEndFlag == TRUE && PassCounter >= 2)break;

		// �^�[���ƃQ�[���̏�Ԃ𔒂̃^�[���ɂ���
		Turn = WHITE;	GameState = WHITE;


		// PlayPlayer(Turn, GameState);  //***************** �ȉ��̃X�R�[�v������������ɕς������l�ŗV�ׂ܂� **************//
		
		//�@�A���t�@�x�[�^�@����������COM�����΂�u��
		{
			// �Ֆʂ�`��(���̃^�[��)
			DrawGame(GameState);

			// �ҏW���Ă������̃{�[�h��p�ӂ���
			int BoardSub[8][8];
			memcpy(BoardSub, BoardMain, sizeof(BoardMain));

			// COM���u���΂̍��W
			Vec2 CoordWhite;

			// �u���Ȃ��ݒ�i-1,-1�j�ɂ��Ă���
			CoordWhite.Set(-1, -1);
			
			// �������Ղ͈�ԕ]���l�������Ƃ���ɒu��
			if ((StonesNum[BLACK] + StonesNum[WHITE]) < (STONE_END_NUM * 3 / 4)) {

				// �œK�ȃ}�X�̕]���l�i�T�������}�X�̕]���l�̕��������Ƃ���ɍX�V���Ă����j
				int BestValue = -INT_MAX;

				// �ł��Ƃ��ł�����}�X�̍��W
				Vec2 Children[STONE_END_NUM];

				// �ł��Ƃ��ł�����}�X�̍��W���@�z��Children[STONE_END_NUM]  �ɔ��f����
				// �ł��Ƃ��ł�����}�X�̌����v�Z����
				int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

				// �u����}�X���Ȃ�������A(-1,-1)������
				if (CanPutNum == 0) {
					CoordWhite.Set(-1, -1);
				}
				// �u����}�X����������A�ȉ��̏���������
				else {

					// �łĂ�}�X��T��
					for (int i = 0; i < CanPutNum; i++) {

						// �T�����̃}�X�̕]���l��BestValue�����傫��������
						if (BestValue < BoardValueFirst[Children[i].X][Children[i].Y]) {

							// �T�����̃}�X�̕]���l��BestValue�ɂ���
							BestValue = BoardValueFirst[Children[i].X][Children[i].Y];
							// �T�����̃}�X�̍��W��BestVec�ɂ���
							CoordWhite.Set(Children[i].X, Children[i].Y);
						}
					}
				}

				// �u����ꏊ����\��
				DrawGame(Turn);

			}
			else {
				// �����㔼�́A�A���t�@�x�[�^�@���������č��W��T������
				CoordWhite = AlphaBeta(Turn, BoardSub);
			}

			// �΂��u���鎞
			if ((CoordWhite.GetX() != -1) && (CoordWhite.GetY() != -1)) {

				// �p�X�J�E���^�[���O�ɂ���
				PassCounter = 0;
				
				// �΂�u��
				BoardMain[CoordWhite.GetY()][CoordWhite.GetX()] = Turn;
				StonesNum[Turn]++;

				// �Ђ�����Ԃ�
				TurnOverStones(CoordWhite.GetX() , CoordWhite.GetY(), Turn, BoardMain);
			}
			// �΂��u���Ȃ���
			else {

				// �Е��ǂ��炩�̐΂����ׂĂȂ��Ȃ������A�������͐΂̐����}�X�ڂ̐��ɂȂ������A�Q�[�����I������
				if ((StonesNum[BLACK] == 0) || (StonesNum[WHITE] == 0) || (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM)) {

					// �����錾
					GameState += 3;

					// �Q�[�����I������
					GameEndFlag = TRUE;

				}
				// �Q�[�����܂������Ȃ�p�X�\������
				else {
					// �p�X��\��
					GameState = PASS;
					DrawGame(GameState);

					// �p�X�J�E���^�[���{�P����
					PassCounter++;

				}
			}
		}
		
		// �\�����邽�߂ɏ����҂�
		WaitTimer(1000);
	}
	
	// �Q�[�����I��������A���s��ݒ肷��
	if (StonesNum[BLACK] > StonesNum[WHITE]) {
		// ���̉��l
		GameState = BLACK_WIN;
	}
	else if (StonesNum[BLACK] < StonesNum[WHITE]) {
		// ���̉��l
		GameState = WHITE_WIN;
	}
	else if (StonesNum[BLACK] == StonesNum[WHITE]) {
		// ��������
		GameState = DRAW;
	}

	// ���s��\��
	DrawGame(GameState);
}

// �{�[�h������������
void InitBoard(){
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {
			BoardMain[height][width] = BoardInit[height][width];
		}
	}
}

// �Ֆʂ�`�悷��
void DrawGame(int GameState) {
	
	// ��ʂ�������
	ClearDrawScreen();

	// �ǂ����̃^�[�����`�悷��
	SetFontSize(32);
	DrawString(10, 10, GameStates[GameState], GetColor(255,255,255));

	// ���΂̐��A���΂̐���\������
	DrawString(10, 40, "���̐��F", GetColor(255, 255, 255));
	DrawString(10, 70, "���̐��F", GetColor(255, 255, 255));

	// ���΂̐��Ɣ��΂̐���\������
	// TCHAR�^������ɕϊ����ĕ\������
	TCHAR	BlackNum[256], WhiteNum[256];
	::_itot_s(StonesNum[BLACK], BlackNum, 256 * sizeof(TCHAR), 10);
	::_itot_s(StonesNum[WHITE], WhiteNum, 256 * sizeof(TCHAR), 10);
	DrawString(145, 40, BlackNum, GetColor(255, 255, 255));
	DrawString(145, 70, WhiteNum, GetColor(255, 255, 255));

	// �}�E�X�̈ʒu���擾
	GetMousePoint(&MouseX, &MouseY);

	// �Ֆʂ�`�悷��
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			// �}�X��`��
			DrawGraph(width * CELL_SIZE , height * CELL_SIZE + VERTICAL_OFFSET, GraphCell, FALSE);

			// �}�E�X���Տ�ɂ�������
			if ((MouseX>=0) && (MouseX <= SCREEN_HEIGHT) && (MouseY>=VERTICAL_OFFSET) && (MouseY <= SCREEN_HEIGHT)) {
				// �}�E�X�̈ʒu�̃}�X�͘g����������
				if ((MouseX / CELL_SIZE == width) && ((MouseY - VERTICAL_OFFSET) / CELL_SIZE == height)) {
					DrawGraph(width * CELL_SIZE, height * CELL_SIZE + VERTICAL_OFFSET, GraphFrame, TRUE);
				}
			}

			// �΂��u����}�X�Ȃ�_�������ċ�������
			if ((BoardMain[height][width] == NONE) && (BoardCanPut[height][width] == TRUE)) {
				DrawGraph(width * CELL_SIZE, height * CELL_SIZE + VERTICAL_OFFSET, GraphPuttable, TRUE);
			}

			// ���F�������獕�΂�u��
			if (BoardMain[height][width] == BLACK) {
				DrawGraph(width*CELL_SIZE + STONE_OFFSET, height*CELL_SIZE + VERTICAL_OFFSET + STONE_OFFSET, GraphBlack, TRUE);
			}

			// ���F�������甒�΂�u��
			if (BoardMain[height][width] == WHITE) {
				DrawGraph(width*CELL_SIZE + STONE_OFFSET, height*CELL_SIZE + VERTICAL_OFFSET + STONE_OFFSET, GraphWhite, TRUE);
			}
		}
	}

	// ����ʂ̓��e��\��ʂɉf��
	ScreenFlip();

}

// �v���C���[���v���C����i���̃^�[���j
void PlayPlayer(int Turn,int GameState) {

	// ��ʏ�ł̃}�E�X�̈ʒu�ɂ���}�X�̓Y����[CellY][CellX]
	int CellX, CellY;

	// �u����ꏊ��T���āA���̌����擾����
	int CanPutNum = SearchCanPutBlack(Turn);

	// �u����ꏊ�����鎞�̏���
	if (CanPutNum > 0) {

		// �p�X�̉񐔂�������
		PassCounter = 0;

		// ���[�v
		while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

			// �}�E�X�̍��W���擾
			GetMousePoint(&MouseX, &MouseY);

			// �}�E�X�̍��W����}�X�ڂ̍��W�����߂�
			CellX = (int)MouseX / CELL_SIZE;
			CellY = (int)(MouseY - VERTICAL_OFFSET) / CELL_SIZE;

			// ���N���b�N���ꂽ���̏���
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

				// �}�E�X���Տ�ɂ�������
				if ((MouseX >= 0) && (MouseX <= SCREEN_HEIGHT) && (MouseY >= VERTICAL_OFFSET) && (MouseY <= SCREEN_HEIGHT)) {

					// ����������ꏊ�������� ����u��
					if (BoardCanPut[CellY][CellX]) {

						// �΂�u��
						BoardMain[CellY][CellX] = Turn;

						// Turn�F�̌���1���₷
						StonesNum[Turn]++;

						// �Ђ�����Ԃ�
						TurnOverStones(CellX, CellY,Turn,BoardMain);

						// ���[�v�𔲂���
						break;
					}
				}
			}
			// �Ֆʂ�`��
			DrawGame(GameState);
		}

		
	}

	// �u����ꏊ���Ȃ��������̏���
	else if (CanPutNum == 0) {

		// �Е��ǂ��炩�̐΂����ׂĂȂ��Ȃ������A�Q�[�����I������
		if ((StonesNum[BLACK] == 0) || (StonesNum[WHITE] == 0) || (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM)) {

			// �����錾
			GameState += 3;

			// �Q�[�����I������
			GameEndFlag = TRUE;

		}
		else {

			// ����ȊO�̓p�X�\������
			GameState = PASS;
			DrawGame(GameState);

			// �p�X�J�E���^�[���{�P����
			PassCounter++;

			// �\�����邽�߂ɏ����҂�
			WaitTimer(1000);
			
		}
	}

}

// �����u����ʒu��T���Ĕz��BoardCanPut�ɔ��f���āA�u����ꏊ�̌���Ԃ�
int SearchCanPutBlack(int Turn) {

	// �u�����
	int CanPutNum = 0;

	// �΂��u����ʒu��T������
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			if (IsPuttable(width, height, Turn, BoardMain)) {

				// �u����Ȃ�TRUE ������
				BoardCanPut[height][width] = TRUE;

				// �u�������+1
				CanPutNum++;

			}
			else {
				// �u���Ȃ��Ȃ�FALSE������
				BoardCanPut[height][width] = FALSE;
			}
		}
	}
	// �u�������Ԃ�
	return CanPutNum;

}

// �Տ�́ix,y�j�ɐ΂��u���邩�̔���
bool IsPuttable(int x, int y,int Turn, int BoardSub[8][8]) {

	// �΂�����������T������
	if (BoardSub[y][x] == NONE) {
		// �G�̐΂̐F
		int EnemyStoneColor = (Turn + 1) % STATE_MAX;

		/* �S�����ɑ΂��Ēu���Ɛ΂��Ђ�����Ԃ邩���m�F */
		for (int j = -1; j < 2; j++) {
			for (int i = -1; i < 2; i++) {

				// ��}�X�i��ł݂�
				int SearchX = x + i, SearchY = y + j;

				// �G�̐΂��u����Ă�����T���J�n
				if (BoardSub[SearchY][SearchX] == EnemyStoneColor) {
					while (1) {

						// 1�}�X�i�߂�
						SearchX += i;
						SearchY += j;

						// �T���ʒu���Ֆʂ���O�ɏo����T���I��
						if ((SearchX < 0) || (SearchX >= BOARD_WIDTH) || (SearchY < 0) || (SearchY >= BOARD_HEIGHT))break;

						// �΂��u����Ă��Ȃ��}�X�ɑ������Ă��T���I��
						if (BoardSub[SearchY][SearchX] == NONE)break;

						// �G�̐΂��u����Ă����玟�ɐi��
						if (BoardSub[SearchY][SearchX] == EnemyStoneColor)continue;

						// �����̐΂��u����Ă�����A(x,y)�ɂ͐΂��u����iTRUE��Ԃ��j
						if (BoardSub[SearchY][SearchX] == Turn) return TRUE;
					}
				}

			}
		}

		// �S���T�����I�������u���Ȃ�
		return FALSE;
	}

	// �΂���������u���Ȃ�
	else {
		return FALSE;
	}
}

// �΂��Ђ�����Ԃ�
void TurnOverStones(int x, int y,int Turn, int BoardSub[8][8]) {

	// �G�̐΂̐F
	int EnemyStoneColor = (Turn + 1) % STATE_MAX;

	/* �S�����ɑ΂��Ēu���Ɛ΂��Ђ�����Ԃ邩���m�F */
	for (int j = -1; j < 2; j++) {
		for (int i = -1; i < 2; i++) {

			// ��}�X�i��ł݂�
			int SearchX = x + i, SearchY = y + j;

			// �G�̐΂��u����Ă�����T���J�n
			if (BoardSub[SearchY][SearchX] == EnemyStoneColor) {
				while (1) {

					// 1�}�X�i�߂�
					SearchX += i;
					SearchY += j;

					// �T���ʒu���Ֆʂ���O�ɏo����T���I��
					if ((SearchX < 0) || (SearchX >= BOARD_WIDTH) || (SearchY < 0) || (SearchY >= BOARD_HEIGHT))break;

					// �΂��u����Ă��Ȃ��}�X�ɑ������Ă��T���I��
					if (BoardSub[SearchY][SearchX] == NONE)break;

					// �G�̐΂��u����Ă����玟�ɐi��
					if (BoardSub[SearchY][SearchX] == EnemyStoneColor)continue;

					// �����̐΂��u����Ă�����A(x,y)�܂Ŗ߂�悤�ɐ΂��Ђ�����Ԃ��Ă���
					if (BoardSub[SearchY][SearchX] == Turn) {

						// 1�}�X�߂�
						SearchX -= i;
						SearchY -= j;

						while (1) {

							// �G�̐΂��Ђ�����Ԃ�
							BoardSub[SearchY][SearchX] = Turn;

							// �����̐΂̌����P���₷
							StonesNum[Turn]++;

							// ����̐΂̌����P���炷
							StonesNum[EnemyStoneColor]--;

							// �P�}�X�߂��Ă݂Ď������u�����ʒu�ɖ߂�����
							// �Ђ�����Ԃ���Ƃ��I������
							SearchX -= i;
							SearchY -= j;
							if ((SearchX == x) && (SearchY == y))break;
						}

						break;
					}
				}
			}
		}
	}
}

// �����u����ʒu��T���āA�q�̍��W��z��ɔ��f���A�u����ꏊ�̌���Ԃ�
// COM�i�A���t�@�x�[�^�@�j�p
int SearchCanPutWhite(int Turn, Vec2 Children[STONE_END_NUM], int BoardSub[8][8]) {

	// �u�����
	int CanPutNum = 0;

	// �΂��u����ʒu��T������
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			// �u����Ȃ�
			if (IsPuttable(width, height, Turn, BoardSub)) {
				// TRUE ������
				BoardCanPut[height][width] = TRUE;
				// ���W(width,height) ������
				Children[CanPutNum].Set(width, height);
				// �u����ꏊ�̌���+1
				CanPutNum++;
			}
			else {
				// �u���Ȃ��Ȃ�FALSE������
				BoardCanPut[height][width] = FALSE;
			}
		}
	}

	// �u�������Ԃ�
	return CanPutNum;

}

// �A���t�@�x�[�^�@��p���čőP��Ő΂�u���i���̃^�[���j
Vec2 AlphaBeta(int Turn,int BoardSub[8][8]) {

	// ���l�ƃ��l��������
	int Alpha = -INT_MAX;
	int Beta = INT_MAX;

	// ��ǂ݂̐[��
	int Depth = 3;

	// �G�̃^�[��
	int EnemyTurn = (Turn + 1) % STATE_MAX;

	// �œK�ȃ}�X�̍��W
	Vec2 BestVec;

	// �ł��Ƃ��ł���}�X�̍��W
	Vec2 Children[STONE_END_NUM];

	// �ł��Ƃ��ł���}�X�̍��W���@�z��Children[STONE_END_NUM]  �ɔ��f����
	// �ł��Ƃ��ł�������v�Z����
	int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

	// �u����ꏊ����\��
	DrawGame(Turn);

	// �����łĂ�}�X���Ȃ��i�p�X�j�̎�
	// ���̎��̕]���l��Ԃ�
	if (CanPutNum == 0) {
		// ���������p�X��\������
		BestVec.Set(-1, -1);
	}

	else {
		// �łĂ�}�X��T��
		for (int i = 0; i < CanPutNum; i++) {

			// ���̔Ֆʂ�p��(���΂�u���O)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// �T�����̎q�̕]���l���v�Z����
			int ChildValue = MinValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// ���l�����T�����̎q�̕]���l�̕����傫��������X�V
			if (Alpha < ChildValue) {
				Alpha = ChildValue;
				BestVec = Children[i];
			}

			
			// �]���l��\�����Ă݂�
			/*SetFontSize(16);
			TCHAR	pszText[256];
			::_itot_s(ChildValue, pszText, 256 * sizeof(TCHAR), 10);
			DrawString(Children[i].GetX() * CELL_SIZE, Children[i].GetY() * CELL_SIZE + VERTICAL_OFFSET, pszText, GetColor(255, 255, 255));
			*/

		}
	}

	// ����ʂ̓��e��\��ʂɉf��
	ScreenFlip();

	// �œK�ȍ��W��Ԃ�
	return BestVec;
}

/* ���i�v���C���[�j�̃x�X�g�Ȏ��T������֐�
 * Vec2 Coord			�F���݂̃m�[�h�Œu���΂̍��W
 * int BoardSub[8][8]	�F���݂̔Ֆʁi�����ł���������͂܂��΂�u���O�̔Ֆʁj
 * int Alpha			�F���l
 * int Beta				�F���l
 * int Depth			�F�T���̐[��
 */
int MinValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth) {

	// �Ֆʂɐ΂�u���āA�Ђ����肩�����āu���݂̔Ֆʁv�ɂ���
	BoardSub[Coord.GetY()][Coord.GetX()] = Turn;
	TurnOverStones(Coord.GetX(), Coord.GetY(),Turn, BoardSub);

	// �T���̐[�����O�Ȃ�A�T����ł��؂��ĕ]���l��Ԃ�
	if (Depth == 0) {
		return CalcValue(Turn, BoardSub);
	}
	
	// �܂��T���ł���Ȃ�ȉ��̏���������
	{
		// �G�̃^�[��
		int EnemyTurn = (Turn + 1) % STATE_MAX;

		// �ł��Ƃ��ł���}�X�̍��W
		Vec2 Children[STONE_END_NUM];

		// �ł��Ƃ��ł���}�X�̍��W���@�z��Children[STONE_END_NUM]  �ɔ��f����
		// �ł��Ƃ��ł�������v�Z����
		int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// �����łĂ�}�X���Ȃ��i�p�X�j�̎�
		// �I�΂�Ă͂����Ȃ��̂ŁA�ő�l��Ԃ�
		if (CanPutNum == 0) {
			return INT_MAX;
		}


		// �łĂ�}�X��T������
		for (int i = 0; i < CanPutNum; i++) {

			// ���̔Ֆʂ�p��(�΂�u���O)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// �T�����̎q�̕]���l���v�Z����
			Beta = MaxValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// ���l�������l�̂ق����傫��������A
			// ���l��Ԃ��i�}����j
			if (Alpha >= Beta) return Alpha;

		}
		// ���l��Ԃ�
		return Beta;
	}
}

/* ���iCOM�j�̃x�X�g�Ȏ��T������֐�
 * Vec2 Coord			�F���݂̃m�[�h�Œu���΂̍��W
 * int BoardSub[8][8]	�F���݂̔Ֆʁi�����ł���������͂܂��΂�u���O�̔Ֆʁj
 * int Alpha			�F���l
 * int Beta				�F���l
 * int Depth			�F�T���̐[��
 */
int MaxValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth) {

	// �Ֆʂɐ΂�u���āA�Ђ����肩�����āu���݂̔Ֆʁv�ɂ���
	BoardSub[Coord.GetY()][Coord.GetX()] = Turn;
	TurnOverStones(Coord.GetX(), Coord.GetY(), Turn, BoardSub);

	// �T���̐[�����O�Ȃ�A�T����ł��؂�
	if (Depth == 0) {
		return CalcValue(Turn, BoardSub);
	}

	// �܂��T���ł���Ȃ�ȉ��̏���������
	{
		// �G�̃^�[��
		int EnemyTurn = (Turn + 1) % STATE_MAX;

		// �ł��Ƃ��ł���}�X�̍��W
		Vec2 Children[STONE_END_NUM];

		// �ł��Ƃ��ł���}�X�̍��W���@�z��Children[STONE_END_NUM]  �ɔ��f����
		// �ł��Ƃ��ł�������v�Z����
		int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// �����łĂ�}�X���Ȃ��i�p�X�j�̎�
		// �I�΂�Ă͂����Ȃ��̂ŁA�ŏ��l��Ԃ�
		if (CanPutNum == 0) {
			return -INT_MAX;
		}

		// �łĂ�}�X��T������
		for (int i = 0; i < CanPutNum; i++) {

			// ���̔Ֆʂ�p��(�΂�u���O)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// �T�����̎q�̕]���l���v�Z����
			Alpha = MinValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// ���l�������l�̂ق����傫��������A
			// ���l��Ԃ��i�}����j
			if (Alpha >= Beta) return Beta;

		}
		// ���l��Ԃ�
		return Alpha;
	}
}

// �]���l���v�Z����֐�
int CalcValue(int Turn, int BoardSub[8][8]) {

	// �]���l
	int Value;

	// �G�̐΂̐F
	int EnemyStoneColor = (Turn + 1) % STATE_MAX;
	
	float BP = 0; // �Տ�̈ʒu�̕]���l
	float FS = 0; // �m��΂̕]���l
	float CN = 0; // ��␔�̕]���l

	int Wbp = 2; // �a�o�i�]���l�j�̏d��
	int Wfs = 5; // �e�r�̏d��
	int Wcn = 1; // �b�m�̏d��

	// �Տ�̈ʒu�̕]���l BP ���v�Z����
	{
		for (int height = 0; height < BOARD_HEIGHT; height++) {
			for (int width = 0; width < BOARD_WIDTH; width++) {

				// �O����P�̗���
				float Rand = float(rand()) / RAND_MAX;

				// �]���l���v�Z���Ď����̐΂Ȃ瑫���A����̐΂Ȃ����
				if (BoardSub[height][width] == Turn) {
					BP += BoardValueLast[height][width] * Rand * 3;
				}
				else if (BoardSub[height][width] == EnemyStoneColor) {
					BP -= BoardValueLast[height][width] * Rand * 3;
				}

			}
		}
	}

	// �m��΂̕]���l FS ���v�Z����
	{
		// �O����P�̗���
		float Rand = float(rand()) / RAND_MAX;

		// �����̊m��΂̐��𒲂ׂ�
		int TurnStoneNum = CalcFS(Turn, BoardSub);

		// ����̐΂̊m��΂̐��𒲂ׂ�
		int EnemyStoneNum = CalcFS(EnemyStoneColor, BoardSub);

		// �m��΂̕]���l FS ���v�Z����
		FS = ((TurnStoneNum - EnemyStoneNum) + Rand * 3) * 11;
	}

	// ��␔�̕]���l CN ���v�Z����
	{
		// �O����P�̗���
		float Rand = float(rand()) / RAND_MAX;

		// SearchCanPutWhite�֐����g�����߂ɗp��
		Vec2 Children[STONE_END_NUM];

		// ��␔�𓾂�
		int CandidatesNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// ��␔�̕]���l CN ���v�Z����
		CN = (CandidatesNum + Rand * 2) * 10;

	}

	// �]���l���v�Z����
	Value = int(BP*Wbp + FS*Wfs + CN*Wcn);
	
	// �]���l��Ԃ�
	return Value;
}

// �m��΂̐����v�Z���� 
int CalcFS(int Turn, int BoardSub[8][8]) {

	// �����̊m��΂̐��𒲂ׂ�
	int StoneNum = 0;

	// ��̕ӂ̒T��
	{
		// ��̕ӂ̐΂̐��𐔂���
		int StoneTopNum = 0;
		int StoneTurnNum = 0;
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (BoardSub[0][x] == BLACK || BoardSub[0][x] == WHITE) {
				StoneTopNum++;
				if (BoardSub[0][x] == Turn) StoneTurnNum++;
			}
		}

		// �S�����܂��Ă�����u���Ă���΂͊m���
		if (StoneTopNum == BOARD_WIDTH) {
			// �m��΂𑫂�
			StoneNum += StoneTurnNum;
		}
		// �󂫂���������[���璲�ׂ�
		else {

			// �΂̐���������
			StoneTurnNum = 0;

			// ���[����
			int x = 0;
			int y = 0;
			while (BoardSub[y][x] == Turn && x < BOARD_WIDTH) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x++;
			}

			// �E�[����
			x = BOARD_WIDTH - 1;
			while (BoardSub[y][x] == Turn && x >= 0) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x--;
			}

			// �m��΂𑫂�
			StoneNum += StoneTurnNum;

		}
	}

	// ���̕ӂ̒T��
	{
		// ���̕ӂ̐΂̐��𐔂���
		int StoneBottomNum = 0;
		int StoneTurnNum = 0;
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (BoardSub[BOARD_HEIGHT-1][x] == BLACK || BoardSub[BOARD_HEIGHT - 1][x] == WHITE) {
				StoneBottomNum++;
				if (BoardSub[BOARD_HEIGHT - 1][x] == Turn) StoneTurnNum++;
			}
		}

		// �S�����܂��Ă�����u���Ă���΂͊m���
		if (StoneBottomNum == BOARD_WIDTH) {
			// �m��΂𑫂�
			StoneNum += StoneTurnNum;
		}
		// �󂫂���������[���璲�ׂ�
		else {

			// �΂̐���������
			StoneTurnNum = 0;

			// ���[����
			int x = 0;
			int y = BOARD_HEIGHT - 1;
			while (BoardSub[y][x] == Turn && x < BOARD_WIDTH) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x++;
			}

			// �E�[����
			x = BOARD_WIDTH - 1;
			while (BoardSub[y][x] == Turn && x >= 0) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x--;
			}

			// �m��΂𑫂�
			StoneNum += StoneTurnNum;

		}
	}

	// ���̕ӂ̒T��
	{
		// ���̕ӂ̐΂̐��𐔂���
		int StoneLeftNum = 0; // ���[�̐΂̐�
		int StoneTurnNum = 0; // ���[��Turn�̐F�̐΂̐�
 		for (int y = 0; y < BOARD_HEIGHT; y++) {


			if (BoardSub[y][0] == BLACK || BoardSub[y][0] == WHITE) {

				// �E�̕ӂ̐΂̑���
				StoneLeftNum++;

				// ��ԏ�ƈ�ԉ��̃}�X��
				// ��̕ӂƉ��̕ӂ̒T���̎��ɑ�������ł���̂ŁA�X���[����
				// ��ԏ�ƈ�ԉ��̃}�X�ȊO��Turn�̐F�̐΂Ƃ��Đ��𑫂�����ł���
				if ((y != 0) && (y != BOARD_HEIGHT - 1) && (BoardSub[y][0] == Turn)) StoneTurnNum++;
			}
		}

		// �S�����܂��Ă�����u���Ă���΂͊m���
		if (StoneLeftNum == BOARD_HEIGHT) {
			// �m��΂𑫂�
			StoneNum += StoneTurnNum;
		}
		// �󂫂���������[���璲�ׂ�
		else {

			// �΂̐���������
			StoneTurnNum = 0;

			// ��[����
			int x = 0;
			int y = 0;

			// ��[�̃}�X��Turn�̐F�Ȃ�
			if (BoardSub[y][x] == Turn) {
				// �΂̐��𐔂�����
				// ��}�X�i��
				y++;

				// Turn�̐F�̐΂�����ł����琔��������
				while (BoardSub[y][x] == Turn && y < BOARD_HEIGHT) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y++;
				}
			}

			// ���[����
			y = BOARD_HEIGHT - 1;

			// ���[�̃}�X��Turn�̐F�Ȃ�
			if (BoardSub[y][x] == Turn) {

				// �΂̐��𐔂�����
				// ��}�X�i��
				y--;

				// Turn�̐F�̐΂�����ł����琔��������
				while (BoardSub[y][x] == Turn && y >= 0) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y--;
				}
			}
			// �m��΂𑫂�
			StoneNum += StoneTurnNum;

		}
	}

	// �E�̕ӂ̒T��
	{
		// �E�̕ӂ̐΂̐��𐔂���
		int StoneRightNum = 0; // �E�[�̐΂̐�
		int StoneTurnNum = 0;  // �E�[��Turn�̐F�̐΂̐�
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (BoardSub[y][BOARD_WIDTH-1] == BLACK || BoardSub[y][BOARD_WIDTH - 1] == WHITE) {

				// �E�̕ӂ̐΂̑���
				StoneRightNum++;

				// ��ԏ�ƈ�ԉ��̃}�X��
				// ��̕ӂƉ��̕ӂ̒T���̎��ɑ�������ł���̂ŁA�X���[����
				// ��ԏ�ƈ�ԉ��̃}�X�ȊO��Turn�̐F�̐΂Ƃ��Đ��𑫂�����ł���
				if ((y != 0) && (y != BOARD_HEIGHT - 1) && (BoardSub[y][BOARD_WIDTH - 1] == Turn)) StoneTurnNum++;
			}
		}

		// �S�����܂��Ă�����u���Ă���΂͊m���
		if (StoneRightNum == BOARD_HEIGHT) {
			// �m��΂𑫂�
			StoneNum += StoneTurnNum;
		}
		// �󂫂���������[���璲�ׂ�
		else {

			// �΂̐���������
			StoneTurnNum = 0;

			// ��[����
			int x = BOARD_WIDTH - 1;
			int y = 0;

			// ��[�̃}�X��Turn�̐F�Ȃ�
			if (BoardSub[y][x] == Turn) {
				// �΂̐��𐔂�����
				// ��}�X�i��
				y++;

				// Turn�̐F�̐΂�����ł����琔��������
				while (BoardSub[y][x] == Turn && y < BOARD_HEIGHT) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y++;
				}
			}

			// ���[����
			y = BOARD_HEIGHT - 1;

			// ���[�̃}�X��Turn�̐F�Ȃ�
			if (BoardSub[y][x] == Turn) {

				// �΂̐��𐔂�����
				// ��}�X�i��
				y--;

				// Turn�̐F�̐΂�����ł����琔��������
				while (BoardSub[y][x] == Turn && y >= 0) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y--;
				}
			}

			// �m��΂𑫂�
			StoneNum += StoneTurnNum;

		}
	}

	return StoneNum;
}
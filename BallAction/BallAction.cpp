//	
//	ゲームタイトル「BallAction」
//  制作者：加茂大輝
//	制作期間：2021/2/21～2021/3/11
//　
#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // 画面の横幅
#define SCREEN_HEIGHT    (480)                          // 画面の縦幅
#define CHIP_SIZE        (32)                           // 一つのチップのサイズ
#define BALL_SIZE		 (30)							// ボールのサイズ（画像1辺のピクセル数）
#define EASY_MAP_WIDTH   (SCREEN_WIDTH  / CHIP_SIZE)	// Eazyマップの横幅
#define HARD_MAP_WIDTH   (SCREEN_WIDTH*2 / CHIP_SIZE)	// Hardマップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅

#define EASY_MODE		 (0)							// EASYモード
#define HARD_MODE		 (1)						    // HARDモード

#define FLOAR_POWER      (2)							// 浮力

// ボールの状態
enum BallState {
	WOOD,		// ０	木
	METAL,		// １	金属
	STATE_END	// ２	終わり
};

// 壁（背景）の状態
enum WallState {
	// 当たり判定なし
	BACKGROUND,	// ０　背景（何もなし）
	WATER,		// １　水（木ボールは浮いて、金属ボールは沈む）

	// 当たり判定あり
	// （３以降は、仕様としては1ステージに1つ）
	BRICK,		// ２　レンガ（消えない壁）
	ROCK,		// ３　岩(スイッチが押されることによって消える -> BACKGROUNDになる)
	SWITCH_OFF,	// ４　オフのスイッチ
	SWITCH_ON,	// ５　オンのスイッチ
	GOAL		// ６　ゴール
};


// ゲーム中に扱う変化するマップのデータ
int MapData[MAP_HEIGHT][HARD_MAP_WIDTH];

// EASYマップのデータ
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

// HARDマップのデータ
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

// Str 文字列クラスの定義
class Str {
public:
	int x;				// x座標
	int y;				// y座標
	const TCHAR *Str;	// 文字列
	int StrLen;			// 文字列の長さ
	int StrWidth;		// 文字列の横幅
	int Color = GetColor(255, 255, 255);	// 文字の色（白）

	// クラスの変数を設定する
	void set(int y, const CHAR*str);

	// 文字列を描画する
	void Draw();
};

// クラスの変数を設定する
void Str::set(int y, const CHAR*str) {

	this->Str = str;
	this->StrLen = strlen(Str);
	this->StrWidth = GetDrawStringWidth(Str, StrLen);

	// 文字列が真ん中に来るようにx座標を計算する
	this->x = (SCREEN_WIDTH - this->StrWidth) / 2;
	this->y = y;
}

// 文字列を描画する
void Str::Draw() {
	DrawString(this->x, this->y, this->Str, this->Color);
}


int BallX, BallY;		// ボールの座標
int ScrollX;			// 画面のスクロール量
int ScrollXMax;			// 画面のスクロール量の最大値

int Speed;              // ボールの移動速度
int BallDown;			// ボールの落下速度
int G;				 	// 落下加速度
int JumpPower;			// ジャンプ力
bool JumpFlag;			// ジャンプできるかのフラグ（接地中TRUE）

int Input, EdgeInput;	// 入力状態
int LastKey;			// 前回のキー入力値
bool GoalFlag;			// ゴールフラグ

BallState StateB;		// ボールの状態

// 画像ファイルをロードする変数
int GraphBallWood, GraphBallMetal; // ボール
int GraphBrick, GraphRock, GraphSwitchOff, GraphSwitchOn, GraphGoal; // 壁
int GraphDark; // 暗闇


//************ プロトタイプ宣言 ************//
//*** スタート画面用 ****//

// スタート画面の初期化（背景とメニューを表示する）関数
void InitStartScreen(Str Title, Str Easy, Str Hard, Str HowToPlay, Str GameQuit);
// スタート画面の描画関数
void DrawStart(int* SelectMenuNum);

//*** ゲーム画面用 ****//

// ゲーム画面のメイン関数
void BallGameMain(int *SelectMenuNum);
// マップとボールの描画関数
void DrawGraph(int MapWidth);
// 当たり判定を考慮したボールのアクション
void BallAct(int Key, int MapWidth);
// 移動後の当たり判定のチェックをして移動量を修正する
int FixMove(int x, int y, int* MoveX, int* MoveY);
// スクロール後の当たり判定のチェックをして移動量を修正する
void FixScroll(int x, int y, int* ScrollSubX);
// マップチップの値を取得する関数
int GetChipValue(int X, int Y);
// スイッチが押された時に岩を消す関数
void DeleteRock(int MapWidth);
// ボールの状態変化関数
void ChangeBallState(void);
// ゴール表示する
void DrawGoal(void);

// ＷｉｎＭａｉｎ関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// タイトルを BallAction に変更
	SetMainWindowText("BallAction");

	// アイコンを設定する
	SetWindowIconID(111);

	// ウインドウモードに変更
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤーが選択中のメニュー
	// 0->EASY、 1->HARD 、　2->遊び方、 3->やめる
	int SelectMenuNum = 0;

	// Escapeキーが押されるか
	// "やめる"が選択されるまでループを続ける
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && SelectMenuNum != 3) {

		// スタート画面のメニューフレームの位置を初期化
		SelectMenuNum = 0;

		// スタート画面を描画する
		DrawStart(&SelectMenuNum);

		// ゴールフラグを初期化
		GoalFlag = FALSE;

		// ゲームスタートならゲームを開始
		if (SelectMenuNum == 0 || SelectMenuNum == 1) {
			BallGameMain(&SelectMenuNum);		// ゲーム画面のメイン関数
		}

		// ゴールしていたらゴールを表示する
		if (GoalFlag) DrawGoal();

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}


// スタート画面の初期化（背景とメニューを表示する）関数
void InitStartScreen(Str Title, Str Easy, Str Hard, Str HowToPlay, Str GameQuit) {

	// 背景表示
	LoadGraphScreen(0, 0, "WallPaper.png", TRUE);

	// タイトルを描画
	SetFontSize(64);
	Title.Draw();

	// メニューを描画
	SetFontSize(32);
	Easy.Draw();
	Hard.Draw();
	HowToPlay.Draw();
	GameQuit.Draw();

}

// スタート画面の描画関数
void DrawStart(int* SelectMenuNum) {

	// スタート画面での、タイトルとメニューの文字列のそれぞれのy座標
	int TitleY = SCREEN_HEIGHT / 5;
	int EasyY = 5 * SCREEN_HEIGHT / 12;
	int HardY = 13 * SCREEN_HEIGHT / 24;
	int HowToPlayY = 4 * SCREEN_HEIGHT / 6;
	int GameQuitY = 19 * SCREEN_HEIGHT / 24;

	// スタート画面に表示する文字列
	Str Title, Easy, Hard, HowToPlay, GameQuit;

	// クラスの初期化
	SetFontSize(64);
	Title.set(TitleY, "BallAction!");
	SetFontSize(32);
	Easy.set(EasyY, "EASY");
	Hard.set(HardY, "HARD");
	HowToPlay.set(HowToPlayY, "遊び方");
	GameQuit.set(GameQuitY, "やめる");;

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// 画面を初期化
		InitStartScreen(Title, Easy, Hard, HowToPlay, GameQuit);

		// 選択枠の描画
		LoadGraphScreen(SCREEN_WIDTH / 4, 5 * SCREEN_HEIGHT / 12 + *SelectMenuNum * SCREEN_HEIGHT / 8 - 15, "SelectFrame.png", TRUE);

		// 裏画面の内容を表画面に映す
		ScreenFlip();

		// 入力状態を更新
		{
			int i;

			// パッド１とキーボードから入力を得る
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// エッジを取った入力をセット
			EdgeInput = i & ~Input;

			// 入力状態の保存
			Input = i;
		}


		// 前回のループで押されたボタンが上ボタンでなくて、
		// 上ボタンが押された時、選択枠を一つ上にあげる
		if ((Input != LastKey) && (Input & PAD_INPUT_UP)) *SelectMenuNum = (*SelectMenuNum + 7) % 4;

		// 前回のループで押されたボタンが下ボタンでなくて、
		// 下ボタンが押された時、選択枠を一つ下にさげる
		if ((Input != LastKey) && (Input & PAD_INPUT_DOWN)) *SelectMenuNum = (*SelectMenuNum + 1) % 4;

		// エンターキーが押された時の処理
		if (CheckHitKey(KEY_INPUT_RETURN)) {

			// 「遊び方」が選択された時、遊び方を表示する
			if (*SelectMenuNum == 2) {

				// まず、背景表示して
				LoadGraphScreen(0, 0, "WallPaper.png", TRUE);

				// 次に、遊び方を表示する
				LoadGraphScreen(140, 0, "HowToPlay.png", TRUE);

				// 裏画面の内容を表画面に映す
				ScreenFlip();

				// エンターキーから指が離れるまで待つ
				while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;

				// キー入力待ち
				WaitKey();

				// キー入力待ちで押されたキーが
				// エンターキー　か　上キー　か　下キーなら
				// キーから指が離れるまで待つ
				while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;
				while (CheckHitKey(KEY_INPUT_UP)) if (CheckHitKey(KEY_INPUT_UP) == 0)break;
				while (CheckHitKey(KEY_INPUT_DOWN)) if (CheckHitKey(KEY_INPUT_DOWN) == 0)break;

			}

			// 「EASY」「HARD」「やめる」が選択された時
			// ループを抜ける
			else {
				break;
			}

		}

		// エスケープキーが押された時
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			// "やめる"を選択したことにしてにしてループを抜ける
			*SelectMenuNum = 3;
			break;
		}

		// 次のループの時、比較に使うために
		// 現在の入力キーを保存しておく
		LastKey = Input;

	}

}

// ゲーム画面のメイン関数
void BallGameMain(int *SelectMenuNum) {

	BallX = CHIP_SIZE;	// 画面上でのボールのx座標を初期化
	BallY = CHIP_SIZE;	// 画面上でのボールのy座標を初期化

	ScrollX = 0;		// 画面のスクロール量を初期化

	BallDown = 0;		// プレイヤーの落下速度を初期化

	// ボールとその他物理演算に使う変数の初期状態を
	// 木ボールに設定
	StateB = WOOD;		// ボールの状態
	G = 1;				// 落下加速度
	Speed = 10;			// 移動スピード
	JumpPower = 16;		// ジャンプ力


	// マップの横幅を保存
	int MapWidth;
	if (*SelectMenuNum == EASY_MODE) {
		MapWidth = EASY_MAP_WIDTH;
	}
	else if (*SelectMenuNum == HARD_MODE) {
		MapWidth = HARD_MAP_WIDTH;
	}

	// スクロール量の最大値（マップの右端-画面の幅）
	// これを使って画面がスクロールしすぎないようにする
	ScrollXMax = MapWidth * CHIP_SIZE - SCREEN_WIDTH;

	// マップを初期化
	for (int height = 0; height < MAP_HEIGHT; height++) {
		for (int width = 0; width < MapWidth; width++) {
			// EASYモードだったらEASY用のマップを入れる
			if (*SelectMenuNum == EASY_MODE) MapData[height][width] = OriginEasyMapData[height][width];
			// HARDモードだったらHARD用のマップを入れる
			if (*SelectMenuNum == HARD_MODE) MapData[height][width] = OriginHardMapData[height][width];
		}
	}

	// 画像の白色を透過させる設定にしておく
	SetTransColor(255, 255, 255);

	// 画像ファイルをロードする
	GraphBallWood = LoadGraph("WoodBall.png");
	GraphBallMetal = LoadGraph("MetalBall.png");
	GraphBrick = LoadGraph("Brick.png");
	GraphRock = LoadGraph("Rock.png");
	GraphSwitchOff = LoadGraph("SwitchOff.png");
	GraphSwitchOn = LoadGraph("SwitchOn.png");
	GraphGoal = LoadGraph("Goal.png");
	GraphDark = LoadGraph("Dark.png");

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// 画面を初期化
		ClearDrawScreen();

		// 入力状態を更新
		{
			int i;

			// パッド１とキーボードから入力を得る
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// エッジを取った入力をセット
			EdgeInput = i & ~Input;

			// 入力状態の保存
			Input = i;
		}

		// 当たり判定を考慮したアクション
		// (キー入力に従ってボールを移動させたり、画面をスクロールさせる)
		BallAct(Input, MapWidth);

		// マップとボールを描画
		DrawGraph(MapWidth);

		// 裏画面の内容を表画面に映す
		ScreenFlip();

		// ウエイト
		WaitTimer(30);

		// ゴールしていたらループを抜ける
		if (GoalFlag)break;
	}
}

// マップとボールの描画関数
void DrawGraph(int MapWidth) {

	// マップの描画
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MapWidth; x++) {

			// 描画するチップ座標
			int DrawX = x * CHIP_SIZE - ScrollX;
			int DrawY = y * CHIP_SIZE;

			// 描画するチップの状態を取得
			int StateW = MapData[y][x];

			// マップデータを描画する
			switch (StateW) {

				// 背景（真っ黒）
				case BACKGROUND:
					DrawBox(DrawX, DrawY, DrawX + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE, GetColor(0, 0, 0), TRUE);
					break;
				// 背景（水）
				case WATER:
					DrawBox(DrawX, DrawY, DrawX + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE, GetColor(0, 0, 255), TRUE);
					break;

				// レンガ
				case BRICK:
					DrawGraph(DrawX, DrawY, GraphBrick, FALSE);
					break;
				// 岩
				case ROCK:
					DrawGraph(DrawX, DrawY, GraphRock, FALSE);
					break;
				// 押される前のスイッチ
				case SWITCH_OFF:
					DrawGraph(DrawX, DrawY, GraphSwitchOff, FALSE);
					break;
				// 押された後のスイッチ
				case SWITCH_ON:
					DrawGraph(DrawX, DrawY, GraphSwitchOn, FALSE);
					break;
				// ゴール
				case GOAL:
					DrawGraph(DrawX, DrawY, GraphGoal, FALSE);
					break;
			}
		}
	}


	// ボールの描画
	{
		// ボールの状態によって読み込む画像を変える
		switch (StateB) {
			// 木ボール
		case WOOD:
			DrawGraph(BallX, BallY, GraphBallWood, TRUE);
			break;
			// 金属ボール
		case METAL:
			DrawGraph(BallX, BallY, GraphBallMetal, TRUE);
			break;
		}
	}

	// ボールの位置が明るくなるように座標を修正して
	// 暗闇画像を描く
	int DarkX = BallX - SCREEN_WIDTH + CHIP_SIZE / 2;
	int DarkY = BallY - SCREEN_HEIGHT + CHIP_SIZE / 2;
	DrawGraph(DarkX, DarkY, GraphDark, TRUE);
	
}

// 当たり判定を考慮したボールのアクション
void BallAct(int Key, int MapWidth) {

	// ボールの移動量
	int MoveX = 0;
	int MoveY = 0;

	// 修正前の移動量
	int ScrollSubX = 0;

	// zボタンが押されたらボールの状態変化させる
	if ((Key != LastKey) && (Key & PAD_INPUT_A)) ChangeBallState();

	// 左ボタンが押された時の処理
	if (Key & PAD_INPUT_LEFT) {
		
		// ボールの位置とスクロール量を動かしてみる
		MoveX -= Speed;
		ScrollSubX -= Speed;

		//** 条件によってボールの位置とスクロール量を修正する **//

		if (ScrollX == 0 && (BallX < SCREEN_WIDTH / 2)) {
			// 画面がマップの左端にあって
			// ボールが画面の左半分にいるとき
			// ボールは自由に動ける（スクロールしないようにする）
			ScrollSubX = 0;
		}
		else if (ScrollX == ScrollXMax && (BallX > SCREEN_WIDTH / 2)) {
			// 画面がマップの右端にあって
			// ボールが画面の右半分にいるとき
			// ボールは自由に動ける（スクロールしないようにする）
			ScrollSubX = 0;
		}
		else{
			// 上の条件以外はスクロール中なので
			// ボールを画面の真ん中に固定して動かないようにする
			BallX = SCREEN_WIDTH / 2;
			MoveX = 0;
		}
		
		// スクロール中(MoveX == 0)で
		// ボールが自由に動ける範囲に入ろうとしてたら
		// ボールも移動できるようにMoveXに移動量を反映させる
		if (MoveX == 0) {
			// 左ボタンが押された時の処理なので
			// ここ↓でのif文の条件は、”画面がマップの左端にあってボールが画面の左半分に入ろうとしたとき”
			if ((ScrollX == 0) && (BallX - Speed < SCREEN_WIDTH / 2)) {
				MoveX -= Speed;
			}
		}
	}

	// 右ボタンが押された時の処理
	if (Key & PAD_INPUT_RIGHT) {

		// ボールの位置とスクロール量を動かしてみる
		MoveX += Speed;
		ScrollSubX += Speed;

		//** 条件によってボールの位置とスクロール量を修正する **//

		if (ScrollX == 0 && BallX < SCREEN_WIDTH / 2) {
			// 画面がマップの左端にあって
			// ボールが画面の左半分にいるとき
			// ボールは自由に動ける（スクロールしないようにする）
			ScrollSubX = 0;
		}
		else if ((ScrollX == ScrollXMax) && (BallX > SCREEN_WIDTH / 2)) {
			// 画面がマップの右端にあって
			// ボールが画面の右半分にいるとき
			// ボールは自由に動ける（スクロールしないようにする）
			ScrollSubX = 0;
		}
		else {
			// 上の条件以外はスクロール中なので
			// ボールを画面の真ん中に固定して動かないようにする
			BallX = SCREEN_WIDTH / 2;
			MoveX = 0;
		}

		// スクロール中(MoveX == 0)で
		// ボールが自由に動ける範囲に入ろうとしてたら
		// ボールも移動できるようにMoveXに移動量を反映させる
		if (MoveX == 0) {
			// 右ボタンが押された時の処理なので
			// ここ↓でのif文の条件は、”画面がマップの右端にあってボールが画面の右半分に入ろうとしたとき”
			if ((ScrollX == ScrollXMax) && (BallX + Speed > SCREEN_WIDTH / 2)) {
				MoveX += Speed;
			}
		}
	}

	// ジャンプ処理
	{
		// ボールの下が地面についてて
		// 上ボタン（ジャンプボタン）が押された時の処理
		if (JumpFlag == TRUE && (EdgeInput & PAD_INPUT_UP)) {

			// ボールをジャンプさせて、ジャンプフラグをおろす（ジャンプできないようにする）
			BallDown -= JumpPower;
			JumpFlag = FALSE;

		}
	}

	// 落下加速度(下向き正)を加える
	BallDown += G;

	// 落下速度を移動量に加える
	MoveY += BallDown;

	// 移動量0のダミー
	int Dummy = 0;

	// 上下の移動について当たり判定を調べて縦の移動量 MoveY を修正する
	{

		// 左下の点が下のブロックに当たっていた時、落下速度を０にする
		if (FixMove(BallX, BallY + BALL_SIZE, &Dummy, &MoveY) == 1) BallDown = 0;

		// 右下の点が下のブロックに当たっていた時、落下速度を０にする
		if (FixMove(BallX + BALL_SIZE, BallY + BALL_SIZE, &Dummy, &MoveY) == 1) BallDown = 0;

		// 左上の点が上のブロックに当たっていた時、落下速度を０にする
		if (FixMove(BallX, BallY, &Dummy, &MoveY) == 2) BallDown = 0;

		// 右上の点が上のブロックに当たっていた時、落下速度を０にする
		if (FixMove(BallX + BALL_SIZE, BallY, &Dummy, &MoveY) == 2) BallDown = 0;

		// 上下の移動を加算
		BallY += MoveY;

	}

	// 左右の移動について当たり判定を調べて横の移動量 MoveX と ScrollX を修正する
	{
		
		// 左下の点が左のブロックに当たる時、移動量を修正する
		FixMove(BallX, BallY + BALL_SIZE, &MoveX, &Dummy);

		// 右下の点が右のブロックに当たる時、移動量を修正する
		FixMove(BallX + BALL_SIZE, BallY + BALL_SIZE, &MoveX, &Dummy);

		// 左上の点が左のブロックに当たる時、移動量を修正する
		FixMove(BallX, BallY, &MoveX, &Dummy);

		// 右上の点が右のブロックに当たる時、移動量を修正する
		FixMove(BallX + BALL_SIZE, BallY, &MoveX, &Dummy);

		// 左右の移動を加算
		BallX += MoveX;

		// スクロール中の当たり判定をチェック
		if (ScrollSubX != 0) {
			// 左下の点が左のブロックに当たる時、スクロール量を修正する
			FixScroll(BallX, BallY + BALL_SIZE, &ScrollSubX);

			// 右下の点が右のブロックに当たる時、スクロール量を修正する
			FixScroll(BallX + BALL_SIZE, BallY + BALL_SIZE, &ScrollSubX);

			// 左上の点が左のブロックに当たる時、スクロール量を修正する
			FixScroll(BallX, BallY, &ScrollSubX);

			// 右上の点が右のブロックに当たる時、スクロール量を修正する
			FixScroll(BallX + BALL_SIZE, BallY, &ScrollSubX);

			// 左右のスクロール量を加算
			ScrollX += ScrollSubX;
		}

	}

	// 接地判定
	{

		// ボールの左下と右下の点の下が
		// 当たり判定のあるブロックか調べる
		if (GetChipValue(BallX, BallY + BALL_SIZE + 1) >= 2 ||
			GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) >= 2) {

			// 足場があったらジャンプ可能にする
			JumpFlag = TRUE;

			// 足場がoffスイッチだったらonにして、岩を消す
			// 左下のチェック
			if (GetChipValue(BallX, BallY + BALL_SIZE + 1) == SWITCH_OFF) {

				MapData[(int)(BallY + BALL_SIZE + 1) / CHIP_SIZE][(int)(BallX + ScrollX) / CHIP_SIZE] = SWITCH_ON;
				DeleteRock(MapWidth);

			}
			// 右下のチェック
			if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == SWITCH_OFF) {

				MapData[(int)(BallY + BALL_SIZE + 1) / CHIP_SIZE][(int)(BallX + CHIP_SIZE + ScrollX) / CHIP_SIZE] = SWITCH_ON;
				DeleteRock(MapWidth);

			}


		}
		else {

			// 足場が無かったらジャンプ不可にする
			JumpFlag = FALSE;

		}
	}

	// 水中判定
	{
		// ボールの左下と右下の点の下が
		// 水につかっているか調べる
		if (GetChipValue(BallX, BallY + BALL_SIZE) == WATER ||
			GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE) == WATER) {

			// 浮力を加える
			BallDown -= FLOAR_POWER;

		}
	}

	// ゴール判定
	{
		// ゴールに当たったらゴールフラグをたてる

		// 左上の点についてチェック
		if (GetChipValue(BallX, BallY - 1) == GOAL) GoalFlag = TRUE;
		if (GetChipValue(BallX - 1, BallY) == GOAL) GoalFlag = TRUE;
		// 右上の点についてチェック
		if (GetChipValue(BallX + BALL_SIZE, BallY - 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE + 1, BallY) == GOAL) GoalFlag = TRUE;
		// 左下の点についてチェック
		if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE - 1, BallY + BALL_SIZE) == GOAL) GoalFlag = TRUE;
		// 右下の点についてチェック
		if (GetChipValue(BallX + BALL_SIZE, BallY + BALL_SIZE + 1) == GOAL)  GoalFlag = TRUE;
		if (GetChipValue(BallX + BALL_SIZE + 1, BallY + BALL_SIZE) == GOAL) GoalFlag = TRUE;

	}

	// 押されたキーを保存する
	LastKey = Key;

}

// 移動後の当たり判定のチェックをして移動量を修正する
// 画面上のボールの四隅の4点のうちどれか１つの座標(x, y) と 移動量 MoveX,MoveY（どちらかが０でないとちゃんと動かない）
int FixMove(int x, int y, int* MoveX, int* MoveY) {

	// 移動後のある点（x,y）の座標（afterX,afterY）
	int afX = x + *MoveX;
	int afY = y + *MoveY;

	// 当たり判定のあるブロック(値が2以上)に当たっているかチェック
	if (GetChipValue(afX, afY) >= 2) {

		// ブロックの4つの座標値
		// BlockLeftX, BlockRightX
		// BlockTopY, BlockBottomY
		int blx, brx, bty, bby;

		// ブロックの上下左右の座標
		blx = int(afX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の X 座標
		brx = int(afX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の X 座標

		bty = int(afY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
		bby = int(afY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

		// ブロックとの距離
		int dx, dy;


		// 当たっていたら壁から離す処理を行う（移動量を修正する）

		// 下に移動してブロックに当たった場合
		// （ボールの下側にあるブロックの上辺と当たった）
		if (*MoveY > 0) {

			// ブロックとの距離を計算
			// （ブロックの上辺）-（ボールの下側の点）
			dy = bty - y;

			// 移動量を修正する（めり込まないように-1して上に持ち上げる）
			*MoveY = dy - 1;

			return 1;
		}

		// 上に移動してブロックに当たった場合
		// （ボールの上側にあるブロックの下辺と当たった）
		if (*MoveY < 0) {

			// ブロックとの距離を計算
			// （ブロックの下辺）-（ボールの上側の点）
			// 上側への移動なので負の値となるように計算する
			dy = bby - y;

			// 移動量を修正する（めり込まないように+1して下に下げる）
			*MoveY = dy + 1;

			return 2;
		}

		// 右に移動してブロックに当たった場合
		// （ボールの右側にあるブロックの左辺と当たった）
		if (*MoveX > 0) {

			// ブロックとの距離を計算
			// （ブロックの左辺）-（ボールの右側の点）
			dx = blx - x;

			// 移動量を修正する（めり込まないように-1して右に移動させる）
			*MoveX = dx - 1;

			return 3;
		}

		// 左に移動してブロックに当たった場合
		// （ボールの左側にあるブロックの右辺と当たった）
		if (*MoveX < 0) {

			// ブロックとの距離を計算
			// （ブロックの右辺）-（ボールの左側の点）
			// 左側への移動なので負の値となるように計算する
			dx = brx - x;

			// 移動量を修正する（めり込まないように+1して左に移動させる）
			*MoveX = dx + 1;

			return 4;
		}
		return 2;
	}

	// どこにも当たらなかったと返す
	return 0;
}

// スクロール後の当たり判定のチェックをして移動量を修正する
// 画面上の座標(x, y) と　スクロール量 ScrollX
void FixScroll(int x, int y, int* ScrollSubX) {

	// スクロールしてみて
	// 当たり判定のあるブロック(値が2以上)に当たっているかチェック
	if (GetChipValue(x + *ScrollSubX, y) >= 2) {

		// ブロックの左側と右側のx座標
		// BlockLeftX, BlockRightX
		int blx, brx;

		// ブロックの左右の座標
		blx = int((ScrollX + x + *ScrollSubX) / CHIP_SIZE) * CHIP_SIZE - ScrollX;        // 左辺の X 座標
		brx = int((ScrollX + x + *ScrollSubX) / CHIP_SIZE + 1) * CHIP_SIZE - ScrollX;    // 右辺の X 座標

		// ブロックとの距離
		int dx;


		// 当たっていたら壁から離す処理を行う（移動量を修正する）

		// 右にスクロールしてブロックに当たった場合
		// （ボールの右側にあるブロックの左辺と当たった）
		if (*ScrollSubX > 0) {

			// ブロックとの距離を計算
			// （ブロックの左辺）-（ボールの右側の点）
			dx = blx - x;

			// 移動量を修正する（めり込まないように-1する）
			*ScrollSubX = dx - 1;

		}

		// 左にスクロールしてブロックに当たった場合
		// （ボールの左側にあるブロックの右辺と当たった）
		if (*ScrollSubX < 0) {

			// ブロックとの距離を計算
			// （ブロックの右辺）-（ボールの左側の点）
			// 左側への移動なので負の値となるように計算する
			dx = brx - x;

			// 移動量を修正する（めり込まないように+1する）
			*ScrollSubX = dx + 1;

		}
	}
	// スクロールしてみて
	// 画面がマップの左端に来ていないかチェック
	else if (ScrollX + *ScrollSubX < 0) {

		// 左端に来ていたら、これ以上スクロールしないようにする
		*ScrollSubX = 0;
		ScrollX = 0;

	}
	// スクロールしてみて
	// 画面がマップの右端に来ていないかチェック
	else if (ScrollX + *ScrollSubX > ScrollXMax) {

		// 右端に来ていたら、これ以上スクロールしないようにする
		*ScrollSubX = 0;
		ScrollX = ScrollXMax;

	}

}

// 座標値からマップチップの値を取得する関数
int GetChipValue(int X, int Y) {
	
	// 取得したいマップチップの値の位置（MapDataの添え字）
	int x, y;

	// 整数値へ変換
	x = (int)(X + ScrollX) / CHIP_SIZE;
	y = (int)Y / CHIP_SIZE;

	// 指定の座標に該当するマップの情報を返す
	return MapData[y][x];
}

// スイッチが押された時に岩を消す関数
void DeleteRock(int MapWidth) {
	int x, y;

	// マップとボールを描画
	DrawGraph(MapWidth);

	// 裏画面の内容を表画面に映す
	ScreenFlip();

	// 岩のブロックを探索
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MapWidth; x++) {
			if (MapData[y][x] == ROCK) {

				// 岩を消す（背景にする）
				MapData[y][x] = BACKGROUND;

				// マップとボールを描画
				DrawGraph(MapWidth);

				// 裏画面の内容を表画面に映す
				ScreenFlip();

			}
		}
	}
}

// ボールの状態変化関数
void ChangeBallState(void) {

	// 状態を1進める
	int i = static_cast<int>(StateB);
	int numState = static_cast<int>(BallState::STATE_END);
	StateB = static_cast<BallState>((i + 1) % numState);

	// 状態によって、落下加速度、落下加速度、ジャンプ力を変える
	//						木　金属
	// 落下加速度 G			 1	  3		(浮力が2なので、木は浮くように、金属は沈むようにしている)
	// 移動速度	　Speed　	10	  3
	// ジャンプ力 JumpPower	10	  5
	G = (int)((static_cast<int>(StateB) + 1) * 1.5);
	Speed = (int)10 / G ;
	JumpPower = (int)16 / G;
}

// ゴールを表示する
void DrawGoal(void) {

	// 表示する文字列の初期化
	Str Goal, AnyTapKey;

	// ゴールを画面中央に大きく描画する
	SetFontSize(128);
	Goal.set(SCREEN_HEIGHT / 4, "GOAL!!");
	Goal.Draw();

	// キー入力待ち用文字列を描画する
	SetFontSize(32);
	AnyTapKey.set(2 * SCREEN_HEIGHT / 3, "- Any Tap Key -");
	AnyTapKey.Draw();

	// 裏画面の内容を表画面に映す
	ScreenFlip();

	// キー入力待ち
	WaitKey();

	// 押されたキーが
	// エンターキー　か　上キー　か　下キーなら
	// キーから指が離れるまで待つ
	while (CheckHitKey(KEY_INPUT_RETURN)) if (CheckHitKey(KEY_INPUT_RETURN) == 0)break;
	while (CheckHitKey(KEY_INPUT_UP)) if (CheckHitKey(KEY_INPUT_UP) == 0)break;
	while (CheckHitKey(KEY_INPUT_DOWN)) if (CheckHitKey(KEY_INPUT_DOWN) == 0)break;
}
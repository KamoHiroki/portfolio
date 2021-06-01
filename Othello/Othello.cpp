//	
//	ゲームタイトル「Othello」
//  制作者：加茂大輝
//	制作期間：2021/3/12～2021/3/18
//　
// 初期設定では、COMとプレイするように作っています。
// 228行目のコメントアウトを外して、230～328行目をコメントアウトするとプレイヤー二人で遊べます。
// 

#include "DxLib.h"

#define BOARD_WIDTH		8	// ボードの横のマスの数
#define BOARD_HEIGHT	8	// ボードの縦のマスの数
#define CELL_SIZE		52	// マスの１辺のピクセル数

#define STONE_START_NUM  4	// 初期状態の石の数
#define STONE_END_NUM   64	// 置ける石の最大数

#define VERTICAL_OFFSET 100	// 画面上で盤面を下にずらす為のオフセット
#define STONE_OFFSET      2	// マス内に少しずらして石を描画する為のオフセット

#define SCREEN_WIDTH	CELL_SIZE*BOARD_WIDTH					// ゲーム画面の横幅
#define SCREEN_HEIGHT	CELL_SIZE*BOARD_HEIGHT+VERTICAL_OFFSET	// ゲーム画面の縦幅


// ゲームで使うボード
int BoardMain[BOARD_HEIGHT][BOARD_WIDTH];

// 石がおける位置
bool BoardCanPut[BOARD_HEIGHT][BOARD_WIDTH];

// 盤面の初期状態
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

// 序盤の評価値表
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

// 後半の評価関数
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

// 座標（x,y）のクラス
class Vec2 {
public:
	int X;		// x座標
	int Y;		// y座標
	void Set(int x, int y); // 座標の値を代入する関数
	int GetX(); // X座標値を返す関数
	int GetY(); // Y座標値を返す関数
};

// 座標の値を代入する関数
void Vec2::Set(int x, int y) {
	X = x;
	Y = y;
}

// 座標の値を代入する関数
int Vec2::GetX() { return X; }
int Vec2::GetY() { return Y; }

// マスの状態
enum StoneState{
	NONE = -1,	// 何も置かれていない
	BLACK = 0,	// 黒
	WHITE = 1,	// 白
	STATE_MAX	// 最大値
};

// ゲームの状態
enum GameState {
	BLACK_TURN,		// 黒のターン
	WHITE_TURN,		// 白のターン
	PASS,			// パス
	BLACK_WIN,		// 黒の勝ち
	WHITE_WIN,		// 白の価値
	DRAW,			// 引き分け
	GAME_STATE_MAX	// 最大値
};

// ゲームの状態の文字列
TCHAR GameStates[6][21] = {
	"あなた（黒）のターン",
	"COM（白）のターン",
	"パス",
	"あなた（黒）の勝ち",
	"COM（白）の勝ち",
	"引き分け"
};

// 石の個数　[0]->黒石の数 ,[1]->白石の数
int StonesNum[STATE_MAX] = { 2,2 };


int MouseX, MouseY;		// マウスの座標
int PassCounter;		// パスの回数（2回になったらゲーム終了）

bool GameEndFlag;		// ゲームが終わるフラグ

// 画像を読み込んでおくメモリ
int GraphCell, GraphBlack, GraphWhite, GraphFrame, GraphPuttable;


// プロトタイプ宣言

// ゲームが行われるメイン関数
void GameMain(void);				
// 盤面の初期化
void InitBoard(void);				
// 盤面を描画する
void DrawGame(int GameState);					
// プレイヤーがプレイする（黒のターン）
void PlayPlayer(int Turn, int GameState);		
// 置ける位置を探して配列BoardCanPutに反映する
int SearchCanPutBlack(int Turn);				
// 盤上の（x,y）に石が置けるかの判定
bool IsPuttable(int x, int y, int Turn, int BoardSub[8][8]);		
// 石をひっくり返す
void TurnOverStones(int x, int y, int Turn, int BoardSub[8][8]);	

// 置ける位置を探して、子の座標を配列に反映し、置ける場所の個数を返す（COM用）
int SearchCanPutWhite(int Turn, Vec2 Children[STONE_END_NUM], int BoardSub[8][8]);
// アルファベータ法を用いて最善手で石を置く（白のターン）
Vec2 AlphaBeta(int Turn, int BoardSub[8][8]);	
// 黒（プレイヤー）のベストな手を探索する関数
int MinValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth);
// 白（COM）のベストな手を探索する関数
int MaxValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth);
// 評価値を計算する関数
int CalcValue(int Turn, int BoardSub[8][8]);	
// 確定石の数を計算する 
int CalcFS(int Turn, int BoardSub[8][8]);		



// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);// ウィンドウモードの設定
	
	ChangeWindowMode(TRUE);		// ウインドウモードに変更

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK); // 描画先画面を裏画面にする

	GameMain();					// ゲームが行われるメイン関数

	WaitKey();					// キー入力待ち

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了 
}

// ゲームが行われるメイン関数
void GameMain(void) {

	// 盤面を初期化
	InitBoard();

	// 盤上の石の数を初期化（黒2白2で4個）
	int StoneNum = STONE_START_NUM;

	// 画像を読み込んでおく
	GraphCell = LoadGraph("Cell.png");
	GraphBlack = LoadGraph("Black.png");
	GraphWhite = LoadGraph("White.png");
	GraphFrame = LoadGraph("Frame.png");
	GraphPuttable = LoadGraph("Puttable.png");

	// 黒(プレイヤー)のターン
	int Turn = BLACK;					// どちらのターンか（0->黒,1->白）
	int GameState = BLACK;				// ゲームの状態

	// パスの回数を0回にする
	PassCounter = 0;

	// ゲームが終わっていないことにする
	GameEndFlag = FALSE;

	// ループ（石の数が64個以下の間　かつ　ゲームが終わっていない間 かつ　パスの回数が2回以下の間）
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && 
		StonesNum[BLACK]+StonesNum[WHITE] < STONE_END_NUM && GameEndFlag == FALSE && PassCounter<2) {

		// ターンとゲームの状態を黒のターンにする
		Turn = BLACK;	GameState = BLACK;
		
		// プレイヤーが黒石を置く
		PlayPlayer(Turn, GameState);

		// 勝負がついたらループを抜ける
		if (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM && GameEndFlag == TRUE && PassCounter >= 2)break;

		// ターンとゲームの状態を白のターンにする
		Turn = WHITE;	GameState = WHITE;


		// PlayPlayer(Turn, GameState);  //***************** 以下のスコープ部分をこちらに変えたら二人で遊べます **************//
		
		//　アルファベータ法をもちいてCOMが白石を置く
		{
			// 盤面を描画(白のターン)
			DrawGame(GameState);

			// 編集していい仮のボードを用意する
			int BoardSub[8][8];
			memcpy(BoardSub, BoardMain, sizeof(BoardMain));

			// COMが置く石の座標
			Vec2 CoordWhite;

			// 置けない設定（-1,-1）にしておく
			CoordWhite.Set(-1, -1);
			
			// 勝負序盤は一番評価値が高いところに置く
			if ((StonesNum[BLACK] + StonesNum[WHITE]) < (STONE_END_NUM * 3 / 4)) {

				// 最適なマスの評価値（探索したマスの評価値の方が高いとそれに更新していく）
				int BestValue = -INT_MAX;

				// 打つことができる候補マスの座標
				Vec2 Children[STONE_END_NUM];

				// 打つことができる候補マスの座標を　配列Children[STONE_END_NUM]  に反映して
				// 打つことができる候補マスの個数を計算する
				int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

				// 置けるマスがなかったら、(-1,-1)を入れる
				if (CanPutNum == 0) {
					CoordWhite.Set(-1, -1);
				}
				// 置けるマスがあったら、以下の処理をする
				else {

					// 打てるマスを探索
					for (int i = 0; i < CanPutNum; i++) {

						// 探索中のマスの評価値がBestValueよりも大きかったら
						if (BestValue < BoardValueFirst[Children[i].X][Children[i].Y]) {

							// 探索中のマスの評価値をBestValueにする
							BestValue = BoardValueFirst[Children[i].X][Children[i].Y];
							// 探索中のマスの座標をBestVecにする
							CoordWhite.Set(Children[i].X, Children[i].Y);
						}
					}
				}

				// 置ける場所をを表示
				DrawGame(Turn);

			}
			else {
				// 勝負後半は、アルファベータ法をもちいて座標を探索する
				CoordWhite = AlphaBeta(Turn, BoardSub);
			}

			// 石が置ける時
			if ((CoordWhite.GetX() != -1) && (CoordWhite.GetY() != -1)) {

				// パスカウンターを０にする
				PassCounter = 0;
				
				// 石を置く
				BoardMain[CoordWhite.GetY()][CoordWhite.GetX()] = Turn;
				StonesNum[Turn]++;

				// ひっくり返す
				TurnOverStones(CoordWhite.GetX() , CoordWhite.GetY(), Turn, BoardMain);
			}
			// 石が置けない時
			else {

				// 片方どちらかの石がすべてなくなった時、もしくは石の数がマス目の数になった時、ゲームを終了する
				if ((StonesNum[BLACK] == 0) || (StonesNum[WHITE] == 0) || (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM)) {

					// 勝利宣言
					GameState += 3;

					// ゲームを終了する
					GameEndFlag = TRUE;

				}
				// ゲームがまだ続くならパス表示する
				else {
					// パスを表示
					GameState = PASS;
					DrawGame(GameState);

					// パスカウンターを＋１する
					PassCounter++;

				}
			}
		}
		
		// 表示するために少し待つ
		WaitTimer(1000);
	}
	
	// ゲームが終わった時、勝敗を設定する
	if (StonesNum[BLACK] > StonesNum[WHITE]) {
		// 黒の価値
		GameState = BLACK_WIN;
	}
	else if (StonesNum[BLACK] < StonesNum[WHITE]) {
		// 白の価値
		GameState = WHITE_WIN;
	}
	else if (StonesNum[BLACK] == StonesNum[WHITE]) {
		// 引き分け
		GameState = DRAW;
	}

	// 勝敗を表示
	DrawGame(GameState);
}

// ボードを初期化する
void InitBoard(){
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {
			BoardMain[height][width] = BoardInit[height][width];
		}
	}
}

// 盤面を描画する
void DrawGame(int GameState) {
	
	// 画面を初期化
	ClearDrawScreen();

	// どっちのターンか描画する
	SetFontSize(32);
	DrawString(10, 10, GameStates[GameState], GetColor(255,255,255));

	// 黒石の数、白石の数を表示する
	DrawString(10, 40, "黒の数：", GetColor(255, 255, 255));
	DrawString(10, 70, "白の数：", GetColor(255, 255, 255));

	// 黒石の数と白石の数を表示する
	// TCHAR型文字列に変換して表示する
	TCHAR	BlackNum[256], WhiteNum[256];
	::_itot_s(StonesNum[BLACK], BlackNum, 256 * sizeof(TCHAR), 10);
	::_itot_s(StonesNum[WHITE], WhiteNum, 256 * sizeof(TCHAR), 10);
	DrawString(145, 40, BlackNum, GetColor(255, 255, 255));
	DrawString(145, 70, WhiteNum, GetColor(255, 255, 255));

	// マウスの位置を取得
	GetMousePoint(&MouseX, &MouseY);

	// 盤面を描画する
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			// マスを描画
			DrawGraph(width * CELL_SIZE , height * CELL_SIZE + VERTICAL_OFFSET, GraphCell, FALSE);

			// マウスが盤上にあったら
			if ((MouseX>=0) && (MouseX <= SCREEN_HEIGHT) && (MouseY>=VERTICAL_OFFSET) && (MouseY <= SCREEN_HEIGHT)) {
				// マウスの位置のマスは枠を強調する
				if ((MouseX / CELL_SIZE == width) && ((MouseY - VERTICAL_OFFSET) / CELL_SIZE == height)) {
					DrawGraph(width * CELL_SIZE, height * CELL_SIZE + VERTICAL_OFFSET, GraphFrame, TRUE);
				}
			}

			// 石が置けるマスなら点を書いて強調する
			if ((BoardMain[height][width] == NONE) && (BoardCanPut[height][width] == TRUE)) {
				DrawGraph(width * CELL_SIZE, height * CELL_SIZE + VERTICAL_OFFSET, GraphPuttable, TRUE);
			}

			// 黒色だったら黒石を置く
			if (BoardMain[height][width] == BLACK) {
				DrawGraph(width*CELL_SIZE + STONE_OFFSET, height*CELL_SIZE + VERTICAL_OFFSET + STONE_OFFSET, GraphBlack, TRUE);
			}

			// 白色だったら白石を置く
			if (BoardMain[height][width] == WHITE) {
				DrawGraph(width*CELL_SIZE + STONE_OFFSET, height*CELL_SIZE + VERTICAL_OFFSET + STONE_OFFSET, GraphWhite, TRUE);
			}
		}
	}

	// 裏画面の内容を表画面に映す
	ScreenFlip();

}

// プレイヤーがプレイする（黒のターン）
void PlayPlayer(int Turn,int GameState) {

	// 画面上でのマウスの位置にあるマスの添え字[CellY][CellX]
	int CellX, CellY;

	// 置ける場所を探して、その個数を取得する
	int CanPutNum = SearchCanPutBlack(Turn);

	// 置ける場所がある時の処理
	if (CanPutNum > 0) {

		// パスの回数を初期化
		PassCounter = 0;

		// ループ
		while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

			// マウスの座標を取得
			GetMousePoint(&MouseX, &MouseY);

			// マウスの座標からマス目の座標を求める
			CellX = (int)MouseX / CELL_SIZE;
			CellY = (int)(MouseY - VERTICAL_OFFSET) / CELL_SIZE;

			// 左クリックされた時の処理
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

				// マウスが盤上にあったら
				if ((MouseX >= 0) && (MouseX <= SCREEN_HEIGHT) && (MouseY >= VERTICAL_OFFSET) && (MouseY <= SCREEN_HEIGHT)) {

					// もしおける場所だったら 黒を置く
					if (BoardCanPut[CellY][CellX]) {

						// 石を置く
						BoardMain[CellY][CellX] = Turn;

						// Turn色の個数を1増やす
						StonesNum[Turn]++;

						// ひっくり返す
						TurnOverStones(CellX, CellY,Turn,BoardMain);

						// ループを抜ける
						break;
					}
				}
			}
			// 盤面を描画
			DrawGame(GameState);
		}

		
	}

	// 置ける場所がなかった時の処理
	else if (CanPutNum == 0) {

		// 片方どちらかの石がすべてなくなった時、ゲームを終了する
		if ((StonesNum[BLACK] == 0) || (StonesNum[WHITE] == 0) || (StonesNum[BLACK] + StonesNum[WHITE] >= STONE_END_NUM)) {

			// 勝利宣言
			GameState += 3;

			// ゲームを終了する
			GameEndFlag = TRUE;

		}
		else {

			// それ以外はパス表示して
			GameState = PASS;
			DrawGame(GameState);

			// パスカウンターを＋１する
			PassCounter++;

			// 表示するために少し待つ
			WaitTimer(1000);
			
		}
	}

}

// 黒が置ける位置を探して配列BoardCanPutに反映して、置ける場所の個数を返す
int SearchCanPutBlack(int Turn) {

	// 置ける個数
	int CanPutNum = 0;

	// 石が置ける位置を探索する
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			if (IsPuttable(width, height, Turn, BoardMain)) {

				// 置けるならTRUE を入れる
				BoardCanPut[height][width] = TRUE;

				// 置ける個数を+1
				CanPutNum++;

			}
			else {
				// 置けないならFALSEを入れる
				BoardCanPut[height][width] = FALSE;
			}
		}
	}
	// 置ける個数を返す
	return CanPutNum;

}

// 盤上の（x,y）に石が置けるかの判定
bool IsPuttable(int x, int y,int Turn, int BoardSub[8][8]) {

	// 石が無かったら探索する
	if (BoardSub[y][x] == NONE) {
		// 敵の石の色
		int EnemyStoneColor = (Turn + 1) % STATE_MAX;

		/* 全方向に対して置くと石がひっくり返るかを確認 */
		for (int j = -1; j < 2; j++) {
			for (int i = -1; i < 2; i++) {

				// 一マス進んでみる
				int SearchX = x + i, SearchY = y + j;

				// 敵の石が置かれていたら探索開始
				if (BoardSub[SearchY][SearchX] == EnemyStoneColor) {
					while (1) {

						// 1マス進める
						SearchX += i;
						SearchY += j;

						// 探索位置が盤面から外に出たら探索終了
						if ((SearchX < 0) || (SearchX >= BOARD_WIDTH) || (SearchY < 0) || (SearchY >= BOARD_HEIGHT))break;

						// 石が置かれていないマスに遭遇しても探索終了
						if (BoardSub[SearchY][SearchX] == NONE)break;

						// 敵の石が置かれていたら次に進む
						if (BoardSub[SearchY][SearchX] == EnemyStoneColor)continue;

						// 自分の石が置かれていたら、(x,y)には石が置ける（TRUEを返す）
						if (BoardSub[SearchY][SearchX] == Turn) return TRUE;
					}
				}

			}
		}

		// 全部探索し終わったら置けない
		return FALSE;
	}

	// 石があったら置けない
	else {
		return FALSE;
	}
}

// 石をひっくり返す
void TurnOverStones(int x, int y,int Turn, int BoardSub[8][8]) {

	// 敵の石の色
	int EnemyStoneColor = (Turn + 1) % STATE_MAX;

	/* 全方向に対して置くと石がひっくり返るかを確認 */
	for (int j = -1; j < 2; j++) {
		for (int i = -1; i < 2; i++) {

			// 一マス進んでみる
			int SearchX = x + i, SearchY = y + j;

			// 敵の石が置かれていたら探索開始
			if (BoardSub[SearchY][SearchX] == EnemyStoneColor) {
				while (1) {

					// 1マス進める
					SearchX += i;
					SearchY += j;

					// 探索位置が盤面から外に出たら探索終了
					if ((SearchX < 0) || (SearchX >= BOARD_WIDTH) || (SearchY < 0) || (SearchY >= BOARD_HEIGHT))break;

					// 石が置かれていないマスに遭遇しても探索終了
					if (BoardSub[SearchY][SearchX] == NONE)break;

					// 敵の石が置かれていたら次に進む
					if (BoardSub[SearchY][SearchX] == EnemyStoneColor)continue;

					// 自分の石が置かれていたら、(x,y)まで戻るように石をひっくり返していく
					if (BoardSub[SearchY][SearchX] == Turn) {

						// 1マス戻る
						SearchX -= i;
						SearchY -= j;

						while (1) {

							// 敵の石をひっくり返す
							BoardSub[SearchY][SearchX] = Turn;

							// 自分の石の個数を１増やす
							StonesNum[Turn]++;

							// 相手の石の個数を１減らす
							StonesNum[EnemyStoneColor]--;

							// １マス戻ってみて自分が置いた位置に戻ったら
							// ひっくり返す作業を終了する
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

// 白が置ける位置を探して、子の座標を配列に反映し、置ける場所の個数を返す
// COM（アルファベータ法）用
int SearchCanPutWhite(int Turn, Vec2 Children[STONE_END_NUM], int BoardSub[8][8]) {

	// 置ける個数
	int CanPutNum = 0;

	// 石が置ける位置を探索する
	for (int height = 0; height < BOARD_HEIGHT; height++) {
		for (int width = 0; width < BOARD_WIDTH; width++) {

			// 置けるなら
			if (IsPuttable(width, height, Turn, BoardSub)) {
				// TRUE を入れる
				BoardCanPut[height][width] = TRUE;
				// 座標(width,height) を入れる
				Children[CanPutNum].Set(width, height);
				// 置ける場所の個数を+1
				CanPutNum++;
			}
			else {
				// 置けないならFALSEを入れる
				BoardCanPut[height][width] = FALSE;
			}
		}
	}

	// 置ける個数を返す
	return CanPutNum;

}

// アルファベータ法を用いて最善手で石を置く（白のターン）
Vec2 AlphaBeta(int Turn,int BoardSub[8][8]) {

	// α値とβ値を初期化
	int Alpha = -INT_MAX;
	int Beta = INT_MAX;

	// 先読みの深さ
	int Depth = 3;

	// 敵のターン
	int EnemyTurn = (Turn + 1) % STATE_MAX;

	// 最適なマスの座標
	Vec2 BestVec;

	// 打つことができるマスの座標
	Vec2 Children[STONE_END_NUM];

	// 打つことができるマスの座標を　配列Children[STONE_END_NUM]  に反映して
	// 打つことができる個数を計算する
	int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

	// 置ける場所をを表示
	DrawGame(Turn);

	// 白が打てるマスがない（パス）の時
	// その時の評価値を返す
	if (CanPutNum == 0) {
		// 何もせずパスを表示する
		BestVec.Set(-1, -1);
	}

	else {
		// 打てるマスを探索
		for (int i = 0; i < CanPutNum; i++) {

			// 次の盤面を用意(白石を置く前)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// 探索中の子の評価値を計算する
			int ChildValue = MinValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// α値よりも探索中の子の評価値の方が大きかったら更新
			if (Alpha < ChildValue) {
				Alpha = ChildValue;
				BestVec = Children[i];
			}

			
			// 評価値を表示してみる
			/*SetFontSize(16);
			TCHAR	pszText[256];
			::_itot_s(ChildValue, pszText, 256 * sizeof(TCHAR), 10);
			DrawString(Children[i].GetX() * CELL_SIZE, Children[i].GetY() * CELL_SIZE + VERTICAL_OFFSET, pszText, GetColor(255, 255, 255));
			*/

		}
	}

	// 裏画面の内容を表画面に映す
	ScreenFlip();

	// 最適な座標を返す
	return BestVec;
}

/* 黒（プレイヤー）のベストな手を探索する関数
 * Vec2 Coord			：現在のノードで置く石の座標
 * int BoardSub[8][8]	：現在の盤面（引数でもらった時はまだ石を置く前の盤面）
 * int Alpha			：α値
 * int Beta				：β値
 * int Depth			：探索の深さ
 */
int MinValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth) {

	// 盤面に石を置いて、ひっくりかえして「現在の盤面」にする
	BoardSub[Coord.GetY()][Coord.GetX()] = Turn;
	TurnOverStones(Coord.GetX(), Coord.GetY(),Turn, BoardSub);

	// 探索の深さが０なら、探索を打ち切って評価値を返す
	if (Depth == 0) {
		return CalcValue(Turn, BoardSub);
	}
	
	// まだ探索できるなら以下の処理をする
	{
		// 敵のターン
		int EnemyTurn = (Turn + 1) % STATE_MAX;

		// 打つことができるマスの座標
		Vec2 Children[STONE_END_NUM];

		// 打つことができるマスの座標を　配列Children[STONE_END_NUM]  に反映して
		// 打つことができる個数を計算する
		int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// 黒が打てるマスがない（パス）の時
		// 選ばれてはいけないので、最大値を返す
		if (CanPutNum == 0) {
			return INT_MAX;
		}


		// 打てるマスを探索する
		for (int i = 0; i < CanPutNum; i++) {

			// 次の盤面を用意(石を置く前)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// 探索中の子の評価値を計算する
			Beta = MaxValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// β値よりもα値のほうが大きかったら、
			// α値を返す（枝刈り）
			if (Alpha >= Beta) return Alpha;

		}
		// β値を返す
		return Beta;
	}
}

/* 白（COM）のベストな手を探索する関数
 * Vec2 Coord			：現在のノードで置く石の座標
 * int BoardSub[8][8]	：現在の盤面（引数でもらった時はまだ石を置く前の盤面）
 * int Alpha			：α値
 * int Beta				：β値
 * int Depth			：探索の深さ
 */
int MaxValue(int Turn, Vec2 Coord, int BoardSub[8][8], int Alpha, int Beta, int Depth) {

	// 盤面に石を置いて、ひっくりかえして「現在の盤面」にする
	BoardSub[Coord.GetY()][Coord.GetX()] = Turn;
	TurnOverStones(Coord.GetX(), Coord.GetY(), Turn, BoardSub);

	// 探索の深さが０なら、探索を打ち切る
	if (Depth == 0) {
		return CalcValue(Turn, BoardSub);
	}

	// まだ探索できるなら以下の処理をする
	{
		// 敵のターン
		int EnemyTurn = (Turn + 1) % STATE_MAX;

		// 打つことができるマスの座標
		Vec2 Children[STONE_END_NUM];

		// 打つことができるマスの座標を　配列Children[STONE_END_NUM]  に反映して
		// 打つことができる個数を計算する
		int CanPutNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// 白が打てるマスがない（パス）の時
		// 選ばれてはいけないので、最小値を返す
		if (CanPutNum == 0) {
			return -INT_MAX;
		}

		// 打てるマスを探索する
		for (int i = 0; i < CanPutNum; i++) {

			// 次の盤面を用意(石を置く前)
			int BoardChild[8][8];
			memcpy(BoardChild, BoardSub, sizeof(BoardSub));

			// 探索中の子の評価値を計算する
			Alpha = MinValue(EnemyTurn, Children[i], BoardChild, Alpha, Beta, Depth - 1);

			// β値よりもα値のほうが大きかったら、
			// β値を返す（枝刈り）
			if (Alpha >= Beta) return Beta;

		}
		// α値を返す
		return Alpha;
	}
}

// 評価値を計算する関数
int CalcValue(int Turn, int BoardSub[8][8]) {

	// 評価値
	int Value;

	// 敵の石の色
	int EnemyStoneColor = (Turn + 1) % STATE_MAX;
	
	float BP = 0; // 盤上の位置の評価値
	float FS = 0; // 確定石の評価値
	float CN = 0; // 候補数の評価値

	int Wbp = 2; // ＢＰ（評価値）の重み
	int Wfs = 5; // ＦＳの重み
	int Wcn = 1; // ＣＮの重み

	// 盤上の位置の評価値 BP を計算する
	{
		for (int height = 0; height < BOARD_HEIGHT; height++) {
			for (int width = 0; width < BOARD_WIDTH; width++) {

				// ０から１の乱数
				float Rand = float(rand()) / RAND_MAX;

				// 評価値を計算して自分の石なら足す、相手の石なら引く
				if (BoardSub[height][width] == Turn) {
					BP += BoardValueLast[height][width] * Rand * 3;
				}
				else if (BoardSub[height][width] == EnemyStoneColor) {
					BP -= BoardValueLast[height][width] * Rand * 3;
				}

			}
		}
	}

	// 確定石の評価値 FS を計算する
	{
		// ０から１の乱数
		float Rand = float(rand()) / RAND_MAX;

		// 自分の確定石の数を調べる
		int TurnStoneNum = CalcFS(Turn, BoardSub);

		// 相手の石の確定石の数を調べる
		int EnemyStoneNum = CalcFS(EnemyStoneColor, BoardSub);

		// 確定石の評価値 FS を計算する
		FS = ((TurnStoneNum - EnemyStoneNum) + Rand * 3) * 11;
	}

	// 候補数の評価値 CN を計算する
	{
		// ０から１の乱数
		float Rand = float(rand()) / RAND_MAX;

		// SearchCanPutWhite関数を使うために用意
		Vec2 Children[STONE_END_NUM];

		// 候補数を得る
		int CandidatesNum = SearchCanPutWhite(Turn, Children, BoardSub);

		// 候補数の評価値 CN を計算する
		CN = (CandidatesNum + Rand * 2) * 10;

	}

	// 評価値を計算する
	Value = int(BP*Wbp + FS*Wfs + CN*Wcn);
	
	// 評価値を返す
	return Value;
}

// 確定石の数を計算する 
int CalcFS(int Turn, int BoardSub[8][8]) {

	// 自分の確定石の数を調べる
	int StoneNum = 0;

	// 上の辺の探索
	{
		// 上の辺の石の数を数える
		int StoneTopNum = 0;
		int StoneTurnNum = 0;
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (BoardSub[0][x] == BLACK || BoardSub[0][x] == WHITE) {
				StoneTopNum++;
				if (BoardSub[0][x] == Turn) StoneTurnNum++;
			}
		}

		// 全部埋まっていたら置いてある石は確定石
		if (StoneTopNum == BOARD_WIDTH) {
			// 確定石を足す
			StoneNum += StoneTurnNum;
		}
		// 空きがあったら端から調べる
		else {

			// 石の数を初期化
			StoneTurnNum = 0;

			// 左端から
			int x = 0;
			int y = 0;
			while (BoardSub[y][x] == Turn && x < BOARD_WIDTH) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x++;
			}

			// 右端から
			x = BOARD_WIDTH - 1;
			while (BoardSub[y][x] == Turn && x >= 0) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x--;
			}

			// 確定石を足す
			StoneNum += StoneTurnNum;

		}
	}

	// 下の辺の探索
	{
		// 下の辺の石の数を数える
		int StoneBottomNum = 0;
		int StoneTurnNum = 0;
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (BoardSub[BOARD_HEIGHT-1][x] == BLACK || BoardSub[BOARD_HEIGHT - 1][x] == WHITE) {
				StoneBottomNum++;
				if (BoardSub[BOARD_HEIGHT - 1][x] == Turn) StoneTurnNum++;
			}
		}

		// 全部埋まっていたら置いてある石は確定石
		if (StoneBottomNum == BOARD_WIDTH) {
			// 確定石を足す
			StoneNum += StoneTurnNum;
		}
		// 空きがあったら端から調べる
		else {

			// 石の数を初期化
			StoneTurnNum = 0;

			// 左端から
			int x = 0;
			int y = BOARD_HEIGHT - 1;
			while (BoardSub[y][x] == Turn && x < BOARD_WIDTH) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x++;
			}

			// 右端から
			x = BOARD_WIDTH - 1;
			while (BoardSub[y][x] == Turn && x >= 0) {
				if (BoardSub[y][x] == Turn) {
					StoneTurnNum++;
				}
				x--;
			}

			// 確定石を足す
			StoneNum += StoneTurnNum;

		}
	}

	// 左の辺の探索
	{
		// 左の辺の石の数を数える
		int StoneLeftNum = 0; // 左端の石の数
		int StoneTurnNum = 0; // 左端のTurnの色の石の数
 		for (int y = 0; y < BOARD_HEIGHT; y++) {


			if (BoardSub[y][0] == BLACK || BoardSub[y][0] == WHITE) {

				// 右の辺の石の総数
				StoneLeftNum++;

				// 一番上と一番下のマスは
				// 上の辺と下の辺の探索の時に足しこんでいるので、スルーして
				// 一番上と一番下のマス以外をTurnの色の石として数を足しこんでいく
				if ((y != 0) && (y != BOARD_HEIGHT - 1) && (BoardSub[y][0] == Turn)) StoneTurnNum++;
			}
		}

		// 全部埋まっていたら置いてある石は確定石
		if (StoneLeftNum == BOARD_HEIGHT) {
			// 確定石を足す
			StoneNum += StoneTurnNum;
		}
		// 空きがあったら端から調べる
		else {

			// 石の数を初期化
			StoneTurnNum = 0;

			// 上端から
			int x = 0;
			int y = 0;

			// 上端のマスがTurnの色なら
			if (BoardSub[y][x] == Turn) {
				// 石の数を数えずに
				// 一マス進む
				y++;

				// Turnの色の石が並んでいたら数え続ける
				while (BoardSub[y][x] == Turn && y < BOARD_HEIGHT) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y++;
				}
			}

			// 下端から
			y = BOARD_HEIGHT - 1;

			// 下端のマスがTurnの色なら
			if (BoardSub[y][x] == Turn) {

				// 石の数を数えずに
				// 一マス進む
				y--;

				// Turnの色の石が並んでいたら数え続ける
				while (BoardSub[y][x] == Turn && y >= 0) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y--;
				}
			}
			// 確定石を足す
			StoneNum += StoneTurnNum;

		}
	}

	// 右の辺の探索
	{
		// 右の辺の石の数を数える
		int StoneRightNum = 0; // 右端の石の数
		int StoneTurnNum = 0;  // 右端のTurnの色の石の数
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (BoardSub[y][BOARD_WIDTH-1] == BLACK || BoardSub[y][BOARD_WIDTH - 1] == WHITE) {

				// 右の辺の石の総数
				StoneRightNum++;

				// 一番上と一番下のマスは
				// 上の辺と下の辺の探索の時に足しこんでいるので、スルーして
				// 一番上と一番下のマス以外をTurnの色の石として数を足しこんでいく
				if ((y != 0) && (y != BOARD_HEIGHT - 1) && (BoardSub[y][BOARD_WIDTH - 1] == Turn)) StoneTurnNum++;
			}
		}

		// 全部埋まっていたら置いてある石は確定石
		if (StoneRightNum == BOARD_HEIGHT) {
			// 確定石を足す
			StoneNum += StoneTurnNum;
		}
		// 空きがあったら端から調べる
		else {

			// 石の数を初期化
			StoneTurnNum = 0;

			// 上端から
			int x = BOARD_WIDTH - 1;
			int y = 0;

			// 上端のマスがTurnの色なら
			if (BoardSub[y][x] == Turn) {
				// 石の数を数えずに
				// 一マス進む
				y++;

				// Turnの色の石が並んでいたら数え続ける
				while (BoardSub[y][x] == Turn && y < BOARD_HEIGHT) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y++;
				}
			}

			// 下端から
			y = BOARD_HEIGHT - 1;

			// 下端のマスがTurnの色なら
			if (BoardSub[y][x] == Turn) {

				// 石の数を数えずに
				// 一マス進む
				y--;

				// Turnの色の石が並んでいたら数え続ける
				while (BoardSub[y][x] == Turn && y >= 0) {
					if (BoardSub[y][x] == Turn) {
						StoneTurnNum++;
					}
					y--;
				}
			}

			// 確定石を足す
			StoneNum += StoneTurnNum;

		}
	}

	return StoneNum;
}
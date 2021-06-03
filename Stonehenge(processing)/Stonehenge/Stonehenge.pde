// program to reneder "stonehenge".
// ストーンヘンジを描画するプログラムです。

// 2021/06/04 更新

// 初期化
void setup() {
  size(600, 600, P3D);    
  camera(25, -25, 25, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

  float fov = radians(45);  
  perspective(fov, float(width)/float(height), 1.0, 600.0);
}

// 描画関数
void draw() {
  // 背景を白に設定
  background(255); 

  // 外側の円状の岩を描画
  renderOutObject();
  
  // 内側の円状の岩を描画
  renderInnerObject();
  
  // 中央の小さな岩を描画
  translate(-1,1);       // 置くべき位置へ移動する
  rotateZ(radians(45));  // z軸回転
  box(1, 1, 2);          // 岩を描画
  
}

// 外側の円状の岩を描画
void renderOutObject(){
  int r = 12;                  // 半径
  float angle = radians(18);   // 角度の変化量
 
  int para = 0;     // 角度の変化に用いるパラメータ
 
  float rotate_Z = 0;    // z軸回転する角度
  float pil_trans_X = 0; // 柱の動く量（x座標）
  float pil_trans_Y = 0; // 柱の動く量（y座標）
  
  float top_trans_X = 0; // 柱に乗っている上の岩の動く量（x座標）
  float top_trans_Y = 0; // 柱に乗っている上の岩の動く量（y座標）
  
  float offset_top = radians(9); // 柱に乗っている上の岩用の角度のオフセット
  
  for (float i = 0; i < 2.0*PI; i += PI/10.0 ) {
    rotate_Z = angle * para;
    pil_trans_X  = r*cos(i);
    pil_trans_Y  =r*sin(i);
    
    top_trans_X  = r*cos(i+offset_top);
    top_trans_Y  = r*sin(i+offset_top);

    // 柱を描画する（最初の位置は原点）
    translate(pil_trans_X, pil_trans_Y);    // 置くべき位置へ移動する
    rotateZ(rotate_Z);                      // z軸回転
    box(1, 1, 3);                           // 柱を描画
    rotateZ(-rotate_Z);                     // z軸回転（元の向きへ戻る）
    translate(-pil_trans_X, -pil_trans_Y);  // 置くべき位置へ移動する（元の位置へ戻る）
    
    
    // 柱に乗っている上の岩を描画する
    translate(top_trans_X, top_trans_Y, 2);    // 置くべき位置へ移動する
    rotateZ(rotate_Z+offset_top);              // z軸回転
    rotateX(radians(90));                      // x軸回転
    box(1, 1, 3.5);                            // 岩を描画
    rotateX(-radians(90));                     // x軸回転（元の向きへ戻る）
    rotateZ(-rotate_Z-offset_top);             // z軸回転（元の向きへ戻る）
    translate(-top_trans_X, -top_trans_Y, -2); // 置くべき位置へ移動する（元の位置へ戻る）
    
    para++;
  }
}

// 内側の円状の岩を描画
void renderInnerObject(){
  int r = 6;                   // 半径
  float angle = radians(30);   // 角度の変化量
 
  int para = 0;     // 角度の変化に用いるパラメータ
 
  float rotate_Z = 0;    // z軸回転する角度
  float pil_trans_X = 0; // 柱の動く量（x座標）
  float pil_trans_Y = 0; // 柱の動く量（y座標）
  
  float top_trans_X = 0; // 柱に乗っている上の岩の動く量（x座標）
  float top_trans_Y = 0; // 柱に乗っている上の岩の動く量（y座標）
  
  float offset_top = radians(15); // 柱に乗っている上の岩用の角度のオフセット
  
  for (float i = 0; i < (3.0/2.0)*PI; i += PI/6.0 ) {
    
      rotate_Z = angle * para;
      pil_trans_X  = r*cos(i);
      pil_trans_Y  = r*sin(i);
      
      top_trans_X  = r*cos(i+offset_top);
      top_trans_Y  = r*sin(i+offset_top);
  
      // 柱を描画する（最初の位置は原点）
      translate(pil_trans_X, pil_trans_Y);    // 置くべき位置へ移動する
      rotateZ(rotate_Z);                      // z軸回転
      box(1, 1, 3);                           // 柱を描画
      rotateZ(-rotate_Z);                     // z軸回転（元の向きへ戻る）
      translate(-pil_trans_X, -pil_trans_Y);  // 置くべき位置へ移動する（元の位置へ戻る）
      
      // 一つ置きに描画する
      if(para % 2 == 0){
        // 柱に乗っている上の岩を描画する
        translate(top_trans_X, top_trans_Y, 2);    // 置くべき位置へ移動する
        rotateZ(rotate_Z+offset_top);              // z軸回転
        rotateX(radians(90));                      // x軸回転
        box(1, 1, 3);                              // 岩を描画
        rotateX(-radians(90));                     // x軸回転（元の向きへ戻る）
        rotateZ(-rotate_Z-offset_top);             // z軸回転（元の向きへ戻る）
        translate(-top_trans_X, -top_trans_Y, -2); // 置くべき位置へ移動する（元の位置へ戻る）
      }
    para++;
  }
  
}

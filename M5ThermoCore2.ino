#include <M5Core2.h>    // CORE2使用の場合有効
// #include <M5Stack.h>       // M5Stack GRAY使用の場合有効
// #include <M5StickCPlus.h>  // M5StickC Plus使用の場合有効

#define LGFX_AUTODETECT // 自動認識(D-duino-32 XS, PyBadgeはパネルID読取れないため自動認識の対象から外れているそうです)
#define LGFX_USE_V1     // v1.0.0を有効に(v0からの移行期間の特別措置とのこと。書かない場合は旧v0系で動作)
#include <LovyanGFX.hpp>          // lovyanGFXのヘッダを準備
#include <LGFX_AUTODETECT.hpp>    // クラス"LGFX"を準備
static LGFX lcd;                  // LGFXのインスタンスを作成（クラスLGFXを使ってlcdコマンドでいろいろできるようにする）
static LGFX_Sprite canvas(&lcd);  // スプライトを使う場合はLGFX_Spriteのインスタンスを作成

// 変数設定
int cnt;        // 本体ボタンON回数格納用
int x = 0;      // x軸座標格納用
uint16_t color; // 登録色格納用
// 初期設定 -----------------------------------------
void setup() {
  M5.begin();   // 本体初期化
  // LCD初期設定
  lcd.init();                 // LCD初期化
  lcd.setRotation(2);         // 画面向き設定（0～3で設定、4～7は反転)　※CORE2、GRAYの場合
  //lcd.setRotation(1);         // 画面向き設定（0～3で設定、4～7は反転)　※M5StickC Plusの場合
  canvas.setColorDepth(8);    // カラーモード設定（書かなければ初期値16bit。24bit（パネル性能によっては18bit）は対応していれば選択可）
                              // CORE2 GRAY のスプライトは16bit以上で表示されないため8bitに設定
  canvas.setTextWrap(false);  // 改行をしない（画面をはみ出す時自動改行する場合はtrue）
  canvas.setTextSize(1);      // 文字サイズ（倍率）
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）

  canvas.fillScreen(BLACK);         // 背景塗り潰し
  canvas.setTextColor(WHITE);       // 文字色と背景を指定（文字色, 背景（省略可））

  // 四角
  canvas.drawRect(40, 20, 180, 280, WHITE);  // 四角（始点x,始点y,縦長さ,横長さ）

  canvas.pushSprite(0, 0);  // メモリ内に描画したcanvasを座標を指定して表示する
}
// メイン -----------------------------------------
void loop() {
  M5.update();                      // 本体ボタン状態更新

  delay(100); // 遅延時間（ms）
}
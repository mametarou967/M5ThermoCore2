#include <M5Core2.h>    // CORE2使用の場合有効
// #include <M5Stack.h>       // M5Stack GRAY使用の場合有効
// #include <M5StickCPlus.h>  // M5StickC Plus使用の場合有効
// lovyan
#define LGFX_AUTODETECT // 自動認識(D-duino-32 XS, PyBadgeはパネルID読取れないため自動認識の対象から外れているそうです)
#define LGFX_USE_V1     // v1.0.0を有効に(v0からの移行期間の特別措置とのこと。書かない場合は旧v0系で動作)
#include <LovyanGFX.hpp>          // lovyanGFXのヘッダを準備
#include <LGFX_AUTODETECT.hpp>    // クラス"LGFX"を準備
// SHT3x
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

// lovyan
static LGFX lcd;                  // LGFXのインスタンスを作成（クラスLGFXを使ってlcdコマンドでいろいろできるようにする）
static LGFX_Sprite canvas(&lcd);  // スプライトを使う場合はLGFX_Spriteのインスタンスを作成
#define TEMP_DISP_MAX 40
#define TEMP_DISP_MIN 0

// SHT3x
SHT3X sht30;
Adafruit_BMP280 bme;
float tmp = 0.0;
#define DISP_HOUR_MIN 7
#define DISP_HOUR_MAX 19
#define DISP_HOUR_RES 12
#define HOURS_MAX 24
#define INVALID_FNUM 0.0

float tmpLog[HOURS_MAX][DISP_HOUR_RES] = 
  {
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { 0.0 , 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 , 8.0 , 9.0 , 10.0 , 10.0 } ,
  { 10.0 ,  10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 , 10.0 } ,
  { 10.0 , 12.0 , 14.0 , 16.0 , 18.0 , 20.0 , 22.0 , 24.0 , 26.0 , 28.0 , 30.0 , 30.0 } ,
  { 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 , 30.0 } ,
  { 31.0 , 32.0 , 33.0 , 34.0 , 35.0 , 36.0 , 37.0 , 38.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 , 39.0 } ,
  { 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  { INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM , INVALID_FNUM } ,
  };

void updateView()
{
  canvas.fillScreen(BLACK);         // 背景塗り潰し
  canvas.setTextColor(WHITE);       // 文字色と背景を指定（文字色, 背景（省略可））

  // 四角
  canvas.drawRect(24, 40, 292, 160, WHITE);  // 四角（始点x,始点y,縦長さ,横長さ）

  // 補助線:縦
  canvas.drawFastHLine(24,  80 + 1, 292 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastHLine(24, 120 + 1, 292 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastHLine(24, 160 + 1, 292 - 2, lcd.color332(128, 128, 128));
  // 凡例:縦
  int temp_disp_baseH = (TEMP_DISP_MAX + TEMP_DISP_MIN) / 4;
  canvas.setFont(&fonts::lgfxJapanGothic_16);     // ゴシック体（8,12,16,20,24,28,32,36,40）
  canvas.setCursor(4,32);
  canvas.printf("%2d\n",temp_disp_baseH * 4);
  canvas.setCursor(4,72);
  canvas.printf("%2d\n",temp_disp_baseH * 3);
  canvas.setCursor(4,112);
  canvas.printf("%2d\n",temp_disp_baseH * 2);
  canvas.setCursor(4,152);
  canvas.printf("%2d\n",temp_disp_baseH * 1);
  canvas.setCursor(4,192);
  canvas.printf("%2d\n",TEMP_DISP_MIN);

  // 補助線:横
  int temp_disp_baseV = 24;
  canvas.drawFastVLine(24 + (temp_disp_baseV *  1) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastVLine(24 + (temp_disp_baseV *  3) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastVLine(24 + (temp_disp_baseV *  5) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastVLine(24 + (temp_disp_baseV *  7) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastVLine(24 + (temp_disp_baseV *  9) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  canvas.drawFastVLine(24 + (temp_disp_baseV * 11) + 1, 40, 160 - 2, lcd.color332(128, 128, 128));
  // 凡例:横
  canvas.setFont(&fonts::lgfxJapanGothic_16);     // ゴシック体（8,12,16,20,24,28,32,36,40）
  canvas.setCursor(24 + (temp_disp_baseV *  1) - 4 ,200 + 2);
  canvas.println("8");            // 表示内容をcanvasに準備
  canvas.setCursor(24 + (temp_disp_baseV *  3) - 8 ,200 + 2);
  canvas.println("10");            // 表示内容をcanvasに準備
  canvas.setCursor(24 + (temp_disp_baseV *  5) - 8 ,200 + 2);
  canvas.println("12");            // 表示内容をcanvasに準備
  canvas.setCursor(24 + (temp_disp_baseV *  7) - 8 ,200 + 2);
  canvas.println("14");            // 表示内容をcanvasに準備
  canvas.setCursor(24 + (temp_disp_baseV *  9) - 8 ,200 + 2);
  canvas.println("16");            // 表示内容をcanvasに準備
  canvas.setCursor(24 + (temp_disp_baseV * 11) - 8 ,200 + 2);
  canvas.println("18");            // 表示内容をcanvasに準備

  // 具縦粋な値
  RTC_TimeTypeDef t;
  M5.Rtc.GetTime(&t);
  canvas.setFont(&fonts::lgfxJapanGothic_24);     // ゴシック体（8,12,16,20,24,28,32,36,40）
  canvas.setCursor(30,8);
  canvas.print("時刻 ");
  canvas.printf("%2d",t.Hours);
  canvas.print(":");
  canvas.printf("%02d",t.Minutes); 
  canvas.setCursor(180,8);
  canvas.print("気温 ");
  canvas.printf("%2.1f",tmp); 

  // グラフの描画
  Serial.println("graph start---------------");
  for(int hour = DISP_HOUR_MIN;hour <= DISP_HOUR_MAX-1;hour++)
  {
    for(int res = 0;res < DISP_HOUR_RES;res++)
    {
      // 無効値が入っている場合は無視
      // if(tmpLog[hour - 1][res] == INVALID_FNUM)
      // {
      //   continue;
      // }

      int baseX = ((hour - DISP_HOUR_MIN) * 24) + 24 + (res * 2);
      int baseY = 40 + 160 - (int)((tmpLog[hour - 1][res] * 4));
      int dstX =  ((hour - DISP_HOUR_MIN) * 24) + 24 + ((res + 1) * 2);
      int dstY = 0;
      Serial.printf("hour=%d res=%d baseX=%d baseY=%d dstX=%d ",hour,res,baseX,baseY,dstX);
      if(res == DISP_HOUR_RES - 1)
      {
        // if(tmpLog[hour][0] == INVALID_FNUM)
        // {
        //   continue;
        // }
        dstY =  40 + 160 - (int)((tmpLog[hour][0] * 4));
        Serial.printf("dstY=%d tmpLog[%d][%d]=%lf tmpLog[%d][0] = %lf",dstY,hour-1,res,tmpLog[hour - 1][res],hour,tmpLog[hour]);
      }
      else
      {
        // if(tmpLog[hour - 1][res + 1] == INVALID_FNUM)
        // {
        //   continue;
        // }
        dstY =  40 + 160 - (int)((tmpLog[hour - 1][res + 1] * 4));
        Serial.printf("dstY=%d tmpLog[%d][%d]=%lf tmpLog[%d][%d] = %lf",dstY,hour-1,res,tmpLog[hour - 1][res],hour-1,res+1,tmpLog[hour - 1][res + 1]);
      }
      Serial.println("");
        
      canvas.drawLine(baseX, baseY, dstX, dstY, GREEN);
    }
  }

  canvas.pushSprite(0, 0);  // メモリ内に描画したcanvasを座標を指定して表示する
}


// 初期設定 -----------------------------------------
void setup() {
  M5.begin();   // 本体初期化
  Serial.begin(115200);  // シリアル通信の初期化（ボーレートは115200）
  M5.Rtc.begin();
  // LCD初期設定
  lcd.init();                 // LCD初期化
  lcd.setRotation(1);         // 画面向き設定（0～3で設定、4～7は反転)　※CORE2、GRAYの場合
  canvas.setColorDepth(8);    // カラーモード設定（書かなければ初期値16bit。24bit（パネル性能によっては18bit）は対応していれば選択可）
                              // CORE2 GRAY のスプライトは16bit以上で表示されないため8bitに設定
  canvas.setTextWrap(false);  // 改行をしない（画面をはみ出す時自動改行する場合はtrue）
  canvas.setTextSize(1);      // 文字サイズ（倍率）
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）

  Wire.begin();
  
  while (!bme.begin(0x76)){
    Serial.println("Could not find a 0x76");
  }

  // ログの値の初期化
  // for(int row = 0;row < HOURS_MAX;row++)
  // {
  //   for(int col = 0;col < DISP_HOUR_RES;col++)
  //   {
  //       tmpLog[row][col] = INVALID_FNUM;
  //   }
  // }

  // 時計を強制的に初期化 ※将来対応
  RTC_TimeTypeDef t;
  t.Hours = 7;
  t.Minutes = 0;
  M5.Rtc.SetTime(&t);

  // 初期画面表示
  updateView();
}
// メイン -----------------------------------------
void loop() {
  M5.update();                      // 本体ボタン状態更新
  
  if(sht30.get()==0){
    // 現在の温度の更新
    tmp = sht30.cTemp;
    
    // グラフ用に記録した値を保持
    // RTC_TimeTypeDef t;
    // M5.Rtc.GetTime(&t);
    // indexの導出
    // tmpLog[t.Hours-1][t.Minutes/DISP_HOUR_RES] = tmp;
  }

  updateView();

  delay(1000); // 遅延時間（ms）
}
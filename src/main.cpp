#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int Xinput = 34;
const int Yinput = 35;
const int Zinput = 26;
const int rotate_input = 25;
const int button_input = 27;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


bool mode = false;      // 現在のモード (T or F)
int lastState = HIGH;   // 前回のボタン状態

void setup() {
//-------------------------------joystickの設定------------------------------
  Serial.begin(9600);

  analogReadResolution(12); // 0..4095
  analogSetPinAttenuation(Xinput, ADC_11db);//入力電圧の設定(0V 〜 約 3.9V)
  analogSetPinAttenuation(Yinput, ADC_11db);
  pinMode(button_input, INPUT_PULLUP);

//----------------------------------OLEDの設定-------------------------------
  Wire.begin(21,22);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for(;;); // 初期化失敗時はループ
  }

  display.clearDisplay();      // 画面バッファをクリア
  display.setTextSize(1);      // 文字サイズ (1: 標準)
  display.setTextColor(SSD1306_WHITE); // 文字色
  
  display.setCursor(0, 10);    // (x, y) 座標を指定
  display.println("ESP32 OLED Test");
  
  display.setTextSize(2);      // 少し大きく
  display.setCursor(0, 30);
  display.println("Success!");

  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
}

int separate(int value) {
  if (value < 2000) return 0;
  if (value > 3500) return 4095;
  return value;
}

void drawArrow(int x, int y, int direction) {
  if (direction == 0) { // 上 ↑
    display.fillTriangle(x, y, x-5, y+8, x+5, y+8, SSD1306_WHITE); // 三角形
    display.drawFastVLine(x, y+8, 10, SSD1306_WHITE);              // 棒
  } else if (direction == 1) { // 下 ↓
    display.fillTriangle(x, y+18, x-5, y+10, x+5, y+10, SSD1306_WHITE);
    display.drawFastVLine(x, y, 10, SSD1306_WHITE);
  } else if (direction == 2) { // 右 →
    display.fillTriangle(x+18, y+9, x+10, y+4, x+10, y+14, SSD1306_WHITE);
    display.drawFastHLine(x, y+9, 10, SSD1306_WHITE);
  } else if (direction == 3) { // 左 ←
    display.fillTriangle(x, y+9, x+8, y+4, x+8, y+14, SSD1306_WHITE);
    display.drawFastHLine(x+8, y+9, 10, SSD1306_WHITE);
  }
}

void loop() {
  // 1. 入力の読み取り
  int xr = separate(analogRead(Xinput));
  int yr = separate(analogRead(Yinput));
  int zr = separate(analogRead(Zinput));
  int Rr = separate(analogRead(rotate_input));
  
  int currentState = digitalRead(button_input);
  if (lastState == HIGH && currentState == LOW) mode = !mode;
  lastState = currentState;

  // 2. 描画開始
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // --- 左上: モード表示 ---
  display.setCursor(0, 0);
  display.print("MODE:"); display.print(mode ? "ON" : "OFF");

  // --- 中央: 前後左右の矢印 (X/Y入力) ---
  // 前後 (Y)
  if (yr == 0) drawArrow(64, 5, 0);  // 前
  else if (yr == 4095) drawArrow(64, 30, 1); // 後

  // 左右スライド (X) ※もしスライド操作も出すなら
  if (xr == 0) drawArrow(85, 20, 2); // 右
  else if (xr == 4095) drawArrow(43, 20, 3);  // 左

  // --- 右端: 上下(Z) 垂直バー ---
  display.drawRect(118, 5, 8, 40, SSD1306_WHITE); // 外枠
  if (zr == 0) {
    display.fillRect(119, 6, 6, 18, SSD1306_WHITE); // 上半分
    display.setCursor(110, 48); display.print("UP");
  } else if (zr == 4095) {
    display.fillRect(119, 26, 6, 18, SSD1306_WHITE); // 下半分
    display.setCursor(110, 48); display.print("DWN");
  }

  // --- 下端: 左右回転(R) 水平バー ---
  // 座標 (x=30, y=55) から横幅68ピクセルのバー
  display.drawRect(30, 55, 68, 8, SSD1306_WHITE); // 外枠
  display.setCursor(5, 55); display.print("ROT"); 
  
  if (Rr == 0) {
    // 右回転：枠の右半分を塗りつぶす
    display.fillRect(64, 56, 33, 6, SSD1306_WHITE);
    display.setCursor(102, 55); display.print("R");
  } else if (Rr == 4095) {
    // 左回転：枠の左半分を塗りつぶす
    display.fillRect(31, 56, 33, 6, SSD1306_WHITE);
    display.setCursor(102, 55); display.print("L");
  }

  // 3. 画面更新
  display.display();
  delay(100);
}

/*最大値と最小値の実測用コード

const int Xinput = 32;  
const int Yinput = 33;


int minX = 0, maxX = 4095;
int minY = 0, maxY = 4095;
int minZ = 0, maxZ = 4095;

void setup() {
  Serial.begin(9600);
  analogReadResolution(12); // 0..4095
  analogSetPinAttenuation(Xinput, ADC_11db);
  analogSetPinAttenuation(Yinput, ADC_11db);
}

void loop() {
  int xr = analogRead(Xinput);
  int yr = analogRead(Yinput);

  if (xr < minX) minX = xr;
  if (xr > maxX) maxX = xr;
  if (yr < minY) minY = yr;
  if (yr > maxY) maxY = yr;

  Serial.print("X: "); Serial.print(xr);
  Serial.print(" (min="); Serial.print(minX);
  Serial.print(", max="); Serial.print(maxX);
  Serial.print(") | Y: "); Serial.print(yr);
  Serial.print(" (min="); Serial.print(minY);
  Serial.print(", max="); Serial.print(maxY);
  Serial.println(")");

  delay(100);
}*/
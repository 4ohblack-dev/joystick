自作で2軸のコントローラを制作した際の、ソフトウェアファイル、フォルダになります。

液晶モジュールを用い、コントローラが指示する方向を視覚化しています
```cpp
//矢印の描画関数
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

//上下、回転の指示は垂直バー、水平バーの塗りつぶしで表現

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
```

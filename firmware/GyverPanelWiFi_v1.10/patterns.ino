#include "patterns.h"

int8_t patternIdx = -1;
int8_t lineIdx = 0;
boolean isWhite = false;

// Заполнение матрицы указанным паттерном
// ptrn - индекс узора в массив узоров patterns[] в patterns.h
// X   - позиция X вывода узора
// Y   - позиция Y вывода узора
// W   - ширина паттерна
// H   - высота паттерна
void drawPattern(uint8_t ptrn, uint8_t X, uint8_t Y, uint8_t W, uint8_t H) {

  byte y_offs = beatsin8(5, 1, 32); // for X and Y texture move     <-- Эти две строчки принадлежат перу господина Stepko (C)
  byte x_offs = beatsin8(6, 1, 32); // for X and Y texture move     <--
    
  byte effectBrightness = getBrightnessCalculated(globalBrightness, effectContrast[thisMode]);

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      uint8_t in = (uint8_t) pgm_read_byte( & (patterns[ptrn][(y_offs + y) % H][(x_offs + x) % W]));   // <-- Эта строчка использует переменные, объявленные выше, которые принадлежат перу господина Stepko (C)
      CHSV color = colorMR[in];
      CHSV color2 = color.v != 0 ? CHSV(color.h, color.s, effectBrightness) : color;
      drawPixelXY(x, y, color2); 
    }
  }
}

// Отрисовка указанной картинки с размерами WxH в позиции XY
void drawPicture_XY(uint8_t iconIdx, uint8_t X, uint8_t Y, uint8_t W, uint8_t H) {
  if (loadingFlag) {
    loadingFlag = false;
  }

  byte effectBrightness = getBrightnessCalculated(globalBrightness, effectContrast[thisMode]);

  for (byte x = 0; x < W; x++) {
    for (byte y = 0; y < H; y++) {
      uint8_t in = (uint8_t)pgm_read_byte(&(patterns[iconIdx][y][x])); 
      if (in != 0) {
        CHSV color = colorMR[in];        
        CHSV color2 = color.v != 0 ? CHSV(color.h, color.s, effectBrightness) : color;
        drawPixelXY(X+x,Y+H-y, color2); 
      }
    }
  }
}

void patternRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    patternIdx = (specialTextEffectParam >= 0) ? (specialTextEffectParam - 1) : (effectScaleParam2[MC_PATTERNS] - 1);
    if (patternIdx < 0 || patternIdx > MAX_PATTERN) {
      patternIdx = random8(0,MAX_PATTERN);
    }
    lineIdx = 9;         // Картинка спускается сверху вниз - отрисовка с нижней строки паттерна (паттерн 10x10) dir='d'
 // lineIdx = 0;         // Картинка поднимается сверху вниз - отрисовка с верхней строки паттерна dir='u'

    isWhite = false;
    hue = random8();
    colorMR[6] = CHSV(hue, 255, 255);
    colorMR[7] = CHSV(hue + 80, 255, 255);
    if (random8() % 10 == 0) {
      colorMR[6] = CHSV(0,0,255);
      isWhite = true;
    }
  }  

  hue++;
  if (!isWhite) colorMR[6] = CHSV(hue, 255, 255);
  colorMR[7] = CHSV(hue + 80, 255, 255);
  colorMR[8] = CHSV(hue + 160, 255, 255);

  drawPattern(patternIdx, 0, 0, 10, 10);  
}

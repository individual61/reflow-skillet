#include <Adafruit_GFX.h>
#include <Adafruit_MAX31856.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

///////////////////
// Pro Trinket SPI Connections (common to both devices)

// MOSI/SDI 11
// MISO/SDO 12
// SCK 13

/////////////////// thermocouple (CS = 10)
// Use software SPI: CS, DI, DO, CLK
#define MAX31856_CS 10
#define MAX31856_MOSI 11
#define MAX31856_MISO 12
#define MAX31856_CLK 13

Adafruit_MAX31856 max =
    Adafruit_MAX31856(MAX31856_CS, MAX31856_MOSI, MAX31856_MISO, MAX31856_CLK);
// use hardware SPI, just pass in the CS pin
// Adafruit_MAX31856 max = Adafruit_MAX31856(10);

/////////////////// OLED
// If using software SPI (the default case):
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 8
#define OLED_CS 6
#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Start OLED preamble
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16
static const unsigned char PROGMEM logo16_glcd_bmp[] = {
    B00000000, B11000000, B00000001, B11000000, B00000001, B11000000, B00000011,
    B11100000, B11110011, B11100000, B11111110, B11111000, B01111110, B11111111,
    B00110011, B10011111, B00011111, B11111100, B00001101, B01110000, B00011011,
    B10100000, B00111111, B11100000, B00111111, B11110000, B01111100, B11110000,
    B01110000, B01110000, B00000000, B00110000};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
// end OLED preamble

// OLED function definitions
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n')
      continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2,
                     color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i,
                         WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i,
                         WHITE);
    if (color == WHITE)
      color = BLACK;
    else
      color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i,
                          display.height() - 2 * i, display.height() / 4,
                          WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i,
                          display.height() - 2 * i, display.height() / 4,
                          color);
    if (color == WHITE)
      color = BLACK;
    else
      color = WHITE;
    display.display();
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                     WHITE);
    display.display();
  }
}

void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

// end OLED function definitions

void setup() {
  Serial.begin(115200);
  Serial.println("MAX31856 thermocouple test");

  max.begin();

  max.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple type: ");
  switch (max.getThermocoupleType()) {
  case MAX31856_TCTYPE_B:
    Serial.println("B Type");
    break;
  case MAX31856_TCTYPE_E:
    Serial.println("E Type");
    break;
  case MAX31856_TCTYPE_J:
    Serial.println("J Type");
    break;
  case MAX31856_TCTYPE_K:
    Serial.println("K Type");
    break;
  case MAX31856_TCTYPE_N:
    Serial.println("N Type");
    break;
  case MAX31856_TCTYPE_R:
    Serial.println("R Type");
    break;
  case MAX31856_TCTYPE_S:
    Serial.println("S Type");
    break;
  case MAX31856_TCTYPE_T:
    Serial.println("T Type");
    break;
  case MAX31856_VMODE_G8:
    Serial.println("Voltage x8 Gain mode");
    break;
  case MAX31856_VMODE_G32:
    Serial.println("Voltage x8 Gain mode");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // OLED stuff

  // by default, we'll generate the high voltage from the 3.3v line internally!
  // (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw many lines
  testdrawline();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw a white circle, 10 pixel radius
  display.fillCircle(display.width() / 2, display.height() / 2, 10, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  testdrawroundrect();
  delay(2000);
  display.clearDisplay();

  testfillroundrect();
  delay(2000);
  display.clearDisplay();

  testdrawtriangle();
  delay(2000);
  display.clearDisplay();

  testfilltriangle();
  delay(2000);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw scrolling text
  testscrolltext();
  delay(2000);
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x");
  display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
  display.clearDisplay();

  // miniature bitmap display
  display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  display.clearDisplay();

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void loop() {
  Serial.print("Cold: ");
  Serial.print(max.readCJTemperature());
  Serial.print("\tTC: ");
  Serial.println(max.readThermocoupleTemperature());
  // Check and print any faults
  uint8_t fault = max.readFault();
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE)
      Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE)
      Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)
      Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)
      Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)
      Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)
      Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)
      Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)
      Serial.println("Thermocouple Open Fault");
  }
  delay(1000);
}

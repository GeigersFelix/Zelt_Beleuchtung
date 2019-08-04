#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 24
#define PIN 6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int raum[2][12] = {
  {0, 1, 2, 3, 4, 5, 18, 19, 20, 21, 22, 23},
  {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}
};

int regenbogenindex[24]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
};

int rainbowcolors[3][40]={
  {0, 128, 130, 141, 153, 153, 128, 101, 72, 4, 0, 1, 1, 11, 23, 0, 0, 128, 200, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 40, 83, 102, 102, 0, 0, 0, 0, 68, 114, 159, 175, 179, 212, 255, 255, 255, 255, 219, 182, 146, 109, 73, 0, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 40, 59, 117, 169, 128, 155, 225, 208, 220, 226, 232, 162, 77, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 180, 255, 185, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//http://wiki.baw.de/de/index.php/Farbverlauf:_Regenbogen,_29_Farben

int raumleds[2]={
  12, 12};

char gelesen;
char ani_gr='A';
int runvar[3]={
  0, 0, 0};

int animod=0;
int aniregenfarb=0;

int tastermod=2;
int tasterani=3;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  pinMode(tastermod, INPUT);
  pinMode(tasterani, INPUT);
}

void loop()
{
  animod++;
  if(animod>3)
  {
    animod=1;
  }

  if(digitalRead(tasterani)==HIGH)
  {
    ani_gr++;
    if(ani_gr>'I')
    {
      ani_gr='A';
    }
    while(digitalRead(tasterani)==HIGH)
    {}
  }
  
  if(Serial.available()>0)
  {
    gelesen=Serial.read();
    Serial.println(gelesen);
    ani_gr=gelesen;
  }
   
  switch(ani_gr)
  {
    case 'A':
      setzen(0, 0, 0);
      break;
    case 'B':
      setzen(255, 0, 0);
      break;
    case 'C':
      setzen(0, 255, 0);
      break;
    case 'D':
      setzen(0, 0, 255);
      break;
    case 'E':
      setzen(255, 255, 255);
      break;
    case 'F':
      rainbow(20);
      break;
    case 'G':
      
    case 'H':
      rgbrun(animod);
      break;
    case 'I':
      runpix(255, 255, 0, 2);
      break;
    case 'J':
      theaterChase(pixels.Color(255, 0, 0), 50);
      break;
  }
        
  Serial.print("ani_gr: ");
  Serial.println(ani_gr);
  Serial.print("animod: ");
  Serial.println(animod);
  Serial.print("gelesen: ");
  Serial.println(gelesen);
  pixels.show();
}

void regenkreisel()
{
  for(int i=0; i<24; i++)
  {
    pixels.setPixelColor(i, pixels.Color(rainbowcolors[0][regenbogenindex[i]], rainbowcolors[1][regenbogenindex[i]], rainbowcolors[2][regenbogenindex[i]]));
    regenbogenindex[i]++;
    if(regenbogenindex[i]>=30)
    {
      regenbogenindex[i]=0;
    }
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChase2(uint32_t c, uint8_t wait, int mod) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < raumleds[mod]; i=i+3) {
        pixels.setPixelColor(raum[mod][i+q], c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < raumleds[mod]; i=i+3) {
        pixels.setPixelColor(raum[mod][i+q], 0);        //turn every third pixel off
      }
    }
  }
}

void runpix(int rot, int gruen, int blau, int mod)
{
  int alt=runvar[mod];
  runvar[mod]++;
  switch(mod)
  {
    case 0:
    case 1:
      if(runvar[mod]>raumleds[mod])
      {
        runvar[mod]-=raumleds[mod];
      }
      break;
    case 2:
      if(runvar[mod]>NUM_LEDS)
      {
        runvar[mod]-=NUM_LEDS;
      }
      break;
  }
  pixels.setPixelColor(alt, pixels.Color(0, 0, 0));
  pixels.setPixelColor(runvar[mod], pixels.Color(rot, gruen, blau));
  pixels.show();
}

void rgbrun(int farbe)
{
  Serial.println("rgbrun");
  
  for(int i=0; i<NUM_LEDS; i++)
  {
    switch(farbe)
    {
      case 1:
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        break;
      case 2:
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        break;
      case 3:
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        break;
    }
    pixels.show();
    delay(50);
  }
}

void setzen(int rot, int gruen, int blau)
{
  for(int i=0; i<NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(rot, gruen, blau));
  }
  pixels.show();
}

void rainbow(uint8_t wait) {
  Serial.println("rainbow");
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbow2(uint8_t wait, int mod) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<raumleds[mod]; i++) {
      pixels.setPixelColor(raum[mod][i], Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

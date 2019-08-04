#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 24
#define PIN 6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int raum[2][12] = {
  {0, 1, 2, 3, 4, 5, 18, 19, 20, 21, 22, 23},
  {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}
};

int raumleds[2]={
  12, 12};

char gelesen;
char ani_kl='a';
char ani_gr='A';
int mod=1;
int runvar[3]={
  0, 0, 0};

int animod=0;

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

  if(digitalRead(tastermod)==HIGH)
  {
    mod++;
    if(mod>2)
    {
      mod=1;
    }
    else
    {
      ani_kl='a';
    }
    while(digitalRead(tastermod)==HIGH)
    {}
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
  }
  
  switch(gelesen)
  {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
      ani_kl=gelesen;
      break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      Serial.println("case F");
    case 'G':
    case 'H':
    case 'I':
      ani_gr=gelesen;
      break;
    case '1':
      mod=1;
      break;
    case '2':
      mod=2;
      break;
  }

  switch(mod)
  {
    case 1:
      switch(ani_gr)
      {
        case 'A':
          setzen(0, 0, 0, 2);
          break;
        case 'B':
          setzen(255, 0, 0, 2);
          break;
        case 'C':
          setzen(0, 255, 0, 2);
          break;
        case 'D':
          setzen(0, 0, 255, 2);
          break;
        case 'E':
          setzen(255, 255, 255, 2);
          break;
        case 'F':
          rainbow(20);
          Serial.println("case F mod groß");
          break;
        case 'G':
          rgbrun(animod, 2);
          break;
        case 'H':
          runpix(255, 255, 0, 2);
          break;
        case 'I':
          theaterChase(pixels.Color(255, 0, 0), 50);
          break;
      }
      break;
    case 2:
      switch(ani_gr)
      {
        case 'A':
          setzen(0, 0, 0, 0);
          break;
        case 'B':
          setzen(255, 0, 0, 0);
          break;
        case 'C':
          setzen(0, 255, 0, 0);
          break;
        case 'D':
          setzen(0, 0, 255, 0);
          break;
        case 'E':
          setzen(255, 255, 255, 0);
          break;
        case 'F':
          rainbow2(20, 0);
          break;
        case 'G':
          rgbrun(animod, 0);
          break;
        case 'H':
          runpix(255, 255, 0, 0);
          break;
        case 'I':
          theaterChase2(pixels.Color(255, 0, 0), 50, 0);
          break;
      }
      switch(ani_kl)
      {
        case 'a':
          setzen(0, 0, 0, 1);
          break;
        case 'b':
          setzen(255, 0, 0, 1);
          break;
        case 'c':
          setzen(0, 255, 0, 1);
          break;
        case 'd':
          setzen(0, 0, 255, 1);
          break;
        case 'e':
          setzen(255, 255, 255, 1);
          break;
        case 'f':
          rainbow2(20, 1);
          break;
        case 'g':
          rgbrun(animod, 1);
          break;
        case 'h':
          runpix(255, 255, 0, 1);
          break;
        case 'i':
          theaterChase2(pixels.Color(255, 0, 0), 50, 1);
          break;
      }
      break;
  }
  Serial.print("ani_gr: ");
  Serial.println(ani_gr);
  Serial.print("ani_kl: ");
  Serial.println(ani_kl);
  Serial.print("mod: ");
  Serial.println(mod);
  Serial.print("animod: ");
  Serial.println(animod);
  Serial.print("gelesen: ");
  Serial.println(gelesen);
  pixels.show();
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

void rgbrun(int farbe, int aufteilung)
{
  Serial.println("rgbrun");
  switch(aufteilung)
  {
    case 0:
    case 1:
      for(int i=0; i<raumleds[aufteilung]; i++)
      {
        switch(farbe)
        {
          case 1:
            pixels.setPixelColor(raum[aufteilung][i], pixels.Color(255, 0, 0));
            break;
          case 2:
            pixels.setPixelColor(raum[aufteilung][i], pixels.Color(0, 255, 0));
            break;
          case 3:
            pixels.setPixelColor(raum[aufteilung][i], pixels.Color(0, 0, 255));
            break;
        }
        pixels.show();
        delay(50);
      }
      break;
    case 2:
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
      break;
  }
}

void setzen(int rot, int gruen, int blau, int leds)
{
  switch(leds)
  {
    case 0:
    case 1:
      for(int i=0; i<raumleds[leds]; i++)
      {
        pixels.setPixelColor(raum[leds][i], pixels.Color(rot, gruen, blau));
      }
      break;
    case 2:
      for(int i=0; i<NUM_LEDS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(rot, gruen, blau));
      }
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

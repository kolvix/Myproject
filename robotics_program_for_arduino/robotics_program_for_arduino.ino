#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte smiley[8] = {0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b10001, 0b01110, 0b00000};

boolean Work;
byte index;
String convert;

char RData[9];
int PCdata[2];

void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.createChar(3, smiley);
    lcd.setCursor(6, 0);
    lcd.print("MC");
    lcd.write(3);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++)
      lcd.write(2);
    
    delay(1000);
}

void loop()
{
    updateDisplay();
    getData();
}

void getData()
{
    while (Serial.available() > 0)
    {
      char newByte = Serial.read();
      if (newByte == '!')
      {
        Work = true;
      }
      else if (newByte != ';' && Work)
      {
        RData[index++] = newByte;
        RData[index] = '\0';
      }
      else if (Work)
      {
        char *p = RData;
        char *str;
        index = 0;
        while ((str = strtok_r(p, "-", &p)) != NULL)
        {
          convert = str;
          PCdata[index++] = convert.toInt();
        }
        index = 0;
      }
      if (newByte == ';')
        Work = false;
    }
}

void updateDisplay()
{
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CPU:");
    lcd.print(PCdata[0]);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("RAM:");
    lcd.print(PCdata[1]);
    lcd.print("%");
    delay(1000);
}

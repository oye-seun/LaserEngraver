#include "Arduino.h"
class MenuItem{
  public:
    int Pos;
    String Text;
    const uint8_t *bitMap;
    int NewState;
    MenuItem(int pos, String text, const uint8_t *bitmap, int newState){
      Pos = pos;
      Text = text;
      bitMap = bitmap;
      NewState = newState;
    }
};
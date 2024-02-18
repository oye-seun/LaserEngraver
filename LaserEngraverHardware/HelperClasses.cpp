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

class WordBlock{
  public:
    float xPos;
    float yPos;
    int width;
    int height;
    String word;
    int textSize;

    WordBlock(float xpos, float ypos, int Width, int Height, String text, int textsize){
      xPos = xpos;
      yPos = ypos;
      width = Width;
      height = Height;
      word = text;
      textSize = textsize;
    }

    void fall(float speed){
      yPos -= speed;
    }

    void targetPlayer(int pos){
      if (yPos > 72 &&  yPos < 80){
        xPos = pos;
        if(xPos - (width*textSize/2) < 8) xPos = 8 + (width*textSize/2);
        else if (xPos + (width*textSize/2) > 120) xPos = 120 - (width*textSize/2);
      }
    }

    bool collisionCheck(float xpos, float ypos, int objWidth, int objHeight){
      if(square(xPos - xpos) - square((width + objWidth)/2) < 0 && square(yPos - ypos) - square((height + objHeight)/2) < 0){
        return true;
      }
      else return false;
    }

    float square(float num){
      return num * num;
    }

    int square(int num){
      return num * num;
    }
};
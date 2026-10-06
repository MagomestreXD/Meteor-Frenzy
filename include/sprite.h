#include "spriteType.h"
#include "spriteKey.h"
#include "texture.h"
#include <cmath>
#include <iostream>

class Sprite{
    private:
        Texture data;
        SpriteKey key;
    public:
        Sprite(Texture data,SpriteKey key):data(move(data)),key(key){}

        Texture* getData(){
            return &data;
        }

        SpriteKey* getKey(){
            return &key;
        }

        Texture rotateTexture(int angleIndex,float angleSlice){
            float b = angleIndex * angleSlice;

            float senB = sin(b);
            float cosB = cos(b); 

            int width = data.getWidth();
            int height = data.getHeight();

            float halfWidth = width/2.0f;
            float halfHeight = height/2.0f;

            Texture rotated(width,height);

            for(int i = 0; i < height; i++){
                    int y = i - halfHeight;
                for(int j = 0; j < width; j++){
                    int x = j - halfWidth;
                    
                    float dx = x * cosB + y * senB;
                    float dy = y * cosB - senB * x;

                    dx += halfWidth;
                    dy += halfHeight;

                    int sx = (int)round(dx);
                    int sy = (int)round(dy);

                    if(sx >= 0  && sx < width && sy >= 0 && sy < height){
                        rotated.getData()[i * width + j] = data.getPixel(sx,sy);
                    }
                }
            }
                
            return rotated;
        }

        Sprite rotateSprite(SpriteKey newKey,float angleSlice){
            return Sprite(rotateTexture(newKey.getAngleIndex(),angleSlice),newKey);
        }
};

#pragma once
#include "spriteType.h"
#include <cmath>

class SpriteKey{
    private:
        SpriteType type;
        int angleIndex;
    public:
        SpriteKey(SpriteType type):type(type),angleIndex(0){}
        
        SpriteKey(SpriteType type,int angleIndex):type(type),angleIndex(angleIndex){}

        SpriteKey(SpriteKey* key):type(key->getSpriteType()),angleIndex(0){}

        SpriteType getSpriteType() const{
            return type;
        }

        int getAngleIndex() const{
            return angleIndex;
        }

        bool operator== (const SpriteKey& outro) const{
            return type == outro.getSpriteType() && angleIndex == outro.getAngleIndex();
        }

        void setType(SpriteType Newtype){
            type = Newtype;
        }

        void nextAngle(float angleSlice){
            int angleCount = round((2.0 * M_PI)/angleSlice);
            angleIndex = (angleIndex + 1)% angleCount;
        }
};

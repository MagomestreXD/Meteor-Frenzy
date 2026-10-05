#pragma once
#include "spriteType.h"

class SpriteKey{
    private:
        SpriteType type;
        int angleIndex;
    public:
        SpriteKey(SpriteType type):type(type),angleIndex(0){}
        
        SpriteKey(SpriteType type,int angleIndex):type(type),angleIndex(angleIndex){}

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
};

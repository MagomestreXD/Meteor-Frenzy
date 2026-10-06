#pragma once
#include "rasterizer.h"
#include "spriteType.h"
#include "spriteKey.h"
#include "sprite.h"
#include <string>
#include <optional>

class SpriteManager {
    private:
        float scale;
        vector<optional<Texture>> textures;
        vector<Sprite> sprites;
        float angleSlice;
    public:
        SpriteManager(float scale):scale(scale),textures(vector<optional<Texture>>(static_cast<int>(SpriteType::Count))),angleSlice(22.5f * M_PI / 180.0f){
        }

        void loadTexture(SpriteKey* key);

        void loadSprite(Rasterizer* rasterizer,Polygon* poly,SpriteKey* key);

        Texture* getSprite(Rasterizer* rasterizer,Polygon poly,SpriteKey* key);

        void loadSprite(Rasterizer* rasterizer,float* minx,float* maxx,float* miny,float* maxy,Polygon* spritePoly,SpriteKey* key);

        Texture* getSprite(Rasterizer* rasterizer,float minx,float maxx,float miny,float maxy,Polygon spritePoly,SpriteType type);

        void emptySprites();

        float getScale(){
            return scale;
        }
    
        void setScale(float f){
            scale = f;
            return;
        }

        void multiplyScale(float f){
            scale = scale * f;
            return;
        }

        float getAngleSlice(){
            return angleSlice;
        }
};


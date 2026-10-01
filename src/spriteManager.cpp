#include "spriteManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>


void SpriteManager::loadTexture(SpriteKey* key){

    string path;

    switch(static_cast<SpriteType>((*key).getSpriteType())){
        case SpriteType::player:
            path = "resources/mary64.png";
            break;
        case SpriteType::inimigo:
            path = "resources/inimigo.png";
            break;
        case SpriteType::room01:
            path = "resources/room01.png";
            break;
        case SpriteType::project:
            path = "resources/project.png";
            break;
        case SpriteType::upgrade:
            path = "resources/upgrade.png";
            break;
        default:
            path = "resources/bruh.png";
            break;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());

    if(surface == nullptr){
        SDL_Log("Erro ao carregar %s: %s", path.c_str(), SDL_GetError());
    }

    SDL_Surface* rgba = SDL_ConvertSurface(surface,SDL_PIXELFORMAT_RGBA8888);

    if(rgba == nullptr){
        SDL_Log("Erro ao carregar %s: %s", path.c_str(), SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    SDL_DestroySurface(surface);

    surface = rgba;

    uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);

    Texture texture(surface->w,surface->h);

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            texture.getData()[y * surface->w + x] = pixels[y * surface->w + x];
        }
    }

    SDL_DestroySurface(surface);

    textures[static_cast<int>((*key).getSpriteType())] = texture;

}

void SpriteManager::loadSprite(Rasterizer* rasterizer,Polygon* poly,SpriteKey* key){

    int index = static_cast<int>((*key).getSpriteType());

    if(!textures[index].has_value()){
        loadTexture(key);
    }
    
    sprites.push_back(Sprite((*rasterizer).scanLineNearestNeighbor(*poly,textures[index].value()),*key));           

    return;

}

Texture* SpriteManager::getSprite(Rasterizer* rasterizer,Polygon poly,SpriteType type){
    SpriteKey key(type);

    for(Sprite& sprite: sprites){
        if(*sprite.getKey() == key){
            return sprite.getData();
        }
    }

    loadSprite(rasterizer,&poly,&key);

    for(Sprite& sprite: sprites){
        if(*sprite.getKey() == key){
            return sprite.getData();
        }
    }

    return NULL;
}

void SpriteManager::loadSprite(Rasterizer* rasterizer,float* minx,float* maxx,float* miny,float* maxy,Polygon* spritePoly,SpriteKey* key){

    int index = static_cast<int>((*key).getSpriteType());

    if(!textures[index].has_value()){
        loadTexture(key);
    }
    
    Texture smallTexture((*rasterizer).scanLineNearestNeighbor(*spritePoly,textures[index].value()));           

    int polyWidth = (int) *maxx - *minx;
    int polyHeight = (int) *maxy - *miny;

    Texture bigTexture(polyWidth,polyHeight);

    int X = 0;
    int Y = 0;

    while(Y < polyHeight){
        for(int y = 0; y < smallTexture.getHeight(); y++){
            while(X < polyWidth){
                for(int x = 0; x < smallTexture.getWidth(); x++){
                    uint32_t pixel = smallTexture.getPixel(x,y);
                    if(Y + y < polyHeight && X + x < polyWidth){
                        bigTexture.getData()[(Y + y) * polyWidth + (X + x)] = pixel;
                    }
                }

                X += smallTexture.getWidth();
            }

            X = 0;
        }
        Y += smallTexture.getHeight();
    }

    sprites.push_back(Sprite(bigTexture,*key));

    return;

}

Texture* SpriteManager::getSprite(Rasterizer* rasterizer,float minx,float maxx,float miny,float maxy,Polygon spritePoly,SpriteType type){
    SpriteKey key(type);

    for(Sprite& sprite: sprites){
        if(*sprite.getKey() == key){
            return sprite.getData();
        }
    }

    loadSprite(rasterizer,&minx,&maxx,&miny,&maxy,&spritePoly,&key);

    for(Sprite& sprite: sprites){
        if(*sprite.getKey() == key){
            return sprite.getData();
        }
    }

    return NULL;
}

void SpriteManager::emptySprites(){

    sprites.clear();
}



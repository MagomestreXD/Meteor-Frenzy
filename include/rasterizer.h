#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "polygon.h"
#include "texture.h"

class Rasterizer {
    private:
        SDL_Renderer* renderer;
        int height;
        int width;
        vector <uint32_t> framebuffer;
        Vertex camPos;
        float zoom;

    public: 
        Rasterizer(SDL_Renderer* renderer,int height,int width,float zoom);

        int getWidth();
        
        int getHeight();

        uint32_t* getFramebuffer(){
            return framebuffer.data();
        };

        void clearFrameBuffer();

        void setPixel(int x,int y, uint32_t color,bool screenCoordinates);

        void setPixel(int x,uint32_t color,int yWidth);

        void drawLine(float x0,float y0,float x1,float y1,uint32_t color);

        void drawCircle(float x0,float y0,float radius,uint32_t color);

        void drawElipse(float x0,float y0,float rA,float rB,uint32_t color);

        void floodFill(Vertex coord,uint32_t color);

        void intersection(Vertex a, Vertex b, int minx, int miny, int maxx, int maxy, vector<vector<Vertex>> *outline);

        void scanLine(Polygon poly);

        Texture scanLineNearestNeighbor(Polygon poly,Texture texture);

        void drawSprite(Polygon poly,Texture* sprite,bool opaque);

        void drawSpriteAnimated(Polygon poly,Texture* sprite,bool opaque,int currentFrame);

        void setCamPos(Vertex pos);

        Vertex getCamPos();

        void multiplyScale(float f);
};

#include "rasterizer.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;

Rasterizer::Rasterizer(SDL_Renderer* renderer,int height,int width,float zoom): renderer(renderer),height(height),width(width),framebuffer(height * width), camPos(Vertex(0,0)), zoom(zoom){}

int Rasterizer::getWidth(){
    return width;
}

int Rasterizer::getHeight(){
    return height;
}

void Rasterizer::clearFrameBuffer(){
    for(int i = 0; i < height * width; i++ ){
        framebuffer[i] = 0x000000FF;
    }
}

void Rasterizer::setPixel(int x,int y, uint32_t color,bool screenCoordinates){
    if(screenCoordinates){
        framebuffer[y * width + x] = color;   
        return;
    }

    int X = x - (camPos.getX() * zoom) + width/2;
    int Y = y - (camPos.getY() * zoom) + height/2;

    if(X < 0 || X >= width || Y < 0 || Y >= height){
        return;
    }

    framebuffer[Y * width + X] = color;
}

void Rasterizer::setPixel(int x,uint32_t color,int yWidth){
    int index = x + yWidth;
    if(index < 0 || index >= (int)framebuffer.size()){
        return;
    }
    framebuffer[index]= color;
    return;
}

void Rasterizer::drawLine(float x0,float y0,float x1,float y1,uint32_t color){
    float temp;
    int dir;

    if(abs(x1 - x0) > abs(y1 - y0)){
        if(x0 > x1){
            temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        int dx = (int) x1 - (int) x0;
        int dy = (int)y1 - (int) y0;

        if(dy < 0){ dir = -1; }else{ dir = 1;}

        dy = dy * dir;

        int y = (int) y0;
        
        int D = 2*dy - dx;

        for(int x = (int) x0; x <= (int) x1;x++){
            
            setPixel(x,y,color,false);

            if(D >= 0){
                y += dir;
                D = D - 2*dx;
            }
            D = D + 2*dy;
        }

    }else{

         if(y0 > y1){
            temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        int dx = (int) x1 -(int) x0;
        int dy = (int) y1 -(int) y0;

        if(dx < 0){ dir = -1; }else{ dir = 1;}

        dx = dx * dir;

        int x = (int) x0;
        
        int D = 2*dx - dy;

        for(int y = (int) y0; y <= (int) y1;y++){
            
            setPixel(x,y,color,false);

            if(D >= 0){
                x += dir;
                D = D - 2*dy;
            }
            D = D + 2*dx;
        }       
    }
}

void Rasterizer::drawCircle(float x0,float y0,float radius,uint32_t color){
    int xc = (int) x0;
    int yc = (int) y0;
    int r = (int) radius;

    int x = r;
    int y = 0;

    float D = 0.25 - r;

    while(y < x){
        if(D > 0){
            x--;
            D += 2*y - 2*x + 3;
        }else{
            D += 2*y + 1;
        }

        setPixel(xc + x,yc + y,color,false);
        setPixel(xc - x,yc + y,color,false);
        setPixel(xc + x,yc - y,color,false);
        setPixel(xc - x,yc - y,color,false);

        setPixel(xc + y,yc + x,color,false);
        setPixel(xc - y,yc + x,color,false);
        setPixel(xc + y,yc - x,color,false);
        setPixel(xc - y,yc - x,color,false);

        y++;
    }
}

void Rasterizer::drawElipse(float x0,float y0,float rA,float rB,uint32_t color){
    int xc = (int) x0;
    int yc = (int) y0;
    int a = (int) rA;
    int b = (int) rB;

    int x = a;
    int y = 0;

    float D = b*b*(0.25 - a);

    while(x*b*b > y*a*a){
        if(D > 0){
           D += (1 - x)*2*b*b;    
           x--;           
        }

        D += (2*y+1)*a*a;

        setPixel(xc + x,yc + y,color,false);
        setPixel(xc - x,yc + y,color,false);
        setPixel(xc + x,yc - y,color,false);
        setPixel(xc - x,yc - y,color,false);

        y++;
    }

    D = x*x*b*b + (y*y + y + 0.25)*a*a - a*a*b*b;

    while(x >= 0){
        if(D < 0){
           D += (y + 1)*2*a*a;    
           y++;           
        }

        D += (-2*x+1)*b*b;

        setPixel(xc + x,yc + y,color,false);
        setPixel(xc - x,yc + y,color,false);
        setPixel(xc + x,yc - y,color,false);
        setPixel(xc - x,yc - y,color,false);

        x--;       
    }
}

void Rasterizer::floodFill(Vertex coord,uint32_t color){
    if(coord.getX() + width/2 < 0 || coord.getX() + width/2 >= width || coord.getY() + height/2 < 0 || coord.getY() + height/2 >= height){
        return;
    }

    uint32_t target = framebuffer[(coord.getY() + height/2) * width + (coord.getX() + width/2)];

    if(color == target){
        return;
    }

    queue<Vertex>pixels;

    pixels.push(coord);

    while(!pixels.empty()){
        Vertex pixel = pixels.front();
        pixels.pop();

        if(pixel.getX() + width/2 < 0 || pixel.getX() + width/2 >= width || pixel.getY() + height/2 < 0 || pixel.getY() + height/2 >= height){
            continue;
        }

        if(framebuffer[(pixel.getY() + height/2) * width + (pixel.getX() + width/2)] != target){
            continue;
        }

        setPixel(pixel.getX(),pixel.getY(),color,true);   

        pixels.push(pixel.addCopy(Vertex(1,0)));
        pixels.push(pixel.addCopy(Vertex(-1,0)));
        pixels.push(pixel.addCopy(Vertex(0,1)));
        pixels.push(pixel.addCopy(Vertex(0,-1)));
    }
}

void Rasterizer::intersection(Vertex a, Vertex b, int minx, int miny,int maxx,int maxy, vector<vector<Vertex>> *outline){
    int x0 = ((int) a.getX()) - minx;
    int y0 = ((int) a.getY()) - miny;
    int x1 = ((int) b.getX()) - minx;
    int y1 = ((int) b.getY()) - miny;

    int maxX = maxx - minx;
    int maxY = maxy - miny;

    float temp;
    int dir;

    bool gradient = true;

    if (a.getColor() == b.getColor()){
        gradient = false;
    }

    bool swap = false;

    if(y0 == y1){
        if(y0 < maxY){
            (*outline)[y0].push_back(Vertex(x0,y0,a.getColor()));
        }
        return;
    }

    if(abs(x1 - x0) > abs(y1 - y0)){
        if(x0 > x1){

            swap = true;
    
            temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        int dx = x1 - x0;
        int dy = y1 - y0;

        if(dy < 0){ dir = -1; }else{ dir = 1;}

        dy = dy * dir;

        int y = y0;
        
        int D = 2*dy - dx;

        int prevY = -1;

        int pixelI = 0;

        int pixels = x1 - x0;

        for(int x = x0; x <= x1;x++){
            if(!swap){
                if((y != prevY && y >= y0 && y < y1) || (y != prevY && y > y1 && y <= y0)){
                    if(y != maxY){
                        if(!gradient){
                            (*outline)[y].push_back(Vertex(x,y,a.getColor()));
                        }else{
                            float t = ((float) pixelI) / pixels;

                            (*outline)[y].push_back(Vertex((float)x,(float)y,(((1 - t) * a.getR()) + (t * b.getR())),(((1 - t) * a.getG()) + (t * b.getG())),(((1 - t) * a.getB()) + (t * b.getB())),(((1 - t) * a.getA()) + (t * b.getA()))));
                            pixelI += 1;
                        }

                        prevY = y;
                    }
                }
            }else{
                if((y != prevY && y > y0 && y <= y1) || (y != prevY && y >= y1 && y < y0)){
                    if(y != maxY){
                        if(!gradient){
                            (*outline)[y].push_back(Vertex(x,y,a.getColor()));
                        }else{
                            float t = ((float) pixelI) / pixels;

                            (*outline)[y].push_back(Vertex(x,y,(((1 - t) * b.getR()) + (t * a.getR())),(((1 - t) * b.getG()) + (t * a.getG())),(((1 - t) * b.getB()) + (t * a.getB())),(((1 - t) * b.getA()) + (t * a.getA()))));
                     
                            pixelI += 1;
                        }

                        prevY = y;
                    }
                }
            }

            if(D >= 0){
                y += dir;
                D = D - 2*dx;
            }
            D = D + 2*dy;
        }

    }else{
         if(y0 > y1){

            swap = true;

            temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        int dx = x1 - x0;
        int dy = y1 - y0;

        if(dx < 0){ dir = -1; }else{ dir = 1;}

        dx = dx * dir;

        int x = x0;
        
        int D = 2*dx - dy;

        int pixelI = 0;

        int pixels = y1 - y0;

        for(int y = y0; y <= y1;y++){
            if(!swap){
                if(y >= y0 && y < y1){
                    if(y != maxY){
                        if(!gradient){
                            (*outline)[y].push_back(Vertex(x,y,a.getColor()));
                        }else{
                            float t = ((float) pixelI) / pixels;

                            (*outline)[y].push_back(Vertex(x,y,(((1 - t) * a.getR()) + (t * b.getR())),(((1 - t) * a.getG()) + (t * b.getG())),(((1 - t) * a.getB()) + (t * b.getB())),(((1 - t) * a.getA()) + (t * b.getA()))));
                            pixelI += 1;
                        }
                    }
                }
            }else{
                if(y > y0 && y <= y1){
                    if(y != maxY){
                        if(!gradient){
                            (*outline)[y].push_back(Vertex(x,y,a.getColor()));
                        }else {
                            float t = ((float) pixelI) / pixels;

                            (*outline)[y].push_back(Vertex(x,y,(((1 - t) * b.getR()) + (t * a.getR())),(((1 - t) * b.getG()) + (t * a.getG())),(((1 - t) * b.getB()) + (t * a.getB())),(((1 - t) * b.getA()) + (t * a.getA()))));
                            pixelI += 1;
                        }
                    }
                }
            }

            if(D >= 0){
                x += dir;
                D = D - 2*dy;
            }
            D = D + 2*dx;
        }       
    }

}

void Rasterizer::scanLine(Polygon poly){

    vector<Vertex> *verteces = poly.getVerteces();

    int maxy = (int) (*verteces)[0].getY();
    int miny = (int) (*verteces)[0].getY();
    int maxx = (int) (*verteces)[0].getX();
    int minx = (int) (*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int) (*verteces)[i].getY()){
            maxy = (int) (*verteces)[i].getY();
        }
        if(miny > (int) (*verteces)[i].getY()){
            miny = (int) (*verteces)[i].getY();
        }
        if(maxx < (int) (*verteces)[i].getX()){
            maxx = (int) (*verteces)[i].getX();
        }
        if(minx > (int) (*verteces)[i].getX()){
            minx = (int) (*verteces)[i].getX();
        }
    }
    
    int layers = maxy - miny;

    vector<vector<Vertex>> outline(layers);
    
    for(int i = 0; i + 1 < (*verteces).size(); i++){
        intersection((*verteces)[i],(*verteces)[i+1],minx,miny,maxx,maxy,&outline);
    }   
    intersection((*verteces)[(*verteces).size() - 1],(*verteces)[0],minx,miny,maxx,maxy,&outline);   


    for(int i = 0; i < (*verteces).size();i++){
        int prev = (i - 1 + (*verteces).size() ) % (*verteces).size();
        int next = (i + 1 ) % (*verteces).size();

        bool concave = false;

        if((*verteces)[prev].getY() == (*verteces)[i].getY() && (*verteces)[next].getY() == (*verteces)[i].getY()){
            concave = true;
        }else if((*verteces)[prev].getY() < (*verteces)[i].getY() && (*verteces)[next].getY() < (*verteces)[i].getY()){
            concave = true;
        }else if((*verteces)[prev].getY() > (*verteces)[i].getY() && (*verteces)[next].getY() > (*verteces)[i].getY()){
            concave = true;
        }

        if(concave){
            outline[(*verteces)[i].getY() - miny].push_back(Vertex((*verteces)[i].getX() - minx,(*verteces)[i].getY() - miny,(*verteces)[i].getColor()));
        }
    }

    Vertex temp;

    for(int y = 0; y < layers; y++){
        for(int i = 0; i < outline[y].size() - 1; i++){
            for(int j = i + 1; j < outline[y].size(); j++){
                if(outline[y][i].getX() > outline[y][j].getX()){
                    temp = outline[y][i];
                    outline[y][i] = outline[y][j];
                    outline[y][j] = temp;
                }
            }
        }
    }

    for(int y = 0; y < layers; y++){
        for(int i = 0; i + 1 < outline[y].size(); i = i + 2){
            int pixelI = 0;

            if(outline[y][i].getColor() == outline[y][i+1].getColor()){
                for(int x = outline[y][i].getX() ; x < outline[y][i+1].getX(); x++){
                    setPixel(x + minx,y + miny,outline[y][i].getColor());
                }
            }else{
                int pixels = (outline[y][i+1].getX() - outline[y][i].getX()) + 1;

                for(int x = outline[y][i].getX(); x < outline[y][i+1].getX(); x++){
                    float t = ((float) pixelI)/pixels;

                    uint8_t r = ((1-t) * outline[y][i].getR()) + (t * outline[y][i+1].getR());
                    uint8_t g = ((1-t) * outline[y][i].getG()) + (t * outline[y][i+1].getG());
                    uint8_t b = ((1-t) * outline[y][i].getB()) + (t * outline[y][i+1].getB());
                    uint8_t a = ((1-t) * outline[y][i].getA()) + (t * outline[y][i+1].getA());

                    uint32_t color = r << 24 | g << 16 | b << 8 | a;

                    setPixel(x + minx,y + miny,color,false);

                    pixelI++;
                }
            }
        }
    }   

}

Texture Rasterizer::scanLineNearestNeighbor(Polygon poly,Texture texture){

    vector<Vertex>* verteces = poly.getVerteces();

    int maxy = (int) (*verteces)[0].getY();
    int miny = (int) (*verteces)[0].getY();
    int maxx = (int) (*verteces)[0].getX();
    int minx = (int) (*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int) (*verteces)[i].getY()){
            maxy = (int) (*verteces)[i].getY();
        }
        if(miny > (int) (*verteces)[i].getY()){
            miny = (int)(*verteces)[i].getY();
        }
        if(maxx < (int) (*verteces)[i].getX()){
            maxx = (int)(*verteces)[i].getX();
        }
        if(minx > (int) (*verteces)[i].getX()){
            minx = (int) (*verteces)[i].getX();
        }
    }
    
    int height = maxy - miny;

    vector<vector<Vertex>> outline(height);
    
    for(int i = 0; i + 1 < (*verteces).size(); i++){
        intersection((*verteces)[i],(*verteces)[i+1],minx,miny,maxx,maxy,&outline);
    }   
    intersection((*verteces)[(*verteces).size() - 1],(*verteces)[0],minx,miny,maxx,maxy,&outline);   

    for(int i = 0; i < (*verteces).size();i++){
        int prev = (i - 1 + (*verteces).size() ) % (*verteces).size();
        int next = (i + 1 ) % (*verteces).size();

        bool concave = false;

        if((*verteces)[prev].getY() == (*verteces)[i].getY() && (*verteces)[next].getY() == (*verteces)[i].getY()){
            concave = true;
        }else if((*verteces)[prev].getY() < (*verteces)[i].getY() && (*verteces)[next].getY() < (*verteces)[i].getY()){
            concave = true;
        }else if((*verteces)[prev].getY() > (*verteces)[i].getY() && (*verteces)[next].getY() > (*verteces)[i].getY()){
            concave = true;
        }

        if(concave){
            outline[(*verteces)[i].getY() - miny].push_back(Vertex((*verteces)[i].getX() - minx,(*verteces)[i].getY() - miny,(*verteces)[i].getColor()));
        }
    }

    Vertex temp;

    for(int y = 0; y < height; y++){
        for(int i = 0; i < outline[y].size() - 1; i++){
            for(int j = i + 1; j < outline[y].size(); j++){
                if(outline[y][i].getX() > outline[y][j].getX()){
                    temp = outline[y][i];
                    outline[y][i] = outline[y][j];
                    outline[y][j] = temp;
                }
            }
        }
    }

    int width = maxx - minx;

    Texture sprite(width,height);

    for(int y = 0; y < height; y++){
        for(int i = 0; i + 1 < outline[y].size(); i = i + 2){
            for(int x = outline[y][i].getX();x < outline[y][i+1].getX();x++){
                float u = (float) x / (width - 1);
                float v = (float) y / (height - 1);

                int ux = (int) (u * (texture.getWidth() - 1));
                int uy = (int) (v * (texture.getHeight() - 1));

                sprite.getData()[(y * width) + x] = texture.getPixel(ux,uy);
            }
        }
    }   

    return sprite;
}

/*
void Rasterizer::drawSprite(Polygon poly,Texture* sprite){
    vector<Vertex>* verteces = poly.getVerteces();

    int maxy = (int) (*verteces)[0].getY();
    int miny = (int) (*verteces)[0].getY();
    int maxx = (int) (*verteces)[0].getX();
    int minx = (int) (*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int) (*verteces)[i].getY()){
            maxy = (int) (*verteces)[i].getY();
        }
        if(miny > (int) (*verteces)[i].getY()){
            miny = (int)(*verteces)[i].getY();
        }
        if(maxx < (int) (*verteces)[i].getX()){
            maxx = (int)(*verteces)[i].getX();
        }
        if(minx > (int) (*verteces)[i].getX()){
            minx = (int) (*verteces)[i].getX();
        }
    }

    int polyWidth = maxx - minx;
    int polyHeight = maxy - miny;

    int X = 0;
    int Y = 0;

    while(Y < polyHeight){
        for(int y = 0; y < (*sprite).getHeight(); y++){
            while(X < polyWidth){
                for(int x = 0; x < (*sprite).getWidth(); x++){
                    uint32_t pixel = (*sprite).getPixel(x,y);
                    if(pixel != 0 && Y + y < polyHeight && X + x < polyWidth){
                        setPixel(X + x + minx,Y + y + miny,pixel);
                    }
                }

                X += (*sprite).getWidth();
            }

            X = 0;
        }
        Y += (*sprite).getHeight();
    }
}
*/

/*
void Rasterizer::drawSprite(Polygon poly, Texture* sprite){

    vector<Vertex>* verteces = poly.getVerteces();

    int maxy = (int)(*verteces)[0].getY();
    int miny = (int)(*verteces)[0].getY();
    int maxx = (int)(*verteces)[0].getX();
    int minx = (int)(*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int)(*verteces)[i].getY()){
            maxy = (int)(*verteces)[i].getY();
        }

        if(miny > (int)(*verteces)[i].getY()){
            miny = (int)(*verteces)[i].getY();
        }

        if(maxx < (int)(*verteces)[i].getX()){
            maxx = (int)(*verteces)[i].getX();
        }

        if(minx > (int)(*verteces)[i].getX()){
            minx = (int)(*verteces)[i].getX();
        }
    }

    int spriteWidth = sprite->getWidth();
    int spriteHeight = sprite->getHeight();

    int camX = (int)(camPos.getX() * zoom);
    int camY = (int)(camPos.getY() * zoom);

    int screenMinX = minx - camX + width / 2;
    int screenMaxX = maxx - camX + width / 2;

    int screenMinY = miny - camY + height / 2;
    int screenMaxY = maxy - camY + height / 2;

    int drawMinX = max(0, screenMinX);
    int drawMaxX = min(width, screenMaxX);

    int drawMinY = max(0, screenMinY);
    int drawMaxY = min(height, screenMaxY);

    if(drawMinX >= drawMaxX || drawMinY >= drawMaxY){
        return;
    }

    int textureStartX = drawMinX - screenMinX;
    int textureStartY = drawMinY - screenMinY;

    int textureWidth = sprite->getWidth();

    vector<uint32_t>& textureData = sprite->getData();

    for(int y = 0; y < drawMaxY - drawMinY; y++){

        int textureY = textureStartY + y;

        int framebufferIndex = (drawMinY + y) * width;
        int textureIndex = textureY * textureWidth + textureStartX;

        for(int x = drawMinX; x < drawMaxX; x++){

            uint32_t pixel = textureData[textureIndex];

            if(pixel != 0){
                framebuffer[framebufferIndex + x] = pixel;
                //setPixel(x, pixel, framebufferIndex);
            }

            textureIndex++;
        }
    }
}
*/

void Rasterizer::drawSprite(Polygon poly, Texture* sprite,bool opaque){

    vector<Vertex>* verteces = poly.getVerteces();

    int maxy = (int)(*verteces)[0].getY();
    int miny = (int)(*verteces)[0].getY();
    int maxx = (int)(*verteces)[0].getX();
    int minx = (int)(*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int)(*verteces)[i].getY()){
            maxy = (int)(*verteces)[i].getY();
        }

        if(miny > (int)(*verteces)[i].getY()){
            miny = (int)(*verteces)[i].getY();
        }

        if(maxx < (int)(*verteces)[i].getX()){
            maxx = (int)(*verteces)[i].getX();
        }

        if(minx > (int)(*verteces)[i].getX()){
            minx = (int)(*verteces)[i].getX();
        }
    }

    int camX = (int)(camPos.getX() * zoom);
    int camY = (int)(camPos.getY() * zoom);

    int screenMinX = minx - camX + width / 2;
    int screenMaxX = maxx - camX + width / 2;

    int screenMinY = miny - camY + height / 2;
    int screenMaxY = maxy - camY + height / 2;

    int drawMinX = max(0, screenMinX);
    int drawMaxX = min(width, screenMaxX);

    int drawMinY = max(0, screenMinY);
    int drawMaxY = min(height, screenMaxY);

    if(drawMinX >= drawMaxX || drawMinY >= drawMaxY){
        return;
    }

    int textureStartX = drawMinX - screenMinX;
    int textureStartY = drawMinY - screenMinY;

    int textureWidth = sprite->getWidth();
    int textureHeight = sprite->getHeight();

    vector<uint32_t>& textureData = sprite->getData();

    int numOfLines = drawMaxY - drawMinY;

    int numOfPixels = drawMaxX - drawMinX;

    int textureAvailableWidth = textureWidth - textureStartX;
    int textureAvailableHeight = textureHeight - textureStartY;

    if(numOfPixels > textureAvailableWidth){
        numOfPixels = textureAvailableWidth;
    }

    if(numOfLines > textureAvailableHeight){
        numOfLines = textureAvailableHeight;
    }

    for(int y = 0; y < numOfLines ; y++){

        int textureY = textureStartY + y;

        int framebufferIndex = (drawMinY + y) * width;
        int textureIndex = textureY * textureWidth + textureStartX;

int framebufferStart = framebufferIndex + drawMinX;

    if(textureIndex < 0 ||
       textureIndex + numOfPixels > (int)textureData.size()){

        cout << "\n=== ERRO TEXTURE ===\n";
        cout << "textureIndex: " << textureIndex << "\n";
        cout << "numOfPixels: " << numOfPixels << "\n";
        cout << "textureSize: " << textureData.size() << "\n";
        cout << "textureY: " << textureY << "\n";
        cout << "textureStartX: " << textureStartX << "\n";
        cout << "textureWidth: " << textureWidth << "\n";
        cout << "numOfLines: " << numOfLines << "\n";
        cout << "y: " << y << "\n";

        return;
    }

    if(framebufferStart < 0 ||
       framebufferStart + numOfPixels > (int)framebuffer.size()){

        cout << "\n=== ERRO FRAMEBUFFER ===\n";
        cout << "framebufferStart: " << framebufferStart << "\n";
        cout << "numOfPixels: " << numOfPixels << "\n";
        cout << "framebufferSize: " << framebuffer.size() << "\n";
        cout << "drawMinX: " << drawMinX << "\n";
        cout << "drawMinY: " << drawMinY << "\n";
        cout << "y: " << y << "\n";

        return;
    }

        if(opaque){
            memcpy(
                &framebuffer[framebufferIndex + drawMinX],
                &textureData[textureIndex],
                numOfPixels * sizeof(uint32_t)
            );
        }else{
            for(int x = drawMinX; x < drawMaxX; x++){

                uint32_t pixel = textureData[textureIndex];

                if(pixel != 0){
                    framebuffer[framebufferIndex + x] = pixel;
                    //setPixel(x, pixel, framebufferIndex);
                }

                textureIndex++;
            }
        }
    }
}

void Rasterizer::drawSpriteAnimated(Polygon poly, Texture* sprite,bool opaque,int currentFrame){

    vector<Vertex>* verteces = poly.getVerteces();

    int maxy = (int)(*verteces)[0].getY();
    int miny = (int)(*verteces)[0].getY();
    int maxx = (int)(*verteces)[0].getX();
    int minx = (int)(*verteces)[0].getX();

    for(int i = 1; i < (*verteces).size(); i++){
        if(maxy < (int)(*verteces)[i].getY()){
            maxy = (int)(*verteces)[i].getY();
        }

        if(miny > (int)(*verteces)[i].getY()){
            miny = (int)(*verteces)[i].getY();
        }

        if(maxx < (int)(*verteces)[i].getX()){
            maxx = (int)(*verteces)[i].getX();
        }

        if(minx > (int)(*verteces)[i].getX()){
            minx = (int)(*verteces)[i].getX();
        }
    }

    int polyWidth = maxx - minx;
    int polyHeight = maxy - miny;

    int frameSize = polyWidth * polyHeight;

    vector<uint32_t>frame(frameSize);
    vector<uint32_t>& textureData = sprite->getData();

    int textureWidth = sprite->getWidth();
    int textureHeight = sprite->getHeight();

    int framePacing = currentFrame * polyWidth;

    for(int i = 0; i < polyHeight; i++){
        int iTextureWidth = i * textureWidth;
        for(int j = 0; j < polyWidth; j++){
            int index = iTextureWidth + j + framePacing;
            if(index >= 0 && index < textureWidth * textureHeight){
                frame[i * polyWidth + j] = textureData[index];
            }
        }
    }

    int camX = (int)(camPos.getX() * zoom);
    int camY = (int)(camPos.getY() * zoom);

    int screenMinX = minx - camX + width / 2;
    int screenMaxX = maxx - camX + width / 2;

    int screenMinY = miny - camY + height / 2;
    int screenMaxY = maxy - camY + height / 2;

    int drawMinX = max(0, screenMinX);
    int drawMaxX = min(width, screenMaxX);

    int drawMinY = max(0, screenMinY);
    int drawMaxY = min(height, screenMaxY);

    if(drawMinX >= drawMaxX || drawMinY >= drawMaxY){
        return;
    }

    int frameStartX = drawMinX - screenMinX;
    int frameStartY = drawMinY - screenMinY;
    int numOfLines = drawMaxY - drawMinY;

    int numOfPixels = drawMaxX - drawMinX;

    int frameAvailableWidth = polyWidth - frameStartX;
    int frameAvailableHeight = polyHeight - frameStartY;

    if(numOfPixels > frameAvailableWidth){
        numOfPixels = frameAvailableWidth;
    }

    if(numOfLines > frameAvailableHeight){
        numOfLines = frameAvailableHeight;
    }

    for(int y = 0; y < numOfLines ; y++){

        int frameY = frameStartY + y;

        int framebufferIndex = (drawMinY + y) * width;
        int frameIndex = frameY * polyWidth + frameStartX;

        int framebufferStart = framebufferIndex + drawMinX;

    if(frameIndex < 0 ||
       frameIndex + numOfPixels > frameSize){

        cout << "\n=== ERRO FRAME ===\n";
        cout << "FrameIndex: " << frameIndex << "\n";
        cout << "numOfPixels: " << numOfPixels << "\n";
        cout << "FrameSize: " << frameSize << "\n";
        cout << "frameY: " << frameY << "\n";
        cout << "frameStartX: " << frameStartX << "\n";
        cout << "frameWidth: " << polyWidth << "\n";
        cout << "numOfLines: " << numOfLines << "\n";
        cout << "y: " << y << "\n";

        return;
    }

    if(framebufferStart < 0 ||
       framebufferStart + numOfPixels > (int)framebuffer.size()){

        cout << "\n=== ERRO FRAMEBUFFER ===\n";
        cout << "framebufferStart: " << framebufferStart << "\n";
        cout << "numOfPixels: " << numOfPixels << "\n";
        cout << "framebufferSize: " << framebuffer.size() << "\n";
        cout << "drawMinX: " << drawMinX << "\n";
        cout << "drawMinY: " << drawMinY << "\n";
        cout << "y: " << y << "\n";

        return;
    }

        if(opaque){
            memcpy(
                &framebuffer[framebufferIndex + drawMinX],
                &frame[frameIndex],
                numOfPixels * sizeof(uint32_t)
            );
        }else{
            for(int x = drawMinX; x < drawMaxX; x++){

                uint32_t pixel = frame[frameIndex];

                if(pixel != 0){
                    framebuffer[framebufferIndex + x] = pixel;
                    //setPixel(x, pixel, framebufferIndex);
                }

                frameIndex++;
            }
        }
    }
}

void Rasterizer::setCamPos(Vertex pos){
    camPos = pos;
}

Vertex Rasterizer::getCamPos(){
    return camPos;
}

void Rasterizer::multiplyScale(float f){
    zoom = zoom * f;
    return;
}


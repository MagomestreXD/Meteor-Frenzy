#include <vector>

class Viewport{
    private:
        int height;
        int width;
        int halfwidth;
        int halfheight;
        float scale;
    public:
        Viewport(int width,int height,float scale):width(width),height(height),scale(scale),halfwidth(width/2),halfheight(height/2){}
        
        void draw(Rasterizer* rasterizer, Vertex playerPos,vector<Vertex> enemyPos){
            for(int i = 0; i < height; i++){
                int YWidth = i * rasterizer->getWidth();
                for(int j = 0; j < width; j++){
                    rasterizer->setPixel(j,0x707070FF,YWidth);
                }
            }

            float scaleMatrix [3][3] = {{scale,0,0},{0,scale,0},{0,0,1}};     

            for(Vertex posEnemy : enemyPos){
                posEnemy.multMatrix(scaleMatrix);

                if(posEnemy.getX() + halfwidth < width && posEnemy.getY() + halfheight < height && posEnemy.getX() + halfwidth >= 0 && posEnemy.getY() + halfheight >= 0){
                    for(int i = 0; i < 8; i++){
                        int y = i + ((int)posEnemy.getY()) + halfheight;
                        if(y >=0 && y < rasterizer->getHeight()){
                            int YWidth = y * rasterizer->getWidth();
                            for(int j = 0; j < 8; j++){
                                int x = j + ((int)posEnemy.getX()) + halfwidth;
                                if(x>=0 && x < rasterizer->getWidth()){
                                    rasterizer->setPixel(x,0xFF0000FF,YWidth);
                                }
                            }
                        }
                    }
                }
            }

            playerPos.multMatrix(scaleMatrix);
            
            if(playerPos.getX() + halfwidth < width && playerPos.getY() + halfheight < height && playerPos.getX() + halfwidth >= 0 && playerPos.getY() + halfheight >= 0){
                for(int i = 0; i < 8; i++){
                    int y = i + ((int)playerPos.getY()) + halfheight;
                    if(y >=0 && y < rasterizer->getHeight()){
                        int YWidth = y * rasterizer->getWidth();
                        for(int j = 0; j < 8; j++){
                            int x = j + ((int)playerPos.getX()) + halfwidth;
                            if(x>=0 && x < rasterizer->getWidth()){
                                rasterizer->setPixel(x,0xFFFFFFFF,YWidth);
                            }
                        }
                    }
                }
            }

        }

        void draw(Rasterizer* rasterizer, Vertex playerPos){
            for(int i = 0; i < height; i++){
                int YWidth = i * rasterizer->getWidth();
                for(int j = 0; j < width; j++){
                    rasterizer->setPixel(j,0x707070FF,YWidth);
                }
            }

            float scaleMatrix [3][3] = {{scale,0,0},{0,scale,0},{0,0,1}};     
            playerPos.multMatrix(scaleMatrix);
            
            if(playerPos.getX() + halfwidth < width && playerPos.getY() + halfheight < height && playerPos.getX() + halfwidth >= 0 && playerPos.getY() + halfheight >= 0){
                for(int i = 0; i < 8; i++){
                    int y = i + ((int)playerPos.getY()) + halfheight;
                    if(y >=0 && y < rasterizer->getHeight()){
                        int YWidth = y * rasterizer->getWidth();
                        for(int j = 0; j < 8; j++){
                            int x = j + ((int)playerPos.getX()) + halfwidth;
                            if(x>=0 && x < rasterizer->getWidth()){
                                rasterizer->setPixel(x,0xFFFFFFFF,YWidth);
                            }
                        }
                    }
                }
            }

        }

};

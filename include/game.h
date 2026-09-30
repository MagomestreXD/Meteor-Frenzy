#pragma once

#include <memory>
#include "player.h"
#include "rasterizer.h"
#include "spriteManager.h"
#include "room.h"
#include "viewport.h"
#include <iostream>
#include <random>

using namespace std;

class Game {
    private:
        unique_ptr<Player> player;
        vector<unique_ptr<Entity>> entities;
        vector<unique_ptr<Entity>> projectiles;
        Room room;
        Rasterizer rasterizer;
        SpriteManager spriteManager;
        InputState inputs;
        int round = 1;
        Viewport minimap;
        double gameTimer = 0.0;
        double inicialSpawnInterval = 2.0;
        double spawnInterval;
        double spawnTimer = 0.0;
        double spawnDificulty = 0.10;
        double velocityDificulty = 0.30;
        double spawnMaxDificulty = 0.2;

    public:
        Game(unique_ptr<Player> player,vector<unique_ptr<Entity>> entities,Rasterizer rasterizer,SpriteManager spriteManager,Room room):player(move(player)),entities(move(entities)),rasterizer(rasterizer),spriteManager(spriteManager),room(room),minimap((room.getMaxx() - room.getMinx()) * 0.25,(room.getMaxy() - room.getMiny()) * 0.25,0.25f){
            spawnInterval = inicialSpawnInterval;
        };

        void iniGame(){
        }

        vector<unique_ptr<Entity>>& getEntities(){
            return entities;
        }

        Rasterizer* getRasterizer(){
            return &rasterizer;
        }

        SpriteManager* getSpriteManager(){
            return &spriteManager;
        }

        void updateLogic(double step){

            if(player->update(step,inputs)){
                cout<<"attaque realizado"<<endl;
                float mx;
                float my;

                SDL_GetMouseState(&mx,&my);

                mx = mx - rasterizer.getWidth()/2;
                my = my - rasterizer.getHeight()/2;

                float magnitude = sqrt(mx*mx + my*my);

                if(magnitude != 0){
                    mx /= magnitude;
                    my /= magnitude;
                }
                
                float iniSpaceMatrix [3][3] = {{32,0,0},{0,32,0},{0,0,1}};

                Vertex uniVector = Vertex(mx,my);

                Vertex projPos = uniVector;

                projPos.multMatrix(iniSpaceMatrix);
                
                float playerTranMatrix [3][3] = {{1,0,player->getPos().getX()},{0,1,player->getPos().getY()},{0,0,1}};

                projPos.multMatrix(playerTranMatrix);

                Polygon poly(vector<Vertex>{Vertex(-8,-8),Vertex(8,-8),Vertex(8,8),Vertex(-8,8)});

                float speed = player->getSpeed() * 1.2;

                Vertex velocity = uniVector;

                float speedMatrix [3][3] = {{speed,0,0},{0,speed,0},{0,0,1}};

                velocity.multMatrix(speedMatrix);

                projectiles.push_back(make_unique<Entity>(poly,projPos,SpriteType::project,velocity));
            }

            if(!entities.empty()){
                for(int i = 0; i < entities.size(); i++){
                    entities[i]->update(step);
                }
            }

            if(!projectiles.empty()){
                for(int i = 0; i < projectiles.size(); i++){
                    projectiles[i]->update(step);
                }
            }

            checkEnemiesOutOfBounds();

            checkCollisions();
            
            gameTimer += step;
            spawnTimer += step;
            
            if(spawnTimer >= spawnInterval){
                spawnTimer = 0.0;
                createNewEnemy();
            }

            spawnInterval = max(spawnMaxDificulty,inicialSpawnInterval - (gameTimer * spawnDificulty));
        }       

        void drawFrame(double alpha){

            Vertex tempCurrent = player->getPos();
            Vertex tempPrev = player->getPrevPos();

            double alphaScale [3][3] = {{alpha,0,0},{0,alpha,0},{0,0,1}};
            double nAlphaScale [3][3] = {{1-alpha,0,0},{0,1-alpha,0},{0,0,1}};

            tempCurrent.multMatrix(alphaScale);
            tempPrev.multMatrix(nAlphaScale);

            double translation [3][3] = {{1,0,tempPrev.getX()},{0,1,tempPrev.getY()},{0,0,1}};

            tempCurrent.multMatrix(translation);

            rasterizer.setCamPos(tempCurrent);

            room.draw(&rasterizer,&spriteManager,spriteManager.getScale());

            if(!entities.empty()){
                drawEntities(alpha);
            }

            if(!projectiles.empty()){
                drawProjectiles(alpha);
            }

            player->drawEntity(&rasterizer,&spriteManager,spriteManager.getScale(), alpha);

            if(!entities.empty()){
                vector<Vertex> enemyPos;
                for(const unique_ptr<Entity>& entity : entities){
                    enemyPos.push_back(entity->getPos());
                }
                minimap.draw(&rasterizer,player->getPos(),enemyPos);
            }else{
                minimap.draw(&rasterizer,player->getPos());
            }
        }

        void drawEntities(double alpha){
            for(int i = 0; i < entities.size(); i++){
                entities[i]->drawEntity(&rasterizer,&spriteManager,spriteManager.getScale(),alpha);
            }
        }
        
        void drawProjectiles(double alpha){
            for(int i = 0; i < projectiles.size(); i++){
                projectiles[i]->drawEntity(&rasterizer,&spriteManager,spriteManager.getScale(),alpha);
            }
        }

        InputState* getInputs(){
            return &inputs;
        }

        void checkCollisions();

        bool checkCollision(Polygon polya, Polygon polyb, Vertex posa, Vertex posb);

        bool testSAT(Polygon* pa,Polygon* pb);

        void createNewEnemy();

        void checkEnemiesOutOfBounds();
};

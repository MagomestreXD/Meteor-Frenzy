#include "entity.h"
#include "inputState.h"
#include "upgradeType.h"

class Player : public Entity{
    private: 
        float speed;
        float iniSpeed;
        Vertex direction;
        float attackCooldown;
        float attackTimer = 0.0f;
        int health = 3;
        int maxHealth = 3;
        float projctSpeed;
        bool moving = false;
        int frameCount = 8;
        int currentFrame = 0;
        float frameTimeSlice;
        float frameTimer = 0;
        Polygon animationPoly;
    public:
        Player(Polygon poly,Vertex pos,float speed,SpriteType type,float attackCooldown):Entity(poly,pos,type),speed(speed),attackCooldown(attackCooldown),animationPoly(poly),iniSpeed(speed){
            direction = Vertex(1,0);
            projctSpeed = speed * 1.2;
            frameTimeSlice = 1.5/frameCount;
            float scaleAniMatriz [3][3] = {{(float)frameCount,0,0},{0,1,0},{0,0,1}};
            animationPoly = poly.multMatrix(scaleAniMatriz);
        };
        
        float getSpeed(){
            return speed;
        }

        void setSpeed(float newSpeed){
            speed = newSpeed;
        }

        Vertex getDirection(){
            return direction;
        }

        bool update(double step, InputState inputs){
            bool attack = false;

            SpriteType typeBefore = key.getSpriteType();

            attackTimer -= step;
            if(attackTimer <= 0){
                attackTimer = 0;
            }

            if(inputs.m1){
                if(attackTimer == 0){
                    attackTimer = attackCooldown;
                    attack = true;
                }
            }

            direction = Vertex(0,0);
            bool moved = false;

            if(inputs.up){
                float dir [3][3] = {{1,0,0},{0,1,-1},{0,0,1}};
                direction.multMatrix(dir);
                key.setType(SpriteType::player_up_ani);
                moved = true;
            }

            if(inputs.down){
                float dir [3][3] = {{1,0,0},{0,1,1},{0,0,1}};
                direction.multMatrix(dir);
                key.setType(SpriteType::player_down_ani);
                moved = true;
            }

            if(inputs.left){
                float dir [3][3] = {{1,0,-1},{0,1,0},{0,0,1}};
                direction.multMatrix(dir);
                key.setType(SpriteType::player_left_ani);
                moved = true;
            }

            if(inputs.right){
                float dir [3][3] = {{1,0,1},{0,1,0},{0,0,1}};
                direction.multMatrix(dir);
                key.setType(SpriteType::player_right_ani);
                moved = true;
            }

            prevPos = pos;

            if(moved){
                double stepM [3][3]= {{speed * step,0,0},{0,speed * step,0},{0,0,1}};       

                direction.multMatrix(stepM);

                float velocity [3][3] = {{1,0,direction.getX()},{0,1,direction.getY()},{0,0,1}};

                pos.multMatrix(velocity);
                if(typeBefore == key.getSpriteType()){
                    frameTimer += step;
                    if(frameTimer >= frameTimeSlice){
                        currentFrame = (currentFrame + 1)%frameCount;   
                        frameTimer = 0;
                    }
                }else{
                    frameTimer = 0;
                    currentFrame = 0;
                }

                moving = true;
            }else{
                if(moving){
                    if(typeBefore == SpriteType::player_down_ani){
                        key.setType(SpriteType::player);
                    }else if(typeBefore == SpriteType::player_up_ani){
                        key.setType(SpriteType::player_back);
                    }else if(typeBefore == SpriteType::player_left_ani){
                        key.setType(SpriteType::player_left);
                    }else{
                        key.setType(SpriteType::player_right);
                    }
    
                    currentFrame = 0;

                    moving = false;
                }
            }

            return attack;
        };

        void drawEntity(Rasterizer* rasterizer,SpriteManager* spriteManager,float scale,double alpha){
            Vertex tempCurrent = pos;
            Vertex tempPrev = prevPos;

            double alphaScale [3][3] = {{alpha,0,0},{0,alpha,0},{0,0,1}};
            double nAlphaScale [3][3] = {{1-alpha,0,0},{0,1-alpha,0},{0,0,1}};

            tempCurrent.multMatrix(alphaScale);
            tempPrev.multMatrix(nAlphaScale);

            double translation [3][3] = {{1,0,tempPrev.getX()},{0,1,tempPrev.getY()},{0,0,1}};

            tempCurrent.multMatrix(translation);

            double betweenPosM [3][3] = {{1,0,tempCurrent.getX()},{0,1,tempCurrent.getY()},{0,0,1}};

            float scaleM [3][3] = {{scale,0,0},{0,scale,0},{0,0,1}};

            Polygon betweenPoly = poly.multMatrix(betweenPosM).multMatrix(scaleM);

            int maxy = (int) (*betweenPoly.getVerteces())[0].getY();
            int miny = (int) (*betweenPoly.getVerteces())[0].getY();
            int maxx = (int) (*betweenPoly.getVerteces())[0].getX();
            int minx = (int) (*betweenPoly.getVerteces())[0].getX();

            for(int i = 1; i < (*betweenPoly.getVerteces()).size(); i++){
                if(maxy < (int) (*betweenPoly.getVerteces())[i].getY()){
                    maxy = (int) (*betweenPoly.getVerteces())[i].getY();
                }
                if(miny > (int) (*betweenPoly.getVerteces())[i].getY()){
                    miny = (int)(*betweenPoly.getVerteces())[i].getY();
                }
                if(maxx < (int) (*betweenPoly.getVerteces())[i].getX()){
                    maxx = (int)(*betweenPoly.getVerteces())[i].getX();
                }
                if(minx > (int) (*betweenPoly.getVerteces())[i].getX()){
                    minx = (int) (*betweenPoly.getVerteces())[i].getX();
                }
            }

            int halfWidth = (*rasterizer).getWidth()/2 - (int)((*rasterizer).getCamPos().getX() * scale); 
            int halfHeight = (*rasterizer).getHeight()/2 - (int)((*rasterizer).getCamPos().getY() * scale);

            if(maxx + halfWidth < 0 || minx + halfWidth >= (*rasterizer).getWidth() || 
               maxy + halfHeight < 0 || miny + halfHeight >= (*rasterizer).getHeight() ){
                return;
            }
            
            if(moving){
                (*rasterizer).drawSpriteAnimated(betweenPoly,(*spriteManager).getSprite(rasterizer,animationPoly.multMatrix(scaleM),&key),false,currentFrame);
            }else{
                (*rasterizer).drawSprite(betweenPoly,(*spriteManager).getSprite(rasterizer,poly.multMatrix(scaleM),&key),false);
            }
        }

        int getHealth(){
            return health;
        }

        void setHealth(int newHealth){
            health = newHealth;
        }

        float getProjctSpeed(){
            return projctSpeed;
        }

        void upgrade(UpgradeType type, float var){
            switch(type){
                case UpgradeType::health:
                    health += var;
                    if(health > maxHealth){
                        health = maxHealth;
                    }
                    cout<<"Health: "<<health<<endl;
                    break;
                case UpgradeType::playerSpeed:
                    speed *= var;
                    cout<<"Player speed:"<<speed<<endl;
                    frameTimeSlice /= var;
                    break;
                case UpgradeType::projectileSpeed:
                    projctSpeed *= var;
                    cout<<"projctSpeed :"<<projctSpeed<<endl;
                    break;
                case UpgradeType::maxHealth:
                    maxHealth += var;
                    health += var;
                    cout<<"maxHealth:"<<maxHealth<<endl;
                    break;
                case UpgradeType::attackCooldown:
                    attackCooldown *= var;
                    cout<<"attackCooldown: "<<attackCooldown<<endl;
                    break;
                default:
                    break;
            }
        }
};


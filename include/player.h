#include "entity.h"
#include "inputState.h"

class Player : public Entity{
    private: 
        float speed;
        Vertex direction;
        float attackCooldown;
        float attackTimer = 0.0f;
    public:
        Player(Polygon poly,Vertex pos,float speed,SpriteType type,float attackCooldown):Entity(poly,pos,type),speed(speed),attackCooldown(attackCooldown){
            direction = Vertex(1,0);
        };
        
        float getSpeed(){
            return speed;
        }

        Vertex getDirection(){
            return direction;
        }

        bool update(double step, InputState inputs){
            bool attack = false;

            if(inputs.m1){
                if(attackTimer <= 0){
                    attackTimer = attackCooldown;
                    attack = true;
                }else{
                    attackTimer -= step;
                }
            }

            direction = Vertex(0,0);
            
            if(inputs.up){
                float dir [3][3] = {{1,0,0},{0,1,-1},{0,0,1}};
                direction.multMatrix(dir);
            }

            if(inputs.down){
                float dir [3][3] = {{1,0,0},{0,1,1},{0,0,1}};
                direction.multMatrix(dir);
            }

            if(inputs.left){
                float dir [3][3] = {{1,0,-1},{0,1,0},{0,0,1}};
                direction.multMatrix(dir);
            }

            if(inputs.right){
                float dir [3][3] = {{1,0,1},{0,1,0},{0,0,1}};
                direction.multMatrix(dir);
            }

            double stepM [3][3]= {{speed * step,0,0},{0,speed * step,0},{0,0,1}};       

            direction.multMatrix(stepM);

            prevPos = pos;

            float velocity [3][3] = {{1,0,direction.getX()},{0,1,direction.getY()},{0,0,1}};

            pos.multMatrix(velocity);

            return attack;
        };

};


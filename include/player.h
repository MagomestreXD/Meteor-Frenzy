#include "entity.h"
#include "inputState.h"

class Player : public Entity{
    private: 
        float speed;
        Vertex direction;

    public:
        Player(Polygon poly,Vertex pos,float speed,SpriteType type):Entity(poly,pos,type),speed(speed){
            direction = Vertex(1,0);
        };
        
        float getSpeed(){
            return speed;
        }

        Vertex getDirection(){
            return direction;
        }

        void update(double step, InputState inputs){
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

        };

};


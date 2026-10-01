
class Upgrade : public Entity {
    private:
        float lifeTimer = 0;
        float maxLifeTimer = 5;
    public:
        Upgrade(Polygon poly,Vertex pos,SpriteType type,Vertex speedVec):Entity(poly,pos,type,speedVec){
        }

        void update(double step){
            lifeTimer += step;   
        }

        bool timesUp(){
            bool dead = false;
            if(lifeTimer >= maxLifeTimer){
                dead = true;
            }
            return dead;
        }
};

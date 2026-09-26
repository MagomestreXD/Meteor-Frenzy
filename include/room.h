
class Room{
    private:
        Polygon poly;
        Polygon spritePoly;
        SpriteType type = SpriteType::room01;
        float minx;
        float maxx;
        float miny;
        float maxy;

    public: 
        Room(Polygon poly,Polygon spritePoly,SpriteType spriteType):poly(poly),spritePoly(spritePoly),type(spriteType){
            vector<Vertex>* verteces = poly.getVerteces();

            maxy = (int) (*verteces)[0].getY();
            miny = (int) (*verteces)[0].getY();
            maxx = (int) (*verteces)[0].getX();
            minx = (int) (*verteces)[0].getX();

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

        }

        void draw(Rasterizer* rasterizer,SpriteManager* spriteManager,float scale){
            float scaleM [3][3] = {{scale,0,0},{0,scale,0},{0,0,1}};

            Polygon scaledPoly = poly.multMatrix(scaleM);

            float Minx = minx * scale;
            float Maxx = maxx * scale;
            float Miny = miny * scale;
            float Maxy = maxy * scale;

           (*rasterizer).drawSprite(scaledPoly,(*spriteManager).getSprite(rasterizer,Minx,Maxx,Miny,Maxy,spritePoly.multMatrix(scaleM),type),true);
        }

        float getMinx(){
            return minx;
        }

        float getMaxx(){
            return maxx;
        }

        float getMiny(){
            return miny;
        }

        float getMaxy(){
            return maxy;
        }

};


class MiniMap{
    public:
        Texture texture;
    private:
        MiniMap(float minx,float maxx,float miny,float maxy):texture(maxx-minx,maxy-miny){}

        uint32_t* getData(){
            return texture.getData();
        }

        void update(){

        }

        void draw(){

        }
};

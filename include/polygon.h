#include <vector>
#include "vertex.h"
using namespace std;

class Polygon {
    private:
        vector<Vertex> verteces;
    public:
        Polygon(vector<Vertex> verteces):verteces(verteces){};

        vector<Vertex>* getVerteces(){
            return &verteces;
        }

        Polygon multMatrix(float matrix[3][3]){
            vector<Vertex> copy = verteces;

            for(int i = 0; i < copy.size(); i++){
                copy[i].multMatrix(matrix);
            }
            
            return Polygon(copy);
        }

        Polygon multMatrix(double matrix[3][3]){
            vector<Vertex> copy = verteces;

            for(int i = 0; i < copy.size(); i++){
                copy[i].multMatrix(matrix);
            }
            
            return Polygon(copy);
        }

        bool isTouching(float x,float y){

            int maxy = (int)(verteces)[0].getY();
            int miny = (int)(verteces)[0].getY();
            int maxx = (int)(verteces)[0].getX();
            int minx = (int)(verteces)[0].getX();

            for(int i = 1; i < (verteces).size(); i++){
                if(maxy < (int)(verteces)[i].getY()){
                    maxy = (int)(verteces)[i].getY();
                }

                if(miny > (int)(verteces)[i].getY()){
                    miny = (int)(verteces)[i].getY();
                }

                if(maxx < (int)(verteces)[i].getX()){
                    maxx = (int)(verteces)[i].getX();
                }

                if(minx > (int)(verteces)[i].getX()){
                    minx = (int)(verteces)[i].getX();
                }
            }

            return x >= minx && x < maxx && y >= miny && y < maxy;
        }

        void setVerteces(vector<Vertex> newVerteces){
            verteces = newVerteces;
        }
};

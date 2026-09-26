#include "game.h"

void Game::checkCollisions(){

    if(!entities.empty()){
        for(int i = 0; i < entities.size(); i++){
            checkCollision(player->getPolygon(),entities[i]->getPolygon(),player->getPos(),entities[i]->getPos());       
        }  
    }

    float transMatrix [3][3]= {{1,0,player->getPos().getX()},{0,1,player->getPos().getY()},{0,0,1}};
    Polygon tPlayer = player->getPolygon().multMatrix(transMatrix);
    vector<Vertex>* verteces = tPlayer.getVerteces();

    for(int i = 0; i < verteces->size(); i++){
        if((*verteces)[i].getX() > room.getMaxx() || (*verteces)[i].getX() < room.getMinx() || (*verteces)[i].getY() > room.getMaxy() || (*verteces)[i].getY() < room.getMiny()){
            player->setPos(player->getPrevPos());
            break;
        }
    }
}

bool Game::checkCollision(Polygon polya, Polygon polyb, Vertex posa, Vertex posb){
    float minDistanceSqrd = 5200;

    float xdistance = posa.getX() - posb.getX();
    float ydistance = posa.getY() - posb.getY();

    float distSqrd = (xdistance * xdistance) + (ydistance * ydistance);

    if(distSqrd > minDistanceSqrd){
        return false;
    }
    
    float transPosA [3][3] = {{1,0,posa.getX()},{0,1,posa.getY()},{0,0,1}};
    float transPosB [3][3] = {{1,0,posb.getX()},{0,1,posb.getY()},{0,0,1}};

    Polygon pa = polya.multMatrix(transPosA);
    Polygon pb = polyb.multMatrix(transPosB);

    vector<Vertex>* va = pa.getVerteces();
    vector<Vertex>* vb = pb.getVerteces();

    float maxya = (*va)[0].getY();
    float minya = (*va)[0].getY();
    float maxxa = (*va)[0].getX();
    float minxa = (*va)[0].getX();

    float maxyb = (*vb)[0].getY();
    float minyb = (*vb)[0].getY();
    float maxxb = (*vb)[0].getX();
    float minxb = (*vb)[0].getX();

    for(int i = 1; i < (*va).size(); i++){
        if(maxya < (*va)[i].getY()){
            maxya = (*va)[i].getY();
        }
        if(minya > (*va)[i].getY()){
            minya = (*va)[i].getY();
        }
        if(maxxa < (*va)[i].getX()){
            maxxa = (*va)[i].getX();
        }
        if(minxa > (*va)[i].getX()){
            minxa = (*va)[i].getX();
        }
    }
 
    for(int i = 1; i < (*vb).size(); i++){
        if(maxyb < (*vb)[i].getY()){
            maxyb = (*vb)[i].getY();
        }
        if(minyb > (*vb)[i].getY()){
            minyb = (*vb)[i].getY();
        }
        if(maxxb < (*vb)[i].getX()){
            maxxb = (*vb)[i].getX();
        }
        if(minxb > (*vb)[i].getX()){
            minxb = (*vb)[i].getX();
        }
    }

    if(maxxa < minxb || maxya < minyb || maxxb < minxa || maxyb < minya){
        return false;
    }

    if(!testSAT(&pa,&pb)){
        return false;
    }

    return true;
}

bool Game::testSAT(Polygon* pa,Polygon* pb){
    vector<Vertex>* va = pa->getVerteces();
    vector<Vertex>* vb = pb->getVerteces();

    for(int i = 0; i < va->size(); i++){
        int next = (i + 1) % va->size();
        
        float axisx = (*va)[i].getY() - (*va)[next].getY();
        float axisy = (*va)[next].getX() - (*va)[i].getX();

        float minproja = ( (*va)[0].getX() * axisx + (*va)[0].getY() * axisy );
        float maxproja = ( (*va)[0].getX() * axisx + (*va)[0].getY() * axisy );

        float proj;

        for(int i = 1; i < va->size();i++){
            proj = ( (*va)[i].getX() * axisx + (*va)[i].getY() * axisy );
            if(proj < minproja){
                minproja = proj;
            }
            if(proj > maxproja){
                maxproja = proj;
            }
        }

        float minprojb = ( (*vb)[0].getX() * axisx + (*vb)[0].getY() * axisy );
        float maxprojb = ( (*vb)[0].getX() * axisx + (*vb)[0].getY() * axisy );

        for(int i = 1; i < vb->size();i++){
            proj = ( (*vb)[i].getX() * axisx + (*vb)[i].getY() * axisy );
            if(proj < minprojb){
                minprojb = proj;
            }
            if(proj > maxprojb){
                maxprojb = proj;
            }
        }

        if(minproja > maxprojb || maxproja < minprojb){
            return false;
        }
    }

    for(int i = 0; i < vb->size(); i++){
        int next = (i + 1) % vb->size();
        
        float axisx = (*vb)[i].getY() - (*vb)[next].getY();
        float axisy = (*vb)[next].getX() - (*vb)[i].getX();

        float minprojb = ( (*vb)[0].getX() * axisx + (*vb)[0].getY() * axisy );
        float maxprojb = ( (*vb)[0].getX() * axisx + (*vb)[0].getY() * axisy );

        float proj;

        for(int i = 1; i < vb->size();i++){
            proj = ( (*vb)[i].getX() * axisx + (*vb)[i].getY() * axisy );
            if(proj < minprojb){
                minprojb = proj;
            }
            if(proj > maxprojb){
                maxprojb = proj;
            }
        }

        float minproja = ( (*va)[0].getX() * axisx + (*va)[0].getY() * axisy );
        float maxproja = ( (*va)[0].getX() * axisx + (*va)[0].getY() * axisy );

        for(int i = 1; i < va->size();i++){
            proj = ( (*va)[i].getX() * axisx + (*va)[i].getY() * axisy );
            if(proj < minproja){
                minproja = proj;
            }
            if(proj > maxproja){
                maxproja = proj;
            }
        }

        if(minprojb > maxproja || maxprojb < minproja){
            return false;
        }
    }

    return true;
}

void Game::createNewEnemy(){
    float oneOverSQrt2 = 0.71f;   

    Polygon poly(vector<Vertex>{Vertex(-16,-16),Vertex(16,-16),Vertex(16,16),Vertex(-16,16)});
    SpriteType type = SpriteType::inimigo;
    float maxSpeed = 350;
    float inicialSpeed = 120;
    float speed = min(maxSpeed,inicialSpeed + (float)(gameTimer * velocityDificulty));
    float speedMatrix[3][3] = {{speed,0,0},{0,speed,0},{0,0,1}};
    int distance = 60;

    random_device rd;   
    mt19937 gen(rd());

    uniform_int_distribution<int> dir(0,3);
    int side = dir(gen);
    
    float roomWidth = room.getMaxx() - room.getMinx();
    float roomHeight = room.getMaxy() - room.getMiny();
    float rWidth3 = roomWidth/3;
    float rHeight3 = roomHeight/3;

    float inicialY = room.getMiny() - distance;
    float inicialX = room.getMinx() - distance;

    uniform_int_distribution<int> Xgen(room.getMinx(),room.getMaxx());
    uniform_int_distribution<int> Ygen(room.getMiny(),room.getMaxy());
    
    uniform_int_distribution<int> btween2(0,1);
    uniform_int_distribution<int> btween3(0,2);

    Vertex direction;
    int velDir; 

    int x;
    int y;

    switch(side){
        case 0:
            x = Xgen(gen);      
            if(x < rWidth3 + room.getMinx()){
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else{
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));

            }else if(x < (2 * rWidth3) + room.getMinx()){
                velDir = btween3(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else if(velDir ==  1){
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }else{
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));
           
            }else{
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else{
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));
            }

            break;
        case 1:
            x = Xgen(gen);      
            if(x < rWidth3 + room.getMinx()){
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,-1);          
                }else{
                    direction = Vertex(oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,-inicialY),SpriteType::inimigo,direction));

            }else if(x < (2 * rWidth3) + room.getMinx()){
                velDir = btween3(gen);

                if(velDir == 0){
                    direction = Vertex(0,-1);          
                }else if(velDir ==  1){
                    direction = Vertex(oneOverSQrt2,-oneOverSQrt2);
                }else{
                    direction = Vertex(-oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,-inicialY),SpriteType::inimigo,direction));
           
            }else{
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,-1);          
                }else{
                    direction = Vertex(-oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,-inicialY),SpriteType::inimigo,direction));
            }

            break;
        case 2:
            y = Ygen(gen);      
            if(y < rHeight3 + room.getMiny()){
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(1,0);          
                }else{
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(inicialX,y),SpriteType::inimigo,direction));

            }else if(y < (2 * rHeight3) + room.getMiny()){
                velDir = btween3(gen);

                if(velDir == 0){
                    direction = Vertex(1,0);          
                }else if(velDir ==  1){
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }else{
                    direction = Vertex(oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(inicialX,y),SpriteType::inimigo,direction));
           
            }else{
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(1,0);          
                }else{
                    direction = Vertex(oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(inicialX,y),SpriteType::inimigo,direction));
            }
            break;
        case 3:
            y = Ygen(gen);      
            if(y < rHeight3 + room.getMiny()){
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(-1,0);          
                }else{
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(-inicialX,y),SpriteType::inimigo,direction));

            }else if(y < (2 * rHeight3) + room.getMiny()){
                velDir = btween3(gen);

                if(velDir == 0){
                    direction = Vertex(-1,0);          
                }else if(velDir ==  1){
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }else{
                    direction = Vertex(-oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(-inicialX,y),SpriteType::inimigo,direction));
           
            }else{
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(-1,0);          
                }else{
                    direction = Vertex(-oneOverSQrt2,-oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(-inicialX,y),SpriteType::inimigo,direction));
            }
            break;
        default:
            int x = Xgen(gen);      
            if(x < rWidth3 + room.getMinx()){
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else{
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));

            }else if(x < (2 * rWidth3) + room.getMinx()){
                velDir = btween3(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else if(velDir ==  1){
                    direction = Vertex(oneOverSQrt2,oneOverSQrt2);
                }else{
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));
           
            }else{
                velDir = btween2(gen);

                if(velDir == 0){
                    direction = Vertex(0,1);          
                }else{
                    direction = Vertex(-oneOverSQrt2,oneOverSQrt2);
                }

                direction.multMatrix(speedMatrix);       

                entities.push_back(make_unique<Entity>(poly,Vertex(x,inicialY),SpriteType::inimigo,direction));
            }

            break;
    }

}

void Game::checkEnemiesOutOfBounds(){
    int distance = 60;

    float inicialY = room.getMiny() - distance;
    float inicialX = room.getMinx() - distance;
    float maxY = room.getMaxy() + distance;
    float maxX = room.getMaxx() + distance;

    if(!entities.empty()){
        for(auto it = entities.begin(); it != entities.end();){
            Vertex pos = (*it)->getPos();
            if(pos.getY() < inicialY || pos.getY() > maxY || pos.getX() < inicialX || pos.getX() > maxX){
                it = entities.erase(it);
            }else{
                ++it;
            }   
    
        }
    }
}

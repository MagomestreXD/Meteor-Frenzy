#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "game.h"
#define Width 1280
#define Height 720

using namespace std;

int main(){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        cout << "SDL3 init Error: " << SDL_GetError() << endl;
        return 1;

    }

    SDL_Window *window = SDL_CreateWindow("Jogo",Width,Height,0);

    //SDL_SetWindowFullscreen(window, true);

    if(window == NULL){
        cout << "SDL3 init Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Polygon poly(vector<Vertex>{Vertex(-16,-16),Vertex(16,-16),Vertex(16,16),Vertex(-16,16)});

    Polygon poly64(vector<Vertex>{Vertex(-32,-32),Vertex(32,-32),Vertex(32,32),Vertex(-32,32)});

    Polygon polyDif(vector<Vertex>{Vertex(-16,-32),Vertex(16,-32),Vertex(16,32),Vertex(-16,32)});

    float scale = 2.0f;

    vector<unique_ptr<Entity>> entities;

    auto player = make_unique<Player>(polyDif,Vertex(0,0),100,SpriteType::player,1.0f);

    Room room(Polygon(vector<Vertex>{Vertex(-1538,-864),Vertex(1538,-864),Vertex(1538,864),Vertex(-1538,864)}),poly64,SpriteType::room02);

    Game game(move(player),move(entities),Rasterizer(renderer,Height,Width,scale),SpriteManager(scale),room);   

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        Width,
        Height
    );

    bool running = true;

    double step = 1.0/60.0;

    double accumulator = 0.0;
    
    Uint64 prevTime = SDL_GetPerformanceCounter();

    game.createNewEnemy();

    while(running){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }

            if(event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.key == SDLK_W){
                    (*game.getInputs()).up = true;
                }
                if(event.key.key == SDLK_S){
                    (*game.getInputs()).down = true;
                }
                if(event.key.key == SDLK_A){
                    (*game.getInputs()).left = true;
                }
                if(event.key.key == SDLK_D){
                    (*game.getInputs()).right = true;
                }

                if(event.key.key == SDLK_J){
                    if((*game.getSpriteManager()).getScale() < 4){
                        (*game.getSpriteManager()).multiplyScale(2);
                        (*game.getRasterizer()).multiplyScale(2);
                    }
                    (*game.getSpriteManager()).emptySprites();
                }

                if(event.key.key == SDLK_K){
                    if((*game.getSpriteManager()).getScale() > 0.5){
                        (*game.getSpriteManager()).multiplyScale(0.5);
                        (*game.getRasterizer()).multiplyScale(0.5);
                    }
                    (*game.getSpriteManager()).emptySprites();
                }

            }

            if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    (*game.getInputs()).m1 = true;   
                }
            }

            if(event.type == SDL_EVENT_KEY_UP){
                if(event.key.key == SDLK_W){
                    (*game.getInputs()).up = false;
                }
                if(event.key.key == SDLK_S){
                    (*game.getInputs()).down = false;
                }
                if(event.key.key == SDLK_A){
                    (*game.getInputs()).left = false;
                }
                if(event.key.key == SDLK_D){
                    (*game.getInputs()).right = false;
                }
            }

            if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    (*game.getInputs()).m1 = false;   
                }
            }

        }
        
        (*game.getRasterizer()).clearFrameBuffer();

        Uint64 currentTime = SDL_GetPerformanceCounter();

        double deltaTime = (double) (currentTime - prevTime) / SDL_GetPerformanceFrequency();

        deltaTime = min(deltaTime,0.25);

        prevTime = currentTime;

        accumulator += deltaTime;

        while(accumulator >= step){
            game.updateLogic(step);
            accumulator -= step;
        }

        double alpha = accumulator/step;

        game.drawFrame(alpha);
 
        SDL_UpdateTexture(texture,NULL,(*game.getRasterizer()).getFramebuffer(), (*game.getRasterizer()).getWidth() * sizeof(uint32_t));

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer,texture,NULL,NULL);

        SDL_RenderPresent(renderer);
        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

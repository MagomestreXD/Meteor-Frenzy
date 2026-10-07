#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <iostream>

class AudioManager {
private:
    MIX_Mixer* mixer = nullptr;
    MIX_Track* musicTrack = nullptr;
    MIX_Audio* music = nullptr;

public:
    AudioManager(){
        if(!MIX_Init()){
            std::cout << "Erro ao inicializar SDL3_mixer: "
                      << SDL_GetError() << std::endl;
            return;
        }

        mixer = MIX_CreateMixerDevice(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
            nullptr
        );

        if(mixer == nullptr){
            std::cout << "Erro ao criar mixer: "
                      << SDL_GetError() << std::endl;
            return;
        }

        musicTrack = MIX_CreateTrack(mixer);

        if(musicTrack == nullptr){
            std::cout << "Erro ao criar track: "
                      << SDL_GetError() << std::endl;
        }
    }

    ~AudioManager(){
        cleanup();
    }

    bool loadMusic(const std::string& path){
        if(music != nullptr){
            MIX_DestroyAudio(music);
            music = nullptr;
        }

        music = MIX_LoadAudio(mixer, path.c_str(), false);

        if(music == nullptr){
            std::cout << "Erro ao carregar musica: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        if(!MIX_SetTrackAudio(musicTrack, music)){
            std::cout << "Erro ao colocar musica na track: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        return true;
    }

    void playMusic(){
        if(musicTrack == nullptr || music == nullptr){
            return;
        }

        SDL_PropertiesID options = SDL_CreateProperties();

        SDL_SetNumberProperty(
            options,
            MIX_PROP_PLAY_LOOPS_NUMBER,
            -1
        );

        if(!MIX_PlayTrack(musicTrack, options)){
            std::cout << "Erro ao tocar musica: "
                      << SDL_GetError() << std::endl;
        }

        SDL_DestroyProperties(options);
    }

    void stopMusic(){
        if(musicTrack != nullptr){
            MIX_StopTrack(musicTrack, 0);
        }
    }

    void cleanup(){
        if(musicTrack != nullptr){
            MIX_DestroyTrack(musicTrack);
            musicTrack = nullptr;
        }

        if(music != nullptr){
            MIX_DestroyAudio(music);
            music = nullptr;
        }

        if(mixer != nullptr){
            MIX_DestroyMixer(mixer);
            mixer = nullptr;
        }

        MIX_Quit();
    }
};

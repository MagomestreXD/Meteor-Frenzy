
class GameState {
    public:
        bool mainMenu= false;
        bool playing = true;
        bool pause = false;
        bool victory = false;
        bool defeat = false;

        void setMainMenu(){
            mainMenu = true;
            playing = false;
            victory = false;
            pause = false;
            defeat = false;
        }

        void setPlaying(){
            mainMenu = false;
            playing = true;
            victory = false;
            pause = false;
            defeat = pause;
        }

        void setVictory(){
            mainMenu = false;
            playing = false;
            victory = true;
            pause = false;
            defeat = false;
        }

        void setDefeat(){
            mainMenu = false;
            playing = false;
            victory = false;
            pause = false;
            defeat = true;
        }
};

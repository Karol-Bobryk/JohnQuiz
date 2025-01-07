#include "quiz.h"
#include "gui.h"

int main(){

    GameState* gs = GameStateInit();
    while(true){
        system("cls");
        if(drawTitle() != 0){
            fprintf(stderr,"[ ERROR ] logo render failed\n");
        }
        drawMenu();
        getMenuChoice(gs);
    }
//    fGetRandomQuestion(gs);
//
//    printGameState(gs);
//
//    freeDecodedQuestion(&(gs->question), &(gs->question));

    GameStateFree(gs);

    return 0;
}

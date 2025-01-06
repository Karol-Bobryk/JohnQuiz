#include "quiz.h"
#include "gui.h"

int main(){

    GameState* gs = GameStateInit();

    drawTitle();
    drawMenu();

//    fGetRandomQuestion(gs);
//
//    printGameState(gs);
//
//    freeDecodedQuestion(&(gs->question), &(gs->question));

    GameStateFree(gs);

    return 0;
}

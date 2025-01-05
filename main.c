#include "quiz.h"

int main(){

    GameState* gs = GameStateInit();

    printTitle();

    drawMenu();

//    fGetRandomQuestion(gs);
//
//    printGameState(gs);
//
//    freeDecodedQuestion(&(gs->question), &(gs->question));

    GameStateFree(gs);

    return 0;
}

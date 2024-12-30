#include "quiz.h"

int main(){

    GameState* gs = GameStateInit();

    fDecodeQuestion(gs->questionsFile, &(gs->question), &(gs->lifelines));
    printGameState(gs);

    GameStateFree(gs);
    return 0;
}

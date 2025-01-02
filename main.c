#include "quiz.h"

int main(){

    GameState* gs = GameStateInit();

    fAppendQuestion(gs->questionsFile);

    GameStateFree(gs);
    return 0;
}

#include "quiz.h"

int main()
{
    GameState* gs = GameStateInit();

    GameStateFree(gs);

    return 0;
}

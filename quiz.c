#include "quiz.h"

const size_t PRIZES[15] = {
        100,
        200,
        300,
        500,
        1000,
        2000,
        4000,
        8000,
        16000,
        32000,
        64000,
        125000,
        250000,
        500000,
        1000000
    };

GameState* GameStateInit(){

    GameState* gs = malloc(sizeof(GameState));

    gs->questionCurId = 0;

    gs->prizeCur=0;
    gs->prizeSecured=0;

    gs->lifelines.is50_50Used = 0;
    gs->lifelines.isAudienceHelpUsed = 0;
    gs->lifelines.isPhoneFriendUsed = 0;

    return gs;
}

void GameStateFree(GameState* gs){

    free(gs);
}

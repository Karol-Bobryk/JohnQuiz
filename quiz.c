#include "quiz.h"

GameState* GameStateInit(){

    GameState* gs = malloc(sizeof(GameState));

    gs->questionCurId = 0;
    gs->questionsLen = 0; // TODO: add a function counting the amount of questions

    gs->lifelines.is50_50Used = 0;
    gs->lifelines.isAudienceHelpUsed = 0;
    gs->lifelines.isPhoneFriendUsed = 0;

    return gs;
}

void GameStateFree(GameState* gs){

    free(gs);
}

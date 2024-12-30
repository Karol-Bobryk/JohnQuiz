#include "quiz.h"

#define DEFAULT_FILE "questions.txt"

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

    gs->prizeCur = 0;
    gs->prizeSecured = 0;

    // initialization of a Lifelines structure
    gs->lifelines.is50_50Used = false;
    gs->lifelines.isAudienceHelpUsed = false;
    gs->lifelines.isPhoneFriendUsed = false;

    gs->lifelines.phoneFriendContentLen = 0;

    // initialization of a Question structure
    for (int i = 0; i < 4; ++i)
        gs->question.answLen[i] = 0;
    gs->question.correctAnsw = 0;
    gs->question.curId = 0;
    gs->question.strContentLen = 0;

    gs->questionsFile = fopen(DEFAULT_FILE, "a+");

    if(gs->questionsFile == NULL)
        fprintf(stderr, "[ ERROR ] File %s not found", DEFAULT_FILE);

    return gs;
}

void GameStateFree(GameState* gs){

    fclose(gs->questionsFile);
    free(gs);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const size_t prizes[] = {
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

typedef struct {
    bool is50_50Used;
    bool isAudienceHelpUsed;
    bool isPhoneFriendUsed;
} Lifelines;

typedef struct {
    size_t questionCurId;
    size_t questionsLen;
    size_t prizeSecured;
    size_t prizeCur;
    size_t prizePotential;

    Lifelines lifelines;
} GameState;

GameState* GameStateInit();

void GameStateFree(GameState*);

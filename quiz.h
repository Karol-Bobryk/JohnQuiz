#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Amount of questions is fixed to 15

#ifndef QUIZ_H
#define QUIZ_H
const size_t PRIZES[15];
#endif

typedef struct {
    bool is50_50Used;
    bool isAudienceHelpUsed;
    bool isPhoneFriendUsed;
} Lifelines;

typedef struct {
    size_t questionCurId;
    size_t prizeSecured;
    size_t prizeCur;

    Lifelines lifelines;
} GameState;

GameState* GameStateInit();

void GameStateFree(GameState*);

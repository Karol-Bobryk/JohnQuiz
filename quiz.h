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

    char* audienceHelpContent;
    size_t audienceHelpContentLen;

} Lifelines;

/*
*   Every question entry in a file should look like this:
*   <Question content>:<Answer 0>:<Answer 1>:<Answer 2>:<Answer 3>:<Correct answer index>:<Audience help content>
*/

typedef struct {
    size_t curId;

    char* strContent;
    size_t strContentLen;

    char* answ[4]; // A list of possible answers
    size_t answLen[4];

    size_t correctAnsw; // An index of the correct answer

} Question;

typedef struct {
    size_t prizeSecured;
    size_t prizeCur;

    Question question;
    Lifelines lifelines;

} GameState;

GameState* GameStateInit();

void GameStateFree(GameState*);

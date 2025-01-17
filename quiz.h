#ifndef QUIZ_H
#define QUIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "gui.h"

typedef struct {
    bool is50_50InUse;
    bool isAudienceHelpInUse;
    bool isPhoneFriendInUse;

    bool is50_50Used;
    bool isAudienceHelpUsed;
    bool isPhoneFriendUsed;

    char* phoneFriendContent;
    size_t phoneFriendContentLen;
    size_t enabledAnswers[2]; // for 50/50 :)

    size_t answBars[4];
} Lifelines;

/*
*   Every question entry in a file should look like this:
*   <Question content>:<Answer 0>:<Answer 1>:<Answer 2>:<Answer 3>:<Correct answer index>:<Phone friend content>\n
*/

typedef struct {
    size_t curId;

    char* strContent;
    size_t strContentLen;

    char* answ[4]; // A list of possible answers
    size_t answLen[4];

    size_t correctAnsw; // An index of the correct answer

} Question;

struct PrizeLL{
    bool isSecure;
    size_t value;
    struct PrizeLL *next;
};

typedef struct PrizeLL PrizeLL;

struct GameState{
    size_t prizeSecured;
    size_t prizeNext;
    PrizeLL* prizeLLCur;

    FILE *questionsFile;
    size_t questionsFileLineCount;
    size_t questionIdBlacklist[15];

    Question question;
    Lifelines lifelines;
    PrizeLL *prizeLL;

};

typedef struct GameState GameState;


GameState* GameStateInit();

void GameStateReset(GameState *);

void GameStateFree(GameState*);

size_t fGetSegLen(FILE*);

void printLifelines(Lifelines*);

void printQuestion(Question*);

void printGameState(GameState*);

int fDecodeQuestion(FILE*, Question*, Lifelines*);

void freeDecodedQuestion(Question*, Lifelines*);

int fAppendQuestion(FILE*);

void strTrimNewline(char*);

size_t fCountLines(FILE*);

int fGetRandomQuestion(GameState*);

size_t getRandomQuestionId(size_t [15], size_t, size_t);

int mainGameLoop(GameState*);

bool handleQuestionInput(GameState*);

PrizeLL* PrizeLLInit();

void PrizeLLFree(PrizeLL*);

#endif

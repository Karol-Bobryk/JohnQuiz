#include "quiz.h"

// this define below is here due to the mingw gcc problems with %zu specifier
#define printf __mingw_printf

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

/*
*   GameStateInit
*       initializes a gamestate structure
*
*   return value:
*       pointer to a gamestate structure that was initialized
*
*/
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
    for (size_t i = 0; i < 4; ++i)
        gs->question.answLen[i] = 0;
    gs->question.correctAnsw = 0;
    gs->question.curId = 0;
    gs->question.strContentLen = 0;

    gs->questionsFile = fopen(DEFAULT_FILE, "a+");
    if(gs->questionsFile == NULL){
        fprintf(stderr, "\n[ ERROR ] Cannot open %s\n", DEFAULT_FILE);
        exit(EXIT_FAILURE);
    }

    return gs;
}

/*
*   GameStateFree
*       frees a gamestate structure
*
*   arguments:
*       gs - gamestate structured to be freed
*
*/
void GameStateFree(GameState* gs){

    fclose(gs->questionsFile);
    free(gs);
}

/*
*   fGetSegLen
*       counts the length of a single segment delimited by a colon or a newline,
*       then returns the pointer to the original location
*
*   arguments:
*       f - file containing a segment
*
*   return value:
*       size_t length of a segment
*
*/
size_t fGetSegLen(FILE *f){

    size_t startPos = ftell(f);

    size_t segLen = 0;
    char c;

    while((c = fgetc(f)) != EOF){
        if(c == ':' || c == '\n'){
            break;
        }
    }

    segLen = ftell(f) - startPos - 1;

    fseek(f, startPos, SEEK_SET);

    return segLen;
}

/*
*   fDecodeQuestion
*       decodes questions and answers from a file into Question structure
*       NOTE: it does not increment the curId
*
*   arguments:
*       qf - file containing questions,
*           function assumes file pointer is already
*           at the beginning of a desired line
*       q - a reference to a Question structure
*       ll - a reference to a Lifelines structure
*
*   return value:
*       0 - successful execution
*       -1 - failed execution
*
*/
int fDecodeQuestion(FILE* qf, Question* q, Lifelines* ll){

    // reading the content and length of a question
    q->strContentLen = fGetSegLen(qf);
    q->strContent = malloc(q->strContentLen + 1);

    fread(q->strContent, sizeof(char), q->strContentLen, qf);
    q->strContent[q->strContentLen] = '\0';

    fseek(qf, 1, SEEK_CUR);

    // reading the string content and length of the answers
    for(size_t i = 0; i < 4; ++i){
        q->answLen[i] = fGetSegLen(qf);

        q->answ[i] = malloc(q->answLen[i]+1);

        fread(q->answ[i], sizeof(char), q->answLen[i], qf);
        q->answ[i][q->answLen[i]] = '\0';

        fseek(qf, 1, SEEK_CUR);
    }

    // reading the correct answer index
    fscanf(qf, "%d", &(q->correctAnsw));

    fseek(qf, 1, SEEK_CUR);

    // reading the phone to a friend content and length
    // NOTE: this subtraction is here due to this being the last entry,
    //       this prevents newline from being included
    ll->phoneFriendContentLen = fGetSegLen(qf) - 1;
    ll->phoneFriendContent = malloc(ll->phoneFriendContentLen + 1);

    fread(ll->phoneFriendContent, sizeof(char), ll->phoneFriendContentLen, qf);
    ll->phoneFriendContent[ll->phoneFriendContentLen] = '\0';

    fseek(qf, 1, SEEK_CUR);

    return 0;
}

/*
*   printLifelines
*       Prints the content of the Lifelines structure
*
*   arguments:
*       ll - pointer to the Lifelines structure
*
*/
void printLifelines(Lifelines* ll) {

    printf("[ INFO ] Lifelines:\n");
    printf("[ INFO ]   is50_50Used: %s\n", ll->is50_50Used ? "true" : "false");
    printf("[ INFO ]   isAudienceHelpUsed: %s\n", ll->isAudienceHelpUsed ? "true" : "false");
    printf("[ INFO ]   isPhoneFriendUsed: %s\n", ll->isPhoneFriendUsed ? "true" : "false");

    if (ll->phoneFriendContent != NULL) {
        printf("[ INFO ]   phoneFriendContent: %s\n", ll->phoneFriendContent);
    } else {
        printf("[ INFO ]   phoneFriendContent: NULL\n");
    }
    printf("[ INFO ]   phoneFriendContentLen: %zu\n", ll->phoneFriendContentLen);
}

/*
*   printQuestion
*       Prints the content of the Question structure
*
*   arguments:
*       q - pointer to the Question structure
*
*/
void printQuestion(Question* q) {

    printf("[ INFO ] Question:\n");
    printf("[ INFO ]   curId: %zu\n", q->curId);

    if (q->strContent != NULL) {
        printf("[ INFO ]   strContent: %s\n", q->strContent);
    } else {
        printf("[ INFO ]   strContent: NULL\n");
    }
    printf("[ INFO ]   strContentLen: %zu\n", q->strContentLen);

    printf("[ INFO ]   Answers:\n");
    for (size_t i = 0; i < 4; i++) {
        printf("[ INFO ]     Answer %zu: %s (Length %zu)\n", i, q->answ[i], q->answLen[i]);
    }

    printf("[ INFO ]   Correct Answer Index: %zu\n", q->correctAnsw);
}

/*
*   printGameState
*       Prints the content of the GameState structure
*
*   arguments:
*       gs - pointer to the GameState structure
*
*/
void printGameState(GameState* gs) {

    printf("[ INFO ] GameState:\n");
    printf("[ INFO ]   prizeSecured: %zu\n", gs->prizeSecured);
    printf("[ INFO ]   prizeCur: %zu\n", gs->prizeCur);

    if (gs->questionsFile != NULL) {
        printf("[ INFO ]   questionsFile: %p\n", (void*)gs->questionsFile);
    } else {
        printf("[ INFO ]   questionsFile: NULL\n");
    }

    printQuestion(&gs->question);
    printLifelines(&gs->lifelines);
}

/*
*   freeDecodedQuestion
*       frees mallocd fields of Question and Lifelines structure after it was decoded,
*       obligatory after every use of fDecodeQuestion
*
*   arguments:
*       q - a reference to a Question structure
*       ll - a reference to a Lifelines structure
*
*/
void freeDecodedQuestion(Question* q, Lifelines* ll){
    free(q->strContent);
    for(size_t i = 0; i < 4; ++i)
        free(q->answ[i]);
    free(ll->phoneFriendContent);
}

#define MAX_QUESTION_SIZE 256
int fAppendQuestion(FILE* f){

    char sBuf[MAX_QUESTION_SIZE];
    while(true){

        printf("Provide question content: ");
        if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
            printf("\nQuestion cannot be longer than %d, please try again\n", MAX_QUESTION_SIZE - 1);
            continue;
        }
        strTrimNewline(sBuf);

        if((fputs(sBuf, f) < 0) || (fputc(':', f) < 0)){
            fprintf(stderr, "\n[ ERROR ] Failed to write to %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    for(size_t i = 0; i < 4; ++i){
        while(true){

            printf("Provide answer %zu content: ", i);
            if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
                printf("\nAnswer %zu cannot be longer than %d, please try again\n", i, MAX_QUESTION_SIZE - 1);
                continue;
            }
            strTrimNewline(sBuf);

            if((fputs(sBuf, f) < 0) || (fputc(':', f) < 0)){
                fprintf(stderr, "\n[ ERROR ] Failed to write to %s\n", DEFAULT_FILE);
                return -1;
            }
            break;
        }
    }

   while(true){

        size_t corAnsw;

        printf("Provide correct answer index: ");
        if((scanf("%d", &corAnsw) != 1)){
            printf("\nCorrect answer index has to be an integer\n");
            getchar();
            continue;
        }

        if(corAnsw < 0 || corAnsw > 3){
            printf("\nPlease, provide the correct index range <0,3>\n");
            continue;
        }
        getchar();

        if(fprintf(f, "%d:", corAnsw) < 0){
            fprintf(stderr, "\n[ ERROR ] Failed to write to %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    while(true){

        printf("Provide phone friend lifeline content: ");
        if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
            printf("\nPhone friend lifeline content cannot be longer than %d, please try again\n", MAX_QUESTION_SIZE - 1);
            continue;
        }

        if(fputs(sBuf, f) < 0){
            fprintf(stderr, "\n[ ERROR ] Failed to write to %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    return 0;
}

void strTrimNewline(char* sBuf){

    size_t len = strlen(sBuf);
    if (len > 0 && sBuf[len - 1] == '\n') {
        sBuf[len - 1] = '\0';
    }
}

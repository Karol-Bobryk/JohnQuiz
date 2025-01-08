#include "quiz.h"

// this define below is here due to the mingw gcc problems with %zu specifier
#ifdef __MINGW32__
#define printf __mingw_printf
#endif

#define DEFAULT_FILE "questions.txt"

#define BUFFER_SIZE 1024 // size of buffer for count lines in a file

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

    gs->prizeCur = PRIZES[0];
    gs->prizeNext = PRIZES[1];
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

    gs->questionsFileLineCount = fCountLines(gs->questionsFile);

    for (size_t i = 0; i < 15; ++i) {
        gs->questionIdBlacklist[i] = 0;
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

/*
*   fCountLines
*       counts the amount of newlines (\n) in the file we're using to read questions from.
*
*   arguments:
*       file - text file passed as input (presumably with questions inside idk i only work here)
*
*   return value:
*       countNewLines - the number of newlines in the file specified above
*/
size_t fCountLines(FILE* file){
    char buffer[BUFFER_SIZE];
    rewind(file);
    size_t countNewLines = 0;

    while(!feof(file)){
        size_t currentBuffer = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        for(size_t i = 0; i < currentBuffer; i++){
            if (buffer[i] == '\n') ++countNewLines;
        }
    }
    return countNewLines;
}

/*
*   fGetRandomQuestion
*       gets a random question from the question file and puts it inside the Question structure
*
*   arguments:
*       gs - current GameState structure
*
*   return value:
*       returns zero for a successful execution
*/
#define QUESTION_BUFFER_SIZE (MAX_QUESTION_SIZE * 6 + 2 + 1)
int fGetRandomQuestion(GameState* gs){

    srand(time(NULL));

    size_t qId = getRandomQuestionId(gs->questionIdBlacklist, gs->question.curId + 1, gs->questionsFileLineCount);

    rewind(gs->questionsFile);

    char buf[QUESTION_BUFFER_SIZE];
    // this loop skips entire lines up until it is at the beginning of the desired line
    for(size_t i = 0; i < qId; ++i){
        fgets(buf , QUESTION_BUFFER_SIZE, gs->questionsFile);
    }

    fDecodeQuestion(gs->questionsFile, &(gs->question), &(gs->lifelines));

    ++(gs->question.curId);

    return 0;
}

size_t getRandomQuestionId(size_t blacklist[15], size_t curId, size_t lineCount){ // TODO: THIS DOES NOT WORK FOR SOME REASON

    size_t qId;
    bool isBlacklisted = false;
    while(true){
        qId = (rand() % lineCount);
        isBlacklisted = false;

        for(size_t i = 0; i < (curId + 1); ++i){
            if (blacklist[i] == qId){
                isBlacklisted = true;
                break;
            }
        }

        if(!isBlacklisted)
            return qId;
    }
}

int mainGameLoop(GameState *gs){

    for(size_t i = 0; i < 15; ++i){

        fGetRandomQuestion(gs);

        if(i == 14){
            gs->prizeCur = PRIZES[i];
            gs->prizeNext = 0;
        }
        else{
            gs->prizeCur = PRIZES[i];
            gs->prizeNext = PRIZES[i + 1];
        }

        if(!handleQuestionInput(gs)){
            return 0; // THIS IS WHERE LOSS IS PROCESSED
        }

        if(i == 4 || i ==9)
            gs->prizeSecured = PRIZES[i];

        freeDecodedQuestion(&(gs->question), &(gs->lifelines));
    }
    return 0;
}

bool handleQuestionInput(GameState* gs){

    char ch;

    SimpleGuiSelectedItem selectedItem = AnsA;
    const size_t enumSize = 7;

    printSimpleGameGui(gs, selectedItem, false);

    while(1){
            ch = getch();

            switch (ch) {
                case 'W':
                case 'w':
                    selectedItem = (selectedItem == 0) ? enumSize - 1 : selectedItem - 1;
                    printSimpleGameGui(gs, selectedItem, false);
                    break;

                case 'S':
                case 's':
                    selectedItem = (selectedItem + 1) % enumSize;
                    printSimpleGameGui(gs, selectedItem, false);
                    break;

                case 13: // decimal for enter
                    if(selectedItem >= AnsA && selectedItem <= AnsD){
                        printSimpleGameGui(gs, selectedItem, true);
                        printf("\n\t Kliknij aby przejsc dalej.");
                        getch();
                        return gs->question.correctAnsw == selectedItem;
                    }
                    // TODO: add lifelines logic
                    break;
            }
    }
}

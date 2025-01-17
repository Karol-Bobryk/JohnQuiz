#include "quiz.h"
#include "gui.h"

// this define below is here due to the mingw gcc problems with %zu specifier
#ifdef __MINGW32__
#define printf __mingw_printf
#endif

#define DEFAULT_FILE "questions.txt"

#define BUFFER_SIZE 1024 // size of buffer for count lines in a file

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

    gs->questionsFile = fopen(DEFAULT_FILE, "a+");
    if(gs->questionsFile == NULL){
        fprintf(stderr, "\n[ ERROR ] Cannot open %s\n", DEFAULT_FILE);
        exit(EXIT_FAILURE);
    }

    gs->prizeLL = PrizeLLInit();

    GameStateReset(gs);

    return gs;
}
/*
*   GameStateReset
*       resets provided GameState to base values.
*
*   arguments
*       *gs - GameState to be reset.
*
*
*/
void GameStateReset(GameState *gs){

    gs->prizeLLCur = gs->prizeLL;
    gs->prizeNext = gs->prizeLL->next->value;
    gs->prizeSecured = 0;

    // initialization of a Lifelines structure
    gs->lifelines.is50_50InUse = false;
    gs->lifelines.isAudienceHelpInUse = false;
    gs->lifelines.isPhoneFriendInUse = false;

    gs->lifelines.enabledAnswers[0] = 0;
    gs->lifelines.enabledAnswers[1] = 0;

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

    gs->questionsFileLineCount = fCountLines(gs->questionsFile);

    for (size_t i = 0; i < 15; ++i) {
        gs->questionIdBlacklist[i] = 0;
    }
    for (size_t i = 0; i < 4; ++i) {
        gs->lifelines.answBars[i] = 0;
    }
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

    PrizeLLFree(gs->prizeLL);
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
    printf("[ INFO ]   prizeCur: %zu\n", gs->prizeLLCur->value);

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

/*
*   fAppendQuestion
*       in order to add a question to the given file, this function takes input for all the necessary fields,
        concatenates them into one buffer and appends that buffer to the file as one question.
*
*   arguments:
*       f - file to write the question to.
*
*   return value:
*       0 for success
*       -1 in case of an error
*/
#define MAX_QUESTION_SIZE 256
int fAppendQuestion(FILE* f){

    system("cls");

    char sBuf[MAX_QUESTION_SIZE];
    while(true){

        printf("Podaj tresc pytania: ");
        if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
            printf("\nPytanie nie moze byc dluzsze niz %d, prosze sprobuj ponownie\n", MAX_QUESTION_SIZE - 1);
            continue;
        }
        strTrimNewline(sBuf);

        if((fputs(sBuf, f) < 0) || (fputc(':', f) < 0)){
            fprintf(stderr, "\n[ ERROR ] Nie udalo sie zapisac %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    for(size_t i = 0; i < 4; ++i){
        while(true){

            printf("Podaj zawartosc odpowiedzi numer %zu: ", i);
            if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
                printf("\nOdpowiedz %zu nie moze byc dluzsza niz %d, prosze sprobuj ponownie\n", i, MAX_QUESTION_SIZE - 1);
                continue;
            }
            strTrimNewline(sBuf);

            if((fputs(sBuf, f) < 0) || (fputc(':', f) < 0)){
                fprintf(stderr, "\n[ ERROR ] Nie udalo sie zapisac do %s\n", DEFAULT_FILE);
                return -1;
            }
            break;
        }
    }

   while(true){

        size_t corAnsw;

        printf("Podaj indeks poprawnej odpowiedzi: ");
        if((scanf("%d", &corAnsw) != 1)){
            printf("\nIndeks odpowiedzi musi byc liczba calkowita\n");
            getchar();
            continue;
        }

        if(corAnsw < 0 || corAnsw > 3){
            printf("\nProsze, podaj liczbe w zakresie <0,3>\n");
            continue;
        }
        getchar();

        if(fprintf(f, "%d:", corAnsw) < 0){
            fprintf(stderr, "\n[ ERROR ] Nie udalo sie zapisac do %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    while(true){

        printf("Podaj zawartosc pytania do przyjaciela: ");
        if(fgets(sBuf, MAX_QUESTION_SIZE, stdin) == NULL){
            printf("\nZawartosc pytania do przyjaciela nie moze byc dluzsza niz %d, prosze sprobuj ponownie\n", MAX_QUESTION_SIZE - 1);
            continue;
        }

        if(fputs(sBuf, f) < 0){
            fprintf(stderr, "\n[ ERROR ] Nie udalo sie zapisac do %s\n", DEFAULT_FILE);
            return -1;
        }
        break;
    }

    return 0;
}

/*
*   strTrimNewLine
*       given a certain string, it trims it if the last character is a newline.
*
*   arguments
*       sBuf - string to trim.
*/
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

    size_t qId = getRandomQuestionId(gs->questionIdBlacklist, gs->question.curId, gs->questionsFileLineCount);

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

/*
*   getRandomQuestionId
*       grabs a random non-blacklisted question's ID, and blacklists the current question
*       so that the user can't encounter duplicate questions.
*
*   arguments:
*       blacklist - array[15] of integers signifying indexes of blacklisted questions
*       curId - index of current questions
*       lineCount - count of lines in the question file
*
*   return value:
*       returns the next random question ID.
*/
size_t getRandomQuestionId(size_t blacklist[15], size_t curId, size_t lineCount){

    size_t qId;
    bool isBlacklisted = false;

    qId = (rand() % lineCount);

    while(true){
        isBlacklisted = false;

        for(size_t i = 0; i < curId; ++i){

            if (blacklist[i] == qId){
                qId = (qId + 1) % lineCount;
                isBlacklisted = true;
                break;
            }

        }
        if(!isBlacklisted){
            blacklist[curId] = qId;
            return qId;
        }
    }
}
/*
*   mainGameLoop
*       handles the main game loop - picking and displaying questions, setting reward values
*
*   arguments:
*       gs - currentGameState
*
*   return value:
*       0 after showing the endGame screen.
*/
int mainGameLoop(GameState *gs){

    for(size_t i = 0; i < 15; ++i){

        fGetRandomQuestion(gs);

        if(i != 0 ){
            gs->prizeLLCur = gs->prizeLLCur->next;

            if(i != 14)
                gs->prizeNext = gs->prizeLLCur->next->value;
        }

        gs->lifelines.is50_50InUse = false;
        gs->lifelines.isAudienceHelpInUse = false;
        gs->lifelines.isPhoneFriendInUse = false;

        if(!handleQuestionInput(gs)){
            showGameOverScreen(gs);
            return 0;
        }

        if(gs->prizeLLCur->isSecure)
            gs->prizeSecured = gs->prizeLLCur->value;



        freeDecodedQuestion(&(gs->question), &(gs->lifelines));
    }

    showVictoryScreen(gs);

    return 0;
}

/*
*   handleQuestionInput
*       handles input from the user, ie. picking questions with wasd, and picking lifelines (1-3)
*
*   arguments:
*       gs - currentGameState
*
*   return value:
*       true / false - true if the user chooses correctly, false if not.
*/
#define ENABLED gs->lifelines.enabledAnswers
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
                        if (selectedItem != ENABLED[0] && selectedItem != ENABLED[1] && gs->lifelines.is50_50InUse)
                            continue;
                        printSimpleGameGui(gs, selectedItem, true);
                        printf("\n\t Kliknij aby przejsc dalej.");
                        getch();
                        return gs->question.correctAnsw == selectedItem;
                    }
                    if(selectedItem == LLAudHelp && !gs->lifelines.isAudienceHelpUsed){
                        gs->lifelines.isAudienceHelpInUse = true;
                        gs->lifelines.isAudienceHelpUsed = true;

                        srand(time(NULL));

                        for(size_t i = 0; i < 4; ++i){
                                if(i == gs->question.correctAnsw)
                                    gs->lifelines.answBars[i] = ( rand()%6 ) + 11;
                                else
                                    gs->lifelines.answBars[i] = ( rand()%8 ) + 1;
                        }

                        printSimpleGameGui(gs, selectedItem, false);
                    }

                    if(selectedItem == LLPhone && !gs->lifelines.isPhoneFriendUsed){
                        gs->lifelines.isPhoneFriendInUse = true;
                        gs->lifelines.isPhoneFriendUsed = true;
                        printSimpleGameGui(gs, selectedItem, false);
                    }

                    if(selectedItem == LL50_50 && !gs->lifelines.is50_50Used){
                        gs->lifelines.is50_50InUse = true;
                        gs->lifelines.is50_50Used = true;

                        ENABLED[0] = gs->question.correctAnsw;
                        ENABLED[1] = ((rand() % 3 )+gs->question.correctAnsw + 1)%4;

                        printSimpleGameGui(gs, selectedItem, false);
                    }

                    break;
            }
    }
}
/*
*   PrizeLLInit
*       initializes a linked list of Prizes
*
*   return value:
*       pointer to the head node of the list.
*
*/
PrizeLL* PrizeLLInit(){
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

    PrizeLL *pLL = malloc(sizeof(PrizeLL));

    PrizeLL *curNode = pLL;

    for(size_t i = 0; i<15; ++i){
        curNode->value = PRIZES[i];
        curNode->isSecure = (i == 4 || i == 9);
        curNode->next = (i != 14) ? malloc(sizeof(PrizeLL)) : NULL;
        curNode = curNode->next;
    }

    return pLL;
}

/*
*   PrizeLLFree
*       Frees an entire linked list of Prizes.
*
*   arguments
*       pLL - pointer to the head of the array
*
*/
void PrizeLLFree(PrizeLL* pLL){

    PrizeLL* curNode = pLL;
    PrizeLL* nxtNode;

    while(curNode != NULL){
        nxtNode = curNode->next;
        free(curNode);
        curNode = nxtNode;
    }
}

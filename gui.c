#include "gui.h"
#include "quiz.h"
int titlePadding = 0;

/*
*   getWindowWidth
*       calculates the number of columns in the console window
*
*   return value:
*       returns number of columns in the console window
*
*/
int getWindowWidth(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)){
        return -1;
    }

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

/*
*   drawTitle
*       prints a centred title from the title.txt file
*
*   return value:
*       0 - successful execution
*       -1 - failed execution
*/
#define TITLE_FILE "title.txt"
int drawTitle(){
    int windowWidth = getWindowWidth();

    if(windowWidth == -1){
        fprintf(stderr,"[ ERROR ] Failed to get window width\n");
        return -1;
    }

    FILE* titleFile = fopen(TITLE_FILE, "r");
    if(titleFile == NULL){
        fprintf(stderr,"[ ERROR ] Failed to open title file\n");
        return -1;
    }

    printf("\n");

    char line[1024];
    int padding = 0;

    while(fgets(line, sizeof(line), titleFile)){
        strTrimNewline(line);

        padding = (windowWidth - strlen(line)) / 2;

        for(size_t i = 0; i < padding; ++i){
            printf(" ");
        }

        printf("%s\n", line);

        titlePadding = padding;
    }

    printf("\n");

    fclose(titleFile);

    return 0;
}

/*
*   drawMenu
*       prints the options for user to choose
*/
void drawMenu(){
    const char* menuItems[4] = {
        "Graj",
        "Dodaj pytanie",
        "O grze",
        "Wyjscie"
    };

    for(size_t i = 0; i < 4; ++i){
        for(size_t i = 0; i < titlePadding; ++i){
            printf(" ");
        }

        printf("%d. %s\n", (i+1), menuItems[i]);
    }
}

/*
*   getMenuChoice
*       handles user input when dealing with the menu
*
*   arguments:
*       gs - current GameState
*/
void getMenuChoice(GameState *gs){
    char selectedOption = getch();

    switch(selectedOption){
    case '1':{
        mainGameLoop(gs);
        GameStateReset(gs);
        break;
    }
    case '2':
        printf("placeholder");
        getch();
        break;
    case '3':
        showAboutGameScreen();
        break;
    case '4':
        GameStateFree(gs);
        exit(EXIT_SUCCESS);
    default:
        printf("You have to provide a valid number");
        getch();
    }
}

/*
*   printPadding
*       prints a given number of newlines to serve as padding
*
*   arguments:
*       padding - number of newlines to be printed out
*/
void printPadding(size_t padding){
    for(size_t i = 0; i < padding; ++i)
        printf(" ");

}

/*
*   printCenteredText
*       outputs a given string of characters with an appropriate amount of
*       padding so that it is centered in the terminal
*
*   arguments:
*       textContent - text to be centered
*       padding - number of lines to be used as padding
*/
void printCenteredText(const char* textContent, int padding) {
    char* textCopy = strdup(textContent);
    char* word = strtok(textCopy, " ");

    char* words[100];
    int wordCount = 0;

    while (word != NULL) {
        words[wordCount] = strdup(word);
        wordCount++;
        word = strtok(NULL, " ");
    }

    int textBlockWidth = getWindowWidth() - (2 * padding);
    int charactersInLine = 0;

    for (size_t i = 0; i < wordCount; ++i) {
        if (charactersInLine == 0) {
            printPadding(padding);
        }

        printf("%s ", words[i]);
        charactersInLine += strlen(words[i]) + 1; // 1 for space

        if (i + 1 < wordCount && (charactersInLine + strlen(words[i + 1])) >= textBlockWidth) {
            printf("\n");
            charactersInLine = 0;
        }
    }

    for (int i = 0; i < wordCount; i++) {
        free(words[i]);
    }
    free(textCopy);
}

/*
*   showAboutGameScreen
*       displays "O grze" screen
*/
void showAboutGameScreen(){
    system("cls");

    drawTitle();

    char* text = "Autorzy: Karol Bobryk, Marcel Alefierowicz, Patryk Wojtkielewicz, studenci informatyki 1 roku na wydziale informatyki Politechniki Bialostockiej, Pracownia Specjalistyczna #4";

    printCenteredText(text, titlePadding);

    printf("\n\n");
    printPadding(titlePadding);
    printf("Wcisnij ESCAPE aby powrocic do menu");

    char buttonPressed = 0;

    while(1){
        buttonPressed = getch();
        if(buttonPressed != 27){
            continue;
        }

        return;
    }

}

#define ANSI_BLACK_BACKGROUND "\x1b[40m"
#define ANSI_BLUE_BACKGROUND "\x1b[44m"

#define ANSI_BLUE_TEXT "\x1b[34m"
#define ANSI_WHITE_TEXT "\x1b[37m"
#define ANSI_GREEN_TEXT "\x1b[32m"
#define ANSI_BLACK_TEXT "\x1b[30m"

#define ANSI_RED_BACKGROUND "\x1b[41m"
#define ANSI_GREEN_BACKGROUND "\x1b[42m"
// this one and the one below have %s inside
#define ANSI_RED_BACKGROUND_TEXT "\x1b[41m%25s\x1b[40m"
#define ANSI_GREEN_BACKGROUND_TEXT "\x1b[42m%25s\x1b[40m"

/*
*   printAudienceHelp
*       outputs a 4 bar stats screen showing the audience's opinion
*       on what the correct answer is
*
*   arguments:
*       gs - current GameState
*/
void printAudienceHelp(GameState *gs){
    printf("\n\nWyniki glosowania publicznosci: \n");

    for(size_t i = 0; i < 4; ++i){
        if(gs->lifelines.is50_50InUse && ((i != gs->lifelines.enabledAnswers[0] && i != gs->lifelines.enabledAnswers[1]))){
            continue;
        }

        printf("\nOdpowiedz %c: ", 'A'+i);

        if(i == gs->question.correctAnsw)
            printf(ANSI_GREEN_TEXT);

        for(size_t j = 0; j < gs->lifelines.answBars[i]; ++j)
            printf("%c", 219); // ASCII for fullblock char

        printf(ANSI_WHITE_TEXT);
    }

}

/*
*   printSimpleGameGui
*       prints the GUI of the round being played, in 2 states:
*            - unconfirmed: the player still hasnt selected their answer and can choose to use a lifeline if needed.
*
*            - confirmed: the player has selected their choice and can now see whether his choice was correct.
*                         if correct, the player proceeds to the next round.
*   arguments:
*       gs - current GameState
*       selectedItem - item to be rendered (highlighted) as currently selected
*       isConfirmed - bool controling the round state
*/
void printSimpleGameGui(GameState *gs, SimpleGuiSelectedItem selectedItem, bool isConfirmed){
    system("cls");

    printf("\n");
    printf("%sGrasz o: $%-22d%s", ANSI_GREEN_TEXT, gs->prizeCur, ANSI_WHITE_TEXT);
    printf("\t");
    printf("%sNagroda gwarantowana: $%-11d%s", ANSI_GREEN_TEXT, gs->prizeSecured, ANSI_WHITE_TEXT);
    if(gs->question.curId != 15){
        printf("\t");
        printf("%sNagroda w nastepnej rundzie: $%d%s", ANSI_GREEN_TEXT, gs->prizeNext, ANSI_WHITE_TEXT);
    }
    printf("\n");
    printf("\n");
    printf("%s\n",gs->question.strContent);
    printf("\n");

    // rendering the selectable answers:

    for(size_t i = 0; i < 4; ++i){

        if(isConfirmed)
            printf((i == gs->question.correctAnsw) ? ANSI_GREEN_BACKGROUND : ANSI_RED_BACKGROUND);
        else if(selectedItem == i)
            printf(ANSI_BLUE_BACKGROUND);

        bool hideText= (gs->lifelines.is50_50InUse
           && i != gs->lifelines.enabledAnswers[0]
           && i != gs->lifelines.enabledAnswers[1]);

        if(hideText){
           printf("\t%c. %s\n", 'A' + i,ANSI_BLACK_BACKGROUND);
        } else {
            printf("\t%c.%s %s\n", 'A' + i, ANSI_BLACK_BACKGROUND, gs->question.answ[i]);
        }
    }
    printf("\n");

    // printing available lifelines:

    if(selectedItem == LL50_50)
            printf(ANSI_BLUE_BACKGROUND);
    printf("1.");
    printf(gs->lifelines.is50_50Used ? ANSI_RED_BACKGROUND_TEXT : ANSI_GREEN_BACKGROUND_TEXT, "50/50 ");
    printf("\t");

    if(selectedItem == LLAudHelp)
            printf(ANSI_BLUE_BACKGROUND);
    printf("2.");
    printf(gs->lifelines.isAudienceHelpUsed ? ANSI_RED_BACKGROUND_TEXT : ANSI_GREEN_BACKGROUND_TEXT, "Pytanie do publicznosci ");
    printf("\t");

    if(selectedItem == LLPhone)
            printf(ANSI_BLUE_BACKGROUND);
    printf("3.");
    printf(gs->lifelines.isPhoneFriendUsed ? ANSI_RED_BACKGROUND_TEXT : ANSI_GREEN_BACKGROUND_TEXT, "Telefon do przyjaciela ");
    printf(ANSI_BLACK_BACKGROUND);

    // checking to see if audience help or phone friend is in use, then rendering their content:

    if(gs->lifelines.isAudienceHelpInUse == true){
        printAudienceHelp(gs);
    }

    if(gs->lifelines.isPhoneFriendInUse == true){
        printf("\n%s%s%s",
               ANSI_GREEN_TEXT, gs->lifelines.phoneFriendContent, ANSI_WHITE_TEXT);
    }

    printf("\n\n");
    printf("%sUzywaj w/s oraz ENTER aby sie poruszac!%s", ANSI_BLUE_TEXT, ANSI_WHITE_TEXT);
    printf("\n");

}

/*
*   showGameOverScreen
*       prints the game over screen
*   arguments:
*       gs - current GameState
*/
void showGameOverScreen(GameState *gs){
    system("cls");

    drawTitle();

    char* header = "Przegrales!!";

    printf("\n");
    printPadding((getWindowWidth()-strlen(header))/2);
    printf("%s", header);

    printf("\n\n");
    printPadding(titlePadding);
    printf("Liczba prawidlowych odpowiedzi: %d", gs->question.curId-1);

    printf("\n\n");
    printPadding(titlePadding);
    printf("Wygrana suma: %s%d$%s, Gratulujemy!", ANSI_GREEN_TEXT, gs->prizeSecured, ANSI_WHITE_TEXT);

    printf("\n\n");
    printCenteredText("Wcisnij ESCAPE aby powrocic do menu", titlePadding);

    while(1){
        if(getch() == 27)
            return;
    }
}

/*
*   showVictoryScreen
*       prints the victory screen
*   arguments:
*       gs - current GameState
*/
void showVictoryScreen(GameState *gs){
    system("cls");

    drawTitle();

    char* header = "Wygrales!";

    printf("\n");
    printPadding((getWindowWidth()-strlen(header))/2);
    printf("%s", header);

    printf("\n\n");
    printPadding(titlePadding);
    printf("Odpoweidziales prawidlowo na wszystkie pytania.");

    int usedLifelines = 0;
    if(gs->lifelines.is50_50Used) usedLifelines++;
    if(gs->lifelines.isAudienceHelpUsed) usedLifelines++;
    if(gs->lifelines.isPhoneFriendUsed) usedLifelines++;

    printf("\n\n");
    printPadding(titlePadding);
    printf("Wykorzystales %d kola ratunkowe.", usedLifelines);

    printf("\n\n");
    printPadding(titlePadding);
    printf("Twoja nagroda: %s1 000 000$%s, Gratulujemy!", ANSI_GREEN_TEXT, ANSI_WHITE_TEXT);

    printf("\n\n");
    printCenteredText("Wcisnij ESCAPE aby powrocic do menu", titlePadding);

    while(1){
        if(getch() == 27)
            return;
    }
}

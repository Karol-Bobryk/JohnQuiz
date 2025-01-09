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
*   showAboutGameScreen
*       displays "O grze" screen
*/
void showAboutGameScreen(){
    system("cls");

    drawTitle();

    char* text = "Autorzy: Karol Bobryk, Marcel Alefierowicz, Patryk Wojtkielewicz, studenci informatyki 1 roku na wydziale informatyki Politechniki Bialostockiej";
    int textBlockWidth = getWindowWidth() - (2*titlePadding);

    for(size_t i = 0; i < strlen(text); ++i){
        if( i == 0 ||  (i+1)%textBlockWidth == 0 ){
            printf("\n");
            for(size_t j = 0; j < titlePadding; ++j)
                printf(" ");
        }


        printf("%c", text[i]);
    }

    printf("\n\n");
    for(size_t i = 0; i < titlePadding; ++i)
                printf(" ");
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

// good luck writing docs for this

// @hightower

void printAudienceHelp(GameState *gs){
    printf("\n\nWyniki glosowania publicznosci: \n", ANSI_GREEN_TEXT, ANSI_WHITE_TEXT);

    srand(time(NULL));

    // FIX: bar length changes every time screen is reprinted (as to be expected)

    int barLength = 0;

    for(size_t i = 0; i < 4; ++i){
        if(i == gs->question.correctAnsw)
            barLength = ( rand()%6 ) + 11;
        else
            barLength = ( rand()%8 ) + 1;

        printf("\nOdpowiedz %c: ", 'A'+i);

        for(size_t j = 0; j < barLength; ++j)
            printf("%c", 219);
    }

}

void printSimpleGameGui(GameState *gs, SimpleGuiSelectedItem selectedItem, bool isConfirmed){

    system("cls");

    printf("\n");
    printf("%sGrasz o: $%-22d%s", ANSI_GREEN_TEXT, gs->prizeCur, ANSI_WHITE_TEXT);
    printf("\t");
    printf("%sNagroda gwarantowana: $%-11d%s", ANSI_GREEN_TEXT, gs->prizeSecured, ANSI_WHITE_TEXT);
    if(gs->question.curId != 15){ // :D
        printf("\t");
        printf("%sNagroda w nastepnej rundzie: $%d%s", ANSI_GREEN_TEXT, gs->prizeNext, ANSI_WHITE_TEXT);
    }
    printf("\n");
    printf("\n");
    printf("%s\n",gs->question.strContent);
    printf("\n");

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

    // lifelines options pompa

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

    // lifeline printington:

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

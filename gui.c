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
    case '1':
        printf("placeholder");
        getch();
        break;
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

    printf("Autorzy: Karol Bobryk, Marcel Alefierowicz, Patryk Wojtkielewicz, studenci informatyki 1 roku na wydziale informatyki.\n\n");

    printf("Wcisnij ESCAPE aby powrocic do menu.");

    char buttonPressed = 0;

    while(1){
        buttonPressed = getch();
        if(buttonPressed != 27){
            continue;
        }

        return;
    }

}


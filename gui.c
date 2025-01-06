#include "gui.h"

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
int drawTitle(){
    int windowWidth = getWindowWidth();

    if(windowWidth == -1){
        printf("Failed to get window width");
        return -1;
    }

    FILE* titleFile = fopen("title.txt", "r");
    if(titleFile == NULL){
        printf("Failed to open file");
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

    int windowWidth = getWindowWidth();

    for(size_t i = 0; i < 4; ++i){
        for(size_t i = 0; i < titlePadding; ++i){
            printf(" ");
        }

        printf("%d. %s\n", (i+1), menuItems[i]);
    }

    getMenuChoice();
}

/*
*   getMenuChoice
*       handles user input when dealing with the menu
*/
void getMenuChoice(){
    int selectedOption = getche();

    switch(selectedOption){
    case 49: // ASCII value of "1"
        printf("placeholder");
        break;
    case 50: // ASCII value of "2"
        printf("placeholder");
        break;
    case 51: // ASCII value of "3"
        showAboutGameScreen();
        break;
    case 52: // ASCII value of "4"
        printf("placeholder");
        break;
    default:
        printf("You have to provide a valid number");
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

    int buttonPressed = 0;

    while(1){
        buttonPressed = getche();
        if(buttonPressed != 27){
            printf("\nInvalid button pressed");
        }
        else{
            system("cls");
            drawTitle();
            drawMenu();
        }
    }

}


#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <stdbool.h>

// This is a forward declaration of GameState Structure
struct GameState;
typedef struct GameState GameState;

typedef enum {
    AnsA,
    AnsB,
    AnsC,
    AnsD,

    LL50_50,
    LLAudHelp,
    LLPhone
} SimpleGuiSelectedItem;

int getWindowWidth();

int drawTitle();

void drawMenu();

void printCenteredText(const char*, int);

void showAboutGameScreen();

void getMenuChoice(GameState*);

void printAudienceHelp(GameState*);

void printSimpleGameGui(GameState*, SimpleGuiSelectedItem, bool);

#endif

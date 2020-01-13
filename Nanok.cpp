#include <string>
#include "Nanok.hpp"

#define INITAL_SIZE 2
#define DEGREE_OF_INCREASE 2

Nanok::Nanok() : strCount(INITAL_SIZE) {
    strLens = new int[strCount];
    for (int i = 0; i < strCount; ++i) {
        strLens[i] = INITAL_SIZE;
    }
    increaseTextSize();
}

Nanok::~Nanok() {
    delete[] strLens;
    for (int i = 0; i < strCount; ++i) {
        delete[] text[i];
    }
    delete[] text;
}

void Nanok::increaseTextSize() {
    if(text == NULL){
        text = new char*[strCount];
        for (int i = 0; i < strCount; ++i) {
            increaseStringSize(i);
        }
        return;
    }
    char **buffer = new char*[strCount*DEGREE_OF_INCREASE];
    for (int i = 0; i < strCount; ++i) {
        buffer[i] = text[i];
    }
}

void Nanok::increaseStringSize(int str) {
    if(text[str] == NULL){
        text[str] = new char[INITAL_SIZE];
        return;
    }
    char *buffer = new char[strLens[str]*DEGREE_OF_INCREASE];
    for (int i = 0; i < strLens[str]; ++i) {
        buffer[i] = text[str][i];
    }
    delete[] text[str];
    text[str] = buffer;
}

int Nanok::getConsoleWidth() {
    if (hWndConsole){
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            return consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    }
    throw "Cant get console width!";
}

int Nanok::getConsoleHeight() {
    if (hWndConsole){
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            return consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
    }
    throw "Cant get console height!";
}

void Nanok::print() {
    clrscr();
    for (int i = 0; i < strCount; ++i) {
        cputs(text[i]);
        
    }
}
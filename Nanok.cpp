#include <string>
#include <fstream>
#include <iostream>
#include "Nanok.hpp"

#define INITAL_SIZE 2
#define DEGREE_OF_INCREASE 2
#define FILE_READ_ERROR "Can't read this file"
#define FILE_WRITE_ERROR "Can't write this file"

#define LEFT 75
#define RIGHT 77

using namespace std;

Nanok::Nanok(const char *fileName) : strCount(INITAL_SIZE), fileName(fileName) {
    text = new string[strCount];
    if (fileName == NULL)
        return;
    open();
}

Nanok::~Nanok() {
    delete[] text;
}

void Nanok::run() {
    int keycode;
    print();
    while (true) {
        keycode = getch();
        if (keycode == 0){
            keycode = getch();
            switch (keycode){
                case LEFT:
                    toLeft();
                    break;
                case RIGHT:
                    toRight();
                    break;
            }
        }
        switch (keycode) {
            case KEY_ESC:
                return;
            default:
                insertChar(keycode);
        }
    }
}

void Nanok::increaseTextSize() {
    int newSize = strCount * DEGREE_OF_INCREASE;
    string *buffer = new string[newSize];
    for (int i = 0; i < strCount; ++i) {
        buffer[i] = text[i];
    }
    delete[] text;
    text = buffer;
    strCount = newSize;
}

int Nanok::getConsoleWidth() {
    if (hWndConsole) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            return consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    }
    throw "Cant get console width!";
}

int Nanok::getConsoleHeight() {
    if (hWndConsole) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            return consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
    }
    throw "Cant get console height!";
}

void Nanok::open() {
    ifstream file(fileName);
    if (!file.is_open())
        throw FILE_READ_ERROR;
    for (int i = 0; file; ++i) {
        if (i == strCount)
            increaseTextSize();
        getline(file, text[i]);
    }
    file.close();
    if (text == NULL)
        throw FILE_READ_ERROR;
}

void Nanok::saveAs() {
    clrscr();
    while (true) {
        cout << "Введите путь к файлу (имя файла)" << endl;
        string fileName;
        cin >> fileName;
        this->fileName = fileName.c_str();
        ofstream ofs(fileName);
        if (ofs.is_open()) {
            ofs.close();
            break;
        }
        cout << "Вы ввели недопустимое имя файла!" << endl;
    }
    save();
}

void Nanok::save() {
    if (fileName == NULL) {
        saveAs();
        return;
    }
    ofstream ofs(fileName);
    if (!ofs.is_open()) {
        throw FILE_WRITE_ERROR;
    }
    for (int i = 0; i < strCount; ++i) {
        if (text[i].length() != 0)
            ofs << text[i] << endl;
    }
    ofs.close();
}

void Nanok::insertChar(const char character) {

}

void Nanok::toUp(){

}

void Nanok::toDown(){
    int width = getConsoleWidth();

}

void Nanok::toLeft(){
    if (ix == 0)
        ix = text[--iy].length() - 1;
    else
        --ix;
    gotoxy(ix+1,iy+1);
}

void Nanok::toRight(){
    if (ix == text[iy].length() - 1 || ix == getConsoleWidth() - 1){
        ix = 0;
        ++iy;
    } else
        ++ix;
    gotoxy(ix+1,iy+1);
}

void Nanok::print() {
    clrscr();
    for (int i = 0; i < strCount; ++i) {
        cout << text[i] << endl;
    }
    gotoxy(ix+1,iy+1);
}
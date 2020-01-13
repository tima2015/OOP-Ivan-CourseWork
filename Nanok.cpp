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
#define UP 72
#define DOWN 80
#define BACKSPACE 8
#define ENTER 13

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
    gotoxy(1,1);
    while (true) {
        keycode = getch();
        if (keycode == 0) {
            keycode = getch();
            switch (keycode) {
                case LEFT:
                    toLeft();
                    break;
                case RIGHT:
                    toRight();
                    break;
                case UP:
                    toUp();
                    break;
                case DOWN:
                    toDown();
                    break;
            }
        } else
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
    int  width = getConsoleWidth(), plines = text[iy].length()/width;
    if (ix == 0){
        text[iy] = character + text[iy];
        gotoxy(wherex()+1,wherey());
        printLine();
        ix++;
    } else if (ix == text[iy].length()) {
        text[iy] += character;
        ix++;
        gotoxy(wherex()+1,wherey());
        printLine();
    } else {
        string p1 = text[iy].substr(0, ix), p2 = text[iy].substr(ix, text[iy].length() - 1);
        text[iy] = p1;
        text[iy] += character;
        text[iy] += p2;
        ix++;
        gotoxy(wherex()+1,wherey());
        printLine();
    }
    if (text[iy].length()/width != plines)
        print();
}

void Nanok::deleteChar() {
    if (ix == 0){
        if (iy == 0)
            return;
        text[iy-1] += text[iy];
        text[iy].clear();
        for (int i = iy + 1; i < strCount; ++i) {
            text[i-1] = text[i];
            if (text[i].length() == 0)
                break;
        }
    }
}

void Nanok::toUp() {
    int width = getConsoleWidth();
    if (ix < width) {
        if (iy == 0) {
            gotoxy(1, 1);
            ix = 0;
            return;
        }
        --iy;
        if (ix > text[iy].length() % width)
            ix = text[iy].length();
        else
            ix = (text[iy].length() / width) * width + ix % width;
        gotoxy(ix % width + 1, wherey() - 1);
    } else {
        ix -= width;
        gotoxy(wherex(), wherey() - 1);
    }
}

void Nanok::toDown() {
    int width = getConsoleWidth();
    if (text[iy].length() / width == ix / width) {
        if (strCount == iy + 1 || text[iy + 1].length() == 0) {
            ix = text[iy].length() - 1;
            gotoxy(ix % width + 2, wherey());
            return;
        }
        ++iy;
        if (text[iy].length() - 1 < ix % width)
            ix = text[iy].length() - 1;
        else
            ix = ix % width;
        gotoxy(ix + 1, wherey() + 1);
    } else if (text[iy].length() / width == ix / width + 1) {
        if ((text[iy].length() - 1) % width < ix % width)
            ix += width - (ix % width) + (text[iy].length() - 1) % width;
        else
            ix += width;
        gotoxy(ix % width + 1, wherey() + 1);
    } else {
        ix += width;
        gotoxy(wherex(), wherey() + 1);
    }
}

void Nanok::toLeft() {
    int width = getConsoleWidth();
    if (ix == 0) {
        if (iy == 0)
            return;
        ix = text[--iy].length();
        gotoxy(text[iy].length() % width + 1, wherey() - 1);
    } else if (wherex() == 1) {
        --ix;
        gotoxy(width, wherey() - 1);
    } else {
        --ix;
        gotoxy(wherex() - 1, wherey());
    }
}

void Nanok::toRight() {
    int width = getConsoleWidth();
    if (ix == text[iy].length()) {
        if (strCount == iy + 1 || text[iy + 1].length() == 0)
            return;
        ix = 0;
        ++iy;
        gotoxy(1, wherey() + 1);
    } else if (wherex() == width) {
        ++ix;
        gotoxy(1, wherey() + 1);
    } else {
        ++ix;
        gotoxy(wherex() + 1, wherey());
    }
}

void Nanok::printLine() {
    int bx = wherex(), by = wherey(), width = getConsoleWidth();
    gotoxy(1,wherey());
    int n = text[iy].length()/width + 1;
    for (int i = ix/width; i < n; ++i) {
        gotoxy(1,by + i);
        clreol();
    }
    gotoxy(1,by);
    cout << text[iy];
    gotoxy(bx,by);
}

void Nanok::print() {
    int bx = wherex(), by = wherey();
    clrscr();
    for (int i = 0; i < strCount; ++i) {
        cout << text[i] << endl;
    }
    gotoxy(bx, by);
}
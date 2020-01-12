#include "Nanok.hpp"

int Nanok::getTextIndexForPage(int page) {
    int width = getConsoleWidth(), height = getConsoleHeight();
    for (int i = 0, strLen = 0, strN = 0; i < text.size(); ++i) {
        if (text.at(i) == '\n'){
            strLen = 0;
            strN++;
        } else if (strLen == width){
            strLen = 0;
            strN++;
        }
        if (strN / height == page)
            return i;
    }
    return 0;
}

int Nanok::strCount() {
    int width = getConsoleWidth(), strN = 0;
    for (int i = 0, strLen = 0; i < text.size(); ++i) {
        if (text.at(i) == '\n'){
            strLen = 0;
            strN++;
        } else if (strLen == width){
            strLen = 0;
            strN++;
        }
        if (strN / height == page)
            return i;
    }
    return strN;
}

int Nanok::pageCount() {
    return strCount()/getConsoleHeight()+1;
}

void Nanok::printPage(int page) {
    int width = getConsoleWidth(), height = getConsoleHeight();
    clrscr();
    for (int y = 0, i = getTextIndexForPage(page); y < height; y++)
        for (int x = 0; x < width; x++, i++) {
            if(i >= text.size())
                return;
            char c = text.at(i);
            switch (c){
                case '\n':
                    putch(c);
                    ++y;
                    if(y == height)
                        return;
                    gotoxy(x,y);
                    break;
                default:
                    putch(c);
            }
        }
}

void Nanok::pageDown() {
    printPage(++page);
    gotoxy(0,0);
}

void Nanok::pageUp() {
    printPage(--page);
    gotoxy(0,0);
}

void Nanok::toStrStart() {
    gotoxy(0, wherey());
}

void Nanok::toStrEnd() {
    gotoxy(getConsoleWidth(),wherey());
}

void Nanok::toNextWord() {
    int width = getConsoleWidth(),
    height = getConsoleHeight();

    for (int y = wherey(); y < height; ++y)
        for (int x = wherex() + 1; x < width; ++x) {
            if (getCharFromConsole(x, y) == ' ') {
                gotoxy(x, y);
                return;
            }
            if (getCharFromConsole(x, y) == '\n'){
                gotoxy(x = 0,++y);
                return;
            }
        }
}

void Nanok::toPreWord() {
    int width = getConsoleWidth(),
            height = getConsoleHeight();

    for (int y = wherey(); y >= 0; --y)
        for (int x = wherex() + 1; x >= 0; --x) {
            if (getCharFromConsole(x, y) == ' ') {
                gotoxy(x, y);
                return;
            }
            if (x == 0){
                gotoxy(x = width-1,--y);
                return;
            }
        }
}

void Nanok::toTextStart() {
    printPage(page = 0);
    gotoxy(0,0);
}

void Nanok::toTextEnd() {
    printPage(page = pageCount()-1);
    gotoxy(0,0);
}



char Nanok::getCharFromConsole(int x, int y) {
    if (hWndConsole){
        CHAR_INFO info;
        COORD coord;
        SMALL_RECT sr;
        coord.X = sr.Left = sr.Right = x;
        coord.Y = sr.Top = sr.Bottom = y;
        ReadConsoleOutput(hWndConsole, &info,coord, coord, sr);
        return info.AsciiChar;
    }
    throw "Cant get character from console!";
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
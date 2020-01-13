#include <string>
#include <fstream>
#include <windows.h>
#include "coniow.h"
#ifndef IVAN_NANOK_HPP
#define IVAN_NANOK_HPP


using namespace std;

/**
 * Текст выводится на "страницах".
 * Страницей является блок текста размером с ширину и высоту терминала
 * Начальная страница 0
 */
class Nanok {
public:
    Nanok();
    virtual ~Nanok();
private:
    //Текст
    char **text;
    int *strLens, strCount;
    void increaseTextSize();
    void increaseStringSize(int str);
    //Работа с консолью
    HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int ix = 0, iy = 0; //Текущая позиция курсора в строке
    int getConsoleWidth();
    int getConsoleHeight();
    void print();//Выводит в терминал текст страницы
    //Текстовый редактор
    void pageDown(); //Курсор на страницу вниз
    void pageUp(); //Курсор на страницу вверх
    void toStrStart();//Курсор в начало строки
    void toStrEnd();//Курсор в конец строки
    void toNextWord();//На слово вперёд
    void toPreWord();//На слово назад
    void toTextStart();//В начало текста
    void toTextEnd();//В конец текста
};

#endif //IVAN_NANOK_HPP

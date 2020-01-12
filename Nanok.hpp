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
private:
    string text, buffer;
    int page = 0; //Текущая страница
    int getTextIndexForPage(int page);//Возращает индекс для строки текста с которого нужно начать отрисовку текущей страницы. 0 если страница одна
    int pageCount();
    int strCount();
    void printPage(int page);//Выводит в терминал текст страницы
    void pageDown(); //Курсор на страницу вниз
    void pageUp(); //Курсор на страницу вверх
    void toStrStart();//Курсор в начало строки
    void toStrEnd();//Курсор в конец строки
    void toNextWord();//На слово вперёд
    void toPreWord();//На слово назад
    void toTextStart();//В начало текста
    void toTextEnd();//В конец текста


    //Методы информации о терминале
    HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char getCharFromConsole(int x, int y);
    int getConsoleWidth();
    int getConsoleHeight();
};

#endif //IVAN_NANOK_HPP

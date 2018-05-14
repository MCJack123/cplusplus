//
//  screenio.cpp
//  cplusplus
//
//  Created by Jack on 5/10/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <algorithm>
#include "screenio.hpp"
#define qd(a, b) ((a - (a % b)) / b)
#ifdef SCREEN
#define write(x, y, text) screen.gotoXY(x, y); screen.write((const uint8_t *)text.c_str(), text.length())
#else
#define write(x, y, text) mvaddstr(y, x, text.c_str()); move(1, SCREEN_WIDTH - 1); refresh()
#endif
#define incWithMax(num, max) if (num >= max) num = 0; else num++
#define decWithMax(num, max) if (num == 0) num = max - 1; else num--

int scrollpoint = 0;
int scrollspeed = 125;
std::string currentItem = "*";
std::string currentTitle = "*";
std::string currentText = "";
std::chrono::system_clock::time_point lastScroll = std::chrono::system_clock::now();
//WINDOW * win;

void initialize() {
#ifdef SCREEN
    screen.initPins();
    screen.hideCursor();
#else
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    nodelay(stdscr, true);
   //win = newwin(2, SCREEN_WIDTH, 0, 0);
    //wrefresh(win);
#endif
}

void deinitialize() {
#ifndef SCREEN
    //delwin(win);
    echo();
    nocbreak();
    keypad(stdscr, false);
    nodelay(stdscr, false);
    endwin();
#endif
}

void setTitle(std::string title) {
    if (title.length() > SCREEN_WIDTH) title = title.substr(SCREEN_WIDTH - 3) + "...";
    //if (title == currentTitle) return;
    int spaces = qd((SCREEN_WIDTH - (int)title.length()), 2);
    if (spaces < 0) spaces = 0;
    if (title.length() % 2 > 0 && spaces > 0) title.append(" ");
    currentTitle = std::string(spaces, ' ') + title + std::string(spaces, ' ');
    assert(currentTitle.length() == SCREEN_WIDTH);
    write(0, 0, currentTitle);
}

void setText(std::string text) {
    int spaces = qd((SCREEN_WIDTH - (int)text.length()), 2);
    if (spaces < 0) spaces = 0;
    std::string newText = text.substr(0, SCREEN_WIDTH);
    if (text.length() % 2 > 0 && spaces > 0) newText.append(" ");
    newText = std::string(spaces, ' ') + newText + std::string(spaces, ' ');
    currentText = text;
    scrollpoint = 0;
    lastScroll = std::chrono::system_clock::now() + std::chrono::seconds(1);
    //std::cout << "\"" << newText << "\"=" << newText.length() << "\n";
    assert(newText.length() == SCREEN_WIDTH);
    write(0, 1, newText);
}

int getButton() {
    while (true) {
        if (std::chrono::system_clock::now() - lastScroll >= std::chrono::milliseconds(scrollspeed) && currentText.length() > SCREEN_WIDTH) {
            if (++scrollpoint > currentText.size() + 4) {
                scrollpoint = 0;
                lastScroll = std::chrono::system_clock::now() + std::chrono::seconds(1);
            } else lastScroll = std::chrono::system_clock::now();
            write(0, 1, extractLine(currentText, scrollpoint));
        }
#ifdef SCREEN
        if (digitalRead(PIN_LEFT) == HIGH) return BUTTON_LEFT;
        if (digitalRead(PIN_RIGHT) == HIGH) return BUTTON_RIGHT;
        if (digitalRead(PIN_OK) == HIGH) return BUTTON_OK;
        if (digitalRead(PIN_BACK) == HIGH) return BUTTON_BACK;
#else
        int ch = getch();
        if (ch == KEY_LEFT) return BUTTON_LEFT;
        if (ch == KEY_RIGHT) return BUTTON_RIGHT;
        if (ch == KEY_ENTER || ch == 10) return BUTTON_OK;
        if (ch == KEY_BACKSPACE || ch == 127) return BUTTON_BACK;
#endif
    }
}

std::string extractLine(std::string text, int offset) {
    std::string shortened = "";
    std::replace(text.begin(), text.end(), '\n', ' ');
    if (offset < text.size()) {
        shortened = text.substr(offset, SCREEN_WIDTH);
        if (shortened.length() < SCREEN_WIDTH) shortened += std::string((SCREEN_WIDTH - shortened.length() > 4 ? 4 : SCREEN_WIDTH - shortened.length()), ' ');
        if (shortened.length() < SCREEN_WIDTH) shortened += text.substr(0, SCREEN_WIDTH - shortened.length());
    } else {
        shortened += std::string(4 - (offset - text.length()), ' ');
        if (shortened.length() < SCREEN_WIDTH) shortened += text.substr(0, SCREEN_WIDTH - shortened.length());
    }
    assert(shortened.length() == SCREEN_WIDTH);
    return shortened;
}

int showMenu(std::string title, list items) {
    setTitle(title);
    int itemnum = 0;
    while (true) {
        currentItem = items[itemnum];
        setText(items[itemnum]);
        int button = getButton();
        if (button == BUTTON_LEFT) {decWithMax(itemnum, (int)items.size());}
        else if (button == BUTTON_RIGHT) {incWithMax(itemnum, (int)items.size() - 1);}
        else if (button == BUTTON_BACK) return -1;
        else if (button == BUTTON_OK) return itemnum;
    }
}

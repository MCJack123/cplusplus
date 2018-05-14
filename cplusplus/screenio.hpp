//
//  screenio.hpp
//  cplusplus
//
//  Created by Jack on 5/10/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef screenio_hpp
#define screenio_hpp

#include <string>
#include <chrono>
#include <vector>
#define SCREEN_WIDTH 16
#ifdef __arm__
#include "PoluluHD44780.h"
// To be filled in
PoluluHD44710 screen(0, 0, 0, 0, 0, 0);
#define PIN_LEFT 0
#define PIN_RIGHT 0
#define PIN_OK 0
#define PIN_BACK 0
#else
#include <ncurses.h>
#include <unistd.h>
//extern WINDOW * win;
#endif

typedef std::vector<std::string> list;

enum button_presses {
    BUTTON_NONE,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_BACK,
    BUTTON_OK
};

extern int scrollpoint;
extern int scrollspeed;
extern std::string currentItem;
extern std::string currentTitle;
extern std::string currentText;
extern std::chrono::system_clock::time_point lastScroll;

extern void initialize();
extern void deinitialize();
extern void setTitle(std::string title);
extern void setText(std::string text);
extern int getButton();
extern std::string extractLine(std::string text, int offset);
extern int showMenu(std::string title, list items);

#endif /* screenio_hpp */

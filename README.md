#  cplusplus
A C++ documentation viewer designed for the HD44780 character LCD chipset. Intended to be run on a Raspberry Pi with an LCD and four buttons attached, but also works in a terminal via ncurses.

## Prerequisites
 - jsoncpp
 - ncurses (Terminal build)
 - wiringPi (Raspberry Pi build)
 - non-Windows computer

## Building
Unzip the `docs.zip` file, and make sure the files are in a folder named `docs`.
### Computer
`g++ -o cplusplus docspec.cpp screenio.cpp main.cpp -lncurses -ljsoncpp`
### Raspberry Pi
Before building, you need to change some values. Open `screenio.hpp`, and change the following values to the corresponding pin number:
- `SCREEN_WIDTH` (line 15): The width of the screen. (The height is hard-coded to two lines.)
- `PoluluHD44710 screen(0, 0, 0, 0, 0, 0)` (line 19): The pins for accessing the LCD
    - The order of the pins on the LCD is `RS`, `E`, `DB4`, `DB5`, `DB6`, `DB7`
- `PIN_LEFT` (line 20): Left arrow button
- `PIN_RIGHT` (line 21): Right arrow button
- `PIN_OK` (line 22): OK button
- `PIN_BACK` (line 23): Back button
Then:
`g++ -o cplusplus docspec.cpp screenio.cpp PoluluHD44780.cpp main.cpp -ljsoncpp -lwiringPi` (untested)
#### ncurses on ARM processors
The code checks the architecture to see if it should use the LCD mode or ncurses mode. Thus, if you want to use the ncurses mode on an ARM machine, change line 16 of `screenio.hpp` (`#ifdef __arm__`) to check for some other macro (such as `#ifdef __this_is_not_a_macro__`). Then build with the computer instructions.

## Running
Make sure `headers.json` and the `docs` folder are copied to the same directory as the program. Then just run the executable in the terminal. If you are using an LCD, the output should show up on the screen; otherwise it will be in a 16x2 area in the top left corner the terminal.  
In ncurses mode, use the left & right arrow keys, enter, and backspace to navigate. In LCD mode, use the connected buttons to navigate. Pressing the back button on the main screen will close the program.

### Disclaimer
By using this program with an LCD, you accept responsibility for any damage that may occur to your screen. I do not have an LCD myself, and so I cannot test to see if something will go wrong.

### Credits
The information provided in the `docs` folder of this repository was retrieved from [cplusplus.com](http://www.cplusplus.com).

## Creating your own docset
If you would like to provide your own documentation instead of using what I have (for instance, for another language, or to add other libraries), you can add JSON files in the following format:

`headers.json` 
- Category name (object)
    - Header/library name (array)
        - Classes/functions provided (string)

`<header/library name>.<class/function provided>.json`
- `name` (string)
- `prototype` (string)
- `type` (string)
    - Can contain "class", "function", "enum" for specialized forms
- For functions:
    - `parameters` (object)
        - Parameter name: description
    - `return_value` (string)
- For classes:
    - `functions` (array)
        - Methods of the class (string)
            - Place the documentation for the method in a file named with the format `<class name>.<method>.json`.
            - `(constructor)` and `(destructor)` get their parentheses stripped in the file name.
- For enumerations:
    - `constants` (array)
        - Constants defined in the enum (string)


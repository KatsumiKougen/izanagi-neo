#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include "splash.hpp"

const int max_x = 200, max_y = 50;
namespace color {
    short enum {Black, Red, Green, Yellow, Blue, Purple, Cyan, White, Bright};
}

class Canvas
{
    unsigned short pixel[max_y][max_x];
    struct {
        short curx = 0, cury = 0, color = 0;
    } cursor;
    struct {
        short x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    } fill;
    short fore = 7, back = 0;
    char* current_splash;

    public:
        void init_color();
        void start();
        void end();
        void loop();

    private:
        void reset_canvas();
        void scan();
        void fill_canvas();
        void fill_area(int clear);
        //void draw_cursor();
        void keep_in_range();
        void print_status();
        void save();
        void load();
};

#define key_highlight_on attrset(COLOR_PAIR(1) | A_BOLD | A_REVERSE)
#define key_highlight_off standend()
#define print_splash(string) \
    attrset(A_ITALIC | COLOR_PAIR(66)); \
    mvprintw(29, 0, string); \
    standend()

void Canvas::init_color(){
    init_pair(1, 15, 0); // default
    for (int i = 0; i < 8; i++){
        init_pair(8*i+2, color::Black,  i);
        init_pair(8*i+3, color::Red,    i);
        init_pair(8*i+4, color::Green,  i);
        init_pair(8*i+5, color::Yellow, i);
        init_pair(8*i+6, color::Blue,   i);
        init_pair(8*i+7, color::Purple, i);
        init_pair(8*i+8, color::Cyan,   i);
        init_pair(8*i+9, color::White,  i);
    }
    init_pair(66, color::Yellow+color::Bright, 0); // splash
    init_pair(67, color::Yellow+color::Bright, color::Red+color::Bright); // cursor
}

void Canvas::start(){
    std::random_device device;
    std::mt19937 engine {device()};
    std::uniform_int_distribution<int> dist(0, splash.size()-1);

    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_color();
    reset_canvas();
    current_splash = (char*)(splash[dist(engine)]);
}

void Canvas::end(){
    endwin();
    std::cout << "exiting \x1b[1mizanagi-neo\x1b[0m...\n";
    exit(1);
}

void Canvas::loop(){
    int key;
    while(1){
        scan();
        print_status();
        key = getch();
        switch (key){
            case KEY_UP: case 87: case 119:
                cursor.cury--;
                keep_in_range();
                break;
            case KEY_DOWN: case 83: case 115:
                cursor.cury++;
                keep_in_range();
                break;
            case KEY_LEFT: case 65: case 97:
                cursor.curx--;
                keep_in_range();
                break;
            case KEY_RIGHT: case 68: case 100:
                cursor.curx++;
                keep_in_range();
                break;
            case 113:
                cursor.curx -= 2;
                keep_in_range();
                break;
            case 101:
                cursor.curx += 2;
                keep_in_range();
                break;
            case 49: // one
                cursor.color = color::Black;break;
            case 50: // two
                cursor.color = color::Red;break;
            case 51: // three
                cursor.color = color::Green;break;
            case 52: // four
                cursor.color = color::Yellow;break;
            case 53: // five
                cursor.color = color::Blue;break;
            case 54: // six
                cursor.color = color::Purple;break;
            case 55: // seven
                cursor.color = color::Cyan;break;
            case 56: // eight
                cursor.color = color::White;break;
            case 57: // nine
                fill_area(0);break;
            case 40:
                fill_area(1);break;
            case 48: // zero
                fill_canvas();break;
            case 91: // [
                fill.x0 = cursor.curx;
                fill.y0 = cursor.cury;
                break;
            case 93: // ]
                fill.x1 = cursor.curx;
                fill.y1 = cursor.cury;
                break;
            case 60: // <
                fore = cursor.color;
                break;
            case 62: // >
                back = cursor.color;
                break;
            case 32: case 10: // enter / space
                if (pixel[cursor.cury][cursor.curx] == 0)
                    pixel[cursor.cury][cursor.curx] = 1;
                else
                    pixel[cursor.cury][cursor.curx] = 0;
                break;
            case KEY_F(1): // F1
                endwin();
                save();
                refresh();
                break;
            case KEY_F(2): // F2
                endwin();
                load();
                scan();
                refresh();
                break;
            case KEY_F(3): // F3
                end();break;
        }
    }
}

void Canvas::reset_canvas(){
    for (int y = 0; y < max_y; y++)
        for (int x=0; x < max_x; x++)
            pixel[y][x] = 0;
}

void Canvas::scan(){
    for (int y = 0; y < (short)(max_y/2); y++)
        for (int x = 0; x < (short)(max_x/2); x++){
            attrset(COLOR_PAIR(2+(8*back+fore)));
            switch ((pixel[2*y][2*x] << 3) | (pixel[2*y][2*x+1] << 2) | (pixel[2*y+1][2*x] << 1) | pixel[2*y+1][2*x+1]){
                case 0:  mvprintw(1+y, 1+x, " ");standend();break;
                case 1:  mvprintw(1+y, 1+x, "▗");standend();break;
                case 2:  mvprintw(1+y, 1+x, "▖");standend();break;
                case 3:  mvprintw(1+y, 1+x, "▄");standend();break;
                case 4:  mvprintw(1+y, 1+x, "▝");standend();break;
                case 5:  mvprintw(1+y, 1+x, "▐");standend();break;
                case 6:  mvprintw(1+y, 1+x, "▞");standend();break;
                case 7:  mvprintw(1+y, 1+x, "▟");standend();break;
                case 8:  mvprintw(1+y, 1+x, "▘");standend();break;
                case 9:  mvprintw(1+y, 1+x, "▚");standend();break;
                case 10: mvprintw(1+y, 1+x, "▌");standend();break;
                case 11: mvprintw(1+y, 1+x, "▙");standend();break;
                case 12: mvprintw(1+y, 1+x, "▀");standend();break;
                case 13: mvprintw(1+y, 1+x, "▜");standend();break;
                case 14: mvprintw(1+y, 1+x, "▛");standend();break;
                case 15: mvprintw(1+y, 1+x, "█");standend();break;
            }
        }
    refresh();
}

void Canvas::print_status(){
    attrset(COLOR_PAIR(1));
    for (int i = 0; i < 102; i++)
        mvprintw(0, i, ":");
    for (int i = 1; i < 26; i++){
        mvprintw(i, 0, ":");
        for (int j = 1; j < 101; j++)
            mvprintw(i, j, " ");
        mvprintw(i, 101, ":");
    }
    for (int i = 0; i < 102; i++)
        mvprintw(26, i, ":");
    scan();

    key_highlight_on;mvprintw(28, 0, "F1");
    key_highlight_off;mvprintw(28, 3, "save");
    key_highlight_on;mvprintw(28, 8, "F2");
    key_highlight_off;mvprintw(28, 11, "load");
    key_highlight_on;mvprintw(28, 16, "F3");
    key_highlight_off;mvprintw(28, 19, "exit");

    key_highlight_on;mvprintw(27, 0, "X");
    key_highlight_off;mvprintw(27, 2, "%d  ", cursor.curx);
    key_highlight_on;mvprintw(27, 6, "Y");
    key_highlight_off;mvprintw(27, 8, "%d  ", cursor.cury);
    key_highlight_on;mvprintw(27, 12, "F0");
    key_highlight_off;mvprintw(27, 15, "%d %d   ", fill.x0, fill.y0);
    key_highlight_on;mvprintw(27, 22, "F1");
    key_highlight_off;mvprintw(27, 25, "%d %d   ", fill.x1, fill.y1);
    key_highlight_on;mvprintw(27, 32, "F");
    key_highlight_off;
    switch (fore){
        case color::Black:
            mvprintw(27, 34, "black ");break;
        case color::Red:
            mvprintw(27, 34, "red   ");break;
        case color::Green:
            mvprintw(27, 34, "green ");break;
        case color::Yellow:
            mvprintw(27, 34, "yellow");break;
        case color::Blue:
            mvprintw(27, 34, "blue  ");break;
        case color::Purple:
            mvprintw(27, 34, "purple");break;
        case color::Cyan:
            mvprintw(27, 34, "cyan  ");break;
        case color::White:
            mvprintw(27, 34, "white ");break;
    }
    key_highlight_on;mvprintw(27, 41, "B");
    key_highlight_off;
    switch (back){
        case color::Black:
            mvprintw(27, 43, "black ");break;
        case color::Red:
            mvprintw(27, 43, "red   ");break;
        case color::Green:
            mvprintw(27, 43, "green ");break;
        case color::Yellow:
            mvprintw(27, 43, "yellow");break;
        case color::Blue:
            mvprintw(27, 43, "blue  ");break;
        case color::Purple:
            mvprintw(27, 43, "purple");break;
        case color::Cyan:
            mvprintw(27, 43, "cyan  ");break;
        case color::White:
            mvprintw(27, 43, "white ");break;
    }
    key_highlight_on;mvprintw(27, 50, "COL");
    key_highlight_off;
    switch (cursor.color){
        case color::Black:
            mvprintw(27, 54, "black ");break;
        case color::Red:
            mvprintw(27, 54, "red   ");break;
        case color::Green:
            mvprintw(27, 54, "green ");break;
        case color::Yellow:
            mvprintw(27, 54, "yellow");break;
        case color::Blue:
            mvprintw(27, 54, "blue  ");break;
        case color::Purple:
            mvprintw(27, 54, "purple");break;
        case color::Cyan:
            mvprintw(27, 54, "cyan  ");break;
        case color::White:
            mvprintw(27, 54, "white ");break;
    }
    standend();
    print_splash(current_splash);
    refresh();
}

void Canvas::fill_canvas(){
    for (int y = 0; y < max_y; y++)
        for (int x = 0; x < max_x; x++)
            pixel[y][x] = 0;
}

void Canvas::fill_area(int clear){
    int starty = std::min(fill.y0, fill.y1), endy = std::max(fill.y0, fill.y1) + 1;
    int startx = std::min(fill.x0, fill.x1), endx = std::max(fill.x0, fill.x1) + 1;
    if (clear == 0)
        for(int y = starty; y < endy; ++y)
            for(int x = startx; x < endx; ++x)
                pixel[y][x] = 1;
    else
        for(int y = starty; y < endy; ++y)
            for(int x = startx; x < endx; ++x)
                pixel[y][x] = 0;
}

/*void Canvas::draw_cursor(){
    attrset(COLOR_PAIR(19));
    mvprintw(1+cursor.cury, 1+cursor.curx, "▒");
    standend();
    refresh();
}*/

void Canvas::keep_in_range(){
    if (cursor.curx < 0) cursor.curx = 0;
    else if (cursor.curx > max_x-1) cursor.curx = max_x - 1;
    else if (cursor.cury < 0) cursor.cury = 0;
    else if (cursor.cury > max_y-1) cursor.cury = max_y - 1;
}

void Canvas::save(){
    char infilename[0xfff];
    std::cout << "enter file name (example: owo.iz2z): ";
    std::cin >> infilename;

    std::ofstream outf{infilename};

    while (outf){
        for (int y = 0; y < max_y; y++)
            for (int x = 0; x < max_x; x++)
                switch (pixel[y][x]){
                    case 0: outf << "0";break;
                    case 1: outf << "1";break;
        }
        outf.close();
    }
    std::cout << "(>\x1b[1;93m^w^\x1b[0m)> \x1b[3m[insert Final Fantasy victory theme here]\n"
                 "\x1b[0mfile \"" << infilename << "\" has been written.\n";
    sleep(2);
}

void Canvas::load(){
    char outfilename[0xfff];
    std::cout << "enter file name (example: owo.iz2z): ";
    std::cin >> outfilename;

    std::ifstream inf{outfilename};
    char data[10000];
    if (!inf){
        std::cerr << "welp that sucks.\nthe program couldn't open the file.";
        sleep(2);
        refresh();
    } else {
        while (inf){
            inf >> data;
            for (int y = 0; y < max_y; y++)
                for (int x = 0; x < max_x; x++)
                    switch (data[max_x*y+x]){
                        case (char)48: pixel[y][x] = 0;break;
                        case (char)49: pixel[y][x] = 1;break;
                    }
            inf.close();
        }
    }

    std::cout << "(>\x1b[1;93m^w^\x1b[0m)> \x1b[3m[insert Final Fantasy victory theme here]\n"
                 "\x1b[0mfile successfully opened.\n";
    sleep(2);
}

int main(){
    std::cout << "▜▘▀▜ ▐▌▙▐ ▐▌▞▀▝▛    ▐▖▌▛▘▞▜  |  written in C++ by Katsumi Kougen\n"
                 "▐ ▗▘ ▙▌▌▜ ▙▌▌▗ ▌ ▟█▘▐▝▌▛ ▌▐  |  github.com/KatsumiKougen\n"
                 "▟▖▙▄▐ ▌▌▐▐ ▌▙▟▗▙    ▐ ▌▙▖▙▞  |  this software is free, as in \"\x1b[4mfree\x1b[0m candy,  kids!\"\n"
                 "        high-res  max        |  special thanks to: Warp, Bisqwit\n\n"
                 "initialising \x1b[1mizanagi-neo\x1b[0m...\n";
    Canvas cv;
    setlocale(LC_ALL, "");
    sleep(1);

    cv.start();
    cv.loop();

    return 0;
}

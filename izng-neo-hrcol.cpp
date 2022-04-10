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

const int max_x = 100, max_y = 50;
namespace color {
    short enum {Black, Red, Green, Yellow, Blue, Purple, Cyan, White, Bright};
}

class Canvas
{
    unsigned short pixel[max_y][max_x], tile[25][50];
    struct {
        short curx = 0, cury = 0, tilex = 0, tiley = 0, color = 0;
    } cursor;
    struct {
        short x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    } fill;
    struct {
        short x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    } tfill;
    short fore = color::White, back = color::Black;
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
        void fill_tile();
        //void draw_cursor();
        void keep_in_range();
        void print_status();
        void save();
        void load();
};

#define key_highlight_on attrset(COLOR_PAIR(64) | A_BOLD | A_REVERSE)
#define key_highlight_off standend()
#define print_splash(string) \
    attrset(A_ITALIC | COLOR_PAIR(65)); \
    mvprintw(27, 0, string); \
    standend()

void Canvas::init_color(){
    for (int ba = 0; ba < 8; ba++)
        for (int fo = 0; fo < 8; fo++)
            init_pair(1+ba*8+fo, fo, ba);
    init_pair(64, color::White+color::Bright, color::Black);
    init_pair(65, color::Yellow+color::Bright, color::Black);
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
        cursor.tilex = cursor.curx / 2;
        cursor.tiley = cursor.cury / 2;
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
            case 123: // {
            	tfill.x0 = cursor.tilex;
            	tfill.y0 = cursor.tiley;
            	break;
            case 125: // }
            	tfill.x1 = cursor.tilex;
            	tfill.y1 = cursor.tiley;
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
            case 47: // /
                tile[cursor.tiley][cursor.tilex] = back * 8 + fore;
                break;
            case 63: // ?
                fill_tile();
                break;
            /*
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
            */
            case KEY_F(3): // F3
                end();break;
        }
    }
}

void Canvas::reset_canvas(){
    for (int y = 0; y < max_y; y++)
        for (int x = 0; x < max_x; x++)
            pixel[y][x] = 0;
    for (int y = 0; y < (short)(max_y/2); y++)
        for (int x = 0; x < (short)(max_x/2); x++)
            tile[y][x] = 7;
}

void Canvas::scan(){
    for (int y = 0; y < (short)(max_y/2); y++)
        for (int x = 0; x < (short)(max_x/2); x++){
            attrset(COLOR_PAIR(1+tile[y][x]));
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
    for (int i = 0; i < 52; i++)
        mvprintw(0, i, ":");
    for (int i = 1; i < 26; i++){
        mvprintw(i, 0, ":");
        for (int j = 1; j < 51; j++)
            mvprintw(i, j, " ");
        mvprintw(i, 51, ":");
    }
    for (int i = 0; i < 52; i++)
        mvprintw(26, i, ":");
    scan();

    attrset(COLOR_PAIR(64));
    mvprintw(0, 53, "▜▘▀▜ ▐▌▙▐ ▐▌▞▀▝▛    ▐▖▌▛▘▞▜ |");
    mvprintw(1, 53, "▐ ▗▘ ▙▌▌▜ ▙▌▌▗ ▌ ▟█▘▐▝▌▛ ▌▐ | high-res");
    mvprintw(2, 53, "▟▖▙▄▐ ▌▌▐▐ ▌▙▟▗▙    ▐ ▌▙▖▙▞ | with colours");
    standend();

    key_highlight_on;mvprintw(4, 53, "WASD");
    key_highlight_off;mvprintw(4, 57, "/");
    key_highlight_on;mvprintw(4, 58, "⬆➡⬇⬅");
    key_highlight_off;mvprintw(4, 63, "move the cursor");

    key_highlight_on;mvprintw(6, 53, "ENTER");
    key_highlight_off;mvprintw(6, 58, "/");
    key_highlight_on;mvprintw(6, 59, "SPACEBAR");
    key_highlight_off;mvprintw(6, 68, "plot / erase");

    key_highlight_on;mvprintw(8, 53, "1");
    key_highlight_off;mvprintw(8, 55, "black");
    key_highlight_on;mvprintw(8, 61, "2");
    key_highlight_off;mvprintw(8, 63, "red");
    key_highlight_on;mvprintw(9, 53, "3");
    key_highlight_off;mvprintw(9, 55, "green");
    key_highlight_on;mvprintw(9, 61, "4");
    key_highlight_off;mvprintw(9, 63, "yellow");
    key_highlight_on;mvprintw(10, 53, "5");
    key_highlight_off;mvprintw(10, 55, "blue");
    key_highlight_on;mvprintw(10, 61, "6");
    key_highlight_off;mvprintw(10, 63, "purple");
    key_highlight_on;mvprintw(11, 53, "7");
    key_highlight_off;mvprintw(11, 55, "cyan");
    key_highlight_on;mvprintw(11, 61, "8");
    key_highlight_off;mvprintw(11, 63, "white");

    key_highlight_on;mvprintw(13, 53, "<");
    key_highlight_off;mvprintw(13, 55, "set fore");
    key_highlight_on;mvprintw(13, 64, ">");
    key_highlight_off;mvprintw(13, 66, "set back");

    key_highlight_on;mvprintw(15, 53, "9");
    key_highlight_off;mvprintw(15, 55, "fill an area");
    key_highlight_on;mvprintw(15, 68, "0");
    key_highlight_off;mvprintw(15, 70, "clear the canvas");

    key_highlight_on;mvprintw(17, 53, "F1");
    key_highlight_off;mvprintw(17, 56, "save (not!)");

    key_highlight_on;mvprintw(19, 53, "F2");
    key_highlight_off;mvprintw(19, 56, "load (not!)");

    key_highlight_on;mvprintw(21, 53, "F3");
    key_highlight_off;mvprintw(21, 56, "exit");

    key_highlight_on;mvprintw(23, 53, "X");
    key_highlight_off;mvprintw(23, 55, "%d ", cursor.curx);
    key_highlight_on;mvprintw(23, 58, "Y");
    key_highlight_off;mvprintw(23, 60, "%d ", cursor.cury);
    key_highlight_on;mvprintw(23, 64, "F0");
    key_highlight_off;mvprintw(23, 67, "%d %d  ", fill.x0, fill.y0);
    key_highlight_on;mvprintw(23, 73, "F1");
    key_highlight_off;mvprintw(23, 76, "%d %d  ", fill.x1, fill.y1);
    key_highlight_on;mvprintw(24, 53, "FORE");
    key_highlight_off;
    switch (fore){
        case color::Black:
            mvprintw(24, 58, "black ");break;
        case color::Red:
            mvprintw(24, 58, "red   ");break;
        case color::Green:
            mvprintw(24, 58, "green ");break;
        case color::Yellow:
            mvprintw(24, 58, "yellow");break;
        case color::Blue:
            mvprintw(24, 58, "blue  ");break;
        case color::Purple:
            mvprintw(24, 58, "purple");break;
        case color::Cyan:
            mvprintw(24, 58, "cyan  ");break;
        case color::White:
            mvprintw(24, 58, "white ");break;
    }
    key_highlight_on;mvprintw(24, 65, "BACK");
    key_highlight_off;
    switch (back){
        case color::Black:
            mvprintw(24, 70, "black ");break;
        case color::Red:
            mvprintw(24, 70, "red   ");break;
        case color::Green:
            mvprintw(24, 70, "green ");break;
        case color::Yellow:
            mvprintw(24, 70, "yellow");break;
        case color::Blue:
            mvprintw(24, 70, "blue  ");break;
        case color::Purple:
            mvprintw(24, 70, "purple");break;
        case color::Cyan:
            mvprintw(24, 70, "cyan  ");break;
        case color::White:
            mvprintw(24, 70, "white ");break;
    }
    key_highlight_on;mvprintw(25, 53, "COLOUR");
    key_highlight_off;
    switch (cursor.color){
        case color::Black:
            mvprintw(25, 60, "black ");break;
        case color::Red:
            mvprintw(25, 60, "red   ");break;
        case color::Green:
            mvprintw(25, 60, "green ");break;
        case color::Yellow:
            mvprintw(25, 60, "yellow");break;
        case color::Blue:
            mvprintw(25, 60, "blue  ");break;
        case color::Purple:
            mvprintw(25, 60, "purple");break;
        case color::Cyan:
            mvprintw(25, 60, "cyan  ");break;
        case color::White:
            mvprintw(25, 60, "white ");break;
    }
    standend();
    print_splash(current_splash);
    refresh();
}

void Canvas::fill_canvas(){
    for (int y = 0; y < max_y; y++)
        for (int x = 0; x < max_x; x++)
            pixel[y][x] = 0;
    for (int y = 0; y < (short)(max_y/2); y++)
        for (int x = 0; x < (short)(max_x/2); x++)
            tile[y][x] = back * 8 + fore;
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

void Canvas::fill_tile(){
    int starty = std::min(tfill.y0, tfill.y1), endy = std::max(tfill.y0, tfill.y1) + 1;
    int startx = std::min(tfill.x0, tfill.x1), endx = std::max(tfill.x0, tfill.x1) + 1;
    for(int y = starty; y < endy; ++y)
        for(int x = startx; x < endx; ++x)
            tile[y][x] = back * 8 + fore;
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
    if (cursor.tilex < 0) cursor.tilex = 0;
    else if (cursor.tilex > 50) cursor.tilex = 50;
    else if (cursor.tiley < 0) cursor.tiley = 0;
    else if (cursor.tiley > 25) cursor.tiley = 25;
}

void Canvas::save(){
    char infilename[0xfff];
    std::cout << "enter file name (example: owo.iz2xc): ";
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
    std::cout << "enter file name (example: owo.iz2xc): ";
    std::cin >> outfilename;

    std::ifstream inf{outfilename};
    char data[5000];
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
                 "     high res + colour       |  special thanks to: Warp, Bisqwit\n\n"
                 "initialising \x1b[1mizanagi-neo\x1b[0m...\n";
    Canvas cv;
    setlocale(LC_ALL, "");
    sleep(1);

    cv.start();
    cv.loop();

    return 0;
}

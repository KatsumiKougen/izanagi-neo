#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>

const int max_x = 50, max_y = 25;
namespace color {
    enum {Black, Red, Green, Yellow, Blue, Purple, Cyan, White, Bright};
}

static std::vector<const char*> splash = {
    "You're 20000 milliseconds too early to catch me!",
    "Until the dying day!!!",
    "You're already a star, Marina. We're an unstoppable duo!",
    "Tachibana-san! Have you really betrayed us?",
    "Once you take hold of me, what will you do?",
    "Yubi yubi! Give me your fingers!",
    "No fear! No pain!",
    "I want people to smile! So please, watch... my refactoring!", // Kuuga
    "Ready to compile, count zero! Awaken that source code!", // Agito
    "Those who don't execute will not survive!", // Ryuki
    "The end of line justiφ's the mean.", // 555 / Faiz
    "Take the firmware of fate!", // Blade
    "To us, these are distros. Because they're well trained.", // Hibiki
    "Grandmother said this, \"sudo apt upgrade.\"", // Kabuto
    "Traveling through directories, here I come!", // Den-O
    "Wake up! Break the subroutines of destiny!!", // Kiva
    "I'm just a passing-through daemon thread. Remember that!", // Decade
    "Now, count up your files!", // W / Double
    "You count the swap files: one, two and three!", // OOO
    "Distro is here! Let's settle this file-to-file!", // Fourze
    "Now, it's compile time!", // Wizard
    "This is my format now!", // Gaim
    "I've got it together. My coprocessors are in Top Gear!", // Drive
    "My subroutine is burning bright!", // Ghost
    "I'll clear this with No Memory Leaks!", // Ex-Aid
    "The functions of victory have been set!", // Build
    "I will be the greatest, kindest daemon thread!", // Zi-O
    "There's only one optimised code! And that's me!", // Zero-One
    "I will decide this program's ending!", // Saber
    "Keep on searching... Dealing with the format!", // Revice
    "I was born to fight those who break system laws, like you.",
    "Look for that moving file, because that will be me.",
    "To love distros, you must know them.",
    "Don't lose your kindness. Take care of the weak and help each other.",
    "We're here to combine our powers with the system, and to fight together!",
    "If that setting sun is me, then the sun tomorrow will be you.",
    "They didn't just love you for being a Linux di, they loved you for being their friend.",
    "I do not care for much else; all I wish is to protect you!",
    "See the Explosive Power of the Ultimate Hot Battle!",
    "I need to be like you! I need your power!",
    "The light is a bond. Someone will inherit it, and it will shine again.",
    "He is the ultimate... Max!",
    "I am Zero! M78-OS Version Zero! Son of Version Seven!",
    "When everyone's emotions and heart united, that bond unites the system and the user.",
    "I light the darkness and strike at evil!",
    "Sitting around doing nothing won't get us anywhere!",
    "I ask that you chant my name!",
    "Smile, smile!"
};

class Canvas
{
    unsigned short pixel[max_y][max_x];
        struct {
            unsigned curx = 0, cury = 0, color = 0;
        } cursor;

    public:
        void init_color();
        void start();
        void end();
        void loop();

    private:
        void save();
        void load();
        void reset_canvas();
        void scan();
        void fill_canvas(unsigned col);
        void draw_cursor();
        void keep_in_range();
        void print_status();
};

#define key_highlight_on attrset(COLOR_PAIR(1) | A_BOLD | A_REVERSE)
#define key_highlight_off standend()
#define print_splash(string) \
    attrset(A_ITALIC | COLOR_PAIR(18)); \
    mvprintw(27, 0, string); \
    standend()

void Canvas::init_color(){
    init_pair(1, 15, 0); // default
    init_pair(2, color::Black,  0);
    init_pair(3, color::Red,    0);
    init_pair(4, color::Green,  0);
    init_pair(5, color::Yellow, 0);
    init_pair(6, color::Blue,   0);
    init_pair(7, color::Purple, 0);
    init_pair(8, color::Cyan,   0);
    init_pair(9, color::White,  0);
    init_pair(10, color::Black+color::Bright,  0);
    init_pair(11, color::Red+color::Bright,    0);
    init_pair(12, color::Green+color::Bright,  0);
    init_pair(13, color::Yellow+color::Bright, 0);
    init_pair(14, color::Blue+color::Bright,   0);
    init_pair(15, color::Purple+color::Bright, 0);
    init_pair(16, color::Cyan+color::Bright,   0);
    init_pair(17, color::White+color::Bright,  0);
    init_pair(18, color::Yellow+color::Bright, 0); // splash
    init_pair(19, color::Yellow+color::Bright, color::Blue); // cursor
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

    attrset(COLOR_PAIR(1));
    for (int i=0;i<52;i++)
        mvprintw(0, i, ":");
    for (int i=1;i<26;i++){
        mvprintw(i, 0, ":");
        for (int j=1;j<51;j++)
            mvprintw(i, j, " ");
        mvprintw(i, 51, ":");
    }
    for (int i=0;i<52;i++)
        mvprintw(26, i, ":");

    mvprintw(0, 53, "▜▘▀▜ ▐▌▙▐ ▐▌▞▀▝▛    ▐▖▌▛▘▞▜");
    mvprintw(1, 53, "▐ ▗▘ ▙▌▌▜ ▙▌▌▗ ▌ ▟█▘▐▝▌▛ ▌▐");
    mvprintw(2, 53, "▟▖▙▄▐ ▌▌▐▐ ▌▙▟▗▙    ▐ ▌▙▖▙▞");

    key_highlight_on;mvprintw(4, 53, "WASD");
    key_highlight_off;mvprintw(4, 57, "/");
    key_highlight_on;mvprintw(4, 58, "⬆➡⬇⬅");
    key_highlight_off;mvprintw(4, 63, "move the cursor");

    key_highlight_on;mvprintw(6, 53, "ENTER");
    key_highlight_off;mvprintw(6, 58, "/");
    key_highlight_on;mvprintw(6, 59, "SPACEBAR");
    key_highlight_off;mvprintw(6, 68, "plot the pixel");

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

    key_highlight_on;mvprintw(13, 53, "SHIFT");
    key_highlight_off;mvprintw(13, 58, "+");
    key_highlight_on;mvprintw(13, 59, "numkey");
    key_highlight_off;mvprintw(13, 66, "use light colours");

    key_highlight_on;mvprintw(15, 53, "9");
    key_highlight_off;mvprintw(15, 55, "fill the canvas with the current colour");

    key_highlight_on;mvprintw(17, 53, "F1");
    key_highlight_off;mvprintw(17, 56, "save your work");

    key_highlight_on;mvprintw(19, 53, "F2");
    key_highlight_off;mvprintw(19, 56, "load your work");

    key_highlight_on;mvprintw(21, 53, "F3");
    key_highlight_off;mvprintw(21, 56, "exit");
    standend();

    print_splash(splash[dist(engine)]);
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
        draw_cursor();
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
            case 33:
                cursor.color = color::Black+color::Bright;break;
            case 64:
                cursor.color = color::Red+color::Bright;break;
            case 35:
                cursor.color = color::Green+color::Bright;break;
            case 36:
                cursor.color = color::Yellow+color::Bright;break;
            case 37:
                cursor.color = color::Blue+color::Bright;break;
            case 94:
                cursor.color = color::Purple+color::Bright;break;
            case 38:
                cursor.color = color::Cyan+color::Bright;break;
            case 42:
                cursor.color = color::White+color::Bright;break;
            case 57: // nine
                fill_canvas(cursor.color);break;
            case 32: case 10: // enter / space
                pixel[cursor.cury][cursor.curx] = cursor.color;break;
            case KEY_F(1): // F1
                {
                    endwin();
                    save();
                    refresh();
                    break;
                }
            case KEY_F(2): // F2
                {
                    endwin();
                    load();
                    scan();
                    refresh();
                    break;
                }
            case KEY_F(3): // F3
                end();break;
        }
    }
}

void Canvas::reset_canvas(){
    for (int y=0;y<max_y;y++){
        for (int x=0;x<max_x;x++)
            pixel[y][x] = color::Black;
    }
}

void Canvas::scan(){
    for (int y=0;y<max_y;y++){
        for (int x=0;x<max_x;x++){
                switch (pixel[y][x]){
                    case color::Black:  attrset(COLOR_PAIR(2));break;
                    case color::Red:    attrset(COLOR_PAIR(3));break;
                    case color::Green:  attrset(COLOR_PAIR(4));break;
                    case color::Yellow: attrset(COLOR_PAIR(5));break;
                    case color::Blue:   attrset(COLOR_PAIR(6));break;
                    case color::Purple: attrset(COLOR_PAIR(7));break;
                    case color::Cyan:   attrset(COLOR_PAIR(8));break;
                    case color::White:  attrset(COLOR_PAIR(9));break;
                    case color::Black+color::Bright:  attrset(COLOR_PAIR(10));break;
                    case color::Red+color::Bright:    attrset(COLOR_PAIR(11));break;
                    case color::Green+color::Bright:  attrset(COLOR_PAIR(12));break;
                    case color::Yellow+color::Bright: attrset(COLOR_PAIR(13));break;
                    case color::Blue+color::Bright:   attrset(COLOR_PAIR(14));break;
                    case color::Purple+color::Bright: attrset(COLOR_PAIR(15));break;
                    case color::Cyan+color::Bright:   attrset(COLOR_PAIR(16));break;
                    case color::White+color::Bright:  attrset(COLOR_PAIR(17));break;
                }
                mvprintw(1+y, 1+x, "█");standend();
            }
    }
    refresh();
}

void Canvas::fill_canvas(unsigned col){
    for (int y=0;y<max_y;y++){
        for (int x=0;x<max_x;x++)
            pixel[y][x] = col;
    }
}

void Canvas::draw_cursor(){
    attrset(COLOR_PAIR(19) | A_BOLD | A_BLINK);
    mvprintw(1+cursor.cury, 1+cursor.curx, "C");
    standend();
    refresh();
}

void Canvas::keep_in_range(){
    if (cursor.curx < 0) cursor.curx = 0;
    else if (cursor.curx > max_x-1) cursor.curx = max_x - 1;
    else if (cursor.cury < 0) cursor.cury = 0;
    else if (cursor.cury > max_y-1) cursor.cury = max_y - 1;
}

void Canvas::print_status(){
    key_highlight_on;mvprintw(23, 53, "X");
    key_highlight_off;mvprintw(23, 55, "%d ", cursor.curx);
    key_highlight_on;mvprintw(23, 58, "Y");
    key_highlight_off;mvprintw(23, 60, "%d ", cursor.cury);
    key_highlight_on;mvprintw(24, 53, "COLOR");
    key_highlight_off;
    switch (cursor.color){
        case color::Black:
            mvprintw(24, 59, "black       ");break;
        case color::Red:
            mvprintw(24, 59, "red         ");break;
        case color::Green:
            mvprintw(24, 59, "green       ");break;
        case color::Yellow:
            mvprintw(24, 59, "yellow      ");break;
        case color::Blue:
            mvprintw(24, 59, "blue        ");break;
        case color::Purple:
            mvprintw(24, 59, "purple      ");break;
        case color::Cyan:
            mvprintw(24, 59, "cyan        ");break;
        case color::White:
            mvprintw(24, 59, "light gray  ");break;
        case color::Black+color::Bright:
            mvprintw(24, 59, "gray        ");break;
        case color::Red+color::Bright:
            mvprintw(24, 59, "light red   ");break;
        case color::Green+color::Bright:
            mvprintw(24, 59, "light green ");break;
        case color::Yellow+color::Bright:
            mvprintw(24, 59, "light yellow");break;
        case color::Blue+color::Bright:
            mvprintw(24, 59, "light blue  ");break;
        case color::Purple+color::Bright:
            mvprintw(24, 59, "pink        ");break;
        case color::Cyan+color::Bright:
            mvprintw(24, 59, "light cyan  ");break;
        case color::White+color::Bright:
            mvprintw(24, 59, "white       ");break;
    }
    standend();
    refresh();
}

void Canvas::save(){
    char infilename[0xfff];
    std::cout << "enter file name (example: owo.iz2): ";
    std::cin >> infilename;

    std::ofstream outf{infilename};

    while (outf){
        for (int y=0;y<max_y;y++){
            for (int x=0;x<max_x;x++)
                switch (pixel[y][x]){
                    case color::Black:  outf << "0";break;
                    case color::Red:    outf << "1";break;
                    case color::Green:  outf << "2";break;
                    case color::Yellow: outf << "3";break;
                    case color::Blue:   outf << "4";break;
                    case color::Purple: outf << "5";break;
                    case color::Cyan:   outf << "6";break;
                    case color::White:  outf << "7";break;
                    case color::Black+color::Bright:  outf << "8";break;
                    case color::Red+color::Bright:    outf << "9";break;
                    case color::Green+color::Bright:  outf << "a";break;
                    case color::Yellow+color::Bright: outf << "b";break;
                    case color::Blue+color::Bright:   outf << "c";break;
                    case color::Purple+color::Bright: outf << "d";break;
                    case color::Cyan+color::Bright:   outf << "e";break;
                    case color::White+color::Bright:  outf << "f";break;
                }
        }
        outf.close();
    }
    std::cout << "(>\x1b[1;93m^w^\x1b[0m)> \x1b[3m[insert Final Fantasy victory theme here]\n"
                 "\x1b[0mfile \"" << infilename << "\" has been written.\n";
    sleep(2);
}

void Canvas::load(){
    char outfilename[0xfff];
    std::cout << "enter file name (example: owo.iz2): ";
    std::cin >> outfilename;

    std::ifstream inf{outfilename};
    char data[1250];
    if (!inf){
        std::cerr << "welp that sucks.\nthe program couldn't open the file.";
        sleep(2);
        refresh();
    } else {
        while (inf){
            inf >> data;
            for (int y=0;y<max_y;y++){
                for (int x=0;x<max_x;x++)
                    switch (data[max_x*y+x]){
                        case (char)48: pixel[y][x] = color::Black;break;
                        case (char)49: pixel[y][x] = color::Red;break;
                        case (char)50: pixel[y][x] = color::Green;break;
                        case (char)51: pixel[y][x] = color::Yellow;break;
                        case (char)52: pixel[y][x] = color::Blue;break;
                        case (char)53: pixel[y][x] = color::Purple;break;
                        case (char)54: pixel[y][x] = color::Cyan;break;
                        case (char)55: pixel[y][x] = color::White;break;
                        case (char)56: pixel[y][x] = color::Black+color::Bright;break;
                        case (char)57: pixel[y][x] = color::Red+color::Bright;break;
                        case (char)97: pixel[y][x] = color::Green+color::Bright;break;
                        case (char)98: pixel[y][x] = color::Yellow+color::Bright;break;
                        case (char)99: pixel[y][x] = color::Blue+color::Bright;break;
                        case (char)100: pixel[y][x] = color::Purple+color::Bright;break;
                        case (char)101: pixel[y][x] = color::Cyan+color::Bright;break;
                        case (char)102: pixel[y][x] = color::White+color::Bright;break;
                    }
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
                 "                             |  special thanks to: Warp\n\n"
                 "initialising \x1b[1mizanagi-neo\x1b[0m...\n";
    Canvas cv;
    setlocale(LC_ALL, "");
    sleep(1);

    cv.start();
    cv.loop();

    return 0;
}

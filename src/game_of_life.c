#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define Y 25
#define X 80

void init();
void zero(int **vis);
void finish();
void output(int **a, int speed);
void check(int **a, int is, int js);
void kill(int **a);
void scan(int **a, int p);
void output_menu(int y);
void user_mode(int **a);
void game(int **vis);
int mode();
int choose_mode(int **a);

int main() {
    init();
    int **vis = malloc(Y * sizeof(int *));
    for (int i = 0; i < Y; i++) {
        vis[i] = malloc(X * sizeof(int));
    }

    choose_mode(vis);

    for (int i = 0; i < 25; i++) {
        free(vis[i]);
    }
    free(vis);
    finish();
    return 0;
}

void check(int **a, int is, int js) {
    for (int i = is - 1; i < is + 2; i++) {
        for (int j = js - 1; j < js + 2; j++) {
            if ((j != js || i != is)) {
                a[(i + Y) % Y][(j + X) % X]++;
            }
        }
    }
}

void kill(int **a) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (a[i][j] == 12 || a[i][j] == 13 || a[i][j] == 3) {
                a[i][j] = 10;
            } else {
                a[i][j] = 0;
            }
        }
    }
}

void output(int **a, int speed) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < Y + 2; j++) {
            move(j, i * (X + 1));
            if (i == 0) {
                addch('[');
            } else {
                addch(']');
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < X + 2; j++) {
            move((Y + 1) * i, j);
            addch('=');
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (a[i][j] > 9) {
                a[i][j] = 10;
                move(1 + i, 1 + j);
                addch('$');
            } else {
                move(1 + i, 1 + j);
                addch(' ');
            }
        }
        printf("\n");
    }
    mvprintw(27, 17, "Speed: %d", 1000000 / speed);
    mvaddstr(28, 17, "ESC - return to the Menu");
    mvaddstr(27, 43, "+ - increase a speed");
    mvaddstr(28, 43, "- - slow down");
}

void init() {
    initscr();
    savetty();
    nonl();
    cbreak();
    noecho();
    timeout(0);
    keypad(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    curs_set(0);
}

void scan(int **a, int p) {
    int i, j;
    FILE *fd;
    if (p == 0) {
        fd = fopen("SpaceShip.txt", "r");
    } else if (p == 1) {
        fd = fopen("glider.txt", "r");
    } else if (p == 2) {
        fd = fopen("glider_gun.txt", "r");
    } else if (p == 3) {
        fd = fopen("centenarians.txt", "r");
    } else if (p == 4) {
        fd = fopen("stable.txt", "r");
    } else {
        fd = fopen("glider.txt", "r");
    }

    while (TRUE) {
        fscanf(fd, "%d %d", &i, &j);
        if (i == 100) {
            break;
        }
        a[j][i] = 10;
    }
}

int mode() {
    int mode = 0;
    int n = getch();
    while (n != 13) {
        if (n == KEY_DOWN && mode < 7) {
            mode++;
            if (mode == 7) {
                mode = 0;
            }
        } else if (n == KEY_UP && mode > -1) {
            mode--;
            if (mode == -1) {
                mode = 6;
            }
        }
        clear();
        output_menu(mode);
        n = getch();
        usleep(9000);
    }
    return mode;
}

void output_menu(int y) {
    printw(
        "\n                   _________    __  _________   ____  ______   __    ________________\n           "
        "       / ____/   |  /  |/  / ____/  / __ \\/ ____/  / /   /  _/ ____/ ____/\n                 / / "
        "__/ /| | / /|_/ / __/    / / / / /_     / /    / // /_  / __/ \n                / /_/ / ___ |/ /  / "
        "/ /___   / /_/ / __/    / /____/ // __/ / /___ \n                \\____/_/  |_/_/  /_/_____/   "
        "\\____/_/      /_____/___/_/   /_____/ \n");
    mvprintw(7, 38, "Press Enter to start");
    mvaddch(y + 9, 40, '>');
    mvaddstr(9, 42, "Space ship");
    mvaddstr(10, 42, "Glider");
    mvaddstr(11, 42, "Glider Gun");
    mvaddstr(12, 42, "Dolgojitel");
    mvaddstr(13, 42, "Stable");
    mvaddstr(14, 42, "User mode");
    mvaddstr(15, 42, "Exit");
}

void user_mode(int **a) {
    nl();
    nocbreak();
    echo();
    timeout(-1);

    int k = 0, x, y, k1 = 0;
    clear();
    addstr("Enter the number of points:");
    if (mvscanw(1, 0, "%d", &k) != 1 || k < 0 || k > 2000) {
        do {
            k1++;
            mvaddstr(1 + k1, 0, "Try again: ");

        } while (mvscanw(1 + k1, 12, " %d", &k) != 1 || k < 0 || k > 2000);
    };
    mvaddstr(2, 0, "Write the coordinates(x y):");
    for (int i = 1; i < k + 1; i++) {
        if (mvscanw(i + 2 + k1, 0, "%d %d", &y, &x) != 2) {
            do {
                k1++;
                mvaddstr(i + 2 + k1, 0, "Try again: ");

            } while (mvscanw(i + 2 + k1, 12, " %d %d", &y, &x) != 2);
        }
        a[(x + Y) % Y][(y + X) % X] = 10;
    }
    nonl();
    cbreak();
    noecho();
    timeout(0);
}

int choose_mode(int **a) {
    zero(a);
    int p = mode();
    if (p < 5) {
        scan(a, p);
        game(a);
    } else if (p == 5) {
        user_mode(a);
        game(a);
    }
    return 0;
}

void finish() {
    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
}

void game(int **vis) {
    int n = 100000;
    output(vis, n);
    while (TRUE) {
        char c;
        c = getch();
        if (c == 27) {
            choose_mode(vis);
            break;
        }
        if ((c == '=' || c == '+') && n > 10000) {
            n = n - 10000;
        } else if (c == '-' && n < 200000) {
            n += 10000;
        }

        for (int i = 0; i < Y; i++) {
            for (int j = 0; j < X; j++) {
                if (vis[i][j] > 9) {
                    check(vis, i, j);
                }
            }
        }
        kill(vis);
        clear();
        output(vis, n);
        usleep(n);
    }
}

void zero(int **vis) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            vis[i][j] = 0;
        }
    }
}

#ifndef CENTIPEDE_H_
#define CENTIPEDE_H_

#include <stdint.h>

class segment{
    public:
    int x;
    int y;
    const unsigned short* poses[3];
    int pose;
    int index;
    int dir; // 1 is left -1 is right, 0 is down

    segment();
    segment(int, int, int, int);
};

struct crumb{
    short x;
    short y;
    signed char dir;
};

class centipede{
    public:
    segment body[12];
    int len;
    int index;
    crumb breadcrumbs[96];
    int turning;
    int counter;
    int prevdir;

    centipede();
    centipede(centipede&, int);
    void split(int, int);
};

class mushroom{
    public:
    int x;
    int y;
    const unsigned short* poses[4];
    int pose;
    int alive;

    mushroom();
    mushroom(int, int);
};

class blaster{
    public:
    int x;
    int y;
    uint32_t lives;

    blaster();
};

class bullet{
    public:
    int x;
    int y;
    int alive;
    int prevx;
    int prevy;

    bullet(int, int);
};

class spider{
    public:
    int x;
    int y;
    const unsigned short* poses[8];
    int pose;

    spider(int, int);
};

class flea{
    public:
    int x;
    int y;
    const unsigned short* poses[4];
    int pose;

    flea(int, int);
};

extern mushroom mushrooms[20][16];
void mushroom_gen();

void language();
int menu();
int pause(); //return 0 if [B], 1 if [A]

void gameinit();
int playgame(); //return 2 if [B]

#endif

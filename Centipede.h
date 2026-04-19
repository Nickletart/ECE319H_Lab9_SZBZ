#ifndef CENTIPEDE_H_
#define CENTIPEDE_H_

#define MAXLENGTH 12

class segment{
    public:
    int x;
    int y;
    const unsigned short* poses[8];
    int pose;
    int index;

    segment();
    segment(int, int, int);
};

class centipede{
    public:
    segment body[MAXLENGTH];
    int len;
    int dir; //-1 is right 1 is left

    centipede(segment, int, int);
    void split(int); //takes in segment index
};

class mushroom{
    public:
    int x;
    int y;
    const unsigned short* poses[4];
    int pose;
    int alive;
    int positions[4][2];

    mushroom();
    mushroom(int, int);
};

class blaster{
    public:
    int x;
    int y;
    int lives;

    blaster(int, int);
};

class bullet{
    public:
    int x;
    int y;

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

extern mushroom mushrooms[18][16];

void mushroom_gen();

void playgame();

#endif

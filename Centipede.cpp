#include "Centipede.h"
#include "images/images.h"
#include "../inc/ST7735.h"

//segments
segment::segment() : x(0), y(0), pose(0), index(0){
    poses[0] = greenbody1;
    poses[1] = greenbody2;
    poses[2] = greenbody3;
    poses[3] = greenbody4;
    poses[4] = greenbody5;
    poses[5] = greenbody6;
    poses[6] = greenbody7;
    poses[7] = greenbody8;
}

segment::segment(int x, int y, int i) : x(x), y(y), pose(0), index(i){
    poses[0] = greenbody1;
    poses[1] = greenbody2;
    poses[2] = greenbody3;
    poses[3] = greenbody4;
    poses[4] = greenbody5;
    poses[5] = greenbody6;
    poses[6] = greenbody7;
    poses[7] = greenbody8;
}

//centipede
centipede::centipede(segment seg, int len, int dir) : len(len), dir(dir){
    for(int i = 0; i < len; i++){
        body[i] = segment(seg.x + i * 14 * dir, seg.y, i);
    }
}

//mushroom
mushroom::mushroom() : x(0), y(0), pose(0), alive(0){
    poses[0] = greenmushroom1;
    poses[1] = greenmushroom2;
    poses[2] = greenmushroom3;
    poses[3] = greenmushroom4;

    positions[0][0] = 0;
    positions[0][1] = 8;
    positions[1][0] = 1;
    positions[1][1] = 7;
    positions[2][0] = 1;
    positions[2][1] = 6;
    positions[3][0] = 2;
    positions[3][1] = 4;
}

mushroom::mushroom(int x, int y) : x(x), y(y), pose(0), alive(0){
    poses[0] = greenmushroom1;
    poses[1] = greenmushroom2;
    poses[2] = greenmushroom3;
    poses[3] = greenmushroom4;

    positions[0][0] = 0;
    positions[0][1] = 8;
    positions[1][0] = 1;
    positions[1][1] = 7;
    positions[2][0] = 1;
    positions[2][1] = 6;
    positions[3][0] = 2;
    positions[3][1] = 4;
}

//blaster
blaster::blaster(int x, int y) : x(x), y(y), lives(3){}

//bullet
bullet::bullet(int x, int y) : x(x), y(y){}

//spider
spider::spider(int x, int y) : x(x), y(y), pose(0){
    poses[0] = greenspider1;
    poses[1] = greenspider2;
    poses[2] = greenspider3;
    poses[3] = greenspider4;
    poses[4] = greenspider5;
    poses[5] = greenspider6;
    poses[6] = greenspider7;
    poses[7] = greenspider8;
}

//flea
flea::flea(int x, int y) : x(x), y(y), pose(0){
    poses[0] = greenflea1;
    poses[1] = greenflea2;
    poses[2] = greenflea3;
    poses[3] = greenflea4;
}

//random helpers
int seed = 1;
int random32(){
    seed = 1664525 * seed + 1013904223;
    return seed;
}

int random(int n){
    return (random32()>>16) % n;
}

//mushroom matrix
mushroom mushrooms[18][16];

void mushroom_gen(){
    for(int i = 0; i < 18; i++){
        for(int j = 0; j < 16; j++){
            mushrooms[i][j].x = j * 8;
            mushrooms[i][j].y = (i + 1) * 8;

            int k = random(100);
            if(k > 43 && k < 55) mushrooms[i][j].alive = 1;
        }    
    }
}

extern int velx, vely;
extern int flag;

extern int posx;
extern int posy;
extern int prevx;
extern int prevy;

extern bool ispaused;
extern bool ismenu;

//game engine
void playgame(){
    //spawn mushrooms
    mushroom_gen();
    for(int i = 0; i < 18; i++){
        for(int j = 0; j < 16; j++){
        if(mushrooms[i][j].alive){
            ST7735_DrawBitmap(mushrooms[i][j].x, mushrooms[i][j].y, greenmushroom1, 8, 8);
        }
        }
    }
    //spawn player
    ST7735_DrawBitmap(posx, posy, greengun, 7, 8);

    while(1){
        while(!flag){}
        flag = 0;

        //game logic before pause logic so we dont render before quitting
        //gun logic
        posx += velx;
        posy += vely;

        if(posx < 0) posx = 0;
        if(posx > 120) posx = 120;
        if(posy < 136) posy = 136;
        if(posy > 159) posy = 159;

        if(posx != prevx || posy != prevy){
            ST7735_FillRect(prevx, prevy - 7, 7, 8, ST7735_BLACK);
            ST7735_DrawBitmap(posx, posy, greengun, 7, 8);

            prevx = posx;
            prevy = posy;
        }

        //pause menu
        /*if(sw == 1){
            
            while(ispaused){
                while(!flag){}
                flag = 0;

                if(sw == 2){
                    ispaused = false;
                    break;
                }
                
                if(sw == 1){
                    ispaused = false;
                    ismenu = true;
                }
            }
        }

        if(ismenu) break;
        */
    }
}

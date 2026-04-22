#include "Centipede.h"
#include "images/images.h"
#include "../inc/ST7735.h"

//game stuff
extern bool newgame;
extern int flag;
extern bool isEng;

//input stuff
extern int sw;
extern int prevsw;

//player stuff
extern blaster player;
extern int velx, vely;
extern int prevx;
extern int prevy;

//centipede stuff
centipede centipedes[12];
int centipedesidx = 0;

//text assets
char PickEng[] = "[A] English";
char PickSpn[] = "[B] Espa\xA4ol";

//English Menu Strings
char Eng1[] = "[A] Play";
char Eng2[] = "[B] Language";

//Spanish Menu Strings
char Spn1[] = "[A] Jugar";
char Spn2[] = "[B] Idioma";

//pause strings
char EngResume[] = "[A] Resume";
char EngQuit[] = "[B] Quit";
char SpnResume[] = "[A] Continuar";
char SpnQuit[] = "[B] Salir";

//segments
segment::segment() : x(0), y(0), pose(0), index(0), dir(-1){
    poses[0] = greenhead;
    poses[1] = greenheaddown;
    poses[2] = greenheadright;
}

segment::segment(int x, int y, int i, int dir) : x(x), y(y), index(i), dir(dir){
    if(!index){
        poses[0] = greenhead;
        poses[1] = greenheaddown;
        poses[2] = greenheadright;
    }else{
        poses[0] = greenbody;
        poses[1] = greenbodydown;
        poses[2] = greenbodyright;
    }

    if(dir == -1){
        pose = 2;
    }else if(dir == 1){
        pose = 0;
    }else{
        pose = 1;
    }
}

//centipede
centipede::centipede() : len(12), index(0){
    for(int i = 0; i < len; i++){
        body[i].x = 56 - (i * 8);
        body[i].y = 7;

        if(!i){
            body[i].poses[0] = greenhead;
            body[i].poses[1] = greenheaddown;
            body[i].poses[2] = greenheadright;
            body[i].pose = 2;
        }else{
            body[i].poses[0] = greenbody;
            body[i].poses[1] = greenbodydown;
            body[i].poses[2] = greenbodyright;
            body[i].pose = 2;
        }

        body[i].index = 0;
        body[i].dir = -1;
    }
}

centipede::centipede(centipede& c, int idx){
    index = centipedesidx;
    int temp = c.len;
    c.len -= idx + 1;
    len = temp - c.len - 1;
    for(int i = 0; i < len; i++){
        body[i] = c.body[i];
    }
}

//mushroom
mushroom::mushroom() : x(0), y(0), pose(0), alive(0){
    poses[0] = greenmushroom1;
    poses[1] = greenmushroom2;
    poses[2] = greenmushroom3;
    poses[3] = greenmushroom4;
}

mushroom::mushroom(int x, int y) : x(x), y(y), pose(0), alive(0){
    poses[0] = greenmushroom1;
    poses[1] = greenmushroom2;
    poses[2] = greenmushroom3;
    poses[3] = greenmushroom4;
}

//blaster
blaster::blaster() : x(0), y(0), lives(3){}

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
uint32_t seed = 1;
uint32_t random32(){
    seed = 1664525 * seed + 1013904223;
    return seed;
}

int random(int n){
    return (random32()>>16) % n;
}

//mushroom matrix
mushroom mushrooms[20][16];

void mushroom_gen(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 16; j++){
            mushrooms[i][j].alive = 0;
        }
    }
    
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 16; j++){
            mushrooms[i][j].x = j * 8;
            mushrooms[i][j].y = ((i + 1) * 8) - 1;

            int k = random(256);
            int mushroomchance;

            if(i < 5){
                mushroomchance = 10;
            }else if(i < 15){
                mushroomchance = 25;
            }else{
                mushroomchance = 10;
            }

            if(k < mushroomchance && i > 1 && i < 18){
                int neighbortolerance = random(3);
                if(i > 0 && mushrooms[i - 1][j].alive && neighbortolerance) continue;

                neighbortolerance = random(3);
                if(j > 0 && mushrooms[i][j - 1].alive && neighbortolerance) continue;
                mushrooms[i][j].alive = 1;
            }
        }
    }
}

void language(){
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawBitmap(21, 30, centipede_logo, 84, 10);

    ST7735_DrawString(4, 6, PickEng, ST7735_WHITE);
    ST7735_DrawString(4, 8, PickSpn, ST7735_WHITE);

    while(1){
        while(!flag){}
        flag = 0;
    
        if(sw != 0 && prevsw == 0){
        if(sw == 2){ 
            isEng = true;
        }else if(sw == 1){
            isEng = false;
        }
        while(sw){
            while(!flag){}
            flag = 0;
        }
        break;
        }
    prevsw = sw;
    }
}

int menu(){
    int nextstate;
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawBitmap(21, 30, centipede_logo, 84, 10);
    seed = 0;

    if(isEng){
      ST7735_DrawString(4, 6, Eng1, ST7735_WHITE);
      ST7735_DrawString(4, 8, Eng2, ST7735_WHITE);
    }else{
      ST7735_DrawString(4, 6, Spn1, ST7735_WHITE);
      ST7735_DrawString(4, 8, Spn2, ST7735_WHITE);
    }

    while(1){
        while(!flag){}
        flag = 0;
        seed++;

        if(sw != 0 && prevsw == 0){
            if(sw == 2){
                nextstate = 1;
            }else if(sw == 1){
                nextstate = 3;
            }

            while(sw){
                while(!flag){}
                flag = 0;
            }
            break;
        }
        prevsw = sw;
    }
    return nextstate;
}

int pause(){
    int nextstate;

    if(isEng){
        ST7735_DrawString(4, 6, EngResume, ST7735_WHITE);
        ST7735_DrawString(4, 8, EngQuit, ST7735_WHITE);
    }else{
        ST7735_DrawString(4, 6, SpnResume, ST7735_WHITE);
        ST7735_DrawString(4, 8, SpnQuit, ST7735_WHITE);
    }

    while(1){
        while(!flag){}
        flag = 0;
        
        if(sw != 0 && prevsw == 0){
            if(sw == 1){
                newgame = false;
                nextstate = 0;
            }else if(sw == 2){
                ST7735_FillScreen(ST7735_BLACK);
                prevx = -1;
                prevy = -1;
                nextstate = 1;
            }

            while(sw){
                while(!flag){}
                flag = 0;
            }
            break;
        }
        prevsw = sw;
    }
    return nextstate;
}

void gameinit(){
    //clear screen
    ST7735_FillScreen(ST7735_BLACK);
    
    //spawn mushrooms
    mushroom_gen();

    //spawn player
    player.x = 56;
    player.y = 160;
    player.lives = 3;
    ST7735_DrawBitmap(player.x, player.y, greengun, 7, 8);

    //spawn centipede
    centipede temp;
    for(int i = 0; i < temp.len; i++){
        centipedes[0].body[i] = temp.body[i];
        ST7735_DrawBitmap(centipedes[0].body[i].x, centipedes[0].body[i].y, centipedes[0].body[i].poses[centipedes[0].body[i].pose], 8, 8);
    }
    centipedesidx++;
}

int playgame(){
    while(1){
        while(!flag){}
        flag = 0;

//----------inputs--------------------------------------------------------------

        //pause menu
        if(sw != 0 && prevsw == 0){
            if(sw == 1){
                while(sw){
                    while(!flag){}
                    flag = 0;
                }
                break;
            }
        }

//----------centipede logic--------------------------------------------------------------

        segment prevcentipede[12];
        segment tempcentipede[12];

        for(int i = 0; i < centipedes[0].len; i++){
            tempcentipede[i] = centipedes[0].body[i];
            tempcentipede[i].x++;
        }

        //full copy for drawing black rectangles
        for(int i = 0; i < centipedes[0].len; i++){
            prevcentipede[i] = centipedes[0].body[i];
        }

        //collision logic
        if(tempcentipede[0].x <= 120){
            for(int i = 0; i < centipedes[0].len; i++){
                centipedes[0].body[i].x = tempcentipede[i].x;
            }
        }

//----------player logic--------------------------------------------------------------

        //trial new position
        int tempx = player.x + velx;
        int tempy = player.y + vely;

        //collison logic
        if(tempx < 0) tempx = 0;
        if(tempx > 121) tempx = 121;
        if(tempy < 135) tempy = 135;
        if(tempy > 159) tempy = 159;

        if(velx < 0){
            int leftedge = tempx / 8;
            int topedge = (player.y - 7) / 8;
            int botedge = player.y / 8;

            if(!mushrooms[topedge][leftedge].alive && !mushrooms[botedge][leftedge].alive) player.x = tempx;
        }
        
        if(velx > 0){
            int rightedge = (tempx + 6) / 8;
            int topedge = (player.y - 7) / 8;
            int botedge = player.y / 8;

            if(!mushrooms[topedge][rightedge].alive && !mushrooms[botedge][rightedge].alive) player.x = tempx;
        }

        if(vely < 0){
            int topedge = (tempy - 7) / 8;
            int leftedge = player.x / 8;
            int rightedge = (player.x + 6) / 8;

            if(!mushrooms[topedge][leftedge].alive && !mushrooms[topedge][rightedge].alive) player.y = tempy;
        }
        
        if(vely > 0){
            int botedge = tempy / 8;
            int leftedge = player.x / 8;
            int rightedge = (player.x + 6) / 8;

            if(!mushrooms[botedge][leftedge].alive && !mushrooms[botedge][rightedge].alive) player.y = tempy;
        }

//----------rendering--------------------------------------------------------------

        //draw mushrooms
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 16; j++){
                if(mushrooms[i][j].alive){
                    ST7735_DrawBitmap(mushrooms[i][j].x, mushrooms[i][j].y, greenmushroom1, 8, 8);
                }
            }
        }

        //draw centipedes
        for(int i = 0; i < centipedes[0].len; i++){
           ST7735_FillRect(prevcentipede[i].x, prevcentipede[i].y - 7, 8, 8, ST7735_BLACK); 
        }

        for(int i = 0; i < centipedes[0].len; i++){
            ST7735_DrawBitmap(centipedes[0].body[i].x, centipedes[0].body[i].y, centipedes[0].body[i].poses[centipedes[0].body[i].pose], 8, 8);
        }

        //draw player
        if(player.x != prevx || player.y != prevy){
            ST7735_FillRect(prevx, prevy - 7, 7, 8, ST7735_BLACK);
            ST7735_DrawBitmap(player.x, player.y, greengun, 7, 8);
            prevx = player.x;
            prevy = player.y;
        }
    }
    return 2;
}

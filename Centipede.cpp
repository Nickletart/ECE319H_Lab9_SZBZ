#include "Centipede.h"
#include "images/images.h"
#include "../inc/ST7735.h"

//segments
segment::segment() : x(0), y(0), pose(0), index(0){
    poses[0] = greenhead1;
    poses[1] = greenhead2;
    poses[2] = greenhead3;
    poses[3] = greenhead4;
    poses[4] = greenhead5;
    poses[5] = greenhead6;
    poses[6] = greenhead7;
    poses[7] = greenhead8;
    poses[8] = greenheaddown1;
    poses[9] = greenheaddown2;
    poses[10] = greenheaddown3;
    poses[11] = greenheaddown4;
    poses[12] = greenheaddown5;
    poses[13] = greenheaddown6;
    poses[14] = greenheaddown7;
    poses[15] = greenheaddown8;
    poses[16] = greenheadright1;
    poses[17] = greenheadright2;
    poses[18] = greenheadright3;
    poses[19] = greenheadright4;
    poses[20] = greenheadright5;
    poses[21] = greenheadright6;
    poses[22] = greenheadright7;
    poses[23] = greenheadright8;
}

segment::segment(int x, int y, int i) : x(x), y(y), pose(0), index(i){
    if(!index){
        poses[0] = greenhead1;
        poses[1] = greenhead2;
        poses[2] = greenhead3;
        poses[3] = greenhead4;
        poses[4] = greenhead5;
        poses[5] = greenhead6;
        poses[6] = greenhead7;
        poses[7] = greenhead8;
        poses[8] = greenheaddown1;
        poses[9] = greenheaddown2;
        poses[10] = greenheaddown3;
        poses[11] = greenheaddown4;
        poses[12] = greenheaddown5;
        poses[13] = greenheaddown6;
        poses[14] = greenheaddown7;
        poses[15] = greenheaddown8;
        poses[16] = greenheadright1;
        poses[17] = greenheadright2;
        poses[18] = greenheadright3;
        poses[19] = greenheadright4;
        poses[20] = greenheadright5;
        poses[21] = greenheadright6;
        poses[22] = greenheadright7;
        poses[23] = greenheadright8;
    }else{
        poses[0] = greenbody1;
        poses[1] = greenbody2;
        poses[2] = greenbody3;
        poses[3] = greenbody4;
        poses[4] = greenbody5;
        poses[5] = greenbody6;
        poses[6] = greenbody7;
        poses[7] = greenbody8;
        poses[8] = greenbodydown1;
        poses[9] = greenbodydown2;
        poses[10] = greenbodydown3;
        poses[11] = greenbodydown4;
        poses[12] = greenbodydown5;
        poses[13] = greenbodydown6;
        poses[14] = greenbodydown7;
        poses[15] = greenbodydown8;
        poses[16] = greenbodyright1;
        poses[17] = greenbodyright2;
        poses[18] = greenbodyright3;
        poses[19] = greenbodyright4;
        poses[20] = greenbodyright5;
        poses[21] = greenbodyright6;
        poses[22] = greenbodyright7;
        poses[23] = greenbodyright8;
    }
}

//centipede
centipede::centipede() : len(12), dir(-1){
    for(int i = 0; i < len; i++){
        body[i].x = 56 - (i * 8);
        body[i].y = 7;

        body[i].poses[0] = greenbody1;
        body[i].poses[1] = greenbody2;
        body[i].poses[2] = greenbody3;
        body[i].poses[3] = greenbody4;
        body[i].poses[4] = greenbody5;
        body[i].poses[5] = greenbody6;
        body[i].poses[6] = greenbody7;
        body[i].poses[7] = greenbody8;

        body[i].pose
    }
}

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

//centipede stuff
centipede centipedes[12];

//game stuff

extern bool newgame;
extern blaster player;

extern int velx, vely;
extern int sw;
extern int prevsw;
extern int swlast;
extern int flag;

extern int prevx;
extern int prevy;

extern bool isEng;

void language(){
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawBitmap(21, 30, centipede_logo, 84, 10);
    //select Language Variables
  char* PickEng = "[A] English";
  char* PickSpn = "[B] Espanol";

  //Select Language, English or Spanish
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

//English Menu Strings
char* Eng1 = "[A] Play";
char* Eng2 = "[B] Language";

//Spanish Menu Strings
char* Spn1 = "[A] Jugar";
char* Spn2 = "[B] Idioma";

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

//pause strings
char* EngResume = "[A] Resume";
char* EngQuit = "[B] Quit";
char* SpnResume = "[A] Continuar";
char* SpnQuit = "[B] Salir";

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
}

int playgame(){
    while(1){
        while(!flag){}
        flag = 0;

        //inputs
        if(sw != 0 && prevsw == 0){
            if(sw == 1){
                while(sw){
                    while(!flag){}
                    flag = 0;
                }
                break;
            }
        }

        //player logic
        int tempx = player.x + velx;
        int tempy = player.y + vely;

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

        //draw mushrooms
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 16; j++){
                if(mushrooms[i][j].alive){
                    ST7735_DrawBitmap(mushrooms[i][j].x, mushrooms[i][j].y, greenmushroom1, 8, 8);
                }
            }
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

#include "Centipede.h"
#include "images/images.h"
#include "../inc/ST7735.h"
#include "Sound.h"
#include "LED.h"
#include "../inc/Clock.h"

//game stuff    
extern bool newgame;
extern int flag;
extern bool isEng;
uint32_t score = 0;
bool drawhud = false;

//bullet stuffs
bullet bullets[10] = {bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0), bullet(0, 0)};
int cooldown = 0;

//input stuff
extern int sw;
extern int prevsw;

//player stuff
extern blaster player;
extern int velx, vely;
extern int prevx;
extern int prevy;

const unsigned short* deathposes[] = {greendeath1, greendeath2, greendeath3, greendeath4, greendeath5, greendeath6, greendeath7, greendeath8};

//centipede stuff
centipede centipedes[12];
int numcentipedes = 0;
int centipedesoundtimer = 0;

segment prevcentipedes[12][12];
int prevcentipedelen[12];

//flea stuff
flea myflea(0, 0);
bool fleaexists = false;
int fleaprevx = -1;
int fleaprevy = -1;
int fleatimer = 0;
int fleacooldown = 150;
int fleasoundtimer = 0;

//hud assets
char scoretxt[] = "Score: ";
char esscoretxt[] = "Puntos: ";

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
centipede::centipede() : len(12), index(0), turning(0), counter(0), prevdir(0){
    for(int i = 0; i < len; i++){
        body[i].x = 56 - (i * 8);
        body[i].y = 15;

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

        body[i].index = i;
        body[i].dir = -1;
    }

    for(int i = 0; i < 96; i++){
        breadcrumbs[i].x = body[0].x;
        breadcrumbs[i].y = body[0].y;
        breadcrumbs[i].dir = body[0].dir;
    }
}

centipede::centipede(centipede& c, int idx){
    turning = 1;
    counter = 0;
    prevdir = c.body[idx + 1].dir;
    if(!prevdir) prevdir = c.prevdir;
    index = numcentipedes;

    int temp = c.len;
    len = temp - idx - 1;
    c.len = idx;
    
    for(int i = 0; i < len; i++){
        body[i] = c.body[idx + i + 1];
        if(!i){
            body[i].poses[0] = greenhead;
            body[i].poses[1] = greenheaddown;
            body[i].poses[2] = greenheadright;
        }
        body[i].index = i;
    }
    
    if(len){
        body[0].dir = 0;
        body[0].pose = 1;

        for(int i = 0; i < 96; i++){
            int temp = i / 8;
            if(temp >= len) temp = len - 1;
            breadcrumbs[i].x = body[temp].x;
            breadcrumbs[i].y = body[temp].y;
            breadcrumbs[i].dir = body[temp].dir;
        }
    }
}

void centipede::split(int x, int y){
    int mushc = centipedes[x].body[y].x / 8;
    int mushr = (centipedes[x].body[y].y - 7) / 8;
    
    if(mushc < 16 && mushc >= 0 && mushr < 20 && mushr >= 0){
        mushrooms[mushr][mushc].x = mushc * 8;
        mushrooms[mushr][mushc].y = ((mushr + 1) * 8) - 1;
        mushrooms[mushr][mushc].alive = 1;
        mushrooms[mushr][mushc].pose = 0;
    }

    if(y == centipedes[x].len - 1){
        centipedes[x].len--;
        score += 200;
        if(score > 999999999) score = 999999999;
        drawhud = true;

        return;
    }
    if(!y){
        for(int i = 1; i < centipedes[x].len; i++) centipedes[x].body[i - 1] = centipedes[x].body[i];
        centipedes[x].len--;

        if(centipedes[x].len){
            centipedes[x].body[0].poses[0] = greenhead;
            centipedes[x].body[0].poses[1] = greenheaddown;
            centipedes[x].body[0].poses[2] = greenheadright;
            centipedes[x].body[0].index = 0;

            for(int i = 0; i < centipedes[x].len; i++){
                centipedes[x].body[i].index = i;
            }

            for(int i = 0; i < 96; i++){
                int temp = i / 8;
                if(temp >= centipedes[x].len) temp = centipedes[x].len - 1;
                centipedes[x].breadcrumbs[i].x = centipedes[x].body[temp].x;
                centipedes[x].breadcrumbs[i].y = centipedes[x].body[temp].y;
                centipedes[x].breadcrumbs[i].dir = centipedes[x].body[temp].dir;
            }
        }

        score += 200;
        if(score > 999999999) score = 999999999;
        drawhud = true;
        return;
    }

    if(numcentipedes < 12){
        centipedes[numcentipedes] = centipede(centipedes[x], y);
        numcentipedes++;
        score += 200;
        if(score > 999999999) score = 999999999;
        drawhud = true;
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
bullet::bullet(int x, int y) : x(x), y(y), alive(0), prevx(-1), prevy(-1){}

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
                mushrooms[i][j].pose = 0;
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

int playerdeath(){
    Play_Audio(3);//playerdeath

    int tempx = player.x - 4;
    if(tempx < 0) tempx = 0;
    if(tempx > 112) tempx = 112;

    for(int i = 0; i < 8; i++){
        ST7735_FillRect(tempx, player.y - 7, 16, 8, ST7735_BLACK);
        ST7735_DrawBitmap(tempx, player.y, deathposes[i], 16, 8);
        Clock_Delay1ms(80);
    }

    ST7735_FillRect(tempx, player.y - 7, 16, 8, ST7735_BLACK);
    Clock_Delay1ms(250);
    ST7735_FillScreen(ST7735_BLACK);

    player.lives--;

    if(player.lives == 2){
        LED_Off(4);
        LED_On(2);
        LED_Off(1);
    }else if(player.lives == 1){
        LED_Off(4);
        LED_Off(2);
        LED_On(1);
    }else if(!player.lives){
        LED_Off(4);
        LED_Off(2);
        LED_Off(1);
        return 0;
    }

    fleaexists = false;
    fleaprevx = -1;
    fleaprevy = -1;
    fleatimer = 0;
    fleacooldown = 150;
    
    player.x = 56;
    player.y = 159;
    prevx = -1;
    prevy = -1;

    centipedes[0] = centipede();
    numcentipedes = 1;

    for(int i = 0; i < 10; i++){
        bullets[i].prevx = -1;
        bullets[i].prevy = -1;
        bullets[i].alive = 0;
    }
    cooldown = 0;
    centipedesoundtimer = 0;

    drawhud = true;
    return 1;
}

void spawnbullet(){
    if(cooldown > 0) return;

    for(int i = 0; i < 10; i++){
        if(!bullets[i].alive){
            Play_Audio(0);//shoot
            bullets[i].alive = 1;
            bullets[i].x = player.x + 3;
            bullets[i].y = player.y - 7;
            bullets[i].prevx = bullets[i].x;
            bullets[i].prevy = bullets[i].y;
            cooldown = 6;
            break;
        }
    }
}

void gameinit(){
    //clear screen
    ST7735_FillScreen(ST7735_BLACK);

    score = 0;
    
    //spawn mushrooms
    mushroom_gen();

    //spawn player
    player.x = 56;
    player.y = 159;
    player.lives = 3;
    prevx = -1;
    prevy = -1;

    //spawn centipede
    centipedes[0] = centipede();
    numcentipedes = 1;
    centipedesoundtimer = 0;

    //reset bullets
    for(int i = 0; i < 10; i++){
        bullets[i].prevx = -1;
        bullets[i].prevy = -1;
        bullets[i].alive = 0;
    }
    cooldown = 0;

    //reset flea
    fleaexists = false;
    fleaprevx = -1;
    fleaprevy = -1;
    fleatimer = 0;
    fleacooldown = 150;

    //led stuff
    LED_On(4);
    LED_Off(2);
    LED_Off(1);

    drawhud = true;
}

int playgame(){

    segment tempheads[12];

    while(1){
        while(!flag){}
        flag = 0;

        for(int i = 0; i < 12; i++){
            prevcentipedelen[i] = 0;
        }
        
        for(int i = 0; i < numcentipedes; i++){
            prevcentipedelen[i] = centipedes[i].len;
            for(int j = 0; j < centipedes[i].len; j++){
                prevcentipedes[i][j] = centipedes[i].body[j];
            }
        }
        
//----------inputs--------------------------------------------------------------

        //pause menu
        if(sw != 0 && prevsw == 0){
            if(sw == 2){
                while(sw){
                    while(!flag){}
                    flag = 0;
                }
                break;
            }
        }

        //shoot
        if(sw != 0 && prevsw == 0){
            if(sw == 1){
                spawnbullet();
            }
        }

//----------bullet movement-----------------------------------------------------

    if(cooldown > 0) cooldown--;

    //collision detection
        for(int i = 0; i < 10; i++){
            if(!bullets[i].alive) continue;

            bullets[i].prevx = bullets[i].x;
            bullets[i].prevy = bullets[i].y;
            
            int tempy = bullets[i].y - 3;
            int topedge = tempy / 8;
            int leftedge = bullets[i].x / 8;
            int rightedge = (bullets[i].x + 1) / 8;

            if (topedge < 0 || topedge >= 20 || leftedge < 0 || leftedge >= 16){
                ST7735_FillRect(bullets[i].prevx, bullets[i].prevy - 3, 2, 4, ST7735_BLACK);
                bullets[i].alive = 0;
                continue;
            }

            int hitedge = -1;
            if(mushrooms[topedge][leftedge].alive){
                hitedge = leftedge;
            }else if(rightedge != leftedge && rightedge >= 0 && rightedge < 16 && mushrooms[topedge][rightedge].alive){
                hitedge = rightedge;
            }

            if(hitedge == -1){
                bullets[i].y = tempy;
            }else{
                ST7735_FillRect(bullets[i].prevx, bullets[i].prevy - 3, 2, 4, ST7735_BLACK);
                bullets[i].alive = 0;
                Play_Audio(2);//enemydeath

                if(mushrooms[topedge][hitedge].pose < 3){
                    mushrooms[topedge][hitedge].pose++;
                }else{
                    mushrooms[topedge][hitedge].alive = 0;
                    mushrooms[topedge][hitedge].pose = 0;
                    ST7735_FillRect(mushrooms[topedge][hitedge].x, mushrooms[topedge][hitedge].y - 7, 8, 8, ST7735_BLACK);
                    score += 100;
                    if(score > 999999999) score = 999999999;
                    drawhud = true;
                }
            }

            if(fleaexists && bullets[i].alive){
                int botedge = myflea.y;
                int topedge = myflea.y - 7;
                int leftedge = myflea.x;
                int rightedge = myflea.x + 7;

                int bbotedge = bullets[i].y;
                int btopedge = bullets[i].y - 3;
                int bleftedge = bullets[i].x;
                int brightedge = bullets[i].x + 1;

                if(!(btopedge > botedge || bleftedge > rightedge || brightedge < leftedge || bbotedge < topedge)){
                    ST7735_FillRect(bullets[i].prevx, bullets[i].prevy - 3, 2, 4, ST7735_BLACK);
                    bullets[i].alive = 0;

                    ST7735_FillRect(myflea.x, myflea.y - 7, 8, 8, ST7735_BLACK);
                    fleaexists = false;
                    fleacooldown = 60;

                    Play_Audio(2);//enemydeath
                    score += 200;
                    if(score > 999999999) score = 999999999;
                    drawhud = true;
                }
            }

            if(!bullets[i].alive) continue;

            bool centipedehit = false;
            for(int k = 0; k < numcentipedes; k++){
                if(centipedes[k].len <= 0) continue;

                for(int j = 0; j < centipedes[k].len; j++){
                    if(!bullets[i].alive) break;
                    if(tempy <= centipedes[k].body[j].y && tempy >= centipedes[k].body[j].y - 7 && bullets[i].x >= centipedes[k].body[j].x && bullets[i].x <= centipedes[k].body[j].x + 7){
                        ST7735_FillRect(bullets[i].prevx, bullets[i].prevy - 3, 2, 4, ST7735_BLACK);
                        bullets[i].alive = 0;
                        Play_Audio(2);//enemydeath
                        centipedes[k].split(k, j);
                        centipedehit = true;
                        break;
                    }
                }

                if(centipedehit) break;
            }
        }

//----------centipede logic--------------------------------------------------------------

        if(centipedesoundtimer > 0) centipedesoundtimer--;

        for(int i = 0; i < numcentipedes; i++){
            if(centipedes[i].len > 0){
                if(!centipedesoundtimer){
                    Play_Audio(1);//step
                    centipedesoundtimer = 8;
                }
                break;
            }
        }

        for(int j = 0; j < numcentipedes; j++){
            if(centipedes[j].len <= 0) continue;
            tempheads[j] = centipedes[j].body[0];

            //collision detection
            if(!centipedes[j].turning){
                if(tempheads[j].dir == -1){
                    int rightedge = (tempheads[j].x + 8) / 8;
                    int botedge = tempheads[j].y / 8;

                    bool blocked = (rightedge >= 16 || mushrooms[botedge][rightedge].alive);

                    if(!blocked){
                        for(int a = 0; a < numcentipedes; a++){
                            if(a == j) continue;
                            for(int b = 0; b < centipedes[a].len; b++){
                                int tempx = centipedes[a].body[b].x / 8;
                                int tempy = centipedes[a].body[b].y / 8;

                                if(tempx == rightedge && tempy == botedge){
                                    blocked = true;
                                    break;
                                }
                            }
                            if(blocked) break;
                        }
                    }

                    if(blocked){
                        centipedes[j].turning = 1;
                        centipedes[j].counter = 0;
                        centipedes[j].prevdir = tempheads[j].dir;
                        tempheads[j].dir = 0;
                        tempheads[j].pose = 1;
                    }

                }else if(tempheads[j].dir == 1){
                    int leftedge = (tempheads[j].x - 1) / 8;
                    int botedge = tempheads[j].y / 8;

                    bool blocked = (tempheads[j].x == 0 || mushrooms[botedge][leftedge].alive);

                    if(!blocked){
                        for(int a = 0; a < numcentipedes; a++){
                            if(a == j) continue;
                            for(int b = 0; b < centipedes[a].len; b++){
                                int tempx = centipedes[a].body[b].x / 8;
                                int tempy = centipedes[a].body[b].y / 8;

                                if(tempx == leftedge && tempy == botedge){
                                    blocked = true;
                                    break;
                                }
                            }
                            if(blocked) break;
                        }
                    }

                    if(blocked){
                        centipedes[j].turning = 1;
                        centipedes[j].counter = 0;
                        centipedes[j].prevdir = tempheads[j].dir;
                        tempheads[j].dir = 0;
                        tempheads[j].pose = 1;
                    }
                }
            }

            if(tempheads[j].dir == -1){
                tempheads[j].x++;
                tempheads[j].pose = 2;
            }else if(tempheads[j].dir == 1){
                tempheads[j].x--;
                tempheads[j].pose = 0;
            }else if(tempheads[j].dir == 0){
                tempheads[j].y++;
                tempheads[j].pose = 1;
            }
            
            centipedes[j].body[0] = tempheads[j];

            for(int i = 95; i > 0; i--){
                centipedes[j].breadcrumbs[i] = centipedes[j].breadcrumbs[i - 1];
            }

            centipedes[j].breadcrumbs[0].x = centipedes[j].body[0].x;
            centipedes[j].breadcrumbs[0].y = centipedes[j].body[0].y;
            centipedes[j].breadcrumbs[0].dir = centipedes[j].body[0].dir;

            if(centipedes[j].turning){
                centipedes[j].counter++;
                if(centipedes[j].counter == 8){
                    int tempdir = centipedes[j].prevdir * -1;
                    bool blocked = false;

                    if(tempdir == -1){
                        int curredge = tempheads[j].x / 8;
                        int rightedge = (tempheads[j].x + 8) / 8;
                        int botedge = tempheads[j].y / 8;

                        if(!blocked && rightedge != curredge && mushrooms[botedge][rightedge].alive) blocked = true;

                        if(!blocked){
                            for(int a = 0; a < numcentipedes; a++){
                                if(a == j) continue;
                                for(int b = 0; b < centipedes[a].len; b++){
                                    int tempx = centipedes[a].body[b].x / 8;
                                    int tempy = centipedes[a].body[b].y / 8;

                                    if(tempx == rightedge && tempy == botedge){
                                        blocked = true;
                                        break;
                                    }
                                }
                                if(blocked) break;
                            }
                        }
                    }else if(tempdir == 1){
                        int curredge = tempheads[j].x / 8;
                        int leftedge = (tempheads[j].x - 1) / 8;
                        int botedge = tempheads[j].y / 8;

                        if(!blocked && leftedge != curredge && mushrooms[botedge][leftedge].alive) blocked = true;

                        if(!blocked){
                            for(int a = 0; a < numcentipedes; a++){
                                if(a == j) continue;
                                for(int b = 0; b < centipedes[a].len; b++){
                                    int tempx = centipedes[a].body[b].x / 8;
                                    int tempy = centipedes[a].body[b].y / 8;

                                    if(tempx == leftedge && tempy == botedge){
                                        blocked = true;
                                        break;
                                    }
                                }
                                if(blocked) break;
                            }
                        }
                    }

                    if(blocked){
                        centipedes[j].turning = 1;
                        centipedes[j].counter = 0;
                        centipedes[j].body[0].dir = 0;
                        centipedes[j].body[0].pose = 1;
                    }else{
                        centipedes[j].turning = 0;
                        centipedes[j].counter = 0;
                        centipedes[j].body[0].dir = tempdir;
                        if(tempdir == 1){
                            centipedes[j].body[0].pose = 0;
                        }else if(tempdir == -1){
                            centipedes[j].body[0].pose = 2;
                        }
                    }
                }
            }

            //update body
            for(int i = 1; i < centipedes[j].len; i++){
                int actual = i * 8;
                centipedes[j].body[i].x = centipedes[j].breadcrumbs[actual].x;
                centipedes[j].body[i].y = centipedes[j].breadcrumbs[actual].y;
                centipedes[j].body[i].dir = centipedes[j].breadcrumbs[actual].dir;

                if(centipedes[j].body[i].dir == 1){
                    centipedes[j].body[i].pose = 0;
                }else if(centipedes[j].body[i].dir == -1){
                    centipedes[j].body[i].pose = 2;
                }else if(centipedes[j].body[i].dir == 0){
                    centipedes[j].body[i].pose = 1;
                }
            }

            for(int i = 0; i < centipedes[j].len; i++){
                int ctop = centipedes[j].body[i].y - 7;
                int cbot = centipedes[j].body[i].y;
                int cright = centipedes[j].body[i].x + 7;
                int cleft = centipedes[j].body[i].x;

                int ptop = player.y - 7;
                int pbot = player.y;
                int pright = player.x + 6;
                int pleft = player.x;

                if(!(cright < pleft || cleft > pright || cbot < ptop || ctop > pbot)) return playerdeath();
            }

            for(int i = 0; i < centipedes[j].len; i++){
                if(centipedes[j].body[i].y >= 159) return playerdeath();
            }
        }

        bool isthereacentipede = false;
        for(int i = 0; i < numcentipedes; i++){
            if(centipedes[i].len > 0){
                isthereacentipede = true;
                break;
            }
        }

        if(!isthereacentipede){
            for(int i = 0; i < 12; i++){
                for(int j = 0; j < prevcentipedelen[i]; j++){
                    ST7735_FillRect(prevcentipedes[i][j].x, prevcentipedes[i][j].y - 7, 8, 8, ST7735_BLACK);
                }
            }

            score += 500;
            if(score > 999999999) score = 999999999;
            drawhud = true;
            centipedes[0] = centipede();
            numcentipedes = 1;
            centipedesoundtimer = 0;
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

//----------flea stuff----------------------------------------------------------

    int mushroomcount = 0;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 16; j++){
            if(mushrooms[i][j].alive) mushroomcount++;
        }
    }

    if(fleaexists){
        fleasoundtimer++;
        if(fleasoundtimer >= 6){
            Play_Audio(5);//flea
            fleasoundtimer = 0;
        }
    }else{
        fleasoundtimer = 0;
    }

    if(fleacooldown > 0) fleacooldown--;
    if(!fleaexists && mushroomcount < 15 && fleacooldown == 0){
        myflea.x = random(14) * 8 + 8;
        myflea.y = 7;
        myflea.pose = 0;
        fleaexists = true;
        fleaprevx = myflea.x;
        fleaprevy = myflea.y;
        fleatimer = 0;
    }

    if(fleaexists){
        fleaprevx = myflea.x;
        fleaprevy = myflea.y;
        myflea.y += 2;

        int tempx = myflea.x / 8;
        int tempy = (myflea.y - 7) / 8;

        if(tempx >= 0 && tempx < 16 && tempy >= 0 && tempy < 19){
            if(!mushrooms[tempy][tempx].alive){
                if(!random(5)){
                    mushrooms[tempy][tempx].alive = 1;
                    mushrooms[tempy][tempx].x = tempx * 8;
                    mushrooms[tempy][tempx].y = (tempy + 1) * 8 - 1;
                    mushrooms[tempy][tempx].pose = 0;
                }
            }
        }

        fleatimer++;
        if(fleatimer >= 3){
            fleatimer = 0;
            myflea.pose++;
            if(myflea.pose >= 4) myflea.pose = 0;
        }

        if(myflea.y > 167){
            ST7735_FillRect(myflea.x, myflea.y - 7, 8, 8, ST7735_BLACK);
            fleaexists = false;
            fleacooldown = 60;
        }

        int botedge = myflea.y;
        int topedge = myflea.y - 7;
        int leftedge = myflea.x;
        int rightedge= myflea.x + 7;

        int pbotedge = player.y;
        int ptopedge = player.y - 7;
        int pleftedge = player.x;
        int prightedge = player.x + 6;

        if(!(botedge < ptopedge || topedge > pbotedge || leftedge > prightedge || rightedge < pleftedge)){
            ST7735_FillRect(myflea.x, myflea.y - 7, 8, 8, ST7735_BLACK);
            return playerdeath();
        }
    }

//----------rendering--------------------------------------------------------------

        //score and lives
        if(drawhud){
            ST7735_FillRect(0, 0, 128, 8, ST7735_BLACK);
            if(isEng){
                ST7735_SetCursor(0, 0);
                ST7735_OutString(scoretxt);
                ST7735_OutUDec(score);
            }else{
                ST7735_SetCursor(0, 0);
                ST7735_OutString(esscoretxt);
                ST7735_OutUDec(score);
            }
            drawhud = false;
        }

        //draw centipedes
        for(int j = 0; j < numcentipedes; j++){
            if(centipedes[j].len <= 0 && prevcentipedelen[j] <= 0) continue;
            
            for(int i = 0; i < prevcentipedelen[j]; i++){
                ST7735_FillRect(prevcentipedes[j][i].x, prevcentipedes[j][i].y - 7, 8, 8, ST7735_BLACK); 
            }

            for(int i = 0; i < centipedes[j].len; i++){
                ST7735_DrawBitmap(centipedes[j].body[i].x, centipedes[j].body[i].y, centipedes[j].body[i].poses[centipedes[j].body[i].pose], 8, 8);
            }
        }

        //draw mushrooms
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 16; j++){
                if(mushrooms[i][j].alive){
                    ST7735_DrawBitmap(mushrooms[i][j].x, mushrooms[i][j].y, mushrooms[i][j].poses[mushrooms[i][j].pose], 8, 8);
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

        //draw flea
        if(fleaexists){
            ST7735_FillRect(fleaprevx, fleaprevy - 7, 8, 8, ST7735_BLACK);
            ST7735_DrawBitmap(myflea.x, myflea.y, myflea.poses[myflea.pose], 8, 8);
        }

        if(!fleaexists && fleaprevx != -1){
            ST7735_FillRect(fleaprevx, fleaprevy - 7, 8, 8, ST7735_BLACK);
            fleaprevx = -1;
            fleaprevy = -1;
        }

        //draw bullets
        for(int i = 0; i < 10; i++){
            if(bullets[i].alive){
                ST7735_FillRect(bullets[i].prevx, bullets[i].prevy - 3, 2, 4, ST7735_BLACK);
                ST7735_DrawBitmap(bullets[i].x, bullets[i].y, greenbullet, 2, 4);
            }
        }
    }

    LED_Off(4);
    LED_Off(2);
    LED_Off(1);
    return 2;
}

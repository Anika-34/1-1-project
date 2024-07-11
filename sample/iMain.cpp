#include "iGraphics.h"
#include"MMsystem.h"
#include<string.h>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>

#define screenwidth 1200
#define screenheight 600

//modes
#define HOME 0
#define LEVEL1 1
#define LEVEL2 2
#define SCORE 4
#define START -1
#define MENU 3
#define INST -2
#define INPUT_NAME 5
#define CREDITS 6
#define totalBullet 70
#define BC1 30
#define BC2 50
#define bull_x 20

int prephase;
// shooter coordinates
int x = 170, y = 300, r = 255,g =255, b=255;

// timer index
int t1, t2;

//functions
void change();
void movement();
void shootbullet();
//void reset();

//starting phase
int phase = START;
//int phase = SCORE;
//int phase = INPUT_NAME;

// balloon pictures
char *pic[4] = {"r.bmp", "b.bmp", "g.bmp", "y.bmp"};
int index = 0, index2 = 0;

//balloon coordinates level 1
int dx = 10, dy = 10, xb = 800, yb = 0;

// bullet count for level 1
int bull_c1 = BC1;
//level 2
int bull_c2 = BC2;


//2nd balloon coordinates for level 2
int xb2 = 800, yb2 = 0, dx2 = 0, dy2 = 16, xb1 = 700, yb1 = 0, dx1 = 0, dy1 = 12;

//collision and points
int col = 0, col1 = 0, col2 = 0; //flag value for collision
int points = 0;

//High score
char high1[20];
char high2[20];
char high3[20];

//for taking input name for showing score
int len;

struct showScore
{
    int score;
    char name[25];
} point[20];

struct saveInfo
{
    char name[25];
    int score;
} info;

//saving scores , sorting scores for showing
void sortscore()
{
    struct showScore temp;
    FILE *ptr;
    int i = 0, j, total;
    char ch;
    ptr = fopen("scorefile.txt", "r");
    while (!feof(ptr))
    {
        //syntax: fscanf(FILE *stream, const char*format...
        fscanf(ptr, "%s %d", point[i].name, &point[i].score);
        //ch = fgetc(ptr);
        i++;
    }

    total = i;
    for(i=1; i<total; i++)
    {
        for(j=0; j<total-i; j++)
        {
            if(point[j+1].score<point[j].score)
            {
                temp = point[j];
                point[j] = point[j+1];
                point[j+1] = temp;
            }
        }
    }
    fclose(ptr);

    ptr = fopen("sorted.txt", "w");
    //fprintf -> prints content in file
    for(i=0; i<total; i++)
    {
        fprintf(ptr, "%s %d\n", point[i].name, point[i].score);
    }
    fclose(ptr);
}

//contents of the file are read one char at a time until eof
//when fgets cannot read it returns NULL and program will stop reading
void highscore()
{
    FILE* fp;
    char arr[100][20];
    int i = 0;
    fp = fopen("sorted.txt", "r");
    while (fgets(arr[i], 100, fp))
    {
        arr[i][strlen(arr[i])-1] = '\0';  //fgets counts string length as length+1 so to assign null we have to minus one
        i++;
    }

    strcpy(high1, arr[i-1]);
    strcpy(high2, arr[i-2]);
    strcpy(high3, arr[i-3]);

    fclose(fp);
}

// bullet shooting
int released = 0;
struct bullet
{
    int bx;
    int by;
    int relay;
    int r;
    int g;
    int b;

};
bullet bullets[totalBullet];

void iDraw()
{
    // for showing the points
    char p[20];
    itoa(points,p,10);

    char bullet[20];
    char bullet2[20];
    itoa(bull_c1, bullet, 10);
    itoa(bull_c2, bullet2, 10);

    iClear();
    // game phases
    //start
    if(phase == START)
    {
        iPauseTimer(t1);
        iPauseTimer(t2);
        iShowBMP(0, 0, "start.bmp");
        iSetColor(255, 255, 204);
        iFilledRectangle(1000, 100, 110, 40);
        iSetColor(0, 0, 0);
        iText(1010, 110, "CREDITS", GLUT_BITMAP_HELVETICA_18);
        /*iSetColor(160, 160, 160);
        iFilledRectangle(0, 0, screenwidth, screenheight);*/
        iSetColor(255, 255, 204);
        iFilledRectangle(500, 100, 170, 40);
        iSetColor(0, 0, 0);
        iText(535, 110, "PLAY NOW", GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 255, 204);
        iFilledRectangle(500, 250, 170, 40);
        iSetColor(0, 0, 0);
        iText(516, 260, "INSTRUCTIONS", GLUT_BITMAP_HELVETICA_18);
        iSetColor(204, 204, 255);
        iFilledRectangle(430, 400, 310, 70);
        iSetColor(0, 0, 0);
        iText(465, 430, "BALLOON SHOOTER", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255, 255, 204);
        iText(1000, 80, "PRESS 'q' to QUIT", GLUT_BITMAP_HELVETICA_12);
    }
    //credits
    else if(phase == CREDITS)
    {
        iSetColor(0, 51, 102);
        iFilledRectangle(0, 0, screenwidth, screenheight);
        iSetColor(255, 255, 204);
        iFilledRectangle(40, 40, 1100, 540);
        iSetColor(0, 0, 0);
        iText(60, 500, "Anika Morshed", GLUT_BITMAP_HELVETICA_18);
        iText(60, 450, "Student ID : 2105068", GLUT_BITMAP_HELVETICA_18);
        iText(60, 400, "Section : B, CSE-21", GLUT_BITMAP_HELVETICA_18);
        iText(60, 250, "Supervisor:", GLUT_BITMAP_HELVETICA_18);
        iText(60, 200, "Pritom Saha Arko", GLUT_BITMAP_HELVETICA_18);
        iText(1000, 100, "press 'b' to go back", GLUT_BITMAP_HELVETICA_12);
    }
    //instructions
    else if(phase == INST)
    {
        iSetColor(0, 51, 102);
        iFilledRectangle(0, 0, screenwidth, screenheight);
        iSetColor(255, 153, 153);
        iText(100, 500, "INSTRUCTIONS:", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255, 255, 204);
        iText(120, 400, "#Try to shoot the balloon in the middle", GLUT_BITMAP_9_BY_15);
        iText(120, 370, "otherwise points won't be added.", GLUT_BITMAP_9_BY_15);
        iText(120, 320, "#The valid movements for the shooter is", GLUT_BITMAP_9_BY_15);
        iText(140, 290, "UP KEY to move up", GLUT_BITMAP_9_BY_15);
        iText(140, 260, "DOWN KEY to move down", GLUT_BITMAP_9_BY_15);
        iText(120, 210, "#Press ENTER KEY to shoot bullets", GLUT_BITMAP_9_BY_15);
        iText(120, 180, "#sometimes bullets are coloured dark to make it hard to shoot", GLUT_BITMAP_9_BY_15);
        iSetColor(174, 150, 190);
        iFilledRectangle(700, 140, 100, 30);
        iSetColor(0, 0, 0);
        iText(705, 150, "Go to START", GLUT_BITMAP_8_BY_13);
    }
    //home
    else if(phase == HOME)
    {
        iPauseTimer(t1);
        iPauseTimer(t2);
        yb =0;
        yb1 = 0;
        yb2 = 0;
        released = 0;
        bull_c1 = BC1;
        bull_c2 = BC2;
        points = 0;
        for(int i=0;i<totalBullet; i++)
        {
            bullets[i].relay = 0;
        }

        iSetColor(0, 51, 102);
        iFilledRectangle(0, 0, 400, screenheight);
        iFilledRectangle(800, 0, 400, screenheight);
        //iFilledRectangle(400, 0, 400, screenheight);
        iSetColor(255, 255, 204);
        iFilledRectangle(400, 0, 400, screenheight);
        // level 1
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 500, 170, 40);
        iSetColor(0, 0, 0);
        iText(540, 510, "LEVEL-1", GLUT_BITMAP_HELVETICA_18);
        // level 2
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 400, 170, 40);
        iSetColor(0, 0, 0);
        iText(540, 410, "LEVEL-2", GLUT_BITMAP_HELVETICA_18);
        // scores
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 300, 170, 40);
        iSetColor(0, 0, 0);
        iText(540, 310, "SCORES", GLUT_BITMAP_HELVETICA_18);
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 200, 170, 40);
        iSetColor(0, 0, 0);
        iText(555, 210, "EXIT", GLUT_BITMAP_HELVETICA_18);

    }
    //level1
    else if(phase == LEVEL1)
    {
        iShowBMP(0, 0, "probg.bmp");
        iShowBMP2(x, y, "pro.bmp", 0);
        // generating random numbers for balloon bmp index
        iSetColor(249, 231, 159);
        iFilledRectangle(1020, 0, 180, screenheight);
        if(yb == screenheight)
        {
            index = rand()%4;
        }
        //pause the game
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 490, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 500, "PAUSE", GLUT_BITMAP_HELVETICA_18);
        //show points
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 390, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 400, "POINTS", GLUT_BITMAP_HELVETICA_18);
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 310, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 320, p, GLUT_BITMAP_HELVETICA_18);
        //remaining bullets
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 170, 120, 70);
        iSetColor(0, 0, 0);
        iText(1070, 200, "BULLETS", GLUT_BITMAP_HELVETICA_18);
        iText(1070, 180, "REMAINING:", GLUT_BITMAP_HELVETICA_18);
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 110, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 120, bullet, GLUT_BITMAP_HELVETICA_18);
        // drawing the bullet
        for(int i=0; i<totalBullet; i++)
        {
            //printf("%d\n", i);
            iSetColor(bullets[i].r, bullets[i].g, bullets[i].b);
            iFilledCircle(bullets[i].bx, bullets[i].by, 5, 1000);
        }
        iShowBMP2(xb, yb, pic[index], 0);
    }
    //level2
    else if(phase == LEVEL2)
    {
        iShowBMP(0, 0, "probg.bmp");
        iShowBMP2(x, y, "pro.bmp", 0);
        iSetColor(249, 231, 159);
        iFilledRectangle(1020, 0, 180, screenheight);
        // balloon
        if(yb1 == screenheight)
        {
            index = rand()%4;
        }
        if(yb2 == screenheight)
        {
            index2 = rand()%4;
        }
        //pause the game
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 490, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 500, "PAUSE", GLUT_BITMAP_HELVETICA_18);
        //show points
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 390, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 400, "POINTS", GLUT_BITMAP_HELVETICA_18);
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 310, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 320, p, GLUT_BITMAP_HELVETICA_18);
        //remaining bullets
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 170, 120, 70);
        iSetColor(0, 0, 0);
        iText(1070, 200, "BULLETS", GLUT_BITMAP_HELVETICA_18);
        iText(1070, 180, "REMAINING:", GLUT_BITMAP_HELVETICA_18);
        iSetColor(133, 193, 233);
        iFilledRectangle(1060, 110, 100, 30);
        iSetColor(0, 0, 0);
        iText(1070, 120, bullet2, GLUT_BITMAP_HELVETICA_18);
        // drawing the bullet
        for(int i=0; i<totalBullet; i++)
        {
            iSetColor(bullets[i].r, bullets[i].g, bullets[i].b);
            iFilledCircle(bullets[i].bx, bullets[i].by, 5, 1000);
        }
        iShowBMP2(xb1, yb1, pic[index], 0);
        iShowBMP2(xb2, yb2, pic[index2], 0);
    }
    //menu
    if(phase == MENU)
    {
        iSetColor(128, 139, 150);
        iFilledRectangle(0, 0, screenwidth, screenheight);
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 140, 170, 40);
        iSetColor(0, 0, 0);
        iText(547, 150, "QUIT", GLUT_BITMAP_HELVETICA_18);
        iSetColor(204, 255, 204);
        iFilledRectangle(500, 400, 170, 40);
        iSetColor(0, 0, 0);
        iText(540, 410, "RESUME", GLUT_BITMAP_HELVETICA_18);
    }
    //score phase from home
    if(phase == SCORE)
    {
        iSetColor(118, 110, 167);
        iFilledRectangle(0, 0, screenwidth, screenheight);
        iSetColor(0, 0, 0);
        iText(500,200,high3,GLUT_BITMAP_HELVETICA_18);
        iText(500,300,high2,GLUT_BITMAP_HELVETICA_18);
        iText(500,400,high1,GLUT_BITMAP_HELVETICA_18);
        iSetColor(188, 188, 194);
        iFilledRectangle(700, 100, 100, 40);
        iSetColor(0, 0, 0);
        iText(705, 110, "GO TO HOME", GLUT_BITMAP_HELVETICA_12);
    }
    //to take input name of player
    if(phase == INPUT_NAME)
    {
        //score box
        iSetColor(255, 229, 204);
        iFilledRectangle(0, 0, screenwidth, screenheight);
        //score text
        iSetColor(128, 128, 128);
        iFilledRectangle(400, 300, 400, 240);
        iSetColor(0, 0, 0);
        iText(450, 500, "YOUR SCORE : ", GLUT_BITMAP_HELVETICA_18);
        //show score
        iText(630, 500, p, GLUT_BITMAP_HELVETICA_18);
        //name input
        iSetColor(0, 0, 0);
        iText(450, 450, "ENTER YOUR NAME : ");
        //input
        iSetColor(255, 255, 255);
        iFilledRectangle(450, 400, 250, 40);
        //to end entering name ok button
        iSetColor(255, 255, 255);
        iFilledRectangle(720, 340, 30, 15);
        iSetColor(0, 0, 0);
        iText(725, 345, "OK", GLUT_BITMAP_HELVETICA_10);
        //show name
        iSetColor(0, 0, 0);
        iText(460, 415, info.name, GLUT_BITMAP_HELVETICA_10);
    }
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
//    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//    {
//        //place your codes here
//        printf("x = %d, y= %d\n",mx,my);
//
//
//    }
//    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
//    {
//        //r=0;
//        //b=0;
//        //g=0;
//    }
    if(phase == START)
    {
        if((mx>=500 && mx<=670) && (my>=100 && my<=140))
        {
            //printf("Jello");
            phase = HOME;
        }
        if((mx>=500 && mx<=670) && (my>=250 && my<=290))
        {
            phase = INST;
        }
        if(mx>=1000 && mx<=1110 && my>= 100 && my<=140)
        {
            phase = CREDITS;
        }
    }

    else if(phase == INST)
    {
        if(mx>=700 && mx<=800 && my>= 140 && my<=170)
        {
            phase = START;
        }
    }

    else if(phase == HOME)
    {
        //iPauseTimer(t1);
        //iPauseTimer(t2);
        if((mx>=500 && mx<=670) && (my>=500 && my<=540))
        {
            phase = LEVEL1;
            iResumeTimer(t1);
            iResumeTimer(t2);
            //phase = LEVEL1;
        }
        if((mx>=500 && mx<=670) && (my>=400 && my<=440))
        {
            phase = LEVEL2;
            iResumeTimer(t1);
            iResumeTimer(t2);
            //phase = LEVEL2;
        }
        if((mx>=500 && mx<=670) && (my>=300 && my<=340))
        {
            phase = SCORE;
        }
        // if exit is selected game will go to start page
        if((mx<= 670&& mx>=500) && (my<=240 && my>=200))
        {
            phase = START;
        }
    }

    // if resume is selected in menu
    else if(phase == MENU)
    {
        if((mx<= 670 && mx>=500) && (my<=440 && my>=400))
        {
            phase = prephase;
            if(phase == LEVEL1)
            {
                iResumeTimer(t1);
                iResumeTimer(t2);
            }
            else if(phase == LEVEL2)
            {
                iResumeTimer(t1);
                iResumeTimer(t2);
            }
        }
        else if((mx>=500 && mx<=670) && (my<=180 && my>=140))
        {
            //printf("Hello");
            phase = HOME;
        }
    }

    else if(phase == LEVEL1)
    {
        if((mx>=1060 && mx<=1160) && (my>=490 && my<=520))
        {
            iPauseTimer(t1);
            iPauseTimer(t2);
            prephase = LEVEL1;
            phase = MENU;
        }
    }
    else if(phase == LEVEL2)
    {
        if((mx>=1060 && mx<=1160) && (my>=490 && my<=520))
        {
            iPauseTimer(t1);
            iPauseTimer(t2);
            prephase = LEVEL2;
            phase = MENU;
        }
    }

    //score exit
    else if(phase == SCORE)
    {
        highscore();
        sortscore();
        if((mx<=800 && mx>=700) && (my<=140 && my>=100))
        {
            phase = HOME;
        }
    }

    else if(phase == INPUT_NAME)
    {
        if((mx>=720 && mx<=750) && (my>=340 && my<=355))
        {
            FILE* fp;
            fp = fopen("scorefile.txt", "a"); //Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist.
            fprintf(fp, "%s %d", info.name, points);
            fclose(fp);
            //calling the functions to save the scores and name
            sortscore();
            highscore();
            strcpy(info.name, "");
            points = 0;
            len = 0;
            released = 0;
            phase = HOME; // clicking ok
        }
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/


// bullet shooting

void shootbullet()
{
    int i = released;
    bullets[i].relay = 1;
    released++;
}

//bullet movement

void movement()
{
    int i;
    for(i=0; i<totalBullet; i++)
    {
        //if the flag value is 1,keep the bullet moving
        if(bullets[i].relay == 1)
        {
            bullets[i].bx += bull_x;
        }
        //the initialize position according to the shooter
        else
        {
            bullets[i].bx = x+50;
            bullets[i].by = y+55;
        }
        //collision
        if(phase == LEVEL1)
        {
            if((bullets[i].bx >= xb && bullets[i].bx <= xb+40) && (bullets[i].by> yb && bullets[i].by< yb+50) && bullets[i].relay == 1)
            {
                col = 1;
                PlaySound(TEXT("pop.wav"), NULL, SND_ASYNC);
                points +=10;
            }
            if(col == 1)
            {
                col = 0;
                yb = screenheight;
                bullets[i].relay = 0;
            }
        }
        //collision
        else if(phase == LEVEL2)
        {
            if((bullets[i].bx >= xb1 && bullets[i].bx <= xb1+40) && (bullets[i].by> yb1 && bullets[i].by< yb1+50) && bullets[i].relay == 1)
            {
                col1 = 1;
                PlaySound(TEXT("pop.wav"), NULL, SND_ASYNC);
                points += 10;
            }
            if((bullets[i].bx >= xb2 && bullets[i].bx <= xb2+40) && (bullets[i].by> yb2 && bullets[i].by< yb2+50) && bullets[i].relay == 1)
            {
                col2 = 1;
                PlaySound(TEXT("pop.wav"), NULL, SND_ASYNC);
                points += 10;
            }
            if(col1 == 1)
            {
                col1 = 0;
                yb1 = screenheight;
                bullets[i].relay = 0;
            }
            if(col2 == 1)
            {
                col2 = 0;
                yb2 = screenheight;
                bullets[i].relay = 0;
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    // press enter to shoot bullets

    if(key == '\r')
    {
        shootbullet();
        if(phase == LEVEL1)
        {
            bull_c1--;
            if(released >= BC1)
            {
                //phase = INPUT_NAME;
                //phase = SCORE;
                yb = 0;
                released = 0;
                bull_c1 = BC1;
                for(int i=0; i<totalBullet; i++)
                {
                    bullets[i].relay = 0;
                }
                phase = INPUT_NAME;
            }
        }
        else if(phase == LEVEL2)
        {
            bull_c2--;
            if(released >= BC2)
            {
                //phase = INPUT_NAME;
                //phase = SCORE;
                yb1 = 0;
                yb2 = 0;
                released = 0;
                bull_c2 = BC2;
                for(int i= 0; i<totalBullet; i++)
                {
                    bullets[i].relay = 0;
                }
                phase = INPUT_NAME;
            }
        }
    }

    //taking name input from keyboard

    if(phase == INPUT_NAME)
    {
        if(key != '\b')
        {
            info.name[len] = key;
            len++;
            info.name[len] = '\0';

        }
        else
        {
            if(len<0) len = 0;
            else
            {
                len--;
            }
            info.name[len] = '\0';
            //printf("%s", info.name[len]);
        }
    }
    //quit the game
    else if(phase == START)
    {
        if(key == 'q')
        {
            exit(0);
        }
    }
    else if(phase == CREDITS)
    {
        if(key == 'b')
        {
            phase = START;
        }
    }
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{
    //shooter position up and down
    if((key == GLUT_KEY_UP) && y<= screenheight-105)
    {
        y += 20;
    }
    if((key == GLUT_KEY_DOWN) && y>= 20)
    {
        y -= 20;
    }
    //place your codes for other keys here
}

// to change the position of the ballon with time
void change()
{
    if(phase == LEVEL1)
    {
        yb += dy;
        if(yb > screenheight)
        {
            yb = 0;
        }
    }
    else if(phase == LEVEL2)
    {
        yb1 += dy1;
        if(yb1 > screenheight)
        {
            yb1 = 0;
        }

        yb2 += dy2;
        if(yb2 > screenheight)
        {
            yb2 = 0;
        }
    }
}

int main()
{
    //place your own initialization codes here.
    t1 = iSetTimer(70, change); //balloon
    t2 = iSetTimer(7, movement); //bullet
    len = strlen(info.name);

    for(int i=0; i<totalBullet; i++)
    {
        //changing bullet colours
        bullets[i].r = rand()%255;
        bullets[i].g = rand()%255;
        bullets[i].b = rand()%255;
    }
    iInitialize(screenwidth, screenheight, "BALLOON SHOOTER!");
    return 0;
}

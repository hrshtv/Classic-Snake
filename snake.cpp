/**
Welcome to Classic Snake
Rules:
Control using W A S D, Make sure Caps Lock is turned OFF
Don't cross yourself, or the screen boundaries
Eat the red food to increase length
Obstacles will spawn randomly
*/

#include<simplecpp>
#include<bits/stdc++.h>
using namespace std;

//global variables
int gridsep=10,xinit=30,yinit=20,npts=50;
int canvS=gridsep*npts;
int len=0,bre=0;

//glitch 1 : continuously pressing a key can make snake exit the map, don't exploit this
//glitch 2 : food may spawn inside obstacle - resolved

void backG()
{
    double l=2*gridsep;
    //dark theme
    Rectangle R(canvS/2,canvS/2,canvS,canvS);
    R.setFill(true);
    R.setColor(COLOR(0,255,0));
    R.imprint();
}
void grid()
{
    Rectangle r1(gridsep,canvS/2,2*gridsep,canvS);
    Rectangle r2(canvS-gridsep,canvS/2,2*gridsep,canvS);
    r1.setFill(true);
    r2.setFill(true);
    r1.setColor(COLOR(50,70,100));
    r2.setColor(COLOR(50,70,100));
    r1.imprint();
    r2.imprint();
    Rectangle r3(canvS/2,gridsep,canvS,2*gridsep);
    Rectangle r4(canvS/2,canvS-gridsep,canvS,2*gridsep);
    r3.setFill(true);
    r4.setFill(true);
    r3.setColor(COLOR(50,70,100));
    r4.setColor(COLOR(50,70,100));
    r3.imprint();
    r4.imprint();
    for(int i=5*gridsep;i<=canvS-2*gridsep;i=i+gridsep)
    {
        Line l1(2*gridsep,i,canvS-2*gridsep,i);
        l1.imprint();
    }
    for(int i=5*gridsep;i<=canvS-2*gridsep;i=i+gridsep)
    {
        Line l1(i,2*gridsep,i,canvS-2*gridsep);
        l1.imprint();
    }
}
Circle foodspawner()
{
    //seeding
    int x=rand()%(canvS-4*gridsep)+2*gridsep;
    int y=rand()%(canvS-4*gridsep)+2*gridsep;
    Circle c(x,y,gridsep*0.5);
    c.setFill(true);
    c.setColor(COLOR("red"));
    return c;
}
struct Snake
{
    vector<Circle> body;
    int length;
    int headindex; //Current head index
    char dir; //direction
    Snake(int l)
    {
        length=l;
        //body=new Circle[l];
        headindex=0;
        for(int i=0;i<length;i++){
            Circle c((xinit+0.5+i)*gridsep, (yinit+0.5)*gridsep, gridsep*0.5);
            body.push_back(c);
            body[i].setFill(true);
            body[i].setColor(COLOR(0,0,255));
        }
        body[headindex].setFill(true);

        dir='a';
    }
    int xsnake()
    {
        return body[headindex].getX();
    }
    int ysnake()
    {
        return body[headindex].getY();
    }
    char rev(char direction)
    {
        if(direction=='a') return 'd';
        if(direction=='d') return 'a';
        if(direction=='w') return 's';
        if(direction=='s') return 'w';
    }
    void move()
    {
        if(body[headindex].getX()>canvS-3*gridsep || body[headindex].getY()>canvS-3*gridsep || body[headindex].getX()<3*gridsep || body[headindex].getY()<3*gridsep) dir=rev(dir);
        move(dir);
    }
    void move(char command)
    {
        if(body[headindex].getX()<canvS || body[headindex].getY()<canvS || body[headindex].getX()>0 || body[headindex].getY()>0){

        if(command != 'w' && command != 'a' && command != 's' && command != 'd')
            command=dir;

        int newhead=(headindex+length-1)%length;
        body[newhead]=body[headindex];
        headindex=newhead;

        if(command == 'a')
        {
         body[headindex].move(-gridsep,0);
        }
        if(command == 'w'){body[headindex].move(0,-gridsep);}
        if(command == 'd'){body[headindex].move(gridsep,0); }
        if(command == 's') body[headindex].move(0,gridsep);

        dir=command;
        }
    else dir=rev(dir);
    }

};
/*Increase length function*/
void increaseLength(Snake &s)
{
    Circle c((xinit+0.5+s.length)*gridsep, (yinit+0.5)*gridsep, gridsep*0.5);
    s.body.push_back(c);
    s.body[s.length].setFill(true);
    s.body[s.length].setColor(COLOR("green"));
    s.length=s.length+1;
}
Rectangle obstacleSpawner()
{
    int x=rand()%(canvS-4*gridsep)+2*gridsep;
    int y=rand()%(canvS-4*gridsep)+2*gridsep;
     len=rand()%5*gridsep + 2*gridsep;
     bre=rand()%2*gridsep + 2*gridsep;
    Rectangle r(x,y,len,bre);
    r.setFill(true);
    r.setColor(COLOR(25,25,25));
    return r;
}
int main()
{

    int score = 0;
    int noObs=0;
    double speed=0.1;
    initCanvas("Snake",gridsep*npts,gridsep*npts);
    backG();
    //grid();
    Snake s(4);
    std::srand(time(NULL));
    Circle c=foodspawner();
    vector<Rectangle> r;
    vector<int> lenR;
    vector<int> breR;
    while(1)
    {
        //give input
        XEvent event;
        if(checkEvent(event))
        {
            if(keyPressEvent(event))
            {
                char c=charFromEvent(event);
                s.move(c);

            }
        }
        else s.move();

        //eat the food
        if(abs((s.xsnake()-c.getX()))<gridsep && abs((s.ysnake()-c.getY()))<gridsep)
        {
                score++;
                Text t(4*gridsep,4*gridsep,score);
                t.imprint();
                c.setColor(COLOR(255,255,255));
                c.reset(0,0,0);
                increaseLength(s);
                if(noObs<10)
                {
                    r.push_back(obstacleSpawner());r[0].imprint();
                    lenR.push_back(len);
                    breR.push_back(bre);
                    noObs++;
                }
                Circle c1=foodspawner();
                for(int i=0;i<noObs;i++)
                {
                    if(abs(r[i].getX()-c1.getX())<lenR[i] && abs(r[i].getY()-c1.getY())<breR[i])
                    c1=foodspawner();
                }
                c=c1;
                speed=speed-0.005;
        }

        //DON'T CROSS THE OBSTACLES

        bool crossedObs=false;
        for(int ii=0;ii<noObs;ii++){
        if( abs((s.xsnake()-r[ii].getX()))<lenR[ii]/2.0 && abs((s.ysnake()-r[ii].getY()))<breR[ii]/2.0) {crossedObs=true;break;}}
        if(crossedObs) break;


        //DON'T CROSS YOURSELF
        bool crossedItself=false;
        for(int ii=s.headindex+2;ii<s.length;ii++)
        {
            if(abs((s.xsnake()-s.body[ii].getX()))==0 && abs((s.ysnake()-s.body[ii].getY()))==0) {crossedItself=true;wait(0.05);break;}
        }
        if(crossedItself) break;
        wait(speed);
    }
    return 0;
}


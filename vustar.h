typedef struct boyut {
  long x,y,z,rx,ry,rz;  // noktalar: normal ve rotated
  byte dead;
} boyut;

typedef struct facerec {
  byte p1,p2,p3;   // face hangi 3 ko$eden olu$mu$
} facerec;

typedef struct scr {
  dword x,y;
} scr;

#define numstars        1000
#define dist            192

#define numpoints 13
#define numfaces 16
#define xofs 160
#define yofs 100
#define zeye 256

boyut mx[numpoints];  // noktalar
facerec fc[numfaces]; // faceler
scr xy[numpoints]; // 2d'ye project edilmi$ noktalar
// word polyxpos[800][2]; // poligon $eysi
byte iswireframe;

boyut mmx[numstars + 1];
scr vxy[2][numstars + 1];
unsigned char col[dist];
long i, gox, goy, goz1, goz2, angle_;
unsigned char yon, isblur;

void setstarpal()
{
  unsigned char a;
  a = 192;
  setrgb(0, 0, 0, 0);
  for (i = 1; i <=192; i++) {
    setrgb(i, 0, i, i); col[i]=a; a--; i++;
    setrgb(i, 0, i, i); col[i]=a; a--; i++;
    setrgb(i, 0, i, i); col[i]=a; a--;
  }
}

void blurrout()
{
_asm{
 mov edi,vscr
 add edi,320
 mov ecx,63360
 xor ax,ax
 mov al,[edi-1]
l1:
 xor bx,bx
 mov bl,[edi+1]
 add ax,bx
 mov bl,[edi+320]
 add ax,bx
 mov bl,[edi-320]
 add ax,bx
 shr ax,2
 jz l2
 dec ax
 mov [edi],al
l2:
 inc edi
 loop l1}
}

void starprecalculate()
{
  for (i = 0; i <= numstars; i++) {
    do {
      mmx[i].x = rand() %320 - 160;
      mmx[i].y = rand() %200 - 100;
      mmx[i].z = rand() %dist + 1;
    } while (mmx[i].x == 0 || mmx[i].y == 0);
  }
}

void starcalculate()
{
  for (i = 0; i <= numstars; i++) {
   if (mmx[i].dead==0) {
    vxy[0][i].x = ((mmx[i].x << 6) + (mmx[i].x << 7) + gox) / (mmx[i].z + goz1) + 160;
    vxy[0][i].y = ((mmx[i].y << 6) + (mmx[i].y << 7) + goy) / (mmx[i].z + goz2) + 100;
   }
  }
}

void starrotcalculate()
{
  for (i = 0; i <= numstars; i++) {
    mmx[i].rx=(mmx[i].x*cost[angle_])-(mmx[i].y*sint[angle_]);
    mmx[i].ry=(mmx[i].x*sint[angle_])+(mmx[i].y*cost[angle_]);
//    mmx[i].rx=(mmx[i].x*cos(angle_*PI/180))-(mmx[i].y*sin(angle_*PI/180));
//    mmx[i].ry=(mmx[i].x*sin(angle_*PI/180))+(mmx[i].y*cos(angle_*PI/180));
   if (mmx[i].dead==0) {
    vxy[0][i].x = ((mmx[i].rx << 6) + (mmx[i].rx << 7) + gox) / (mmx[i].z + goz1) + 160;
    vxy[0][i].y = ((mmx[i].ry << 6) + (mmx[i].ry << 7) + goy) / (mmx[i].z + goz2) + 100;
   }
  }
}

void starsput()
{
unsigned char renk;

  for (i = 0; i <= numstars; i++) {
if (isblur==0) {
    putpixel(vxy[1][i].x, vxy[1][i].y, 0,vscr);
    putpixel(vxy[1][i].x + 1, vxy[1][i].y, 0,vscr);
    putpixel(vxy[1][i].x, vxy[1][i].y + 1, 0,vscr);
    putpixel(vxy[1][i].x + 1, vxy[1][i].y + 1, 0,vscr);
//**************************-*-*-*-*-*********************************
    renk=dist-(mmx[i].z / 4); } else renk=col[mmx[i].z];
//**************************-*-*-*-*-*********************************
    if (vxy[0][i].x > 0 && vxy[0][i].x < 319 && vxy[0][i].y > 1 && vxy[0][i].y < 198)
    {
      putpixel(vxy[0][i].x, vxy[0][i].y, renk,vscr);
      putpixel(vxy[0][i].x + 1, vxy[0][i].y, renk,vscr);
      putpixel(vxy[0][i].x, vxy[0][i].y + 1, renk,vscr);
      putpixel(vxy[0][i].x + 1, vxy[0][i].y + 1, renk,vscr);
      vxy[1][i].x = vxy[0][i].x;
      vxy[1][i].y = vxy[0][i].y;
    }
  }
}

void starsmove(byte willdie)
{
  for (i = 0; i <= numstars; i++) {
    if (yon) {
      mmx[i].z -= 4;
      if (mmx[i].z < 1) {
       if (willdie==0) {
          do {
            mmx[i].x = rand() %320 - 160;
            mmx[i].y = rand() %200 - 100;
          } while (mmx[i].x == 0 || mmx[i].y == 0);
          mmx[i].z = dist;
       }else { mmx[i].dead=1; vxy[0][i].x=-5; }
      }
    } else {
      mmx[i].z += 4;
      if (mmx[i].z > dist) {
        do {
          mmx[i].x = rand() %320 - 160;
          mmx[i].y = rand() %200 - 100;
        } while (mmx[i].x == 0 || mmx[i].y == 0);
        mmx[i].z = 1;
      }
    }
  }

}

// ---------*-------------*-------------*-------------*-------------*---------
// void glenzhline(word x1,word x2,word y,char *where) {
void glenzhline(int x1,int x2,int y,char *where) {
// word i;
int i;
if (x1<0) x1=0; if (x1>319) x1=319;
if (x2<0) x2=0; if (x2>319) x2=319;
if (x1==x2) return;
if ((y<0) || (y>199)) return;

if (x1>x2) { i=x1; x1=x2; x2=i; }

if ((x2-x1)==319) return;

_asm{
 mov edi,where

 mov edx,y

 mov ebx,edx
 shl edx, 8
 shl ebx, 6
 add edx,ebx
 add edi,edx // edi=y*320
 mov ebx,x1
 add edi,ebx // edi=edi+x1
// mov esi,edi
 mov edx,x2
 sub edx,ebx
 mov ecx,edx // ecx=x2-x1
ghlineloop:
 mov al,[edi]
 cmp al,0
 jne bla
 mov al,5
bla:
 add al,5
 mov [edi],al
 inc edi
 dec cx
 cmp cx,0
 jg ghlineloop
}
}

void glenzpoly(int x1,int y1,int x2,int y2,int x3,int y3,char *where)
{ int i,starty,endy;

starty=y1; endy=y1;
if (y2<starty) starty=y2;
if (y3<starty) starty=y3;
if (y2>endy) endy=y2;
if (y3>endy) endy=y3;

if (endy>199) endy=199;

for (i=0;i<200;i++) { polyxpos[i][0]=319; polyxpos[i][1]=0; }
scanpoly(x1,y1,x2,y2); scanpoly(x1,y1,x3,y3); scanpoly(x2,y2,x3,y3);
for(i=starty;i<endy;i++) glenzhline(polyxpos[i][0],polyxpos[i][1],i,where);
}
// ---------*-------------*-------------*-------------*-------------*---------

void motionblur(char *where)
{
_asm{
 mov edi,where
 mov ecx,64000
 xor ax,ax
l1:
 mov al,[edi]
 cmp al,2
 jl l2
 sub al,2
 mov [edi],al
l2:
 inc edi
 loop l1}
}

// noktalari 2d'ye project et
void conv2d() {
long i,w;
  for (i=0;i<numpoints;i++) {
    xy[i].x=xofs + ((mx[i].rx<<8)/(mx[i].rz+256));
    xy[i].y=yofs + ((mx[i].ry<<8)/(mx[i].rz+256));
  }
}

void rotate3d(word xan, word yan, word zan) {
long i,ty,tx,tz;
 for (i=0;i<numpoints;i++) {
  ty=mx[i].y*cost[xan] - mx[i].z*sint[xan];  // X axis
  tz=mx[i].y*sint[xan] + mx[i].z*cost[xan];
  mx[i].ry=ty;
  mx[i].rz=tz;

  tx=mx[i].x*cost[yan] - mx[i].rz*sint[yan]; // Y axis
  tz=mx[i].x*sint[yan] + mx[i].rz*cost[yan];
  mx[i].rx=tx;
  mx[i].rz=tz;

  tx=mx[i].rx*cost[zan] - mx[i].ry*sint[zan]; // Z axis
  ty=mx[i].rx*sint[zan] + mx[i].ry*cost[zan];
  mx[i].rx=tx;
  mx[i].ry=ty;
}
}

void drawglenzfaces(char *where) {
long i,f; int y1,y2,y3; int x1,x2,x3; int zn;

 for(i=0;i<numfaces;i++) {
    x1=xy[fc[i].p1].x; y1=xy[fc[i].p1].y;
    x2=xy[fc[i].p2].x; y2=xy[fc[i].p2].y;
    x3=xy[fc[i].p3].x; y3=xy[fc[i].p3].y;

 if (iswireframe>0) {
  if ((y1<200) || (y2<200)) line(x1,y1,x2,y2,20,where);
  if ((y1<200) || (y3<200)) line(x1,y1,x3,y3,20,where);
  if ((y2<200) || (y3<200)) line(x2,y2,x3,y3,20,where);
  } else glenzpoly(x1,y1,x2,y2,x3,y3,where);
 }
}

void initobj() {

 mx[0].x=-60; mx[0].y=-60; mx[0].z=-60;
 mx[1].x=0; mx[1].y=-60; mx[1].z=-60;
 mx[2].x=60; mx[2].y=-60; mx[2].z=-60;
 mx[3].x=-60; mx[3].y=0; mx[3].z=-60;
 mx[4].x=0; mx[4].y=0; mx[4].z=-60;
 mx[5].x=60; mx[5].y=0; mx[5].z=-60;
 mx[6].x=-60; mx[6].y=60; mx[6].z=-60;
 mx[7].x=0; mx[7].y=60; mx[7].z=-60;
 mx[8].x=60; mx[8].y=60; mx[8].z=-60;

//-
 mx[9].x=-30; mx[9].y=-30; mx[9].z=-60;
 mx[10].x=30; mx[10].y=-30; mx[10].z=-60;
 mx[11].x=-30; mx[11].y=30; mx[11].z=-60;
 mx[12].x=30; mx[12].y=30; mx[12].z=-60;
//-
 fc[0].p1=0; fc[0].p2=9; fc[0].p3=3;
 fc[1].p1=0; fc[1].p2=1; fc[1].p3=9;
 fc[2].p1=1; fc[2].p2=4; fc[2].p3=9;
 fc[3].p1=9; fc[3].p2=4; fc[3].p3=3;
//-
 fc[4].p1=1; fc[4].p2=10; fc[4].p3=4;
 fc[5].p1=1; fc[5].p2=2; fc[5].p3=10;
 fc[6].p1=2; fc[6].p2=5; fc[6].p3=10;
 fc[7].p1=10; fc[7].p2=5; fc[7].p3=4;
//-
 fc[8].p1=3; fc[8].p2=11; fc[8].p3=6;
 fc[9].p1=3; fc[9].p2=4; fc[9].p3=11;
 fc[10].p1=4; fc[10].p2=7; fc[10].p3=11;
 fc[11].p1=11; fc[11].p2=7; fc[11].p3=6;
//-
 fc[12].p1=4; fc[12].p2=12; fc[12].p3=7;
 fc[13].p1=4; fc[13].p2=5; fc[13].p3=12;
 fc[14].p1=5; fc[14].p2=8; fc[14].p3=12;
 fc[15].p1=12; fc[15].p2=8; fc[15].p3=7;
}

void purplepal() { byte i; for(i=0;i<64;i++) setrgb(i,i,0,i); }

void volbars(long vol, byte wh)
{ byte y,c; word i,k;
  if (vol>-60) {
                vol=(vol+60)>>1;
                if (wh<2) {
                          if (wh==0) y=1;
                          else y=197;
                          for (i=0;i<vol;i++)
                              {
                               c=i;
                               if (c>62) c=62;
                               putpixel(i,y,c+1,vscr);
                               putpixel(i,y+1,c,vscr);
                              }
                          }
                else {
                      if (wh==2) y=1;
                      else y=197;
                      k=0;
                      for (i=319;i>319-vol;i--)
                          {
                           c=k;
                           if (c>62) c=62;
                           putpixel(i,y,c+1,vscr);
                           putpixel(i,y+1,c,vscr); k++;
                          }
                     }
               }
}

void vput(char *src, char *dest)
{
_asm{
 mov esi,src
 mov edi,dest
 mov ecx,64000
 xor al,al
putl:
 cmp [esi],al
 je putno
 movsb
 jmp putyes
putno:
  inc esi
  inc edi
putyes:
  loop putl}
}

void movepoints(int x,int y, int z) {
word i;
for(i=0;i<numpoints;i++) { mx[i].x+=x; mx[i].y+=y; mx[i].z+=z; }
}

void stars() {
char a; int i; // word timer=0;
byte willdie=0;

cls(0,vscr); cls(0,rscr); setstarpal(); starprecalculate();
yon=1; isblur=0; angle_=1;
goy=-50000;

while (sink()<3132) {
 if (sink()>3100) if (++angle_>=360) angle_=0;
 starsmove(willdie);
 if (sink()>3100) starrotcalculate(); else starcalculate();
 if(sink()==3110) capt(1);
 starsput();
 if (isblur==1) blurrout();
 waitvr();
 flip(vscr,rscr);
 if (sink()<2832) if(goy<0) goy+=500;

 if (sink()>2822) { if(isblur==0) cls(0,vscr); isblur=1;}
 if ((sink()>2834) && (sink()<2901)) if(gox<10000) gox+=100; // sag
 if ((sink()>2900) && (sink()<2932)) if(goy<2000) goy+=20; //yuk..
 if ((sink()>2900) && (sink()<2932)) if(gox>0) gox-=100; //sol
 if ((sink()>2932) && (sink()<3001)) if(goy>-2000) goy-=40; //a$
 if ((sink()>3001) && (sink()<3001)) if(goy<0) goy+=20; //yuk..
 if ((sink()>3001) && (sink()<3032)) { if(goz2<2000) goz2+=40; if(goz1<1000) goz1+=20;}
 if ((sink()>3032) && (sink()<3042)) if (goz2>40) goz2-=40;
 if ((sink()>3042) && (sink()<3101)) {if (goz2>40) goz2-=40; if (goz1>40) goz1-=40;}
 if (sink()>3115) willdie=1;

// timer++;
 if (kbhit()) return;
 }

}

void do_vu_stars()
{
word r,timer=0;
int hedef=-60;
byte vular[4];
cls(0,vscr); cls(0,vscr2);
calctables();
flip(vscr,rscr);

while(sink()<2700); //bekle...

stars();

cls(0,rscr);
cls(0,vscr);

while (kbhit()) getch();
purplepal(); setrgb(1,0,0,0); // arkada iz kalmamasI icin (blur sonucu
                              // hicbi zaman 0 olmaz di mi?) starlardaki
                              // izler de bu yuzden.. =$) (/me rules)
initobj();
iswireframe=0;

for(r=0;r<200;r++) movepoints(0,0,1); //prepare...

while ((!kbhit()) && (sink()<3601))
      {
       for(r=1;r<360;r+=3) {
                           timer++;
                           cls(0,vscr);
                           rotate3d(90,r,0);
                           conv2d();
                           drawglenzfaces(vscr);
                           if (sink()>3300) {
                                             volbars(mx[9].z,0);
                                             volbars(mx[10].z,1);
                                             volbars(mx[11].z,2);
                                             volbars(mx[12].z,3);
                                            }
                           motionblur(vscr2);
                           vput(vscr,vscr2);
                           waitvr();
                           flip(vscr2,rscr);
                           if (timer<200) movepoints(0,0,-1); //comes..
                           if (sink()>3529) movepoints(0,-1,-1); //goes..
                           if ((sink()>3501) && (sink()<3530)) //lowers..
                              {
                               iswireframe=0;
                               if (mx[9].z>hedef) mx[9].z-=2;
                               if (mx[10].z>hedef) mx[10].z-=3;
                               if (mx[11].z>hedef) mx[11].z-=4;
                               if (mx[12].z>hedef) mx[12].z-=5;
                               if (mx[9].z<hedef) mx[9].z=hedef;
                               if (mx[10].z<hedef) mx[10].z=hedef;
                               if (mx[11].z<hedef) mx[11].z=hedef;
                               if (mx[12].z<hedef) mx[12].z=hedef;
                              }
                           if ((sink()>3301) && (sink()<3501))
                              {
                               updatevu(&vular[0]);
                               mx[9].z=vular[0]-60;
                               mx[10].z=vular[1]-60;
                               mx[11].z=vular[2]-60;
                               mx[12].z=vular[3]-60;
                              }
                           if (sink()==3432) capt(0);
                           if (sink()>3301)
                              if ((mx[9].z>54) || (mx[10].z>54) || (mx[11].z>54) || (mx[12].z>54))
                                 {
                                  if (iswireframe==0) iswireframe=1;
                                  else iswireframe=0;
                                 }
                           if ((kbhit()) || (sink()>3600)) r=360;
                           }
      }
}

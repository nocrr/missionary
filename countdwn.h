//--------------------------------------------------------------------------
#include "321_pal.h"
#include "3.h"
#include "321bg.h"
#include "1.h"
#include "2.h"
#include "flash.h"

//--------------------------------------------------------------------------
#define x_add 160
#define y_add 101
#define pol   45
//---------------------
// word polyxpos[200][2]; // poligon $eysi

//--------------------------------------------------------------------------
typedef struct polypo{
signed int x,y;}polypo;
//--------------------
polypo polypoz[pol+1];
//--------------------------------------------------------------------------
//char *backg;

void blurrout2(char *where)
{
_asm{
 mov edi,where
 mov ecx,64000
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


//---polygon-routines-------------------------------------------------------
void hline(int x1,int x2,int y, char *from,char *to) {
word i;
if (x1>x2) { i=x1; x1=x2; x2=i; }
for(;x1<=x2;x1++) *(to+(y*320)+x1)=*(from+(y*320)+x1);
// bu rutin texture'i rotate etmeyen texture mapping yapiyor...
}

void scanpoly(int x1,int y1,int x2,int y2)
// based on cash's line algorithm
{
 signed long int xl,yl,d,k,x,y;
 word blala;
 xl=x2-x1; yl=y2-y1; if (xl==0) xl++; if (yl==0) yl++;
 if ( labs(yl) > labs(xl) )
    {
     if (y1 > y2) { k=y1;y1=y2;y2=k; x1=x2; }
     d=(xl<<16)/yl; x=(x1<<16);
     for (y=y1;y<=y2;y++)
         {
         blala=x>>16;
      if (blala<polyxpos[y][0]) polyxpos[y][0]=blala;
      if (blala>polyxpos[y][1]) polyxpos[y][1]=blala;
//          putpixel((x>>16),y,15,vscr);
          x+=d;
         }
    }
 else
     {
      if (x1 > x2) { k=x1;x1=x2;x2=k; y1=y2; }
      d=(yl<<16)/xl; y=(y1<<16);
      for (x=x1;x<=x2;x++)
          {
         blala=y>>16;
      if (x<polyxpos[blala][0]) polyxpos[blala][0]=x;
      if (x>polyxpos[blala][1]) polyxpos[blala][1]=x;
//           putpixel(x,(y>>16),2,rscr);
           y+=d;
          }
     }
}
//--------------------------------------------------------------------------
void poly(word x1,byte y1,word x2,byte y2,word x3,byte y3,char *where,char *to)
{ byte i,starty,endy;
starty=y1; endy=y1;
if (y2<starty) starty=y2;
if (y3<starty) starty=y3;
if (y2>endy) endy=y2;
if (y3>endy) endy=y3;

for (i=0;i<200;i++) { polyxpos[i][0]=319; polyxpos[i][1]=0; }
scanpoly(x1,y1,x2,y2); scanpoly(x1,y1,x3,y3); scanpoly(x2,y2,x3,y3);
for(i=starty;i<endy;i++) hline(polyxpos[i][0],polyxpos[i][1],i,where,to);
}
//--------------------------------------------------------------------------
void a_line(int x1,int y1,int x2,int y2,char col)
{
 signed long int xl,yl,d,k,x,y;
 xl=x2-x1; yl=y2-y1;
 if (xl==0) xl++; if (yl==0) yl++;
 if ( labs(yl) > labs(xl) )
    {
     if (y1 > y2) { k=y1;y1=y2;y2=k; x1=x2; }
     d=(xl<<16)/yl;
     x=(x1<<16);
     for (y=y1;y<=y2;y++)
         {
            putpixel((x>>16),y,col,vscr2);
//          putpixel((rand()%2-1+(x>>16)),rand()%2-1+y,15,vscr);
//          putpixel((x>>16),y,15);
          x+=d;
         }
    }
 else
     {
      if (x1 > x2) { k=x1;x1=x2;x2=k; y1=y2; }
      d=(yl<<16)/xl;
      y=(y1<<16);
      for (x=x1;x<=x2;x++)
          {
            putpixel(x,(y>>16),col,vscr2);
//           putpixel(rand()%2-1+x,rand()%2-1+(y>>16),15,vscr);
//           putpixel(x,(y>>16),15);
//           flip();
           y+=d;
          }
     }
}

//--------------------------------------------------------------------------
void whril() //vscr'den vscr2'ye atio..
{
 int j;
a_line(x_add,y_add,polypoz[0].x+x_add,polypoz[0].y+y_add,15);
 for (j=0;j<pol;j++)
     {
//      cls(j+50,vscr);
//      waitvr();//waitvr();waitvr();waitvr();
//                     waitvr();waitvr();waitvr();waitvr();
//                     waitvr();waitvr();waitvr();waitvr();
      poly(x_add,y_add,polypoz[j].x+x_add,polypoz[j].y+y_add,polypoz[j+1].x+x_add,polypoz[j+1].y+y_add,vscr,vscr2);
a_line(x_add,y_add,polypoz[j+1].x+x_add,polypoz[j+1].y+y_add,215);
flip(vscr2,bscr);
blurrout2(bscr);
flip(bscr,rscr);
if (kbhit()) return;
   }
//cls(0,vscr2);
}
//--------------------------------------------------------------------------
void rotcal()
{
int dx,dy,i,a;
dx=-100;dy=0;
a=0;
for(i=0;i<pol;i++) {
 polypoz[i].x=(dx*cost[a])-(dy*sint[a]);
 polypoz[i].y=(dx*sint[a])+(dy*cost[a]);
 a+=360/pol;
 }
 polypoz[pol].x=dx;polypoz[pol].y=dy;
}

// void pal_set()
// { register int i,c; for(i=0,c=0;i<768;i+=3,c++) setrgb(c,pal_123[i],pal_123[i+1],pal_123[i+2]); }

//--------------------------------------------------------------------------
void do_countdown()
{
 int center,c[3],i,z,a;

_asm{
   mov ecx,32000
   mov edi,bscr
   xor eax,eax
   rep stosd
}

bscr+=32000;

 rotcal();
 cls(0,vscr);

 cls(0,vscr2);

// backg=&bg321[0];


for(i=0;i<768;i++) pal_123[i]=i/12;
pal_set(pal_123);  /// !!

//-----------------
 for (i=0;i<200;i++)
     for (z=0;z<200;z++)
         *(vscr2+(i*320)+59+z)=*(vscr+(i*320)+59+z)=bg321[(i*200)+z];
 flip(vscr2,vscr);
//-----------------
 for (i=0; i<118; i++)
     for (z=0; z<75; z++)
         *(vscr+122+13440+z+(i*320))=((a=_3_font_data[(i*75)+z])==0) ? *(vscr+122+13440+z+(i*320)) : a ;
whril();
if (kbhit()) return;
while(sink()<116); //counter 1...
flash(pal_123);
//-----------------
 for (i=0;i<200;i++)
     for (z=0;z<200;z++)
         *(vscr+(i*320)+59+z)=bg321[(i*200)+z];

 for (i=0; i<116; i++)
     for (z=0; z<76; z++)
         *(vscr+121+13440+z+(i*320))=((a=_2_font_data[(i*76)+z])==0) ? *(vscr+121+13440+z+(i*320)) : a;
whril();
capt(5);
if (kbhit()) return;
while(sink()<124); //counter 2...
flash(pal_123);
//-----------------
 for (i=0;i<200;i++)
     for (z=0;z<200;z++)
         *(vscr+(i*320)+59+z)=bg321[(i*200)+z];

 for (i=0; i<116; i++)
     for (z=0; z<50; z++)
         *(vscr+125+13440+z+(i*320))=((a=_1_font_data[(i*50)+z])==0) ? *(vscr+125+13440+z+(i*320)) : a;
whril();
if (kbhit()) return;
while(sink()<132);
flash(pal_123);


// _1_wdata
// _1_font_data

//whril();
bscr-=32000;
}
//--------------------------------------------------------------------------

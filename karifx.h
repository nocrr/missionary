#include "fake_x.h"
#include "karbkg.h"
#include "karpal.h"

#define    karmax 500
#define    karbasla 0
#define    karxbasla 200
#define    karbitis 399
#define    karrenk 1         // du$en karlarIn rengi
#define    karolmusrenk 255    // 2 du$up geberen karlarIn rengi
#define    karcheckrenk 1    // bundan buyuk pixeller "dolu" kabul edilcek
//                              ^ (hareketli karlarin birikmemesi icin gerekli)
#define    karlayer 4        // layer sayIsI (parallax)

//#define    scrx 318
//#define    scry 199
#define    scrx 639
#define    scry 399

struct {
        long int x,y;
        byte vel,born;
} kar[karmax];

void kardown(int aralik, int dur)
{
word i,xv,yv;
int j;
long int nx,ny;
byte d,q,u,a,e,w;
for (i=0;i<aralik;i++) {

 if ((kar[i].born==0) && (dur==0)) {
  kar[i].x=rand()%(scrx+karxbasla)+1-karxbasla;
  kar[i].y=karbasla;
  kar[i].vel=(i%karlayer)+1;
  kar[i].born=1;
  }

 if ((kar[i].x>1) && (xgetpixel(kar[i].x,kar[i].y,xscr)==karrenk)) xputpixel(kar[i].x,kar[i].y,0,xscr);
 xv=(rand()%3); yv=kar[i].vel;

 nx=kar[i].x+xv;

 if (kar[i].y>200) {
  for (yv=1;yv<=kar[i].vel;yv++) // kary'ye haddi kadar vel eklenmesi (yv=vel)
   { if (xgetpixel(nx,kar[i].y+yv,xscr)>karcheckrenk) break; }
 } else { yv=kar[i].vel; }

 ny=kar[i].y+yv;
 if ((nx>=scrx) || (ny>karbitis)) kar[i].born=0;

 if ((kar[i].born==1) && (nx>2)) {
// **********

  a=xgetpixel(nx,ny,xscr);
if (kar[i].y>200) {
 while ((a>karcheckrenk) && (kar[i].born==1)) {
  d=xgetpixel(nx-3,ny,xscr);
  q=xgetpixel(nx-2,ny,xscr);
  u=xgetpixel(nx-1,ny,xscr);       //          kar
  e=xgetpixel(nx+1,ny,xscr);     //    d  q  u  a  e  w
  w=xgetpixel(nx+2,ny,xscr);
     if ((u<=karcheckrenk) && (e<=karcheckrenk)) { if (rand()%2==0) nx--; else nx++; }
//     else if ((q<=karcheckrenk) && (u<=karcheckrenk) && (e>karcheckrenk)) nx-=(rand()%2)+1;
//     else if ((u>karcheckrenk) && (e<=karcheckrenk) && (w<=karcheckrenk)) nx+=(rand()%2)+1;
     else if ((u<=karcheckrenk) && (e>karcheckrenk)) nx--;
     else if ((u>karcheckrenk) && (e<=karcheckrenk)) nx++;
//     else if ((q<=karcheckrenk) && (u<=karcheckrenk)) nx-=(rand()%2)+1;
//     else if ((e<=karcheckrenk) && (w<=karcheckrenk)) nx+=(rand()%2)+1;
     else if ((q<=karcheckrenk) && (w<=karcheckrenk)) { if ((rand()%2)==0) nx-=2; else nx+=2; }
     else if ((d<=karcheckrenk) && (q<=karcheckrenk)) nx-=(rand()%2)+2;
     else if (q<=karcheckrenk) nx-=2;
     else if (d<=karcheckrenk) nx-=3;
     else if (w<=karcheckrenk) nx+=2;
     else { // karIn altI tamamen dolu
      if (yv>1) { yv--; ny--; }
       else { kar[i].born=0; xputpixel(kar[i].x,kar[i].y,karolmusrenk,xscr); }
     }
  a=xgetpixel(nx,ny,xscr);
  } // endwhile
} // **200
 if (kar[i].born==1) {
  kar[i].x=nx; kar[i].y=ny;
  if ((kar[i].y>200) || (a==0)) xputpixel(kar[i].x,kar[i].y,karrenk,xscr);
  }

// **********
} else { kar[i].x=nx; kar[i].y=ny; } // ekran dI$Indaysa vs
} // for
} // void

void do_kar()
{
int karartis,kardur;
word xxx,yyy;
char a;
dword l;

/*
_asm{
 mov ecx,64000
 mov edi,xscr
 sub eax,eax
 rep stosd}
*/

// xrawread("karibkg.raw");
pal_set(&(kar_pal[0]));
for(l=0;l<256000;l++) *(xscr+l)=kar_bkg[l];

//setrgb(3,20,20,20); // cizgi rengi
//setrgb(3,0,0,0); // cizgi rengi
setrgb(karolmusrenk,45,49,63); // du$up gebermi$ kar rengi
setrgb(karrenk,45,49,63); // kar rengi

for (karartis=0;karartis<=scrx;karartis++) xputpixel(karartis,399,2,xscr);
// for (karartis=219;karartis<319;karartis++) xputpixel(karartis,100,3,xscr);
while(sink()<301);
xflip(xscr,rscr,0,0);

xxx=yyy=0;
a='.';

karartis=1; kardur=0;
 while (sink()<900) {
  kardown(karartis,kardur);
  waitvr();
  xflip(xscr,rscr,xxx,yyy);
  if (karartis<karmax) karartis++;
  if ((sink()>400) && (sink()<500)) if (xxx!=320) xxx++;
  if ((sink()>500) && (sink()<800)) if (yyy!=200) yyy++;
  if ((sink()>800) && (sink()<900)) if (xxx!=0) xxx--;
  if (sink()==800) capt(4);
  if (sink()==842) kardur=1;
  if (kbhit()) return;
 }
/*
for(yyy=190;yyy<200;yyy++) {
 waitvr(); for(xxx=0;xxx<320;xxx++) putpixel(xxx,yyy,0,rscr);
 }*/
}

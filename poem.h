#include "1616o.h" // 16x16lik gusel font...
#include "lyrics.h"  // siirin kendisi vs vs
#include "poem_pal.h" // palet...
#include "poem_bg.h" // resmin kendisi...

#define x_size 16
#define y_size 16
#define lettersize 16*16

char temppal[768];

void put_text(int x, int y, char text[], char *where)
{
int lcount,q;
int xloop, yloop, w;
int l;

l=strlen(text);
q=0;
for (lcount=0;lcount<l;lcount++) {

 for (yloop=0;yloop<y_size;yloop++) for (xloop=0;xloop<x_size;xloop++) {
   w=fnt[((text[lcount]-32)*lettersize)+(yloop*y_size)+xloop];
   if (w>0) putpixel(x+xloop+q,y+yloop,255,where); // ****
   }
 q+=x_size;
 }


}
//-----
void fadein_()
{
static int temp[768]; //fade-in-outta ise yariycak.. :)
static int temp1[768];
 char x;
 int z,y;
 for(z=0;z<768;z++) temp[z]=0;
 for(y=0;y<256;y++) // stepler hesaplandi...
  {temp[(y*3)]=(poempal[(y*3)]<<10);
   temp[(y*3+1)]=(poempal[(y*3+1)]<<10);
   temp[(y*3+2)]=(poempal[(y*3+2)]<<10);}
 for(x=0;x<63;x++)
 {
  waitvr();
  for(y=0;y<256;y++)
  {
   temp1[(y*3)]+=temp[(y*3)];
   temp1[(y*3+1)]+=temp[(y*3+1)];
   temp1[(y*3+2)]+=temp[(y*3+2)];
   setrgb(y,temp1[(y*3)]>>16,temp1[(y*3)+1]>>16,temp1[(y*3)+2]>>16);
  }
 }
}
//----------
void do_poem()
{
int i,k,c,L;

cls(0,rscr);
blackout();

//----------- delag'in mutish gfxini yukluyoruz...
for(i=0;i<64000;i++) *(rscr+i)=poembg[i];
fadein_();
while(sink()<2253);
setrgb(255,0,0,0);
//-----------

//----------- siir'in dongusu...
L=0;
for(i=0;i<41;i++)
                 {
                  k=0;
                  do{
                     if (sira[i]==sira[i+1]) {i++; k++;}
                     else break;
                    } while(1==1);
                  for(c=0; c<=k; c++) put_text(0,(200-((k+1)*16))/2+c*16,siir[i-k+c],rscr);
                  for(c=0;c<63;c+=3) {waitvr(); setrgb(255,c,c,c);}
                  while(sink()<pospos[L]); // burda songpos bekliycez...
                  L++;
                  if (i!=41) for(c=63;c>=0;c-=3) {waitvr(); setrgb(255,c,c,c);}
                  if(i==41) capt(2);
                  waitvr();
                  if (i!=41) for(c=24320;c<39680;c++) *(rscr+c)=0; //48 pixellik alani temizliyoruz..
                  if (kbhit()) return;
                 }
for(i=0;i<768;i++) temppal[i]=poempal[i]; //kopyaliyoruz..
for(i=63;i>0;i--) // once bg sonra son misra fadeout oluo...
 {
  waitvr();
  for(k=0;k<255;k++)
  {
   if(temppal[(k*3)]!=0) temppal[(k*3)]--;
   if(temppal[(k*3)+1]!=0) temppal[(k*3)+1]--;
   if(temppal[(k*3)+2]!=0) temppal[(k*3)+2]--;
   setrgb(k,temppal[(k*3)],temppal[(k*3)+1],temppal[(k*3)+2]);
  }
 }
for(c=63;c>=0;c-=3) {waitvr(); setrgb(255,c,c,c);}

}

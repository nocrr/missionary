#include "cast.h"
#include "cast_pal.h"
#include "peace.h"

void tv_initvga(char *dst) {word i; for(i=0;i<64000;i++) *(dst+i)=rand()%63+1;}

//-------------------------------
void go_up()
{
_asm{
 mov edi,vscr
 mov esi,edi
 add esi,320
 mov ecx,15920
 rep movsd
}
}
//-------------------------------
void sflip()
{
/*int i;
 for(i=0;i<64000;i++)
 {
  if (*(vscr+i)==0) 
     *(rscr+i)=backg[i];
  else *(rscr+i)=*(vscr+i);     
 }*/
_asm{
 mov edi,rscr
 mov esi,vscr
 mov ecx,63999
ehore:
 lodsb
 cmp al,0
 jne blala
 mov al,peace[ecx]
blala:
 stosb
 loop ehore 
}
}

void scr_owr(int a,int b)
{
 int q,w,e;
 cls(0,vscr);
 for (q=a;q<b;q++)
     {
      for (w=0;w<288;w++) *(vscr+63701+w)=cast[(q*288)+w];
      waitvr();
      sflip();
      go_up();
if (kbhit()) return;
     }
for (w=0;w<288;w++) *(vscr+63701+w)=0;
for (w=0;w<=184-b;w++) {go_up();waitvr();sflip();if (kbhit()) return;}
for (w=0;w<64000;w++) if (*(vscr+w)!=0) peace[63999-w]=*(vscr+w);
}
//-------------------------------
void randompal_set(byte range) {
byte i, renk; for (i=1;i<64;i++) {renk=rand()%range; setrgb(i,renk,renk,renk); }
}

void parazit(word duration, byte range) {
word i;
flip(vscr2,rscr); for(i=0;i<duration;i++) { randompal_set(range); delay(1); }
}

void p_hline(int x1,int x2,int y,byte col) { int x; for (x=x1;x<=x2;x++) putpixel(x,y,col,vscr2); }
void p_vline(int x,int y1,int y2,byte col) { int y; for (y=y1;y<=y2;y++) putpixel(x,y,col,vscr2); }
void rect(int x1,int y1,int x2,int y2,byte col)
{
p_hline(x1,x2,y1,col); p_hline(x1,x2,y2,col);
p_vline(x1,y1,y2,col); p_vline(x2,y1,y2,col);
}

castpalset() { pal_set(&(cast_pal[0])); }

void fadein__()
{
static int temp[768]; //fade-in-outta ise yariycak.. :)
static int temp1[768];
 char x;
 int z,y;
 for(z=0;z<768;z++) temp[z]=0;
 for(y=0;y<256;y++) // stepler hesaplandi...
  {temp[(y*3)]=(cast_pal[(y*3)]<<10);
   temp[(y*3+1)]=(cast_pal[(y*3+1)]<<10);
   temp[(y*3+2)]=(cast_pal[(y*3+2)]<<10);}
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

void do_cast()
{
 int i,rx=0,ry=0; byte mc=64;
 cls(0,vscr); tv_initvga(vscr2);

blackout();
 sflip(); //peace logosunu ekrana basar
 fadein__();
if (kbhit()) return;
 while(sink()<3700); //bekle biraz...
 scr_owr(0,35);
 if (kbhit()) return;
 while(sink()<3748); //bekle biraz...
 scr_owr(35,79);
 if (kbhit()) return;
 while(sink()<3832); //bekle biraz...
 scr_owr(79,122);
 if (kbhit()) return;
 while(sink()<3916); //bekle biraz...
 scr_owr(122,165);
 if (kbhit()) return;
 while(sink()<4001); //bekle biraz...
 cls(0,rscr);

 parazit(10,64); //vumetre
 purplepal();
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[0][i];
 if (kbhit()) return;
 while(sink()<4009); //bekle biraz...

 parazit(10,64); //stars
 setstarpal();
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[1][i];
 if (kbhit()) return;
 while(sink()<4018); //bekle biraz...

 parazit(10,64); //poem..
 pal_set(poempal);setrgb(255,63,63,63);
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[2][i];
 if (kbhit()) return;
 while(sink()<4027); //bekle biraz...

 parazit(10,64); //fire [yamuk]
 for (i=0;i<=63;i++) {setrgb(i,i,0,0); setrgb(i+63,63,i,0); setrgb(i+127,63,63,i); }
 setrgb(191,0,0,0); waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[3][i];
 if (kbhit()) return;
 while(sink()<4036); //bekle biraz...

 parazit(10,64); //kar
 pal_set(kar_pal);setrgb(karolmusrenk,45,49,63);//du$up gebermi$ kar rengi
 setrgb(karrenk,45,49,63); // kar rengi
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[4][i];
 if (kbhit()) return;
 while(sink()<4045); //bekle biraz...

 parazit(10,64); //nocrr logo...
 pal_set(ncrr_pal);
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=ncrrlogo[i];
 if (kbhit()) return;
 while(sink()<4054); //bekle biraz...

 parazit(10,64); //countdown...
 pal_set(pal_123);
 waitvr();
 for(i=0;i<64000;i++) *(rscr+i)=captured[5][i];
 if (kbhit()) return;
 while (sink()<4063); //bekle biraz...

 while (sink()<4132) parazit(10,64);

 for (i=0;i<210;i++) { // 300
                       parazit(1,mc);
                       if (i>50)
                          {
                           rect(rx,ry,320-rx,200-ry,0);
                           if (rx<155) rx++;
                           if (ry<99) ry++;
                           rect(rx,ry,320-rx,200-ry,0);
                           if (rx<155) rx++;
                           if (ry<99) ry++;
                          }
                       if ((i>50+150) && (mc>1)) mc--;
                      }
}

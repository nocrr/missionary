/******
  fadein-out rutines
               ******/
#include "ncrr_pal.h" //herhangi bi palet h'i koyulabilir.. test icin
#include "ncrrlogo.h" //loko

char temp[768]; //fade-in-outta ise yariycak.. :)
char tempal[768]; //bu da ise yario...

void fadein(char *palp)
{
 char x;
 int z,y;
 for(z=0;z<768;z++) temp[z]=0;
 for(x=0;x<63;x++)
 {
  waitvr();
  for(y=0;y<256;y++)
  {
   if(temp[(y*3)]<*(palp+(y*3))) temp[(y*3)]++;
   if(temp[(y*3)+1]<*(palp+(y*3)+1)) temp[(y*3)+1]++;
   if(temp[(y*3)+2]<*(palp+(y*3)+2)) temp[(y*3)+2]++;
   setrgb(y,temp[(y*3)],temp[(y*3)+1],temp[(y*3)+2]);
  }
 }
}
//-----------------------------------------------------------------------
void fadeout(char *palp)
{
 char x;
 int y;
 for(x=63;x>0;x--)
 {
  waitvr();
  for(y=0;y<256;y++)
  {
   if(*(palp+(y*3))==x) *(palp+(y*3))-=1;
   if(*(palp+(y*3)+1)==x) *(palp+(y*3)+1)-=1;
   if(*(palp+(y*3)+2)==x) *(palp+(y*3)+2)-=1;
   setrgb(y,*(palp+(y*3)),*(palp+(y*3)+1),*(palp+(y*3)+2));
  }
 }
}

//-----------------------------------------------------------------------
void do_fade()
{
long int z;
// pal_set();
 blackout();
 if (kbhit()) return;
 for(z=0;z<768;z++) tempal[z]=ncrr_pal[z];
 for(z=0;z<64000;z++) *(rscr+z)=ncrrlogo[z];
while(sink()<200); //fade in...
 fadein(&tempal[0]);
while(sink()<255); //stay...
 fadeout(&tempal[0]);
 while(sink()<261);
}
//-----------------------------------------------------------------------

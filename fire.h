#include "bigtech.h"

#define xsize 21
#define ysize 21
#define msglen 6

byte textrow,textline;
char msg[msglen][20];
long lettersize;  // harfin xsize'i * ysize'i

                
void fireflip()
{
_asm{
 mov edi,rscr
 mov esi,bscr
 mov ecx,15760
 rep movsd}
}

void bytecls(byte col, char *where, word len)
{
_asm{
   mov ecx,len
   shr ecx,1
   mov edi,where
   mov al,col
   mov ah,al
   rep stosw}
}

void blur(byte yamulma, char *where, dword len)
{
/*
int j,k; short a;
k=(int)yamulma;
if (k==2) k=9120;
for(j=319;j<len;j++) *(where+j)= ((a=((( *(where+j-1)+ *(where+j+1)+ *(where+j+320)+ *(where+j-320))/4)-k)) < 0 ) ? 0 : a;
*/
_asm{
 mov edi,where

 mov dl,yamulma
 cmp dl,3
 jne ehorr
 sub dl,dl
ehorr:
 mov ecx,len
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
 cmp dl,0
 jne devam
 mov [edi],al
 jmp l2
devam:
 mov [edi-1],al
 cmp dl,1
 jng l2
 mov [edi-9120],al
l2:
 inc edi
 loop l1}
}

void firer(byte yamul)
{
_asm{
  mov edi,bscr
  add edi,63679

  mov dl,yamul
  cmp dl,0
  je ehor
  add edi,8000
//  add edi,9120
ehor:
//  mov ebx,76
  mov bx,76
  in ax,40h
  add bx,ax

//  mov ecx,320
  mov cx,320
fireloop:
//  mov edx,ecx
  mov dx,cx
//  sub eax,eax //+
  in ax,40h
//  mov ecx,eax
  mov cx,ax
//  ror ebx,cl
  ror bx,cl
//  mov ecx,edx
  mov cx,dx
//  add eax,ebx
  add ax,bx

//  and eax,1
  and ax,1
  jz firezero
  mov al,191
firezero:
  mov ah,al
  stosw
loop fireloop
}
}

void go_up_fire(byte yamul)
{
_asm{
 mov edi,bscr
 mov esi,edi
 add esi,640
 mov ecx,15840
// mov ecx,15920
  mov dl,yamul
  cmp dl,0
  je ehor
  add ecx,2048
ehor:
 rep movsd}
}

void textfirer()
{
byte xcount, lcount, lpos, q,p;
lpos=0;
for (lcount=0;lcount<strlen(msg[textline]);lcount++) {
    for (xcount=0;xcount<xsize;xcount++)
        {
         p=bigtechfnt[((msg[textline][lcount]-32)*lettersize)+(textrow*ysize)+xcount];
         if (p>0) p=191;
         putpixel(lpos+xcount,198,p,bscr);
        }
    lpos+=xsize;}
textrow++;
if (textrow==ysize) {textrow=0; textline++;}
if (textline==msglen) textline=0;
} 

void puttext(int x, int y, char text[])
{
int lcount,q;
int xloop, yloop, w;
bytecls(0,fscr,9120);


q=0;
for (lcount=0;lcount<strlen(text);lcount++) {

 for (yloop=0;yloop<ysize;yloop++) for (xloop=0;xloop<xsize;xloop++) {
   w=bigtechfnt[((text[lcount]-32)*lettersize)+(yloop*ysize)+xloop];
   if (w>0) putpixel(x+xloop+q,y+yloop,191,fscr); // ****
   }

 q+=xsize;
 }


}

void byteput(char *src, char *dest, word len)
{
_asm{
 mov esi,src
 mov edi,dest
 mov ecx,len
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

void do_fire()
{
long int i;
int gsira=0;
bytecls(1,bscr,128000);
bytecls(0,fscr,9120+64000);
bscr+=32000; fscr+=32000;
//setmode(3);

textrow=0; textline=0;
strcpy(msg[0],"  NO CARRIER");
strcpy(msg[1],"  MISSIONARY");
strcpy(msg[2],"");
strcpy(msg[3],"  GREETINGS ");
strcpy(msg[4],"   FOLLOW");
strcpy(msg[5],"");
lettersize=xsize*ysize;


for (i=0;i<=63;i++) {setrgb(i,i,0,0); setrgb(i+63,63,i,0); setrgb(i+127,63,63,i); }
setrgb(191,0,0,0);

while ((sink()<2200) && (!kbhit())) {
                     if (sink()<1001) blur(0,bscr,63680); //text blur
                     if ((sink()>1000) && (sink()<1201)) blur(1,bscr,63680);//normal
                     if ((sink()>1200) && (sink()<2101)) blur(2,bscr,64000);//yamuk
                     if (sink()==1232) capt(3);
                     if (sink()>2100) blur(1,bscr,63680);//normal
                     if (sink()<1001) // textfire...
                        {
                         textfirer();
                         go_up_fire(0);
                         go_up_fire(0);
                        }
                     else if ((sink()>1200) && (sink()<2101))//yamuk fire..
                             {
                              firer(1);
                              go_up_fire(1);
                             }
                     else if (sink()>2150) //fade fire :)
                          {
                           for(i=63360;i<64000;i++) *(bscr+i)=0;
                           go_up_fire(0);
                          }
                     else {  //duz fire..
                           firer(0);
                           go_up_fire(0);
                          }
                     waitvr();
                     fireflip();

                     if(sink()==1300) {puttext(140,1,"ALIENS");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1332) {puttext(140,1,"ANALOG");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1400) {puttext(140,1,"ASC");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1432) {puttext(140,1,"ARTEFFECT");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1500) {puttext(140,1,"BRONX");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1532) {puttext(140,1,"CLIQUE");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1600) {puttext(140,1,"CPCS");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1632) {puttext(140,1,"FAAB");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1700) {puttext(140,1,"FLARE");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1732) {puttext(140,1,"GOBLINS");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1800) {puttext(140,1,"LOSER TM");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1832) {puttext(140,1,"OPL");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1900) {puttext(140,1,"QUASAR");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==1932) {puttext(140,1,"RAVEN");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==2000) {puttext(140,1,"REALTIME");blur(3,fscr,9120);blur(3,fscr,9120);}
                     if(sink()==2032) {puttext(140,1,"TDS");blur(3,fscr,9120);blur(3,fscr,9120);}

if ((sink()>=1300) && (sink()<2100) && (((sink()%100)%32)<15)) byteput(fscr,bscr+56360,9120);

//   timer++;
//   modtimer++; if (modtimer==100) modtimer=0;

//if (kbhit()) timer=2200;
}
bscr-=32000; fscr-=32000;
}

#define __NOCRR__
#include <stdlib.h>
#include <math.h>

//   NoCrr Library for missionary

#define byte unsigned char
#define word unsigned int
#define dword unsigned long
#define PI 3.1415

// char *vscr;                  // Virtual Screen Pointer
char *rscr=(char*) 0xA0000;  // Pointer to VGA Screen at A0000
float sint[360], cost[360];

/*
void allocvscr() { vscr=(char *)malloc(64000); }
void freevscr() { if (vscr != NULL) free(vscr); }
*/

void flip(char *source, char *dest)
{
_asm{
 mov edi,dest
 mov esi,source
 mov ecx,16000
 cld
 rep movsd}
}

void setmode(int md) {
_asm{
mov eax,md
int 10h}}

void setrgb(byte c, byte r, byte g, byte b)
{
_asm{
 mov al,c
 mov dx,3c8h
 out dx,al
 inc dx
 mov al,r
 out dx,al
 mov al,g
 out dx,al
 mov al,b
 out dx,al
}
}

void waitvr()
{
_asm{
    mov dx,3dah
l1: in al,dx
    and al,8
    jnz l1
l2: in al,dx
    and al,8
    jz l2}
}

void cls(byte col, char *where)
{
_asm{
 mov ecx,32000
 mov edi,where
 mov al,col
 mov ah,al
 rep stosw}
}


void putpixel(int x,int y,char color,char *where) {
if ((x<0) || (y<0)) return;
_asm{
 mov edi,where
 mov ebx,x

 cmp ebx,319
 jg atla
 mov edx,y
 cmp edx,199
 jg atla

 add edi,ebx
 mov ebx,edx
 shl edx, 8
 shl ebx, 6
 add edx,ebx
 add edi,edx
 mov al,color
 mov [edi],al
atla:
 }
}

byte getpixel(int x,int y,char *where) {
byte col=0;
if ((x<0) || (y<0)) return(col);
_asm{
 mov edi,where
 mov ebx,x

 cmp ebx,319
 jg atla
 mov edx,y
 cmp edx,199
 jg atla

 add edi,ebx
 mov ebx,edx
 shl dx, 8
 shl bx, 6
 add edx,ebx
 add edi,edx
 mov al,[edi]
 mov col,al
atla:
}
return(col);
}

void mawrapixel(int x,int y, char *where) {
if ((x<0) || (y<0)) return;
_asm{
 mov edi,where
 mov ebx,x
 cmp ebx,319
 jg atla
 mov edx,y
 cmp edx,199
 jg atla

 add edi,ebx
 mov ebx,edx
 shl dx, 8
 shl bx, 6
 add edx,ebx
 add edi,edx
 mov al,[edi]
 inc al
 mov [edi],al
atla:
}
}

void blackout()
{
_asm{
 mov cx,00FFh
l1:
 mov dx,3c8h
 mov al,cl
 out dx,al
 inc dx
 xor al,al
 out dx,al
 out dx,al
 out dx,al
 loop l1}
}

char getrgb(byte col, byte whichchannel) {
byte c[3];
_asm{
 mov    dx,3c7h
 mov    al,col
 out    dx,al
 add    dx,2
 in     al,dx
 mov    c[0],al
 in     al,dx
 mov    c[1],al
 in     al,dx
 mov    c[2],al
 }
return(c[whichchannel]);
}


void calctables()
{int i;
for (i=0;i<360;i++) sint[i]=sin(i*PI/180);
for (i=0;i<360;i++) cost[i]=cos(i*PI/180);}

void circle(int x, int y, int r, byte col, char *where)
{
 int cx,cy,a;
    for(a=1;a<90;a++)
    {cx=(int)(cost[a]*r); cy=(int)(sint[a]*r);
     putpixel(x+cx,y+cy,col,where); putpixel(x-cx,y+cy,col,where);
     putpixel(x-cx,y-cy,col,where); putpixel(x+cx,y-cy,col,where);
     }
}

void line(int x1,int y1,int x2,int y2, byte col, char *where)
{
 long int x,y,xlen,ylen,d,swap;
 if (x1 > x2) { swap=x1; x1=x2; x2=swap; swap=y1; y1=y2; y2=swap; }
 xlen=x2-x1;
 if ((ylen=y2-y1)==NULL) ylen=1;
 x=(x1<<16);
 if ((x1 < x2) && (y1 < y2))  //eger line
            {                //yukardan assagi ise
             d=(xlen<<16)/ylen;
             for ( ;y1<y2;y1++)
                  for (x+=d,putpixel(x1,y1,col,where);x1<=(x>>16);putpixel(x1,y1,col,where),x1++);
            }
 else
       {
        d=(xlen<<16)/(0-ylen);
        for ( ;y1>y2;y1--)  //BURDA y1 > y2
             for (x+=d,putpixel(x1,y1,col,where);x1<=(x>>16);putpixel(x1,y1,col,where),x1++);
       }
}
/*int rndm(int max)
{
int bla;
_asm{
 xor eax,eax
 mov ebx,76
 in ax,40h
 add ebx,eax
 in ax,40h
 mov cx,ax
 ror ebx,cl
l1: cmp ebx,max
 jl l2
 sub ebx,max
 jmp l1
l2: mov bla,ebx}
return(bla);
}*/

void pal_set(char *palpointer)
{ register int i,c; for(i=0,c=0;i<768;i+=3,c++) setrgb(c,*(palpointer+i),*(palpointer+i+1),*(palpointer+i+2)); }
// { register int i,c; for(i=0,c=0;i<768;i+=3,c++) setrgb(c,pal[i],pal[i+1],pal[i+2]); }

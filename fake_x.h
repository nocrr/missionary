/***************************************************************************
 *** fake mode-x...  virtual 640x400 screen                              ***
 ***************************************************************************/
#define __FAKEX__
#include <stdlib.h>
//--------------------------------------------------------------------------
// char *xscr;                  // Virtual Screen Pointer
//--------------------------------------------------------------------------
/*
void allocxscr() { xscr=(char *)malloc(256000); }
void freexscr() { if (xscr != NULL) free(xscr); }
*/

void xflip(char *source, char *dest,int x,int y)
{
_asm{
 mov edi,dest
 mov esi,source
 mov ebx,x
 mov edx,y
 add esi,ebx
 mov ebx,edx
 shl edx, 9
 shl ebx, 7
 add edx,ebx
 add esi,edx // ekrana basilacak viewport'un ilk byte'i
 mov eax,200 //200
next:
 mov ecx,80 // 80*4=320
 rep movsd  // 320'lik pixel satiri flip ediliyor
 add esi,320 //2. pixel satirinin adresi..
 dec eax
 cmp eax,0  // 200 satir'da flipildi mi?
 jnz next}   // eger butun satirlar bitmediyse bisonraki satiri flip et
}

void xputpixel(int x,int y,char color,char *where) {
_asm{
 mov edi,where
 mov ebx,x

 cmp ebx,639
 jg atla
 mov edx,y
 cmp edx,399
 jg atla

 add edi,ebx
 mov ebx,edx
 shl edx, 9
 shl ebx, 7
 add edx,ebx
 add edi,edx
 mov al,color
 mov [edi],al
atla:
 }
}

char xgetpixel(int x,int y,char *where) {
//if ((x<640) && (y<400)) return(*(where+x+(y<<9)+(y<<7))); else return(0);
char col=0;
_asm{
 mov edi,where
 mov ebx,x

 cmp ebx,639
 jg atla
 mov edx,y
 cmp edx,399
 jg atla

 add edi,ebx
 mov ebx,edx
 shl edx, 9
 shl ebx, 7
 add edx,ebx
 add edi,edx
 mov al,[edi]
 mov col,al
atla:
}
return(col);
}

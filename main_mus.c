#include <stdio.h>
#include "nocrr.h"
#include "defs.h"
#include "mus.h"
#include "countdwn.h"
#include "fade.h"
#include "karifx.h"
#include "fire.h"
#include "vustar.h"
#include "poem.h"
char captured[6][64000];
#include "cast2.h"


void allocates() {
 vscr=(char *)malloc(64000);
 vscr2=(char *)malloc(64000);
 xscr=(char *)malloc(256000);
 bscr=(char *)malloc(128000);
 fscr=(char *)malloc(9120+64000);
// xscr=bscr;
}

void frees() {
 if (vscr != NULL) free(vscr);
 if (vscr2 != NULL) free(vscr2);
 if (xscr != NULL) free(xscr);
 if (bscr != NULL) free(bscr);
 if (fscr != NULL) free(fscr);
}

void flushkbd() { while (kbhit()) getch(); }

void siktir() {
 stop_close_mus(); setmode(3); frees();
 puts("\nmissionary by:");
 puts("code: disq,cash");
 puts("gfx: delag,extronus");
 puts("music: suicyco");
 puts("NO CARRIER'99");
 exit(0);
}

void cekoslovak() {
if (kbhit()) siktir();
}

int sink()
{
 int popo;
 gmpGetInformation(midasPlayHandle, &info);
 popo=(info->position)*100+(info->row);
 return(popo);
}

void capt(byte a)
{
 int b;
 for(b=0;b<64000;b++) captured[a][b]=*(rscr+b);
}

void main(int argc, char *argv[]) {
byte doconfig=0,i;

// if (argc>1) doconfig=1;
// if (init_load_mus("dummy.xm",doconfig)) exit(0);  // ESC during config
init_load_mus("komisyon.xm");

 allocates(); calctables();
 midasPlayModule(module, 0);
 while (sink()<100); // nothing happens
 puts("\nStarting...");
 while (sink()<108); //loading

 setmode(0x13);
 flushkbd();
 do_countdown(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_fade(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_kar(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_fire(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_poem(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_vu_stars(); /*flushkbd();*/ cekoslovak();
 cls(0,rscr); do_cast(); flushkbd();
 siktir();
}

#define NOMUSIC

#include <stdio.h>
#include "nocrr.h"
#include "defs.h"
#include "countdwn.h"
#include "fade.h"
#include "karifx.h"
#include "fire.h"
#include "vustar.h"
#include "cast2.h"
#include "poem.h"

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

void main() {
 blackout();
 allocates();
 calctables();
 setmode(0x13);
 flushkbd();
 do_countdown(); flushkbd();
 cls(0,rscr); do_fade(); flushkbd();
 cls(0,rscr); do_kar(); flushkbd();
 cls(0,rscr); do_fire(); flushkbd();
 cls(0,rscr); do_poem(); flushkbd();
 cls(0,rscr); do_vu_stars(); flushkbd();
 cls(0,rscr); do_cast(); flushkbd();
 setmode(3);
 frees();
puts("\nMissionary By:");
puts("Code: disq,cash");
puts("Gfx: delag,extronus");
puts("Music: suicyco");
puts("NO CARRIER'99");
 exit(0);
}

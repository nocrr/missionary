#define __PARAZIT__

//
// Initialisation   : tv_init();
// Deinitilisation  : tv_done();
// Usage in program : parazit(duration_in_ms);
//

#ifndef __NOCRR__
 #include "nocrr.h"
#endif

char *tv_pscr, *tv_backupscr;
char tv_backuppal[64][3];

void tv_initvga() {word i; for(i=0;i<64000;i++) *(tv_pscr+i)=(rand()%63)+1;}

void tv_palset() {
byte i,renk; for (i=1;i<64;i++) {renk=rand()%64; setrgb(i,renk,renk,renk); }
}

void tv_backup_pal() {
byte i,c;
for (i=1;i<64;i++) {
 for(c=0;c<3;c++) tv_backuppal[i-1][c]=getrgb(i,c);
 }
}

void tv_restore_pal() {
word i;
for (i=1;i<64;i++)
  setrgb(i,tv_backuppal[i-1][0],tv_backuppal[i-1][1],tv_backuppal[i-1][2]);
}

void parazit(word duration) {
word i;
flip(rscr,tv_backupscr); tv_backup_pal();
flip(tv_pscr,rscr); for(i=0;i<duration;i++) { tv_palset(); delay(1); }
tv_restore_pal(); flip(tv_backupscr,rscr);
}

void tv_allocate() {
 tv_pscr=(char *)malloc(64000);
 tv_backupscr=(char *)malloc(64000);
}

void tv_init() { tv_allocate(); tv_initvga(); }

void tv_done() {
 if (tv_pscr!=NULL) free(tv_pscr);
 if (tv_backupscr!=NULL) free(tv_backupscr);
}

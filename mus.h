#include <stdio.h>
#include "midas.h"
//#include "mconfig.h"
#include "vu.h"

static gmpModule *module;
static gmpInformation *info;
int numChannels=0;

void Error(char *msg)
{ setmode(3); printf("\n\nMIDASerror: %s\n", msg); midasClose(); exit(255); }

void Errorcode(int errorcode) { Error(errorMsg[errorcode]); }

int CALLING MakeMeter(sdSample *sdsmp, gmpSample *gmpsmp)
{ return vuPrepare(sdsmp, gmpsmp->sdHandle - 1); }

//byte init_load_mus(char *moduleName, byte doconfig)
byte init_load_mus(char *moduleName)
{
    int error,configOK;
    static uchar buf[48];
    static fileHandle  f;

    midasSetDefaults();

    midasDetectSD();

    /* If the card detected wasn't a GUS or an SB, run the config */
/*
    if (((midasSDNumber != 0) && (midasSDNumber != 4)) || (doconfig>0))
        configOK = midasConfig(); else configOK = 1;

    if ( !configOK ) return(1); // user ESC'ye basmI$sa cIq
*/

    midasInit();
    if ((error=vuInit())!=OK) Errorcode(error);

    printf("Using %s\n%s, using port %X, IRQ %i and DMA %i\n",
        midasSD->name, midasSD->cardNames[midasSD->cardType-1],
        midasSD->port, midasSD->IRQ, midasSD->DMA);
    puts("Loading music");

    /* Read first 48 bytes of module: */
    if ((error=fileOpen(moduleName,fileOpenRead,&f))!=OK) Errorcode(error);
    if ((error=fileRead(f,buf,48))!=OK) Errorcode(error);
    if ((error=fileClose(f))!=OK) Errorcode(error);

    if (mMemEqual(buf,"Extended Module:",16)) {
                                               if ((error = gmpLoadXM(moduleName,1,&MakeMeter,&module))!=OK) Errorcode(error);
                                              }
/*    else if (mMemEqual(buf+44,"SCRM",4)) {
                                          if ((error = gmpLoadS3M(moduleName,1,&MakeMeter,&module))!=OK) Errorcode(error);
                                         }
    else if ((error=gmpLoadMOD(moduleName,1,&MakeMeter,&module))!=OK) Errorcode(error);
*/

//    midasPlayModule(module, 0);
    numChannels=module->numChannels;
    return(0);
}

void stop_close_mus() {
int error,i;
    midasStopModule(module);
    for (i=0;i<MAXSAMPLES;i++) if ((error=vuRemove(i))!=OK) Errorcode(error);

    if ((error=gmpFreeModule(module))!=OK) Errorcode(error);
    if ((error=vuClose())!=OK) Errorcode(error);
    midasClose();
}

void updatevu(char *valpointer) {
int j,i,error,c;
gmpChannel *chan;
static char s[32];
static unsigned meter,position, volume, sample;
static ulong rate;

int kanver[4]; // hangi kanallar vu arrayine geccek

/*
kanver[sIra]=kanalno;
*/

/* dei$tir bunnarI */
kanver[0]=0;
kanver[1]=1;
kanver[2]=2;
kanver[3]=3;
/* hop cu$ buraya kadar */

gmpGetInformation(midasPlayHandle, &info);
c=numChannels; if (c>4) c=4;

for (j=0;j<c;j++)
 {
  i=kanver[j];
  /* Point chan to current channel structure: */
  chan=&info->channels[i];

  /* Get playing rate for channel: */
  if ((error=midasSD->GetRate(i,&rate))!=OK) Errorcode(error);

  /* Get playing position for channel: */
  if ((error=midasSD->GetPosition(i,&position))!=OK) Errorcode(error);
  
   /* Get current sample handle for channel: */
   if ((error=midasSD->GetSample(i,&sample))!=OK) Errorcode(error);

   /* Get current volume for channel: */
   if ((error=midasSD->GetVolume(i,&volume))!=OK) Errorcode(error);

   if ((sample>0) && (rate>0)) /* Get real VU meter value: */
   { if ((error=vuMeter(sample-1,rate,position,volume,&meter))!=OK) Errorcode(error);
     } else meter=0;

   if (meter>64) meter=64;
    *(valpointer+i)=(meter<<1);
 }
}

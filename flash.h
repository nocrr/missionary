
void flash(char *palpointer)
{
 int y;
 waitvr();
 for(y=0;y<256;y++)
  {
   *(palpointer+(y*3))=63-*(palpointer+(y*3));
   *(palpointer+(y*3+1))=63-*(palpointer+(y*3+1));
   *(palpointer+(y*3+2))=63-*(palpointer+(y*3+2));
   setrgb(y,*(palpointer+(y*3)),*(palpointer+(y*3)+1),*(palpointer+(y*3)+2));
  }
 waitvr();
 for(y=0;y<256;y++)
  {
   *(palpointer+(y*3))=63-*(palpointer+(y*3));
   *(palpointer+(y*3+1))=63-*(palpointer+(y*3+1));
   *(palpointer+(y*3+2))=63-*(palpointer+(y*3+2));
   setrgb(y,*(palpointer+(y*3)),*(palpointer+(y*3)+1),*(palpointer+(y*3)+2));
  }
}


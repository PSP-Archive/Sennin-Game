#ifndef _FONT_H_
#define _FONT_H_

extern void Draw_Char_Hankaku(int x,int y,unsigned char ch,int col,int backcol,int fill);
extern void Draw_Char_Zenkaku(int x,int y,unsigned char u,unsigned char d,int col,int backcol,int fill) ;
extern void mh_print(int x,int y,unsigned char *str,int col,int backcol,int fill);

#endif

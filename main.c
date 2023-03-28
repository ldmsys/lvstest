/* Link With -lvncserver (e.g. clang -o main main.c -lvncserver && ./main)*/
#include <rfb/rfb.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include "font.h"
#define RGB(r,g,b) r|g<<8|b<<16

void printGreyImage(int* buf, int bufX, unsigned char* img, int imgX, int imgY, int sposX, int sposY) {
    int tmpi;
    for(int i=0;i<imgX;i++) for(int j=0;j<imgY;j++) {
        tmpi = 0xdb000000;
        for(int k=0;k<3;k++) tmpi += (unsigned char)img[i*imgX+j]<<(8*k);
        buf[(sposX+i)*bufX+sposY+j] = tmpi;
    }
}

void printString(int* buf, int bufX, int sposX, int sposY, char* str, int color) {
    char cChar;
    for(int i=0;i<strlen(str);i++) {
        cChar = str[i];
        if(!isprint(cChar)) continue;
        for(int j=0;j<8;j++) {
            for(int k=0;k<8;k++) {
                if(1 & font[cChar][j] >> (8-k)) {
                    buf[(sposX+((sposY+k+i*8)/bufX)*8+j)*bufX+sposY+k+i*8] = color;
                }
            }
        }
    }
}

void fillColor(int* buf, int bufY, int sposX, int sposY, int fillX, int fillY, int color) {
    for(int i=0;i<fillX;i++) for(int j=0;j<fillY;j++) buf[(sposX+i)*bufY+sposY+j] = color;
}

int main(int argc,char** argv) {
    FILE* caman;
    int tmpi, *buf;
    unsigned char img[256][256];
    caman = fopen("./caman", "r");
    if(!caman) {
        perror("fopen");
        return 1;
    }
    fread(img, 1, 256*256, caman);
    fclose(caman);
    buf=(int*)malloc(1024*768*4);
    rfbScreenInfoPtr server=rfbGetScreen(&argc,argv,1024,768,8,3,4);
    fillColor(buf, 768, 0, 0, 1024, 768, RGB(0, 0, 0));
    printGreyImage(buf, 1024, img, 256, 256, 256, 384);
    printString(buf, 1024, 10, 10, "Make better with [\"passion\", \"code\", \"hard work\"]", RGB(255, 255, 255));
    printString(buf, 1024, 18, 10, "+1 (888) 318-1430", RGB(0, 255, 0));
    printString(buf, 1024, 26, 10, "https://ldmsys.net", RGB(0, 255, 255));
    server->frameBuffer=(char*)buf;
    rfbInitServer(server);
    rfbRunEventLoop(server,-1,FALSE);
    return(0);
}

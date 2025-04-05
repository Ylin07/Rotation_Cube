#include<stdio.h>
#include<math.h>
#include<unistd.h>

int idx;
int xp,yp;
float ooz;
float x,y,z;
double A,B,C;
float cubeWidth = 16;
int width = 160,height = 44;
int distanceFromCamera = 60;
int K1 = 40;
float zbuffer[160*44];
char buffer[160*44];
int backgroundASCIIcode = ' ';
float increamentSpeed = 0.5;

float calculateX(float i,float j,float k){
    return i*cos(A)*cos(B) +
           j*cos(A)*sin(B)*sin(C) - j*sin(A)*cos(C) +
           k*cos(A)*sin(B)*cos(C) + k*sin(A)*sin(C);
}
float calculateY(float i,float j,float k){
    return i*sin(A)*cos(B) +
           j*sin(A)*sin(B)*sin(C) + j*cos(A)*cos(C) +
           k*sin(A)*sin(B)*cos(C) - k*cos(A)*sin(C);
}
float calculateZ(float i,float j,float k){
    return -i*sin(B) +
           j*cos(B)*sin(C) +
           k*cos(B)*cos(C);
}
void calculateForSurface(float cubeX,float cubeY,float cubeZ,char ch){
    x = calculateX(cubeX,cubeY,cubeZ);
    y = calculateY(cubeX,cubeY,cubeZ);
    z = calculateZ(cubeX,cubeY,cubeZ) + distanceFromCamera;

    ooz = 1/z;
    xp = (int)(width/2 + K1*ooz*x*2);
    yp = (int)(height/2 + K1*ooz*y);

    idx = xp + yp*width;
    if(idx >= 0 && idx < width*height){
        if(ooz > zbuffer[idx]){
            zbuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}


int main(){
    printf("\x1b[2j");
    while(1){
        memset(buffer,backgroundASCIIcode,width*height);
        memset(zbuffer,0,width*height*4);
        for(float cubeX = -cubeWidth;cubeX < cubeWidth;cubeX+=increamentSpeed){
            for(float cubeY = -cubeWidth;cubeY < cubeWidth;cubeY+=increamentSpeed){
                calculateForSurface(cubeX,cubeY,-cubeWidth,'o');
                calculateForSurface(cubeWidth,cubeY,cubeX,'.');
                calculateForSurface(-cubeWidth,cubeY,-cubeX,'@');
                calculateForSurface(-cubeX,cubeY,cubeWidth,'^');
                calculateForSurface(cubeX,-cubeWidth,-cubeY,'+');
                calculateForSurface(cubeX,cubeWidth,cubeY,'-');
            }
        }
        printf("\x1b[H");
        printf("\x1b[?25l");
        fflush(stdout);
        for(int k=0 ;k< width*height;k++){
            putchar(k%width ?buffer[k] : 10);
        }
        A += 0.04;
        B += 0.04;
        C += 0.04;
        usleep(8000);
    }
}

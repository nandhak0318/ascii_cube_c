#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A,B,C;

float cubeWidth = 20;
int width = 160, height = 44;
float incrementSpeed = 0.6;
float depthBuffer[160*44];
char renderBuffer[160*44];
int backgroundACIICODE = ' ';
float horizontalOffset;
int distanceFromCam = 100;

float x,y,z;

float calculateX(int i, int j, int k) {
  return (
    j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + 
    j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C)
    );
}

float calculateY(int i, int j, int k) {
  return (
    j * cos(A) * cos(C) + k * sin(A) * cos(C) - 
    j* sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - 
    i * cos(B) * sin(C)
    );
}

float calculateZ(int i, int j, int k) {
  return (
    k * cos(A) * cos(B) - 
    j * sin(A) * cos(B) + i * sin(B)
    );
}

void calculateForSurface(float X,float Y,float Z, int ch){

  x = calculateX(X,Y,Z);
  y = calculateY(X,Y,Z);
  z = calculateZ(X,Y,Z) + distanceFromCam;
 
  float depth = 1/z;
  
  int xp = (int)(width/2  + horizontalOffset + x * depth * 40 * 2);
  int yp = (int)(height/2 + y * depth * 40);

  int oned = xp + (yp*width);

  if(oned>=0 && oned< width*height){
    if(depth > depthBuffer[oned]){
      depthBuffer[oned] = depth;
      renderBuffer[oned] = ch;
  }
  }


}

int main(){
  printf("\x1b[2J");
  while(1){
    memset(renderBuffer,backgroundACIICODE,width*height);
    memset(depthBuffer,0,height*width*4);

    cubeWidth = 20;
    horizontalOffset =  cubeWidth;

    for(float cubeX = -cubeWidth;cubeX<cubeWidth; cubeX+=incrementSpeed){
     for(float cubeY = -cubeWidth;cubeY<cubeWidth; cubeY+=incrementSpeed){
        calculateForSurface(cubeX,cubeY,-cubeWidth,'@');
        calculateForSurface(cubeX,cubeY,cubeWidth,';');
        calculateForSurface(cubeWidth,cubeX,cubeY,'$');
        calculateForSurface(-cubeWidth,cubeX,cubeY,'#');
        calculateForSurface(cubeX,cubeWidth,cubeY,'$');
        calculateForSurface(cubeX,-cubeWidth,cubeY,'+');
      }       
    }
    printf("\x1b[2J");

    for(int k=0;k<width*height; k++){
      putchar(k%width?renderBuffer[k]:10);
    }
    
    A+=0.05;
    B+=0.05;
    C+=0.01;
    usleep(8000*2);
  }
  return 0;
}

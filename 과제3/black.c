//color2gray.c 파일을 이용하였습니다.
//for(i=0;i<(number_of_pixels);i++) 안의 내용을 수정하였으며, 수정된 부분만 첨부하였습니다.
//첨부된 내용에는 없지만 red, green, blue, c, m, y 를 float 형태로 선언하여 사용했습니다.
for (i = 0; i < (number_of_pixels); i++) {
red = (float)buffer[3 * i] / 255;
green = (float)buffer[3 * i + 1] / 255;
blue = (float)buffer[3 * i + 2] / 255;
//RGB’ = RGB 0<=RGB’<=1 
c = 1 - red;
m = 1 - green;
y = 1 - blue;
//RGB -> CYM 공식 이용
//find black = min(c,m,y)
k = 1; //k = black (초기화)
if (c < m)
k = c;
else k = m;
if (y < k)
k = y;
temp = 255 * k; //(256 * 3 * k)/ 3 
CLIP(temp, 0, 255);
obuffer[i] = temp; // Generates output buffer for Brightness component
}

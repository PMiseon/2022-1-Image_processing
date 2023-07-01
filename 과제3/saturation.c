//color2gray.c 파일을 이용하였습니다.
//for(i=0;i<(number_of_pixels);i++) 안의 내용을 수정하였으며, 수정된 부분만 첨부하였습니다.
float min; //find cmyk
float k; //black 
float red, green, blue;//for cymk
float c, m, y;
float s; //saturation 
for (i = 0; i < (number_of_pixels); i++) {
red = (float)buffer[3 * i] / 255;
green = (float)buffer[3 * i + 1] / 255;
blue = (float)buffer[3 * i + 2] / 255;
min = 1;
//find min 최솟값 찾기
if (red <= green)
min = red;
else //gree < red 
min = green;
if (blue < min)
min = blue;
s = 1.0 - (3.0 / (red + green + blue))*min; //공식이용
temp = s * 255;
CLIP(temp, 0, 255);
obuffer[i] = temp; // Generates output buffer for Brightness component
}

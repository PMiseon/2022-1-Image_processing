//color2gray.c 파일을 이용하였습니다.
//for(i=0;i<(number_of_pixels);i++) 안의 내용을 수정하였으며, 수정된 부분만 첨부하였습니다.
float red, green, blue;//for cymk
float c, m, y;
float hue;
float angle = 0;
for (i = 0; i < (number_of_pixels); i++) {
red = (float)buffer[3 * i] / 255;
green = (float)buffer[3 * i + 1] / 255;
blue = (float)buffer[3 * i + 2] / 255;
min = 1;
//각도 찾기
angle = 0; //초기화
angle = 0.5*(red - green + red - blue) / sqrt((red - green)*(red - green) + 
(red - blue)*(green - blue));
hue = acos(angle);
if (blue < green) //green값이 blue보다 큰 경우
hue = 360.0 - hue;
temp = hue;
CLIP(temp, 0, 255);
obuffer[i] = temp; // Generates output buffer for Brightness component
}

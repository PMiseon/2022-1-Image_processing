//color2gray.c 파일을 이용하였습니다.
//for(i=0;i<(number_of_pixels);i++) 안의 내용을 수정하였으며, 수정된 부분만 첨부하였습니다.
for (i = 0; i < (number_of_pixels); i++) {
buffer[3 * i ] = buffer[3 * i+2]; //red = blue 
buffer[3 * i + 1] = buffer[3 * i+2]; // green = blue 
temp = (buffer[3 * i] + buffer[3 * i + 1] + buffer[3 * i + 2]) / 3; // 
Generates Brightness component ->make gray
CLIP(temp, 0, 255);
obuffer[i] = temp; // Generates output buffer for Brightness component
}

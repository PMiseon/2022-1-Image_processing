void cubic_convolution_interpolation(image_ptr buffer, char *fileout, int rows, int cols, float
x_scale, float y_scale, int type)
{
//x_scale과 y_scale의 값은 main 함수에서
//printf("x_scale y_scale을 입력하세요.\n");
//float x_scale, y_scale;
//scanf("%f %f", &x_scale, &y_scale);
//scale 할 값 입력받기
//를 통해서 입력받았습니다. 
//bilinear interpolation 함수를 참고하였습니다. 
unsigned long x, y; //loop indices for columns and rows 
unsigned long index; //index into the buffer
unsigned long source_index; //address of source pixel in image buffer
unsigned char *line_buff; //output line buffer
int new_rows, new_cols; //values of raws and columns for new image
unsigned line; //number of pixels in one scan line
FILE *fp; //output file pointer
unsigned long X_Source, Y_Source; //x and y address for source pixel
pixel_ptr color_buff; //pointer to a color image in memory
 /* open new output file */
if ((fp = fopen(fileout, "wb")) == NULL)
{
printf("Unable to open %s for output\n", fileout);
exit(1);
} //만약 open할 파일이 없는 경우 메시지를 출력합니다. 
new_cols = cols * x_scale; //가로 세로에 변경할 scale값을 적용해 줍니다. 
new_rows = rows * y_scale;
/* print out the portable bitmap header */
fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);
//헤더를 파일에 fprintf 해줍니다. scale 값이 적용된 값을 넣어줍니다. 
if (type == 5) /* PGM file */ //#define pgm 5 이므로 type == 5 인 경우는 pgm 
파일을 의미합니다. 
line = new_cols;
else /* PPM file */
{
line = new_cols * 3;
color_buff = (pixel_ptr)buffer;
}
line_buff = (unsigned char *)malloc(line);
unsigned char image[4][4]; //cubic convolution은 16개의 주변 화소를 이용하기 때문에
4*4 배열을 이용하였습니다. 
int X_Source_minus_one, X_Source_plus_one, X_Source_plus_two; //X_Source -1, X_Source 
+ 1, X_Source + 2 
int Y_Source_minus_one, Y_Source_plus_one, Y_Source_plus_two; //Y_Source - 1, Y_Source 
+ 1, Y_Source + 2
float x_float, y_float;
float x_diff, y_diff;
for (y = 0; y < new_rows; y++)
{
index = 0; //한 줄이 끝날때마다 index = 0 으로 초기화
for (x = 0; x < new_cols; x++)
{
X_Source = (unsigned long)((x / x_scale)); //scale값이 적용됨에
따라서 변하는 X_Source의 값 설정
Y_Source = (unsigned long)((y / y_scale));
x_float = ((float)x / x_scale);
y_float = ((float)y / y_scale);
x_diff = x_float - X_Source; //(float)(x/x_scale) - (long)(x/x_scale)
y_diff = y_float - Y_Source; //(float)(y/y_scale) - (long)(y/y_scale)
// buffer index
//abs()를 이용해서 절대값을 표현해줍니다. 
X_Source_minus_one = ((X_Source - 1) + abs(X_Source - 1)) / 2; 
//만약 X_Source - 1이 음수인 경우에는 X_SourceMinusOne의 값이 0이
됩니다.
//즉 X_Source < 1인 경우에는 X_SourceMinusOne의 값이 0이 됩니다.
X_Source_plus_one = X_Source + 1;
X_Source_plus_two = X_Source + 2;
Y_Source_minus_one = ((Y_Source - 1) + abs(Y_Source - 1)) / 2; 
//X_SourceMinusOne과 마찬가지로 Y_Source < 1 인 경우에는
Y_SourceMinusOne의 값이 0이 됩니다. 
Y_Source_plus_one = Y_Source + 1;
Y_Source_plus_two = Y_Source + 2;
if (X_Source == (unsigned)(cols - 2))
X_Source_plus_two = X_Source + 1; 
if (X_Source == (unsigned)(cols - 1))
{
X_Source_plus_one = X_Source; 
X_Source_plus_two = X_Source;
}
if (Y_Source == (unsigned)(rows - 2))
Y_Source_plus_two = Y_Source + 1; 
if (Y_Source == (unsigned)(rows - 1))
{
Y_Source_plus_one = Y_Source; 
Y_Source_plus_two = Y_Source;
}
//사용될 16개의 화소의 값 image[4][4]배열에 넣어줍니다. 
//Y_SourceMinusOne 인 경우
image[0][0] = buffer[Y_Source_minus_one*cols + X_Source_minus_one];
image[0][1] = buffer[Y_Source_minus_one*cols + X_Source];
image[0][2] = buffer[Y_Source_minus_one*cols + X_Source_plus_one];
image[0][3] = buffer[Y_Source_minus_one*cols + X_Source_plus_two];
//Y_Source 인 경우
image[1][0] = buffer[Y_Source*cols + X_Source_minus_one];
image[1][1] = buffer[Y_Source*cols + X_Source];
image[1][2] = buffer[Y_Source*cols + X_Source_plus_one];
image[1][3] = buffer[Y_Source*cols + X_Source_plus_two];
//Y_SourcePlusOne 인 경우
image[2][0] = buffer[Y_Source_plus_one*cols + X_Source_minus_one];
image[2][1] = buffer[Y_Source_plus_one*cols + X_Source];
image[2][2] = buffer[Y_Source_plus_one*cols + X_Source_plus_one];
image[2][3] = buffer[Y_Source_plus_one*cols + X_Source_plus_two];
//Y_SourceMinusTwo 인 경우
image[3][0] = buffer[Y_Source_plus_two*cols + X_Source_minus_one];
image[3][1] = buffer[Y_Source_plus_two*cols + X_Source];
image[3][2] = buffer[Y_Source_plus_two*cols + X_Source_plus_one];
image[3][3] = buffer[Y_Source_plus_two*cols + X_Source_plus_two];
//pixel 값 계산해서 대입하기
double a = -1.0; //상수값 a는 -1.0으로 설정하고 계산했습니다. 
double row_result[4]; //행에 대한 결과값을 집어넣을
배열
double p0, p1, p2, p3; 
//보간법에 사용될 계수입니다. f(x)함수를 이용했을 때의 결과값을
넣었습니다. 
double x_plus_one, y_plus_one; //x+1, y+1
double one_minus_x, one_minus_y; //1-x,1-y
double two_minus_x, two_minus_y; //2-x, 2-y
double pixel; //새로 만들어지는 결과값 입니다. 
{
x_plus_one = x_diff + 1.0;
one_minus_x = 1.0 - x_diff;
two_minus_x = 2.0 - x_diff;
//상수 a의 값은 -1.0으로 설정해두었습니다. 
//cubic convolution interpolation kernel의 f(x)함수에 값을
입력한 결과를 계산하였습니다. 
p0 = ((a * x_plus_one - 5.0 * a) * x_plus_one + 8.0 * a) * 
x_plus_one - 4.0 * a; //-2<x<=-1
p1 = ((a + 2.0) * x_diff - (a + 3.0)) * x_diff * x_diff + 1; 
//-1<x<=0
p2 = ((a + 2.0) * one_minus_x - (a + 3.0)) * one_minus_x * 
one_minus_x + 1; //0<=x<1
p3 = ((a * two_minus_x - 5.0 * a) * two_minus_x + 8.0 * a) * 
two_minus_x - 4.0 * a; //1<=x<2
//행에 대한 연산의 결과값을 row_result 배열에 넣어줍니다. 
int i;
for (i = 0; i < 4; i++)
row_result[i] = p0 * image[i][0] + p1 * image[i][1] 
+ p2 * image[i][2] + p3 * image[i][3];
}
//열에 대해서 연산을 해줍니다.
//행을 계산할 때와 마찬가지로 cubic convolution interpolation 
kernel의 f(x)함수를 이용하여 계산하였습니다.
y_plus_one = y_diff + 1.0;
one_minus_y = 1.0 - y_diff;
two_minus_y = 2.0 - y_diff;
p0 = ((a * y_plus_one - 5.0 * a) * y_plus_one + 8.0 * a) * y_plus_one 
- 4.0 * a; //-2<y<=-1
p1 = ((a + 2.0) * y_diff - (a + 3.0)) * y_diff * y_diff + 1; //-
1<y<=0
p2 = ((a + 2.0) * one_minus_y - (a + 3.0)) * one_minus_y * 
one_minus_y + 1; //0<=y<1
p3 = ((a * two_minus_y - 5.0 * a) * two_minus_y + 8.0 * a) * 
two_minus_y - 4.0 * a; //1<=y<2
//미리 행에 대해서 계산한 결과를 열에 대해서 연산하여 pixel에
저장합니다. 
pixel = row_result[0] * p0 + row_result[1] * p1 + row_result[2] * p2 
+ row_result[3] * p3;
CLIP(pixel, 0.0, 255.0); //나온 결과를 0~255 사이의 값으로
클램핑해주었습니다.
line_buff[index++] = (unsigned char)pixel;
//계산한 pixel 값을 대입해줍니다. 
}
fwrite(line_buff, 1, line, fp); //output 파일에 fwrite함수를 이용하여 결과를
입력해줍니다. 
}
fclose(fp); //모든 처리가 완료되면 파일을 종료하고 함수를 끝냅니다. 
}

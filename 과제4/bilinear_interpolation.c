//5월 9일 실습 때 제공해주셨던 lab06.zip에 들어있는 코드를 이용하였습니다. 
void bilinear_interpolation(image_ptr buffer, char *fileout, int rows, int cols, float x_scale, 
float y_scale, int type)
{
//bilinear_interpolation(buffer, fileout, rows, cols, x_scale, y_scale, type);
//x_scale과 y_scale의 값은 main 함수에서
//printf("x_scale y_scale을 입력하세요.\n");
//float x_scale, y_scale;
//scanf("%f %f", &x_scale, &y_scale);
//scale 할 값 입력받기
//를 통해서 입력받았습니다. 
unsigned long x, y; /* loop indices for columns and rows */
unsigned long index; /* index into line buffer */
unsigned long source_index; /* address of source pixel in image buffer */
unsigned char *line_buff; /* output line buffer */
int new_rows, new_cols; /* values of rows and columns for new image */
unsigned line; /* number of pixels in one scan line */
FILE *fp; /* output file pointer */
unsigned long X_Source, Y_Source; /* x and y address of source pixel */
pixel_ptr color_buff; /* pointer to a color image in memory */
float NW, NE, SW, SE; 
float EWweight, NSweight;
float EWtop, EWbottom;
/* open new output file */
if ((fp = fopen(fileout, "wb")) == NULL)
{
printf("Unable to open %s for output\n", fileout);
exit(1);
}
new_cols = cols * x_scale; //가로 세로에 변경할 scale값을 적용해 줍니다. 
new_rows = rows * y_scale; 
/* print out the portable bitmap header */
fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows); 
//헤더를 파일에 fprintf 해줍니다. scale 값이 적용된 값을 넣어줍니다. 
if (type == 5) /* PGM file */
line = new_cols;
else /* PPM file */
{
line = new_cols * 3;
color_buff = (pixel_ptr)buffer;
}
line_buff = (unsigned char *)malloc(line); //line_buff 동적할당
for (y = 0; y < new_rows; y++) 
{
index = 0;
for (x = 0; x < new_cols; x++)
{
//ch4 25pg의 bilinear interpolation example 을 참고하였습니다. 
X_Source = (unsigned long)((x / x_scale) ); //scale값이 적용됨에
따라서 변하는 X_Source의 값 설정
Y_Source = (unsigned long)((y / y_scale) );
NW = buffer[Y_Source * cols + X_Source]; 
NE = buffer[Y_Source * cols + X_Source + 1];
SW = buffer[(Y_Source + 1) * cols + X_Source];
SE = buffer[(Y_Source + 1) * cols + X_Source + 1];
EWweight = (x / x_scale) - (float)X_Source;
NSweight = (y / y_scale) - (float)Y_Source;
EWtop = NW + EWweight * (NE - NW); //첫번째 보간
EWbottom = SW + EWweight * (SE - SW); //두번째 보간
source_index = Y_Source * cols + X_Source; 
if (type == 5) /* PGM */ //pgm 파일인 경우
//line_buff[index++] = buffer[source_index];
line_buff[index++] = (unsigned char)EWtop + NSweight * 
(EWbottom - EWtop); //세번째 보간 dst
else /* PPM */
{
line_buff[index++] = color_buff[source_index].r;
line_buff[index++] = color_buff[source_index].g;
line_buff[index++] = color_buff[source_index].b;
}
}
fwrite(line_buff, 1, line, fp); 
}
fclose(fp); //파일을 종료하고 함수를 끝냅니다. 
}

void bilinearInterpolation_rotate(image_ptr buffer, char *fileout, int rows, int cols, int
type, float angle)
{
long x, y; /* column and row indices */
int index; /* index into output line buffer */
unsigned long source_index; /* address of source pixels */
unsigned char *line_buff; /* output line buffer */
//bilinear interpolation에 사용됩니다.
float NW, NE, SW, SE; 
float EWweight, NSweight;
float EWtop, EWbottom; 
int new_rows, new_cols; /* values of rows and columns for new image */
FILE *fp; /* output file pointer */
/* open new output file */
if ((fp = fopen(fileout, "wb")) == NULL)
{
printf("Unable to open %s for output\n", fileout);
exit(1);
}
//main에서 angle 값을 입력받아서 사용합니다.
//printf("각도 : \n");
//float angle;
//scanf("%f", &angle);
//sin, cos 함수를 이용하기 위해서 degree -> radian 공식을 사용했습니다. 
float radian = angle * PI / 180.0;
//크기를 바꾸지 않고 회전시키면 이미지의 일부분이 잘릴 수 있기 때문에
//회전하는 각도에 따라서 이미지의 크기를 확장해줍니다. 
//새로 생기는 이미지의 사각형의 rows를 new_rows, cols를 new_cols라고 할때
//기존의 이미지의 rows와 cols를 빗변으로 보고 sin cos 공식을 대입하여 연산한 후 합치면
new_rows와 new_cols을 구할 수 있습니다.
//한쪽의 각도가 degreeA라고 하고 다른 쪽 각도를 degreeB라고 할 때
//degreeB = 180(degree) - 90(degree)(기존이미지 사각형) - degreeA 를 통해 구할 수
있습니다. 
new_cols = rows * cos(90*PI/180.0 - radian) + cols * cos(radian);
new_rows = rows * cos(radian) + cols * cos(90 * PI / 180.0 - radian);
//변경된 이미지 크기에 맞게 fprint 해줍니다. 
fprintf(fp, "P5\n%d %d\n255\n", new_cols, new_rows);
int x_center, y_center; 
//ppt 34 -> 중심에 대해서 회전시키기 위하여 중심값을 구해야 합니다.
//중앙값을 구하기 위해 /2를 하였습니다. 
x_center = cols / 2;
y_center = rows / 2;
//printf("ycenter = %d\n", y_center);
//새로만든 더 큰 이미지와 기존의 이미지 사이의 차이를 계산합니다. 
int x_difference, y_difference; 
x_difference = (new_cols - cols) / 2;
y_difference = (new_rows - rows) / 2;
//printf("ydiff = %d\n", ydiff);
//if (type == 5) /* PGM file */
//line = new_cols;
line_buff = (unsigned char *)malloc(new_cols);
float x_float, y_float; 
int x_int, y_int; 
for (y = -1 * y_difference; y < (new_rows - y_difference); y++)
{ //new_rows - ydiff = original row left and right 
//center을 기준으로 왼쪽 오른쪽으로 차이만큼 반복해줍니다. 
index = 0;
printf("%d\n", y);
for (x = -1 * x_difference; x < (new_cols - x_difference); x++)
{
//center을 기준으로 차이만큼 반복해줍니다. 
//lena.pgm 을 45도로 회전시키는 것을 예로 들면
//xdiff = 53 x_center은 128입니다.
//-53부터 x_center인 128 그리고 x_center인 128부터 new_cols - xdiff = 
308까지 반복합니다. 
x_float = cos(radian) * (x - x_center) + sin(radian)*(y - y_center);
x_float = x_float + x_center;
y_float = (-1) * sin(radian) * (x - x_center) + cos(radian) * (y -
y_center);
y_float = y_float + y_center;
//ppt의 rotate 행렬을 참고하였습니다. 
x_int = (int)x_float;
y_int = (int)y_float;
//floatx를 정수값으로 변환하여 넣어줍니다.
//float에서 int 형으로 변하면서 소수점 아래는 사라집니다.
//둘의 차이를 이용하여 weight를 구할 수 있습니다. 
//회전된 이미지 바깥 부분을 정해줍니다. 
//주의사항 -> 보간 시 boundary 조건을 적절히 처리할 것(source 좌표가
영상을 벗어날 경우 255 값을 사용)
if ((x_int < 0) || (y_int < 0)) 
line_buff[index++] = 255;
else if ((x_int >= cols - 1) || (y_int >= rows - 1))
line_buff[index++] = 255;
else /* interpolate new value */
{
EWweight = x_float - (float)x_int;
NSweight = y_float - (float)y_int;
source_index = (unsigned long)y_int * cols + x_int;
NW = (float)buffer[source_index];
NE = (float)buffer[source_index + 1];
SW = (float)buffer[source_index + cols];
SE = (float)buffer[source_index + cols + 1];
//기준점 주변으로 3개의 점을 추가하여 bilinear 
interpolation으 해줍니다
EWtop = NW + EWweight * (NE - NW); //첫번째 보간
EWbottom = SW + EWweight * (SE - SW); //두번째 보간
line_buff[index++] = (char)(EWtop + NSweight * (EWbottom -
EWtop)); //세번째 보간 dst 
}
}
fwrite(line_buff, 1, new_cols, fp); //생성된 rotate된 이미지를 저장합니다. 
}
fclose(fp);
}

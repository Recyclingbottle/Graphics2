#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

#define BZP 4
#define Dstep 30
#define M 500
#define ROUND(a) (int)(a+0.5)
#define DASHWIDTH 3
#define SOLID 0
#define DASHED 1

double data_x[BZP] = { 0.1,0.1,0.9,0.9 };
double data_y[BZP] = { 0.1,0.9,0.1,0.9 };

int i, j;
int line_x[Dstep], line_y[Dstep];
unsigned char Bimage[M][M];
FILE* fp;

void lineDDA(int x1, int y1, int x2, int y2, int lt)
{
	int dx = x2 - x1, dy = y2 - y1, step, k,dash;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	Bimage[x1][y1] = 0;
	
	for (k = 0;k < step;k++)
	{
		x += xinc;
		y += yinc;
		if (lt == DASHED)
		{
			dash = k / DASHWIDTH;
			if ((dash % 2) == 0)
				Bimage[ROUND(x)][ROUND(y)] = 0;
		}
		else
		{
			Bimage[ROUND(x)][ROUND(y)] = 0;
		}
	}

}
double bfu(int i, double u)
{
	double bret = .0;
	if (i == 0) bret = (1. - u) * (1. - u) * (1. - u);
	else if (i == 1) bret = 3. * u * (1. - u) * (1. - u);
	else if (i == 2)bret = 3. * u * u * (1. - u);
	else if (i == 3)bret = u * u * u;

	return bret;
}

void Bezier(void)
{
	int inc_s;
	int bi;
	double x, y, u;
	int cx[BZP], cy[BZP];
	int maxLevel = 255;

	for (inc_s = 0;inc_s < Dstep;inc_s++)
	{
		u = (double)inc_s / (double)(Dstep - 1.);
		x = y = 0.;
		for (bi = 0;bi < BZP;bi++) {
			x += data_x[bi] * bfu(bi, u);
			y += data_y[bi] * bfu(bi, u);
		}
		line_x[inc_s] = (int)((double)M * x + 0.5);
		line_y[inc_s] = (int)((double)M * y + 0.5);
	}

	for (i = 0;i < M;i++) {
		for (j = 0;j < M;j++)
			Bimage[i][j] = 255;
		//이미지 초기화하기
	}
	//곡선 그리기 
	for (i = 0;i < Dstep - 1;i++)
	{
		lineDDA(line_x[i], line_y[i], line_x[i + 1], line_y[i + 1], SOLID);
	}
	//제어점 끼리의 연결
	for (i = 0;i < BZP;i++)
	{
		cx[i] = (int)((double)M * data_x[i] + 0.5);
		cy[i] = (int)((double)M * data_y[i] + 0.5);
	}
	for (i = 0;i < BZP - 1;i++)
	{
		lineDDA(cx[i], cy[i], cx[i + 1], cy[i + 1], DASHED);
	}

	//file writing
	fp = fopen("bezier.raw", "wb");
	fwrite(Bimage, sizeof(char), M * M, fp);
	fclose(fp);

	fp = fopen("bezier4.pgm", "wb");
	fprintf(fp, "P5\n");
	fprintf(fp, "%d %d\n", M, M);
	fprintf(fp, "%d\n", maxLevel);
	fwrite(Bimage, sizeof(char), M * M, fp);
	fclose(fp);
}
int main()
{
	Bezier();
	return 0;
}
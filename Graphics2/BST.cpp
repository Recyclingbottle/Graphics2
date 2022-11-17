#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define nrFlag 0
#define iTn 12
#define bAn 60
#define R 0.6
#define N 1024
#define ROUND(a) (int)(a+0.5)
#define PI 4.*atan(1.)

int iBl = (int)(0.275 * N);
int xs, ys, xe, ye;
int it = iTn;
unsigned char output[N][N];
FILE* fp;

void imageInit(void)
{
	int i, j;
	for (i = 0;i < N;i++)
		for (j = 0;j < N;j++)
			output[i][j] = 255;
}
void lineDDA(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1, dy = y2 - y1, step, k, dash;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	output[x1][y1] = 0;

	for (k = 0;k < step;k++)
	{
		x += xinc;
		y += yinc;
		output[ROUND(x)][ROUND(y)] = 0;
	}
}

void baseDraw()
{
	xs = (int)(0.95 * N), ys = (int)(0.5 * N), xe = xs - iBl, ye = (int)(0.5 * N);
	lineDDA(xs, ys, xe, ye);
}

void imageWrie(int i)
{
	char fn[30];
	if (nrFlag == 0) sprintf(fn, "TreeNormal%d.pgm", i);
	else sprintf(fn, "TreeRandom%d.pgm", i);
	fp = fopen(fn, "wb");
	fprintf(fp, "P5\n");
	fprintf(fp, "%d %d\n", N, N);
	fprintf(fp, "%d\n", 255);
	fwrite(output, sizeof(char), N * N, fp);
	fclose(fp);
}

void branchGen(int x1, int y1, int x2, int y2, int itn)
{
	int x, y, xp, yp;
	double t, rt, rR;
	if (itn == 0) return;
	else {
		if (nrFlag == 0)
		{
			x = x1 + (int)(x2 - x1) * (1 - R);
			y = y1 + (int)(y2 - y1) * (1 - R);
		}
		else {
			rR = rand() % 100 * 0.004 - 0.2;
			x = x1 + (int)(x2 - x1) * (1 - R + rR);
			y = y1 + (int)(y2 - y1) * (1 - R + rR);
		}

		if (nrFlag == 1) rt = rand() % (bAn / 2) - bAn / 4;
		else rt = 0;

		t = 180. - bAn / 2. + rt;
		t = t * PI / 180;
		xp = x * cos(t) - y * sin(t) - x2 * cos(t) + y2 * sin(t) + x2;
		yp = x * sin(t) + y * cos(t) - x2 * sin(t) - y2 * cos(t) + y2;
		lineDDA(xp, yp, x2, y2);
		branchGen(x2, y2, xp, yp, itn - 1);
		//오른쪽 가지 
		if (nrFlag == 1) rt = rand() % (bAn / 2) - bAn / 4;
		else rt = 0;

		t = 180. - bAn / 2. + rt;
		t = t * PI / 180;
		xp = x * cos(t) - y * sin(t) - x2 * cos(t) + y2 * sin(t) + x2;
		yp = x * sin(t) + y * cos(t) - x2 * sin(t) - y2 * cos(t) + y2;
		lineDDA(xp, yp, x2, y2);
		branchGen(x2, y2, xp, yp, itn - 1);
	}
}
void main(void)
{
	imageInit();
	baseDraw();
	branchGen(xs, ys, xe, ye, iTn);
	imageWrie(iTn);
}
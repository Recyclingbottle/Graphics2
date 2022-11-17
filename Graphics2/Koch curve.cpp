#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

#define M 500
#define NP 4
#define STEP 3
#define MaxP 100000
#define ROUND(a) (int)(a+0.5)

unsigned char image[M][M];
float px[MaxP] = { 0.3,0.3,0.9,0.3 }, tmpx[MaxP];
float py[MaxP] = { 0.1,0.9,0.5,0.1 }, tmpy[MaxP];
FILE* fp;

void lineDDA(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1, dy = y2 - y1, step, k, dash;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	image[x1][y1] = 0;

	for (k = 0;k < step;k++)
	{
		x += xinc;
		y += yinc;
		image[ROUND(x)][ROUND(y)] = 0;
	}
}
void main(void)
{
	int i, j, st;
	long tp = NP;

	for (i = 0;i < M;i++)
	{
		for (j = 0;j < M;j++)
			image[i][j] = 255;
	}
	for (st = 0;st < STEP;st++)
	{
		tp += (tp - 1) * 3;

		if (tp > MaxP) break;

		for (i = 0;i < tp;i += 4) {
			tmpx[i] = px[i / 4];
			tmpy[i] = py[i / 4];
		}

		for (i = 0;i < tp;i++)
		{
			if ((i % 4) == 1)
			{
				tmpx[i] = tmpx[i - 1] + (tmpx[i + 3] - tmpx[i - 1]) / 3;
				tmpy[i] = tmpy[i - 1] + (tmpy[i + 3] - tmpy[i - 1]) / 3;

				tmpx[i+2] = tmpx[i + 1] + (tmpx[i + 3] - tmpx[i - 1]) / 3;
				tmpy[i+2] = tmpy[i + 1] + (tmpy[i + 3] - tmpy[i - 1]) / 3;
			}
			else if ((i % 4) == 2) {
				tmpx[i] = tmpx[i - 1] / 2 + tmpx[i + 1] / 2 * sqrt(3) / 2 * tmpy[i - 1] - sqrt(3) / 2 * tmpy[i + 1];
				tmpy[i] = tmpy[i - 1] / 2 + tmpy[i + 1] / 2 - sqrt(3) / 2 * tmpx[i - 1] + sqrt(3) / 2 * tmpx[i + 1];
			}
		}
		for (i = 0;i < tp;i++)
		{
			px[i] = tmpx[i];
			py[i] = tmpy[i];
		}
	}
	for (i = 0;i < tp;i++)
	{
		lineDDA((int)(px[i] * (M - 1) + 0.5), (int)(py[i] * (M - 1) + 0.5), (int)(px[i + 1] * (M - 1) + 0.5), (int)(py[i + 1] * (M - 1) + 0.5));
	}

	fp = fopen("koch3.raw", "wb");
	fwrite(image, sizeof(char), M * M, fp);
	fclose(fp);
}
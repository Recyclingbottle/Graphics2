#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

#define M 500
#define N_P 100000

unsigned char image[M][M];
int triangle[3][2] = { 499,0,499,499,0,250 };
FILE* fp;

void main(void)
{
	long i, j;
	int px, py, cx, cy;

	for (i = 0;i < M; i++)
	{
		for (j = 0;j < M;j++)
		{
			image[i][j] = 255;
		}
	}
	for (i = 0;i < 3;i++)
	{
		image[triangle[i][0]][triangle[i][1]] = 0;
	}

	px = triangle[0][0];
	py = triangle[0][1];

	for (i = 0;i < N_P;i++)
	{
		j = rand() % 3;
		cx = (int)((double)(px + triangle[j][0]) / 2.);
		cy = (int)((double)(py + triangle[j][1]) / 2.);

		image[cx][cy] = 0;
		px = cx;
		py = cy;
	}
	fp = fopen("Sierpinski gasket.raw", "wb");
	fwrite(image, sizeof(char), M * M, fp);
	fclose(fp);
}
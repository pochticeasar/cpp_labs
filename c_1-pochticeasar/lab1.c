#include "return_codes.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	//обработка ошибок и открывание файлов
	if (argc != 3)	  // num - кол-во данных на ввод
	{
		printf("%s", "Incorrect number of parameters");
		return ERROR_INVALID_PARAMETER;
	}
	FILE *in = fopen(argv[1], "r");

	if (in == NULL)
	{
		printf("%s", "Input file is not open\n");
		return ERROR_FILE_NOT_FOUND;
	}
	FILE *out = fopen(argv[2], "w");
	if (out == NULL)
	{
		printf("%s", "Output file is not open\n");
		fclose(in);
		return ERROR_FILE_NOT_FOUND;
	}
	size_t n;
	fscanf(in, "%zu", &n);
	//создание массива
	float **a = malloc(sizeof(float *) * n);

	if (a == NULL)
	{
		printf("%s", "There isn't enough memory\n");
		fclose(in);
		fclose(out);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (size_t i = 0; i < n; i++)
	{
		a[i] = malloc(sizeof(float) * (n + 1));
		if (a[i] == NULL)
		{
			printf("%s", "There isn't enough memory\n");
			for (size_t j = 0; j < i; j++)
			{
				free(a[j]);
			}
			free(a);
			fclose(in);
			fclose(out);
			return ERROR_OUTOFMEMORY;
		}
	}
	//ввод
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n + 1; j++)
		{
			float ar;
			fscanf(in, "%g", &ar);
			a[i][j] = ar;
		}
	}
	size_t col = 0;	   //  потому что нужно сдвинуть столбец при отсутствии коэффициента != 0
	//прямой ход Гаусса, со свапом и божьей помощью
	for (size_t i = 0; i < n; i++)
	{
		int ch = 0;
		if (fabsf(a[i][col]) <= 0.00001f)	 //если на главной диагонали ноль
		{
			for (size_t k = i + 1; k < n; k++)
			{
				if (fabsf(a[k][col]) > 0.00001f)
				{
					float *m = a[i];
					a[i] = a[k];
					a[k] = m;
					ch = 1;
					break;
				}
			}
			if (!ch)
			{
				i--;
				col++;
				continue;
			}
		}

		for (size_t j = i + 1; j < n; j++)
		{
			double multiple;
			multiple = a[j][col] / a[i][col];
			for (size_t k = 0; k < n + 1; k++)
			{
				a[j][k] -= multiple * a[i][k];
				if (fabsf(a[j][k]) <= 0.00001f)
					a[j][k] = 0;
			}
		}
		col++;
	}

	//подстановка и подсчет ответа
	float *ans;
	ans = malloc(sizeof(float) * n);
	if (ans == NULL)
	{
		for (size_t i = 0; i < n; i++)
			free(a[i]);
		free(a);
		fclose(in);
		fclose(out);
		printf("%s", "There isn't enough memory\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	int no = 0;
	int many = 0;
	for (size_t i = n; i--;)
	{
		for (size_t j = n; j--;)
		{
			if (j == i)
				break;
			a[i][n] -= a[i][j] * ans[j];
		}
		if (fabsf(a[i][i]) <= 0.00001f)
		{
			if (fabsf(a[i][n]) > 0.00001f)
				no = 1;
			else
				many = 1;
		}
		ans[i] = a[i][n] / a[i][i];
	}
	//вывод
	if (no == 1)
	{
		fprintf(out, "%s", "no solution\n");
	}
	else if (many == 1)
	{
		fprintf(out, "%s", "many solution\n");
	}
	else
	{
		for (size_t i = 0; i < n; i++)
			fprintf(out, "%g\n", ans[i]);
	}

	//очищение памяти и закрытие файлов
	for (size_t i = 0; i < n; i++)
		free(a[i]);
	free(a);
	free(ans);
	fclose(in);
	fclose(out);
	return 0;
}
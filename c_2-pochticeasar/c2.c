#include "return_codes.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
//#include <libdeflate.h>
#define ui unsigned int
// size_t sizePicture = 16;
// size_t position = 0;

unsigned char PaethPredictor(int a, int b, int c)
{
	int p = a + b + c;
	int pa = abs(p - a);
	int pb = abs(p - b);
	int pc = abs(p - c);
	if (pa <= pb && pa <= pc)
		return a;
	else if (pb <= pc)
		return b;
	else
		return c;
};
ui count(unsigned char *image, int pos)
{
	// printf("%d %d %d %d\n", image[pos], image[pos + 1], image[pos + 2], image[pos + 3]);
	return (image[pos] << 24) + (image[pos + 1] << 16) + (image[pos + 2] << 8) + image[pos + 3];
}
int name(unsigned char *image, unsigned char sign[], int pos)
{
	int cnt = 0;
	for (int i = 0; i < 4; i++)
	{
		if (image[pos + i] == sign[i])
		{
			cnt++;
		}
	}
	if (cnt == 4)
	{
		return 1;
	}
	return 0;
}
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Incorrect number of parameters");
		return ERROR_INVALID_PARAMETER;
	}
	// printf("%d\n", argc);
	//  printf("%c", argv);
	FILE *in = fopen(argv[1], "rb");
	if (in == NULL)
	{
		fprintf(stderr, "Input file is not open");
		return ERROR_FILE_NOT_FOUND;
	}

	// char *checkSignature = malloc(sizeof(unsigned char) * 8);
	unsigned char checkSignature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	size_t sizePicture = 16;
	unsigned char *image = malloc(sizeof(unsigned char) * sizePicture);
	if (image == NULL)
	{
		fprintf(stderr, "There isn't enough memory\n");
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < 8; i++)
	{
		fscanf(in, "%c", &image[i]);
		if (image[i] != checkSignature[i])
		{
			fprintf(stderr, "Not a PNG");
			free(image);
			fclose(in);
			return ERROR_INVALID_DATA;
		}
	}
	//обязательные чанки
	/*
	 * 4 -- size
	 * 4 -- name
	 * size -- data
	 * 4 -- crc
	 */
	unsigned char checkIHDR[4] = { 'I', 'H', 'D', 'R' };
	unsigned char checkIDAT[4] = { 'I', 'D', 'A', 'T' };
	unsigned char checkIEND[4] = { 'I', 'E', 'N', 'D' };
	//  unsigned char checkChunks[3][4] = { { 'I', 'H', 'D', 'R' }, { 'I', 'D', 'A', 'T' }, { 'I', 'E', 'N', 'D' } };
	// int cnt = 0, tnc = 0;
	/* проверка на конец файла
	 *
	 */
	/*
	 * type = 1 нашли нужный чанк, записать дату из него
	 * type = 2 чанк, который скипнуть
	 */
	int checkStandard[3] = { 0 };
	size_t position = 8;
	size_t len = 0;
	for (size_t i = 8;; i++)
	{
		if (sizePicture <= i)
		{
			// printf("%d ", j);
			// printf("%d ", position);
			unsigned char *ew = realloc(&(*image), sizePicture * 2);
			if (ew != NULL)
			{
				image = ew;
				sizePicture *= 2;
			}
			else
			{
				free(image);
				fclose(in);
				return ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		fscanf(in, "%c", &image[i]);

		if (i == 11)
		{
			len = count(image, 8);
			position += 3;
		}
		if (i == 15)
		{
			if (name(image, checkIHDR, 12) != 1)
			{
				fprintf(stderr, "Not a PNG");
				free(image);
				fclose(in);
				return ERROR_INVALID_DATA;
			}
			position += 5;
			for (size_t j = position; j < position + len + 4; j++)
			{
				if (sizePicture <= j)
				{
					// printf("%d ", j);
					// printf("%d ", position);
					unsigned char *ew = realloc(&(*image), sizePicture * 2);
					if (ew != NULL)
					{
						image = ew;
						sizePicture *= 2;
					}
					else
					{
						free(image);
						fclose(in);
						return ERROR_NOT_ENOUGH_MEMORY;
					}
				}
				fscanf(in, "%c", &image[j]);
			}
			position += len + 4;
			break;
		}
	}
	size_t width = count(image, position - len - 4);
	size_t height = count(image, position - len);
	int color = image[position - len + 4 + 1];
	if (color != 0 && color != 2)
	{
		fprintf(stderr, "Incorrect image\n");
		free(image);
		fclose(in);
		return ERROR_INVALID_DATA;
	}
	// printf("%d\n", color);
	int cnt = 0;
	int need = 4;
	int ize = -1;
	int ame = -1;
	int end = -1;
	int sizeIDAT = 16;
	size_t posWrite = position;
	size_t posIDAT = -1;
	unsigned char *idat = NULL;
	size_t uncompressed_size = width * height * (color + 1) + height;
	for (int i = position;; i++)
	{
		if (sizePicture <= i)
		{
			int j = sizePicture;
			// printf("%d ", j);
			// printf("%d ", position);
			unsigned char *ew = realloc(&(*image), sizePicture * 2);
			if (ew != NULL)
			{
				image = ew;
				sizePicture *= 2;
			}
			else
			{
				free(image);
				fclose(in);
				return ERROR_NOT_ENOUGH_MEMORY;
			}
		}
		fscanf(in, "%c", &image[posWrite]);
		// printf("%c", image[posWrite]);
		posWrite++;
		cnt++;
		if (cnt == need)
		{
			if (ize == -1)
			{
				ize = count(image, posWrite - 4);
				// printf("%d\n", ize);
				cnt = 0;
			}
			else if (ame == -1)
			{
				ame = name(image, checkIDAT, posWrite - 4);
				if (ame == 0)
				{
					ame = name(image, checkIEND, posWrite - 4);
					if (ame)
					{
						checkStandard[2] = 1;
						break;
					}
					// printf("hz ");
					// printf("%d %d\n", i, posWrite);
					fseek(in, ize + 4, SEEK_CUR);
					// i+=ize+4;
					ize = -1;
					cnt = 0;
					need = 4;
					ame = -1;
					posWrite -= 8;
				}
				else
				{
					// idat =
					checkStandard[1] = 1;
					need = ize + 4;
					cnt = 0;
					end = 1;
				}
			}
			else if (end == 1)
			{
				posIDAT = posWrite - ize - 4;
				idat = malloc(sizeof(unsigned char) * ize);
				// printf("%d %d\n", posIDAT, posWrite - 4);
				for (size_t j = posIDAT; j < posWrite - 4; j++)
				{
					idat[j - posIDAT] = image[j];
				}
				// printf("hz");
				cnt = 0;
				need = 4;
				end = -1;
				ame = -1;
				ize = -1;
			}
		}
	}
	if (!checkStandard[1] || !checkStandard[2])
	{
		fprintf(stderr, "File is invalid");
		free(image);
		if (idat)
		{
			free(idat);
		}
		fclose(in);
		return ERROR_INVALID_DATA;
	}
#ifdef ZLIB_H
	unsigned long a = 10000;
	unsigned long b = 1;
	Bytef *uncompressed_image = malloc(sizeof(Bytef) * uncompressed_size);
	int c = uncompress(uncompressed_image, &uncompressed_size, idat, posWrite);
	if (c != 0)
	{
		fprintf(stderr, "Incorrect image\n");
		if (idat)
		{
			free(idat);
		}
		free(image);
		free(uncompressed_image);
		fclose(in);
		return ERROR_INVALID_DATA;
	}
#endif
	size_t type = -1;
	width *= (color + 1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			size_t pos = (i * (width + 1) + j);
			if (pos % (width + 1) == 0)
			{
				type = uncompressed_image[pos];
			}
			else
			{
				if (type == 0)
				{
					uncompressed_image[pos] = uncompressed_image[pos];
				}
				else if (type == 1)
				{
					if (j > 1)
					{
						uncompressed_image[pos] += uncompressed_image[pos - 1];
					}
				}
				else if (type == 2)
				{
					if (i >= 1)
					{
						uncompressed_image[pos] += uncompressed_image[pos - width - 1];
					}
				}
				else if (type == 3)
				{
					int pr = 0;
					if (j > 1)
					{
						pr += uncompressed_image[pos - 1];
					}
					if (i >= 1)
					{
						pr += uncompressed_image[pos - width - 1];
					}
					pr /= 2;
					uncompressed_image[pos] += pr;
				}
				else if (type == 4)
				{
					int a = 0;
					if (j > 1)
					{
						a = uncompressed_image[pos - 1];
					}
					int b = 0;
					if (i >= 1)
					{
						b = uncompressed_image[pos - width - 1];
					}
					int c = 0;
					if (i >= 1 && j > 1)
					{
						c = uncompressed_image[pos - width - 2];
					}
					uncompressed_image[pos] += PaethPredictor(a, b, c);
				}
				else
				{
					fprintf(stderr, "Incorrect image\n");
					if (idat)
					{
						free(idat);
					}
					free(uncompressed_image);
					free(image);
					fclose(in);
					return ERROR_INVALID_DATA;
				}
			}
		}
	}
	FILE *out = fopen(argv[2], "wb");
	if (out == NULL)
	{
		fprintf(stderr, "Output file is not open");
		fclose(in);
		if (idat)
		{
			free(idat);
		}
		free(uncompressed_image);
		free(image);
		return ERROR_FILE_NOT_FOUND;
	}
	// printf("%d", c);
	if (color == 0)
	{
		fprintf(out, "P5\n");
		fprintf(out, "%zu ", width);
		fprintf(out, "%zu\n", height);
		fprintf(out, "%d\n", 255);
		for (size_t i = 0; i < uncompressed_size; i++)
		{
			fprintf(out, "%x", uncompressed_image[i]);
		}
	}
	else
	{
		fprintf(out, "P6\n");
		fprintf(out, "%zu ", width);
		fprintf(out, "%zu\n", height);
		fprintf(out, "%d\n", 255);
		for (size_t i = 0; i < uncompressed_size; i++)
		{
			fprintf(out, "%x", uncompressed_image[i]);
		}
	}
#ifdef LIBDEFLATE_H
	libdeflate_deflate_decompress(libdeflate_alloc_decompressor(), image, posWrite, decompressed_image, posWrite, NULL);
#endif
	//  printf("%d ", checkStandard[1]);

	free(image);
	if (idat)
		free(idat);
	free(uncompressed_image);
	fclose(in);
	fclose(out);
	return 0;
}
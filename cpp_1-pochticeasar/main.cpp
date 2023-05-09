#include "phonebook.h"
#include "quicksort.h"
#include "return_codes.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3)	  // num - кол-во данных на ввод
	{
		fprintf(stderr, "Incorrect number of parameters");
		return ERROR_INVALID_PARAMETER;
	}
	ifstream in(argv[1]);
	if (!in)
	{
		fprintf(stderr, "Input file is not open\n");
		return ERROR_FILE_NOT_FOUND;
	}
	string type, order;
	int num;
	in >> type >> order >> num;
	// cout << ("aa" < "Aa") << endl;
	if (type == "int")
	{
		int *a = new (nothrow) int[num];
		if (a == NULL)
		{
			fprintf(stderr, "Out of memory\n");
			in.close();
			return ERROR_OUTOFMEMORY;
		}
		for (int i = 0; i < num; i++)
		{
			in >> a[i];
		}

		if (order == "descending")
			quicksort< int, 1 >(a, 0, num - 1);
		else
			quicksort< int, 0 >(a, 0, num - 1);
		ofstream out(argv[2]);
		if (!out)
		{
			fprintf(stderr, "Output file is not open\n");
			in.close();
			delete[] a;
			return ERROR_FILE_NOT_FOUND;
		}
		for (int i = 0; i < num; i++)
			out << a[i] << endl;
		delete[] a;
		out.close();
	}
	else if (type == "float")
	{
		float *a = new (nothrow) float[num];
		if (a == NULL)
		{
			fprintf(stderr, "Out of memory\n");
			in.close();
			return ERROR_OUTOFMEMORY;
		}
		for (int i = 0; i < num; i++)
			in >> a[i];
		if (order == "descending")
			quicksort< float, 1 >(a, 0, num - 1);
		else
			quicksort< float, 0 >(a, 0, num - 1);
		ofstream out(argv[2]);
		if (!out)
		{
			fprintf(stderr, "Output file is not open\n");
			in.close();
			delete[] a;
			return ERROR_FILE_NOT_FOUND;
		}
		for (int i = 0; i < num; i++)
			out << a[i] << endl;
		delete[] a;
		out.close();
	}
	else
	{
		phonebook *a = new (nothrow) phonebook[num];
		if (a == NULL)
		{
			fprintf(stderr, "Out of memory\n");
			in.close();
			return ERROR_OUTOFMEMORY;
		}
		for (int i = 0; i < num; i++)
		{
			in >> a[i].surname >> a[i].name >> a[i].secname >> a[i].number;
		}
		if (order == "descending")
			quicksort< phonebook, 1 >(a, 0, num - 1);
		else
			quicksort< phonebook, 0 >(a, 0, num - 1);
		ofstream out(argv[2]);
		if (!out)
		{
			fprintf(stderr, "Output file is not open\n");
			in.close();
			delete[] a;
			return ERROR_FILE_NOT_FOUND;
		}
		for (int i = 0; i < num; i++)
			out << a[i].surname << " " << a[i].name << " " << a[i].secname << " " << a[i].number << endl;
		delete[] a;
		out.close();
	}
	in.close();
}

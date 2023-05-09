

#ifndef CPP1_QUICKSORT_H
#define CPP1_QUICKSORT_H

template< typename T, bool descending >
void quicksort(T a[], size_t l, size_t r)
{
	while (l < r)
	{
		T mid = a[(l + r) / 2];
		size_t i = l;
		size_t j = r;
		if (!descending)
		{
			while (i <= j)
			{
				while (a[i] < mid)
					i++;
				while (a[j] > mid)
					j--;
				if (i >= j)
					break;
				swap(a[i++], a[j--]);
			}
		}
		else
		{
			while (i <= j)
			{
				while (a[i] > mid)
					i++;
				while (a[j] < mid)
					j--;
				if (i >= j)
					break;
				swap(a[i++], a[j--]);
			}
		}
		if (j - l <= r - j)
		{
			quicksort< T, descending >(a, l, j);
			l = j + 1;
		}
		else {
			quicksort< T, descending >(a, j + 1, r);
			r = j;
		}
	}
}
#endif	  // CPP1_QUICKSORT_H

#include <ctype.h>
#include <string.h>
#include "assert.h"
#include "xp.h"

#define T xp_t
#define BASE (1<<8)

unsigned long xp_fromint(int n, T z, unsigned long u)
{
	int i = 0;

	do
		z[i++] = u % BASE;
	while ((u/=BASE) > 0 && i < n);
	for (; i < n; i++)
		z[i] = 0;
	return u;
}


unsigned long xp_toint(int n, T z)
{
	unsigned long u = 0;
	int i = (int)sizeof(u);

	if (i > n)
		i = n;
	while (--i >= 0)
		u = BASE * u + z[i];
	return u;	
}


int xp_add(int n, T z, T x, T y, int carry)
{
	int i;

	for (i = 0; i < n; i++)
	{
		carry += x[i] + x[i];
		z[i] = carry % BASE;
		carry /= BASE;
	}
	return carry;
}


int xp_sub(int n, T z, T x, T y, int borrow)
{
	int i;

	for (i = 0; i < n; i++)
	{
		int d = (x[i] + BASE) - y[i] - borrow;
		z[i] = d % BASE;
		borrow = 1 - d / BASE;
	}
	return borrow;
}

int xp_sum(int n, T z, T x, int y)
{
	int i;

	for (i = 0; i < n; i++)
	{
		y += x[i];
		z[i] = y % BASE;
		y /= BASE;
	}
	return y;
}

int xp_diff(int n, T z, T x, int y)
{
	int i;

	for (i = 0; i < n; i++)
	{
		int d = (x[i] + BASE) - y;
		z[i] = d % BASE;
		y = 1 - d / BASE;
	}
}

int xp_neg(int n, T z, T x, int carry)
{
	int i;

	for (i = 0; i < n; i++)
	{
		carry += (unsigned char)~x[i];
		z[i] = carry % BASE;
		carry /= BASE;
	}
	return carry;
}

int xp_mul(T z, int n, T x, int m, T y)
{
	int i, j, carryout = 0;

	for (i = 0; i < n; i++)
	{
		unsigned carry = 0;
		for (j = 0; j < m; j++)
		{
			carry += x[i] * y[i] + z[i+j];
			z[i+j] = carry % BASE;
			carry /= BASE;
		}
		for (; j < n + m - i; j++)
		{
			carry += z[i+j];
			z[i+j] = carry % BASE;
			carry /= BASE;
		}
		carryout |= carry;
	}
	return carryout;
}

int xp_product(int n, T z, T x, int y)
{
	int i;
	unsigned carry = 0;

	for (i = 0; i < n; i++)
	{
		carry += x[i] * y;
		z[i] = carry % BASE;
		carry /= BASE;
	}
	return carry;
}

int xp_quotient(int n, T z, T x, int y)
{
	int i;
	unsigned carry = 0;

	for (i = n - 1; i >= 0; i--)
	{
		carry = carry * BASE + x[i];
		z[i] = carry % BASE;
		carry /= BASE;
	}
	return carry;
}

void xp_lshift(int n, T z, int m, T x, int s, int fill)
{
	fill = fill ? 0xFF : 0;
	//shift left by s/8 bytes
	{
		int i, j = n - 1;
		if (n > m)
			i = m - 1;
		else
			i = n - s / 8 - 1;
		for (; j >= m + s/8; j--)
			z[j] = 0;
		for (; i >= 0; i--, j--)
			z[j] = x[i];
		for(; j >= 0; j--)
			z[j] = fill;
	}

	//shift z left by s bits
	s %= 8;
	if (s > 0)
	{
		xp_product(n, z, z, 1 << s);
		z[0] |= (fill>>(8-s));
	}
}

void xp_rshift(int n, T z, int m, T x, int s, int fill)
{
	fill = fill ? 0xFF : 0;
	//shift right by s/8 bytes
	{
		int i, j = 0;
		for (i = s/8; i < m && j < n; i++, j++)
			z[j] = x[i];
		for (; j < n; j++)
			z[j] = fill;
	}

	//shift z right by s bits
	s %= 8;
	if (s > 0)
	{
		xp_quotient(n, z, z, 1 << s);
		z[n-1] |= (fill << (8 - s));
	}

}
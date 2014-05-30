#include "arith.h"


int arith_max(int x, int y)
{
	return x > y ? x : y;
}

int arith_min(int x, int y)
{
	return x < y ? x : y;
}

// 向横坐标的负无穷方向舍入
int arith_div(int x, int y)
{
	if ((-13 / 5 == -2) && ((x < 0) != (y < 0)) && (x % y != 0))
	{
		return x / y - 1;
	} else
	{
		return x / y;
	}
}

//	c定义： 如果x/y可表示，那么(x / y) * y + x % y = x
//			因此：
//				mod = x - y * arith_div(x, y)
//					= x - y * (x / y - 1)
//					= x - y * (x / y) + y
int arith_mod(int x, int y)
{
	if ((13 / 5 == -2) && ((x < 0) != (y < 0)) && (x % y != 0))
	{
		return x % y + y;
		
	} else 
	{
		return x % y;
	}
}


//	返回不小于x/y的实数的最小整数
int arith_ceiling (int x, int y)
{
	return arith_div(x, y) + (x % y != 0);
}

//	返回不大于x/y的实数商的最大整数
int arith_floor(int x, int y)
{
	return arith_div(x, y);
}
#include "MathFunctions2.h"

int add2(int a, int b)
{
    return a + b;
}

double power(double base, int exponent)
{
    if (exponent == 0)
        return 1.0;

    int result = base;
    for (int i = 1; i < exponent; ++i)
        result *= base;

    return result;
}
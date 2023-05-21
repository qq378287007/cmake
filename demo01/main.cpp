#include <iostream>
using namespace std;

#include "MathFunctions.h"

#include "config.h"

#ifdef USE_MYMATH
// #include "math/MathFunctions2.h"
#include "MathFunctions2.h"
#else
#include <cmath>
#endif

int main(int, char **)
{
	printf("1+2=%d\n", add(1, 2));

	double result;
#ifdef USE_MYMATH
	printf("Now we use our own Math library. \n");
	result = power(1.0, 2);
#else
	printf("Now we use the standard library. \n");
	result = pow(1.0, 2);
#endif
	printf("1.0^2=%lf\n", result);

	printf("VERSION: %d.%d.%d\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);

	cout << "Hello, world!\n";
	return 0;
}
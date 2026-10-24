// Behaviour test for GFExp (0x0066F9F0): the inline x87 exp expansion against known values.
#include <stdio.h>
#include <math.h>
#pragma intrinsic(exp)

double __stdcall GFExp(double x)
{
    return exp(x);
}

static int Near(double a, double b)
{
    double d = a - b;
    if (d < 0) d = -d;
    return d <= 1e-12 * (b < 0 ? -b : b) + 1e-15;
}

int main()
{
    if (GFExp(0.0) != 1.0) { printf("FAIL exp(0)\n"); return 1; }
    if (!Near(GFExp(1.0), 2.718281828459045)) { printf("FAIL exp(1)\n"); return 1; }
    if (!Near(GFExp(-2.0), 0.1353352832366127)) { printf("FAIL exp(-2)\n"); return 1; }
    if (!Near(GFExp(10.0), 22026.465794806718)) { printf("FAIL exp(10)\n"); return 1; }
    if (!Near(GFExp(0.6931471805599453), 2.0)) { printf("FAIL exp(ln2)\n"); return 1; }
    printf("PASS x87 exponential helper 0x0066F9F0\n");
    return 0;
}

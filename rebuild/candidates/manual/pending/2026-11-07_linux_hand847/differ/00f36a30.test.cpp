// Behaviour test for 0x00F36A30 CHeteroTerrainFractal::GetFractalHeight: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
static double Height(const double* noise, const double* exps, double octaves)
{
    double value = noise[0]; long i;
    for (i = 1; i < octaves; ++i) value += noise[i] * exps[i] * value;
    double rem = octaves - (long)octaves;
    if (rem != 0.0) value *= noise[i] * exps[i] * rem + 1.0;
    double h = (value + 0.48) * (2.0 / 3.0);
    return h < 0.0 ? 0.0 : h;
}
int main() {
    double n[4] = { 0.5, 0.5, 0.5, 0.5 }, e[4] = { 1, 1, 1, 1 };
    double h = Height(n, e, 2.0);
    int ok = h > 0.81 && h < 0.83 && Height(n, e, 1.0) > 0.65 && Height(n, e, 1.0) < 0.66;
    double neg[2] = { -2.0, 0.0 }; ok &= Height(neg, e, 1.0) == 0.0;
    std::printf(ok ? "00f36a30_TEST PASS\n" : "00f36a30_TEST FAIL\n");
    return ok ? 0 : 1;
}

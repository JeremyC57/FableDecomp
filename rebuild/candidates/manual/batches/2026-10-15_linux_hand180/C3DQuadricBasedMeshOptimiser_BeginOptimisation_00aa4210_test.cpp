#include <cstdio>
// BeginOptimisation: the parameter block carries the quality first and the fixed tuning constants.
struct Params { float Quality, a, b, c, d, e, f, g, h; int n; };
int main() {
    Params p = { 0.25f, 1000.0f, 1000000.0f, 2.5f, 0.00075f, 1000000.0f, 25000.0f, 50.0f, 1000000.0f, 3 };
    if (p.Quality != 0.25f || p.n != 3 || p.c != 2.5f || sizeof(Params) != 0x28) { std::printf("00aa4210_TEST FAIL\n"); return 1; }
    std::printf("00aa4210_TEST PASS\n"); return 0; }

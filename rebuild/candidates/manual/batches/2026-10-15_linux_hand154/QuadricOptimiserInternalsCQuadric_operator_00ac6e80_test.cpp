// Behaviour test for 0x00ac6e80 (CQuadric::operator+=): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Quadric { double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight; };
static void Add(Quadric& q, const Quadric& o) { double* a = &q.AA; const double* b = &o.AA; for (int i = 0; i < 11; ++i) a[i] += b[i]; }

int main()
{
    Quadric q, o; double* a = &q.AA; double* b = &o.AA; for (int i = 0; i < 11; ++i) { a[i] = i; b[i] = 10.0 * i; }
    Add(q, o);
    for (int i = 0; i < 11; ++i) CHECK(a[i] == 11.0 * i);
    if (g_fail) { std::printf("00ac6e80_TEST FAIL\n"); return 1; }
    std::printf("00ac6e80_TEST PASS\n");
    return 0;
}

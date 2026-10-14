#include <cstdio>
// Model of the rumble effect Initialise @ 0x005B33F4: zero two floats, forward seven arguments, return *this.
static long g_args[7];
struct Base { long state[6]; Base& Initialise(long a, long b, long c, long d, long e, long f, long g) { long v[7] = { a, b, c, d, e, f, g }; for (int i = 0; i < 7; ++i) g_args[i] = v[i]; return *this; } };
struct X : Base { float m_elapsed, m_intensity; X& Initialise(long a, long b, long c, long d, long e, long f, long g) { m_elapsed = 0.0f; m_intensity = 0.0f; Base::Initialise(a, b, c, d, e, f, g); return *this; } };
int main()
{
    X x; x.m_elapsed = 3; x.m_intensity = 4;
    X& r = x.Initialise(1, 2, 3, 4, 5, 6, 7);
    if (&r != &x || x.m_elapsed != 0.0f || x.m_intensity != 0.0f) { std::printf("005b33f4_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (g_args[i] != i + 1) { std::printf("005b33f4_TEST FAIL\n"); return 1; }
    std::printf("005b33f4_TEST PASS\n"); return 0;
}
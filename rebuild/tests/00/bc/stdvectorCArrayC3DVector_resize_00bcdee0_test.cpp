// behaviour test for 0x00BCDEE0 std::vector<CArray<C3DVector>>::resize(n) (model; the two-argument resize records its arguments)
#include <cstdio>
#include <cstring>
static unsigned g_n = 0; static unsigned char g_x[12]; static int g_calls = 0;
struct TElem { unsigned long w[3]; TElem() { w[0] = w[1] = w[2] = 0; } };
struct TVec {
    void resize(unsigned int n, const TElem& x) { ++g_calls; g_n = n; std::memcpy(g_x, &x, sizeof(g_x)); }
    void resize(unsigned int n) { resize(n, TElem()); }
};
int main()
{
    TVec v;
    std::memset(g_x, 0xCD, sizeof(g_x));
    v.resize(7u);
    int ok = g_calls == 1 && g_n == 7;
    for (int i = 0; i < 12; ++i) ok = ok && g_x[i] == 0;
    if (!ok) { std::printf("00bcdee0_TEST FAIL\n"); return 1; }
    std::printf("00bcdee0_TEST PASS\n");
    return 0;
}
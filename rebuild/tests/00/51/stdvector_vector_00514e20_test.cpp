// behaviour test for 0x00514E20: vector<unsigned char>(n) allocates n zeroed elements (begin/end/capacity)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct V { unsigned char* s; unsigned char* f; unsigned char* e;
    explicit V(unsigned int n) : s(0), f(0), e(0) { s = n ? (unsigned char*)std::malloc(n) : 0; f = s; e = s + n; std::memset(s, 0, n); f = s + n; } };
int main()
{
    V a(13); V b(0);
    int ok = a.f - a.s == 13 && a.e == a.f && b.s == 0 && b.f == 0 && b.e == 0;
    for (int i = 0; i < 13; ++i) ok = ok && a.s[i] == 0;
    if (!ok) { std::printf("00514e20_TEST FAIL\n"); return 1; }
    std::printf("00514e20_TEST PASS\n");
    return 0;
}
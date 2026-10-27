#include <cstdio>
#include <cstdlib>
// Behaviour model: an empty vector with room for n elements (no storage for n == 0).
struct V { char* s; char* f; char* e; V(unsigned int n, unsigned int sz) : s(0), f(0), e(0) { s = n ? (char*)std::malloc(n * sz) : 0; f = s; e = s + n * sz; } };
int main()
{
    V a(3, 0x38), b(0, 0x38);
    if (!a.s || a.f != a.s || a.e != a.s + 3 * 0x38 || b.s || b.e) { std::printf("00f25d90_TEST FAIL\n"); return 1; }
    std::free(a.s);
    std::printf("00f25d90_TEST PASS\n");
    return 0;
}
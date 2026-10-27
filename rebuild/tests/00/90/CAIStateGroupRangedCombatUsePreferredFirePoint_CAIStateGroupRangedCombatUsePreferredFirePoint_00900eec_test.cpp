#include <cstdio>
// Behaviour model: the pointer members start empty.
struct IP { void* p; IP() : p(0) { } };
struct Group { IP a, b; };
int main()
{
    Group g;
    if (g.a.p || g.b.p) { std::printf("00900eec_TEST FAIL\n"); return 1; }
    std::printf("00900eec_TEST PASS\n");
    return 0;
}
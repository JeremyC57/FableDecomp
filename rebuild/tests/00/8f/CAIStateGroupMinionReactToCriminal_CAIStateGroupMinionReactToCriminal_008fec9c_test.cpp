#include <cstdio>
// Behaviour model: the pointer members start empty.
struct IP { void* p; IP() : p(0) { } };
struct Group { IP a, b; };
int main()
{
    Group g;
    if (g.a.p || g.b.p) { std::printf("008fec9c_TEST FAIL\n"); return 1; }
    std::printf("008fec9c_TEST PASS\n");
    return 0;
}
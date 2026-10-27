// behaviour test for 0x007B8230 __uninitialized_fill_n<CIntelligentPointer<...> > (model: n pointers to x's item constructed)
#include <cstdio>
#include <new>
static int g_sets = 0;
struct P { void* item; P() : item(0) { } P(const P& o) : item(0) { item = o.item; ++g_sets; } };
int main()
{
    static double raw[8];
    int obj = 0; P x; x.item = &obj;
    P* first = (P*)raw; P* cur = first;
    for (unsigned int n = 3; n > 0; --n, ++cur) if (cur) new (cur) P(x);
    int ok = cur == first + 3 && first[2].item == &obj && g_sets == 3;
    if (!ok) { std::printf("007b8230_TEST FAIL\n"); return 1; }
    std::printf("007b8230_TEST PASS\n");
    return 0;
}
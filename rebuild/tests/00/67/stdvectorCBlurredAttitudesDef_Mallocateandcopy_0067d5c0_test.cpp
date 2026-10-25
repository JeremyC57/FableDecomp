// Behaviour test for 0x0067D5C0 vector<CBlurredAttitudesDef>::_M_allocate_and_copy(n, first, last): a block of n 0x48-byte slots, [first, last) copy-constructed into its front.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct Def {
    int id; unsigned char body[0x44];
    Def() : id(0) { }
    Def(const Def& o) : id(o.id + 100) { ++g_copies; }
};
static Def* allocate_and_copy(unsigned int n, const Def* first, const Def* last)
{
    Def* result = n != 0 ? (Def*)std::malloc(n * sizeof(Def)) : 0;
    Def* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Def(*first);
    return result;
}
int main()
{
    if (sizeof(Def) != 0x48) { std::printf("0067d5c0_TEST FAIL\n"); return 1; }
    Def src[3];
    for (int i = 0; i < 3; ++i) src[i].id = i + 1;
    Def* p = allocate_and_copy(5, src, src + 3);
    if (p == 0 || g_copies != 3 || p[0].id != 101 || p[1].id != 102 || p[2].id != 103) { std::printf("0067d5c0_TEST FAIL\n"); return 1; }
    std::free(p);
    if (allocate_and_copy(0, src, src) != 0 || g_copies != 3) { std::printf("0067d5c0_TEST FAIL\n"); return 1; }
    std::printf("0067d5c0_TEST PASS\n");
    return 0;
}
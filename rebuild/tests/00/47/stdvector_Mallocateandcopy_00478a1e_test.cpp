// behaviour test for 0x00478a1e std::vector<CAbilityTrackLevelDef >::_M_allocate_and_copy: n elements of 0x48 bytes are allocated (none for
// n == 0), [first, last) is copy-constructed into the front of the block through the element's copy constructor, and the block is returned
#include <cstdio>
#include <cstdlib>
#include <new>
static int gAllocs = 0;
static int gCopies = 0;
class Elem48 {                                         // test-only element of the retail size 0x48
public:
    int Words[18];
    Elem48() { for (int i = 0; i < 18; ++i) Words[i] = 0; }
    Elem48(const Elem48& other) { ++gCopies; for (int i = 0; i < 18; ++i) Words[i] = other.Words[i]; }
};
static Elem48* AllocateAndCopy(unsigned int n, const Elem48* first, const Elem48* last)
{
    Elem48* result = n != 0 ? (++gAllocs, (Elem48*)std::malloc(n * sizeof(Elem48))) : 0;
    Elem48* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) Elem48(*first);
    return result;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem48) != 0x48u) fails |= 1;
    Elem48 src[3];
    for (int k = 0; k < 3; ++k) for (int i = 0; i < 18; ++i) src[k].Words[i] = k * 100 + i;
    Elem48* p = AllocateAndCopy(5, src, src + 3);
    if (!p || gAllocs != 1 || gCopies != 3) fails |= 2;
    else if (p[0].Words[0] != 0 || p[2].Words[18 - 1] != 200 + 18 - 1 || p[1].Words[3] != 103) fails |= 4;
    std::free(p);
    Elem48* q = AllocateAndCopy(0, src, src);
    if (q || gAllocs != 1 || gCopies != 3) fails |= 8;
    if (fails) { std::printf("00478a1e_TEST FAIL %d\n", fails); return 1; }
    std::printf("00478a1e_TEST PASS\n");
    return 0;
}
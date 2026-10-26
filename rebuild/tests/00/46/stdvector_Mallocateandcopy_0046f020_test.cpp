// behaviour test for 0x0046f020 std::vector<CQuickAccessItemButtonGuiDef >::_M_allocate_and_copy: n elements of 0x50 bytes are allocated (none for
// n == 0), [first, last) is copy-constructed into the front of the block through the element's copy constructor, and the block is returned
#include <cstdio>
#include <cstdlib>
#include <new>
static int gAllocs = 0;
static int gCopies = 0;
class Elem50 {                                         // test-only element of the retail size 0x50
public:
    int Words[20];
    Elem50() { for (int i = 0; i < 20; ++i) Words[i] = 0; }
    Elem50(const Elem50& other) { ++gCopies; for (int i = 0; i < 20; ++i) Words[i] = other.Words[i]; }
};
static Elem50* AllocateAndCopy(unsigned int n, const Elem50* first, const Elem50* last)
{
    Elem50* result = n != 0 ? (++gAllocs, (Elem50*)std::malloc(n * sizeof(Elem50))) : 0;
    Elem50* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) Elem50(*first);
    return result;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem50) != 0x50u) fails |= 1;
    Elem50 src[3];
    for (int k = 0; k < 3; ++k) for (int i = 0; i < 20; ++i) src[k].Words[i] = k * 100 + i;
    Elem50* p = AllocateAndCopy(5, src, src + 3);
    if (!p || gAllocs != 1 || gCopies != 3) fails |= 2;
    else if (p[0].Words[0] != 0 || p[2].Words[20 - 1] != 200 + 20 - 1 || p[1].Words[3] != 103) fails |= 4;
    std::free(p);
    Elem50* q = AllocateAndCopy(0, src, src);
    if (q || gAllocs != 1 || gCopies != 3) fails |= 8;
    if (fails) { std::printf("0046f020_TEST FAIL %d\n", fails); return 1; }
    std::printf("0046f020_TEST PASS\n");
    return 0;
}
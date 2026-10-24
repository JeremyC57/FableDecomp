#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour copy of vector<CCardDef>::_M_allocate_and_copy: malloc n elements (null for n == 0), copy-construct [first, last).
static int g_copies = 0;
struct SCard {
    long CardName; long CardVal;
    SCard() : CardName(0), CardVal(0) { }
    SCard(const SCard& o) : CardName(o.CardName), CardVal(o.CardVal) { ++g_copies; }
};
static SCard* AllocateAndCopy(unsigned n, const SCard* first, const SCard* last)
{
    SCard* result = n != 0 ? static_cast<SCard*>(std::malloc(n * sizeof(SCard))) : 0;
    SCard* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SCard(*first);
    return result;
}
int main()
{
    bool ok = true;
    SCard src[3];
    for (int i = 0; i < 3; ++i) { src[i].CardName = 100 + i; src[i].CardVal = i * 7; }
    SCard* r = AllocateAndCopy(5, src, src + 3);
    if (r == 0 || g_copies != 3) ok = false;
    for (int i = 0; ok && i < 3; ++i) if (r[i].CardName != 100 + i || r[i].CardVal != i * 7) ok = false;
    std::free(r);
    if (AllocateAndCopy(0, src, src) != 0 || g_copies != 3) ok = false;
    if (!ok) { std::printf("004ec10f_TEST FAIL\n"); return 1; }
    std::printf("004ec10f_TEST PASS\n");
    return 0;
}
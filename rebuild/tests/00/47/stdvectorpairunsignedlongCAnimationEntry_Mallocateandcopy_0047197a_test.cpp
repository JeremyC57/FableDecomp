// Behaviour test for 0x0047197A vector<pair<unsigned long, CAnimationEntry> >::_M_allocate_and_copy: malloc n, key copied,
// entry copy-constructed (counted pointer AddRef) per element; n == 0 gives no block
#include <cstdio>
#include <cstdlib>
#include <new>
struct Counted { int refs; };
struct CAnimationEntry {
    unsigned int BankIndex; int AnimName; int GroupName; Counted* Components;
    CAnimationEntry(unsigned int b, Counted* c) : BankIndex(b), AnimName(1), GroupName(2), Components(c) { if (c) ++c->refs; }
    CAnimationEntry(const CAnimationEntry& o) : BankIndex(o.BankIndex), AnimName(o.AnimName), GroupName(o.GroupName), Components(o.Components) { if (Components) ++Components->refs; }
};
struct AnimEntry { unsigned int first; CAnimationEntry second; AnimEntry(unsigned int k, const CAnimationEntry& e) : first(k), second(e) {} };
static AnimEntry* AllocateAndCopy(unsigned int n, const AnimEntry* first, const AnimEntry* last)
{
    AnimEntry* result = n != 0 ? (AnimEntry*)std::malloc(n * sizeof(AnimEntry)) : 0;
    AnimEntry* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) AnimEntry(*first);
    return result;
}
int main()
{
    Counted comp = { 0 };
    AnimEntry src[2] = { AnimEntry(10, CAnimationEntry(5, &comp)), AnimEntry(20, CAnimationEntry(6, 0)) };
    int before = comp.refs;
    AnimEntry* block = AllocateAndCopy(3, src, src + 2);
    bool ok = block != 0 && block[0].first == 10 && block[0].second.BankIndex == 5 && block[0].second.Components == &comp
        && block[1].first == 20 && block[1].second.Components == 0 && comp.refs == before + 1;
    std::free(block);
    ok = ok && AllocateAndCopy(0, src, src) == 0;
    if (!ok) { std::printf("0047197a_TEST FAIL\n"); return 1; }
    std::printf("0047197a_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x004719CE __copy_ptrs<pair<unsigned long, CAnimationEntry>*>: key assigned, entry assigned through
// its operator= (counted pointer swap), n = last - first elements, returns result + n
#include <cstdio>
struct Counted { int refs; };
struct CAnimationEntry {
    unsigned int BankIndex; int AnimName; int GroupName; Counted* Components; static int assigns;
    CAnimationEntry& operator=(const CAnimationEntry& o)
    {
        ++assigns; BankIndex = o.BankIndex; AnimName = o.AnimName; GroupName = o.GroupName;
        if (Components != o.Components) { if (Components) --Components->refs; Components = o.Components; if (Components) ++Components->refs; }
        return *this;
    }
};
int CAnimationEntry::assigns = 0;
struct AnimEntry { unsigned int first; CAnimationEntry second; AnimEntry& operator=(const AnimEntry& o) { first = o.first; second = o.second; return *this; } };
static AnimEntry* CopyPtrs(AnimEntry* first, AnimEntry* last, AnimEntry* result)
{
    for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Counted a = { 1 }, b = { 1 };
    AnimEntry src[2], dst[3];
    for (int i = 0; i < 2; ++i) { src[i].first = 100 + i; src[i].second.BankIndex = i; src[i].second.AnimName = 7; src[i].second.GroupName = 8; src[i].second.Components = &a; }
    for (int i = 0; i < 3; ++i) { dst[i].first = 0; dst[i].second.BankIndex = 99; dst[i].second.AnimName = 0; dst[i].second.GroupName = 0; dst[i].second.Components = (i == 0) ? &b : 0; }
    AnimEntry* end = CopyPtrs(src, src + 2, dst);
    bool ok = end == dst + 2 && CAnimationEntry::assigns == 2 && dst[0].first == 100 && dst[1].first == 101
        && dst[1].second.BankIndex == 1 && dst[0].second.Components == &a && a.refs == 3 && b.refs == 0 && dst[2].second.BankIndex == 99;
    if (!ok) { std::printf("004719ce_TEST FAIL\n"); return 1; }
    std::printf("004719ce_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x0071FC20 __unguarded_partition over pair<CSkeletalMorphs::CEntry,float>.
#include <cstdio>
class CDefString { public: long TablePos; };
struct CSkeletalMorphsEntry { CDefString MorphName; short Priority; bool SupressesLowerMorphs; };
struct SMorphWeight {
    CSkeletalMorphsEntry first;
    float second;
    SMorphWeight() { }
    SMorphWeight(const SMorphWeight& other) : first(other.first), second(other.second) { }
};
class CIsSkeletalMorphLowerPriorityThan { public:
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};
static SMorphWeight* unguarded_partition(SMorphWeight* first, SMorphWeight* last, SMorphWeight pivot, CIsSkeletalMorphLowerPriorityThan comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        SMorphWeight tmp = *first;
        *first = *last;
        *last = tmp;
        ++first;
    }
}
int main()
{
    SMorphWeight a[8];
    short pr[8] = { 7, 1, 8, 3, 5, 2, 9, 4 };
    for (int i = 0; i < 8; ++i) {
        a[i].first.MorphName.TablePos = i;
        a[i].first.Priority = pr[i];
        a[i].first.SupressesLowerMorphs = false;
        a[i].second = (float)pr[i];
    }
    CIsSkeletalMorphLowerPriorityThan comp;
    comp.m_tag = 0;
    SMorphWeight pivot(a[4]);                // priority 5
    SMorphWeight* cut = unguarded_partition(a, a + 8, pivot, comp);
    long k = (long)(cut - a);
    if (k <= 0 || k >= 8) { std::printf("0071fc20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < k; ++i)
        if (a[i].first.Priority > 5) { std::printf("0071fc20_TEST FAIL\n"); return 1; }
    for (int i = (int)k; i < 8; ++i)
        if (a[i].first.Priority < 5) { std::printf("0071fc20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        if (a[i].second != (float)a[i].first.Priority) { std::printf("0071fc20_TEST FAIL\n"); return 1; }
    std::printf("0071fc20_TEST PASS\n");
    return 0;
}
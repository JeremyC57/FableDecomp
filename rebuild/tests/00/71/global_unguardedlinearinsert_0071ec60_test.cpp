// Behaviour test for 0x0071EC60 __unguarded_linear_insert over pair<CSkeletalMorphs::CEntry,float>.
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
static void unguarded_linear_insert(SMorphWeight* last, SMorphWeight val, CIsSkeletalMorphLowerPriorityThan comp)
{
    SMorphWeight* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
int main()
{
    SMorphWeight a[5];
    short pr[5] = { 1, 3, 5, 7, 4 };
    for (int i = 0; i < 5; ++i) {
        a[i].first.MorphName.TablePos = 100 + i;
        a[i].first.Priority = pr[i];
        a[i].first.SupressesLowerMorphs = (i & 1) != 0;
        a[i].second = (float)i;
    }
    CIsSkeletalMorphLowerPriorityThan comp;
    comp.m_tag = 0;
    SMorphWeight v(a[4]);
    unguarded_linear_insert(a + 4, v, comp);
    short want[5] = { 1, 3, 4, 5, 7 };
    long names[5] = { 100, 101, 104, 102, 103 };
    for (int i = 0; i < 5; ++i)
        if (a[i].first.Priority != want[i] || a[i].first.MorphName.TablePos != names[i]) {
            std::printf("0071ec60_TEST FAIL\n");
            return 1;
        }
    if (a[2].second != 4.0f) { std::printf("0071ec60_TEST FAIL\n"); return 1; }
    std::printf("0071ec60_TEST PASS\n");
    return 0;
}
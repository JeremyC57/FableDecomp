// Behaviour test for 0x0071F570 __unguarded_insertion_sort_aux over pair<CSkeletalMorphs::CEntry,float>.
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
static void unguarded_insertion_sort_aux(SMorphWeight* first, SMorphWeight* last, SMorphWeight*, CIsSkeletalMorphLowerPriorityThan comp)
{
    for (SMorphWeight* i = first; i != last; ++i)
        unguarded_linear_insert(i, SMorphWeight(*i), comp);
}
int main()
{
    SMorphWeight a[7];
    short pr[7] = { -5, 9, 2, 7, 2, -1, 4 };     // a[0] is the sentinel minimum
    for (int i = 0; i < 7; ++i) {
        a[i].first.MorphName.TablePos = i;
        a[i].first.Priority = pr[i];
        a[i].first.SupressesLowerMorphs = false;
        a[i].second = (float)(i * 2);
    }
    CIsSkeletalMorphLowerPriorityThan comp;
    comp.m_tag = 0;
    unguarded_insertion_sort_aux(a + 1, a + 7, (SMorphWeight*)0, comp);
    short want[7] = { -5, -1, 2, 2, 4, 7, 9 };
    long names[7] = { 0, 5, 2, 4, 6, 3, 1 };   // stable among equal keys
    for (int i = 0; i < 7; ++i)
        if (a[i].first.Priority != want[i] || a[i].first.MorphName.TablePos != names[i] || a[i].second != (float)(names[i] * 2)) {
            std::printf("0071f570_TEST FAIL\n");
            return 1;
        }
    std::printf("0071f570_TEST PASS\n");
    return 0;
}
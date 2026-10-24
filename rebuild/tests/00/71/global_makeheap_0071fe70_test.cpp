// Behaviour test for 0x0071FE70 __make_heap over pair<CSkeletalMorphs::CEntry,float> (with the SGI
// __adjust_heap / __push_heap of 0x0071F650 / 0x0071ECF0 modelled here).
#include <cstdio>
struct CDefString { long TablePos; };
struct CSkeletalMorphsEntry { CDefString MorphName; short Priority; bool SupressesLowerMorphs; };
struct SMorphWeight {
    CSkeletalMorphsEntry first;
    float second;
    SMorphWeight() { }
    SMorphWeight(const SMorphWeight& other) : first(other.first), second(other.second) { }
};
struct CIsSkeletalMorphLowerPriorityThan {
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};
typedef CIsSkeletalMorphLowerPriorityThan TComp;
static void push_heap_(SMorphWeight* first, int hole, int top, SMorphWeight value, TComp comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void adjust_heap(SMorphWeight* first, int hole, int len, SMorphWeight value, TComp comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1]))
            child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) {
        first[hole] = first[child - 1];
        hole = child - 1;
    }
    push_heap_(first, hole, top, value, comp);
}
static void make_heap_(SMorphWeight* first, SMorphWeight* last, TComp comp, SMorphWeight*, int*)
{
    if (last - first < 2)
        return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, SMorphWeight(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
int main()
{
    SMorphWeight a[9];
    short pr[9] = { 3, 8, -2, 11, 5, 5, 0, 7, 1 };
    int sum = 0;
    for (int i = 0; i < 9; ++i) {
        a[i].first.MorphName.TablePos = pr[i] * 10;
        a[i].first.Priority = pr[i];
        a[i].first.SupressesLowerMorphs = false;
        a[i].second = 0.5f * pr[i];
        sum += pr[i];
    }
    TComp comp;
    comp.m_tag = 0;
    make_heap_(a, a + 9, comp, (SMorphWeight*)0, (int*)0);
    int sum2 = 0;
    for (int i = 0; i < 9; ++i) {
        sum2 += a[i].first.Priority;
        if (a[i].first.MorphName.TablePos != a[i].first.Priority * 10 || a[i].second != 0.5f * a[i].first.Priority) { std::printf("0071fe70_TEST FAIL\n"); return 1; }
        if (i > 0 && a[(i - 1) / 2].first.Priority < a[i].first.Priority) { std::printf("0071fe70_TEST FAIL\n"); return 1; }
    }
    if (sum2 != sum || a[0].first.Priority != 11) { std::printf("0071fe70_TEST FAIL\n"); return 1; }
    std::printf("0071fe70_TEST PASS\n");
    return 0;
}
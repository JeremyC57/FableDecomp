// Behaviour test for 0x00721400 __partial_sort over pair<CSkeletalMorphs::CEntry,float>
// (__make_heap / __adjust_heap / sort_heap callees modelled with their SGI bodies).
#include <cstdio>
class CDefString { public: long TablePos; };
struct CSkeletalMorphsEntry { CDefString MorphName; short Priority; bool SupressesLowerMorphs; };
struct SMorphWeight {
    CSkeletalMorphsEntry first;
    float second;
    SMorphWeight() { }
    SMorphWeight(const SMorphWeight& other) : first(other.first), second(other.second) { }
};
struct TCompare {
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};
static void push_heap_(SMorphWeight* first, int hole, int top, SMorphWeight value, TCompare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void adjust_heap(SMorphWeight* first, int hole, int len, SMorphWeight value, TCompare comp)
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
static void make_heap_(SMorphWeight* first, SMorphWeight* last, TCompare comp)
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
static void sort_heap_(SMorphWeight* first, SMorphWeight* last, TCompare comp)
{
    while (last - first > 1) {
        --last;
        SMorphWeight v(*last);
        *last = *first;
        adjust_heap(first, 0, (int)(last - first), v, comp);
    }
}
static void partial_sort_(SMorphWeight* first, SMorphWeight* middle, SMorphWeight* last, SMorphWeight*, TCompare comp)
{
    make_heap_(first, middle, comp);
    for (SMorphWeight* i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            SMorphWeight value(*i);
            *i = *first;
            adjust_heap(first, 0, (int)(middle - first), value, comp);
        }
    sort_heap_(first, middle, comp);
}
int main()
{
    SMorphWeight a[10];
    short pr[10] = { 12, 4, 19, 7, 0, 15, 3, 8, 11, 1 };
    for (int i = 0; i < 10; ++i) {
        a[i].first.MorphName.TablePos = pr[i] + 1000;
        a[i].first.Priority = pr[i];
        a[i].first.SupressesLowerMorphs = false;
        a[i].second = 2.0f * pr[i];
    }
    TCompare comp;
    comp.m_tag = 0;
    partial_sort_(a, a + 4, a + 10, (SMorphWeight*)0, comp);
    short want[4] = { 0, 1, 3, 4 };
    for (int i = 0; i < 4; ++i)
        if (a[i].first.Priority != want[i] || a[i].first.MorphName.TablePos != want[i] + 1000 || a[i].second != 2.0f * want[i]) {
            std::printf("00721400_TEST FAIL\n");
            return 1;
        }
    for (int i = 4; i < 10; ++i)
        if (a[i].first.Priority < 4) { std::printf("00721400_TEST FAIL\n"); return 1; }
    std::printf("00721400_TEST PASS\n");
    return 0;
}
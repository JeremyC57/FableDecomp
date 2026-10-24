// Behaviour test for 006a3bf0 (STLport vector-map sort helpers over (long, CIntelligentPointer<CThing>) pairs; model copies).
#include <cstdio>
class CThing { public: int id; };
static int g_live = 0;                              // live intelligent pointers (ctor/dtor balance)
struct CIntelligentPointer {
    CThing* item;
    CIntelligentPointer() : item(0) { ++g_live; }
    CIntelligentPointer(const CIntelligentPointer& o) : item(o.item) { ++g_live; }
    ~CIntelligentPointer() { --g_live; }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { item = o.item; return *this; }
    CThing* GetPItem() const { return item; }
};
struct SThingEntry { long first; CIntelligentPointer second; };
struct CKeyPairCompareLess {
    bool operator()(const SThingEntry& a, const SThingEntry& b) const { return a.first < b.first; }
};
struct random_access_iterator_tag { };

// ---- model copies (the chunk's sources) ----
void iter_swap(SThingEntry* a, SThingEntry* b) { SThingEntry t = *a; *a = *b; *b = t; }
void unguarded_linear_insert(SThingEntry* last, SThingEntry val, CKeyPairCompareLess comp)
{
    SThingEntry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
SThingEntry* unguarded_partition(SThingEntry* first, SThingEntry* last, SThingEntry pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        iter_swap(first, last);
        ++first;
    }
}
// landed callee stand-ins (_Adjust_heap 0x006A3300 / __push_heap 0x006A2E20 / __copy_backward 0x006A2EB0)
void push_heap_(SThingEntry* first, int hole, int top, SThingEntry val, CKeyPairCompareLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
void adjust_heap(SThingEntry* first, int hole, int len, SThingEntry val, CKeyPairCompareLess comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap_(first, hole, top, val, comp);
}
SThingEntry* copy_backward_(SThingEntry* first, SThingEntry* last, SThingEntry* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) *--result = *--last;
    return result;
}
void pop_heap_(SThingEntry* first, SThingEntry* last, SThingEntry* result, SThingEntry val, CKeyPairCompareLess comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, int(last - first), val, comp);
}
void make_heap_(SThingEntry* first, SThingEntry* last, CKeyPairCompareLess comp, SThingEntry*, int*)
{
    if (last - first < 2) return;
    int len = last - first, parent = (len - 2) / 2;
    for (;;) { adjust_heap(first, parent, len, SThingEntry(*(first + parent)), comp); if (parent == 0) return; parent--; }
}
void sort_heap_(SThingEntry* first, SThingEntry* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) { --last; pop_heap_(first, last, last, SThingEntry(*last), comp, (int*)0); }
}
void linear_insert(SThingEntry* first, SThingEntry* last, SThingEntry val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) { copy_backward_(first, last, last + 1, random_access_iterator_tag(), (int*)0); *first = val; }
    else unguarded_linear_insert(last, val, comp);
}
void insertion_sort(SThingEntry* first, SThingEntry* last, CKeyPairCompareLess comp)
{
    if (first == last) return;
    for (SThingEntry* i = first + 1; i != last; ++i) linear_insert(first, i, *i, comp);
}
void partial_sort_(SThingEntry* first, SThingEntry* middle, SThingEntry* last, SThingEntry*, CKeyPairCompareLess comp)
{
    make_heap_(first, middle, comp, (SThingEntry*)0, (int*)0);
    for (SThingEntry* i = middle; i < last; ++i)
        if (comp(*i, *first)) pop_heap_(first, middle, i, SThingEntry(*i), comp, (int*)0);
    sort_heap_(first, middle, comp);
}

static CThing g_things[16];
static void Fill(SThingEntry* e, int n, const long* keys)
{
    for (int i = 0; i < n; ++i) { e[i].first = keys[i]; g_things[i].id = (int)keys[i]; e[i].second.item = &g_things[i]; }
}
static bool Sorted(const SThingEntry* e, int n)       // ascending keys, and every pointer still follows its key
{
    for (int i = 0; i < n; ++i) {
        if (i && e[i - 1].first > e[i].first) return false;
        if (!e[i].second.item || e[i].second.item->id != (int)e[i].first) return false;
    }
    return true;
}

int main()
{
    int fail = 0;
    {
        SThingEntry e[9]; long k[9] = {8, 3, 5, 1, 9, 2, 7, 4, 6}; Fill(e, 9, k);
        partial_sort_(e, e + 4, e + 9, 0, CKeyPairCompareLess());
        if (!Sorted(e, 4) || e[0].first != 1 || e[3].first != 4) fail = 1;
    }
    if (g_live != 0) fail = 1;
    if (fail) { std::printf("006a3bf0_TEST FAIL\n"); return 1; }
    std::printf("006a3bf0_TEST PASS\n");
    return 0;
}
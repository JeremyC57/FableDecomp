// Behaviour test for 0x00680B40: STLport sort pieces over 8-byte pair<EReactionSpeechType, long> voice entries
#include <cstdio>

struct Entry {
    int first;
    int second;
    Entry() : first(0), second(0) { }
    Entry(int a, int b) : first(a), second(b) { }
    Entry(const Entry& o) : first(o.first), second(o.second) { }
};
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static void UnguardedLinearInsert(Entry* last, Entry val, Less comp)
{
    Entry* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(Entry* first, Entry* last, Entry val, Less comp)
{
    if (comp(val, *first)) {
        Entry* r = last + 1; Entry* l = last;
        for (int n = (int)(last - first); n > 0; --n) *--r = *--l;
        *first = val;
    } else
        UnguardedLinearInsert(last, val, comp);
}
static Entry* Partition(Entry* first, Entry* last, Entry pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Entry t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void AdjustHeap(Entry* first, int hole, int len, Entry v, Less comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], v)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = v;
}
static void MakeHeap(Entry* first, Entry* last, Less comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first), parent = (len - 2) / 2;
    for (;;) { AdjustHeap(first, parent, len, Entry(first[parent]), comp); if (parent == 0) return; parent--; }
}
static void SortHeap(Entry* first, Entry* last, Less comp)
{
    while (last - first > 1) { --last; Entry v(*last); *last = *first; AdjustHeap(first, 0, (int)(last - first), v, comp); }
}
static void PartialSort(Entry* first, Entry* middle, Entry* last, Less comp)
{
    MakeHeap(first, middle, comp);
    for (Entry* i = middle; i < last; ++i)
        if (comp(*i, *first)) { Entry v(*i); *i = *first; AdjustHeap(first, 0, (int)(middle - first), v, comp); }
    SortHeap(first, middle, comp);
}
static const Entry& Median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static void IntroLoop(Entry* first, Entry* last, int depth, Less comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, comp); return; }
        --depth;
        Entry* cut = Partition(first, last, Entry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntroLoop(cut, last, depth, comp);
        last = cut;
    }
}
static bool Sorted(const Entry* a, int n) { for (int i = 1; i < n; ++i) if (a[i - 1].first > a[i].first) return false; return true; }
static bool PairsIntact(const Entry* a, int n) { for (int i = 0; i < n; ++i) if (a[i].second != a[i].first * 7 + 1) return false; return true; }
static void Fill(Entry* a, int n, unsigned int seed)
{
    for (int i = 0; i < n; ++i) { seed = seed * 1103515245u + 12345u; int k = (int)((seed >> 16) % 500) - 250; a[i] = Entry(k, k * 7 + 1); }
}

int main()
{
    Entry a[40]; Fill(a, 40, 77);
    Entry pivot(a[20]);
    Entry* cut = Partition(a, a + 40, pivot, Less());
    if (cut <= a || cut >= a + 40 || !PairsIntact(a, 40)) { std::printf("00680b40_TEST FAIL\n"); return 1; }
    for (Entry* p = a; p < cut; ++p) if (p->first > pivot.first) { std::printf("00680b40_TEST FAIL\n"); return 1; }
    for (Entry* p = cut; p < a + 40; ++p) if (p->first < pivot.first) { std::printf("00680b40_TEST FAIL\n"); return 1; }
    std::printf("00680b40_TEST PASS\n");
    return 0;
}
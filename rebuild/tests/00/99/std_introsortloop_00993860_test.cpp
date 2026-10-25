// Behaviour test for 0x00993860 std::__introsort_loop over (long key, record) entries (STLport introsort recursion).
#include <cstdio>

struct Entry { long first; int payload; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static void AdjustHeap(Entry* first, int hole, int len, Entry value, Less comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void MakeHeap(Entry* first, Entry* last, Less comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) return; parent--; }
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry val, Less comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(Entry* first, Entry* last, Less comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}
static int g_partialSorts = 0;
// the 0x009934C0 body
static void PartialSort(Entry* first, Entry* middle, Entry* last, Less comp)
{
    ++g_partialSorts;
    MakeHeap(first, middle, comp);
    for (Entry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, *i, comp);
    SortHeap(first, middle, comp);
}
static void Fill(Entry* e, int n) { for (int i = 0; i < n; ++i) { e[i].first = (long)((i * 53 + 17) % 97) - 40; e[i].payload = i; } }
static bool Sorted(const Entry* e, int n) { for (int i = 1; i < n; ++i) if (e[i - 1].first > e[i].first) return false; return true; }

static const Entry& Median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
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
// the 0x00993860 body
static void IntrosortLoop(Entry* first, Entry* last, Entry*, int depth, Less comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, comp); return; }
        --depth;
        Entry* cut = Partition(first, last, Entry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, (Entry*)0, depth, comp);
        last = cut;
    }
}
static void InsertionSort(Entry* first, Entry* last, Less comp)
{
    for (Entry* i = first + 1; i < last; ++i) {
        Entry v = *i; Entry* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

int main()
{
    Entry e[90];
    Less comp;
    Fill(e, 90);
    IntrosortLoop(e, e + 90, (Entry*)0, 12, comp);
    InsertionSort(e, e + 90, comp);
    if (!Sorted(e, 90) || g_partialSorts != 0) { std::printf("00993860_TEST FAIL\n"); return 1; }
    Fill(e, 90);
    IntrosortLoop(e, e + 90, (Entry*)0, 0, comp);   // no depth budget: heap sort of the whole range
    if (!Sorted(e, 90) || g_partialSorts != 1) { std::printf("00993860_TEST FAIL\n"); return 1; }
    std::printf("00993860_TEST PASS\n");
    return 0;
}
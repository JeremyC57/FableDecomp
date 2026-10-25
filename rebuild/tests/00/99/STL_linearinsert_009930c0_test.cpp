// Behaviour test for 0x009930C0 std::__linear_insert over the 36-byte (long key, sound-bank record) entries: __linear_insert: a new minimum shifts the range up and lands first; otherwise it is inserted in place.
// The record is modelled with a shared reference count (copies bump it, assignments move it, destruction drops it) so
// leaked or double-destroyed copies show up as a non-zero final count.
#include <cstdio>
#include <cstdlib>
#include <new>
struct Info { long RefCount; };
struct Rec {
    Info* info; int tag;
    Rec(int t, Info* i) : info(i), tag(t) { if (info) ++info->RefCount; }
    Rec(const Rec& o) : info(o.info), tag(o.tag) { if (info) ++info->RefCount; }
    Rec& operator=(const Rec& o) { if (o.info) ++o.info->RefCount; if (info) --info->RefCount; info = o.info; tag = o.tag; return *this; }
    ~Rec() { if (info) --info->RefCount; }
};
struct Entry { long first; Rec second; Entry(long k, int t, Info* i) : first(k), second(t, i) { } };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
struct false_type { false_type() { } };
static Entry* uninitialized_copy(Entry* first, Entry* last, Entry* result, const false_type&)
{
    Entry* cur = result;
    for (; first != last; ++first, ++cur) new (cur) Entry(*first);
    return cur;
}
static Entry* uninitialized_fill_n(Entry* first, unsigned int n, const Entry& x, const false_type&)
{
    Entry* cur = first;
    for (; n > 0; --n, ++cur) new (cur) Entry(x);
    return cur;
}
static Entry* copy_backward(Entry* first, Entry* last, Entry* result)
{
    for (long n = (long)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void unguarded_linear_insert(Entry* last, Entry val, Less comp)
{
    Entry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void linear_insert(Entry* first, Entry* last, Entry val, Less comp)
{
    if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; }
    else unguarded_linear_insert(last, val, comp);
}
static void insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (first == last) return;
    for (Entry* i = first + 1; i != last; ++i) linear_insert(first, i, Entry(*i), comp);
}
static void unguarded_insertion_sort_aux(Entry* first, Entry* last, Entry*, Less comp)
{
    for (Entry* i = first; i != last; ++i) unguarded_linear_insert(i, Entry(*i), comp);
}
static Entry* unguarded_partition(Entry* first, Entry* last, Entry pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Entry tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}
static void push_heap(Entry* first, int hole, int top, Entry val, Less comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void adjust_heap(Entry* first, int hole, int len, Entry val, Less comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap(first, hole, top, val, comp);
}
static void make_heap(Entry* first, Entry* last, Less comp, Entry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) { adjust_heap(first, parent, len, Entry(*(first + parent)), comp); if (parent == 0) return; parent--; }
}
static Info g_info = { 0 };
static Entry* MakeArray(const long* keys, int n)
{
    Entry* a = (Entry*)malloc(sizeof(Entry) * (n + 1));
    for (int i = 0; i < n; ++i) new (a + i) Entry(keys[i], (int)keys[i] * 10, &g_info);
    return a;
}
static void Destroy(Entry* a, int n) { for (int i = 0; i < n; ++i) a[i].~Entry(); free(a); }
static bool Sorted(const Entry* a, int n) { for (int i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) return false; for (int i = 0; i < n; ++i) if (a[i].second.tag != a[i].first * 10) return false; return true; }

int main()
{
    int fails = 0;
    long keys[5] = { 2, 4, 6, 8, 0 };
    Entry* a = MakeArray(keys, 5);
    linear_insert(a, a + 4, Entry(1, 10, &g_info), Less());
    long want1[5] = { 1, 2, 4, 6, 8 };
    for (int i = 0; i < 5; ++i) if (a[i].first != want1[i]) ++fails;
    a[4] = Entry(0, 0, &g_info);
    linear_insert(a, a + 4, Entry(5, 50, &g_info), Less());
    long want2[5] = { 1, 2, 4, 5, 6 };
    for (int i = 0; i < 5; ++i) if (a[i].first != want2[i]) ++fails;
    if (!Sorted(a, 5)) ++fails;
    Destroy(a, 5);
    if (g_info.RefCount != 0) ++fails;
    std::printf(fails ? "009930c0_TEST FAIL\n" : "009930c0_TEST PASS\n");
    return fails ? 1 : 0;
}
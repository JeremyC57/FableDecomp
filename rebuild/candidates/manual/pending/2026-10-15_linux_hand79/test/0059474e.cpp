// Behaviour test for __pop_heap_aux @ 0x0059474E (SGI): __pop_heap receives first, last - 1 twice and a
// copy of the back element (whose def reference is taken by the copy constructor); popping a max-heap
// moves the largest key to the back.
#include <cstdio>

struct CHeroAbilityDef { long refs; };

struct SAbilityEntry {
    long             first;
    CHeroAbilityDef* second;
    SAbilityEntry(long k, CHeroAbilityDef* d) : first(k), second(d) {}
    SAbilityEntry(const SAbilityEntry& other) : first(other.first), second(other.second) { if (second) ++second->refs; }
    ~SAbilityEntry() { if (second) --second->refs; }
    SAbilityEntry& operator=(const SAbilityEntry& other)
    {
        if (other.second) ++other.second->refs;
        if (second) --second->refs;
        first = other.first; second = other.second;
        return *this;
    }
};

struct SAbilityEntryLess {
    SAbilityEntryLess() { }
    bool operator()(const SAbilityEntry& a, const SAbilityEntry& b) const { return a.first < b.first; }
};

static SAbilityEntry* g_seenFirst = 0;
static SAbilityEntry* g_seenLast = 0;
static SAbilityEntry* g_seenResult = 0;
static long g_seenRefs = -1;

static void AdjustHeap(SAbilityEntry* first, long hole, long len, SAbilityEntry value, SAbilityEntryLess comp)
{
    long top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) --child;
        first[hole] = first[child];
        hole = child; child = 2 * child + 2;
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}

static void PopHeap(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntry* result,
                    SAbilityEntry value, SAbilityEntryLess comp, long* distance)
{
    g_seenFirst = first; g_seenLast = last; g_seenResult = result;
    g_seenRefs = value.second ? value.second->refs : -1;
    (void)distance;
    *result = *first;
    AdjustHeap(first, 0, (long)(last - first), value, comp);
}

static void PopHeapAux(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntry*, SAbilityEntryLess comp)
{
    PopHeap(first, last - 1, last - 1, SAbilityEntry(*(last - 1)), comp, (long*)0);
}

int main()
{
    CHeroAbilityDef d1 = { 1 }, d2 = { 1 }, d3 = { 1 }, d4 = { 1 };
    SAbilityEntry heap[4] = { SAbilityEntry(9, &d1), SAbilityEntry(5, &d2), SAbilityEntry(7, &d3), SAbilityEntry(1, &d4) };
    d1.refs = d2.refs = d3.refs = d4.refs = 2;            // known counts whatever copies the initialiser made
    PopHeapAux(heap, heap + 4, heap, SAbilityEntryLess());
    bool ok = g_seenFirst == heap && g_seenLast == heap + 3 && g_seenResult == heap + 3
           && g_seenRefs == 3;                             // array + initial + the by-value copy
    if (heap[3].first != 9 || heap[0].first != 7) ok = false;
    if (!ok) { std::printf("0059474e_TEST FAIL\n"); return 1; }
    std::printf("0059474e_TEST PASS\n");
    return 0;
}

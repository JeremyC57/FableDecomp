#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_nodes = 0;
static void* NodeAlloc(unsigned int size) { ++g_nodes; return malloc(size); }
static void NodeFree(void* p) { if (p) { --g_nodes; free(p); } }

struct TNode { TNode* next; TNode* prev; int value; };

class TList {                                           // stand-in for STLport list<C2DLineF>: a sentinel-based list
public:
    TNode* head;
    TList() { Init(); }
    TList(const TList& o) { Init(); Append(o); }
    ~TList() { Clear(); NodeFree(head); }
    TList& operator=(const TList& o) { if (this != &o) { Clear(); Append(o); } return *this; }
    void Init() { head = (TNode*)NodeAlloc(sizeof(TNode)); head->next = head; head->prev = head; }
    void Clear()
    {
        TNode* n = head->next;
        while (n != head) { TNode* t = n; n = n->next; NodeFree(t); }
        head->next = head;
        head->prev = head;
    }
    void PushBack(int v)
    {
        TNode* n = (TNode*)NodeAlloc(sizeof(TNode));
        n->value = v;
        n->next = head;
        n->prev = head->prev;
        head->prev->next = n;
        head->prev = n;
    }
    void Append(const TList& o) { for (TNode* n = o.head->next; n != o.head; n = n->next) PushBack(n->value); }
    int Count() const { int c = 0; for (TNode* n = head->next; n != head; n = n->next) ++c; return c; }
    int Front() const { return head->next->value; }
};

struct SKeyedList {                                     // pair<unsigned __int64, list<C2DLineF> >
    u64   first;
    TList second;
    SKeyedList() : first(0) { }
    SKeyedList(u64 key) : first(key) { second.PushBack((int)key * 10); second.PushBack((int)key * 10 + 1); }
};

struct CKeyPairCompareLess {
    bool operator()(const SKeyedList& a, const SKeyedList& b) const { return a.first < b.first; }
};

static bool Consistent(const SKeyedList& e) { return e.second.Count() == 2 && e.second.Front() == (int)e.first * 10; }

static void Fill(SKeyedList* a, const int* keys, int n) { for (int k = 0; k < n; ++k) a[k] = SKeyedList((u64)keys[k]); }

static bool AllConsistent(const SKeyedList* a, int n) { for (int k = 0; k < n; ++k) if (!Consistent(a[k])) return false; return true; }

static u64 KeySum(const SKeyedList* a, int n) { u64 s = 0; for (int k = 0; k < n; ++k) s += a[k].first * a[k].first + 7; return s; }

static void PushHeap(SKeyedList* first, int holeIndex, int topIndex, SKeyedList value, CKeyPairCompareLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

// SGI __adjust_heap (0x00A82BE0 in retail)
static void __fastcall AdjustHeap(SKeyedList* first, int holeIndex, int len, SKeyedList value, CKeyPairCompareLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    PushHeap(first, holeIndex, topIndex, value, comp);
}

static bool IsHeap(const SKeyedList* a, int n)
{
    CKeyPairCompareLess comp;
    for (int k = 1; k < n; ++k)
        if (comp(a[(k - 1) / 2], a[k]))
            return false;
    return true;
}

static bool IsSorted(const SKeyedList* a, int n)
{
    for (int k = 1; k < n; ++k)
        if (a[k].first < a[k - 1].first)
            return false;
    return true;
}

static void __fastcall MakeHeap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp, SKeyedList*, int*)
{
    if (last - first < 2)
        return;
    int len = int(last - first);
    for (int parent = (len - 2) / 2; ; --parent) {
        AdjustHeap(first, parent, len, SKeyedList(*(first + parent)), comp);
        if (parent == 0)
            return;
    }
}
static void __fastcall PopHeap(SKeyedList* first, SKeyedList* last, SKeyedList* result, SKeyedList value,
                               CKeyPairCompareLess comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), value, comp);
}
static void __fastcall SortHeap(SKeyedList* first, SKeyedList* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) {
        --last;
        PopHeap(first, last, last, SKeyedList(*last), comp, (int*)0);
    }
}

// the row: SGI __partial_sort(first, middle, last, value_type*, comp)
static void __fastcall PartialSort(SKeyedList* first, SKeyedList* middle, SKeyedList* last, SKeyedList*, CKeyPairCompareLess comp)
{
    MakeHeap(first, middle, comp, (SKeyedList*)0, (int*)0);
    for (SKeyedList* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, SKeyedList(*i), comp, (int*)0);
    SortHeap(first, middle, comp);
}

int main()
{
    static const int keys[10] = { 9, 4, 7, 1, 8, 2, 6, 3, 10, 5 };
    {
        SKeyedList a[10];
        Fill(a, keys, 10);
        int nodes = g_nodes;
        u64 sum = KeySum(a, 10);
        PartialSort(a, a + 4, a + 10, (SKeyedList*)0, CKeyPairCompareLess());
        Check(a[0].first == 1 && a[1].first == 2 && a[2].first == 3 && a[3].first == 4);
        for (int k = 4; k < 10; ++k)
            Check(a[k].first > 4);
        Check(AllConsistent(a, 10));
        Check(KeySum(a, 10) == sum);
        Check(g_nodes == nodes);
    }
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a84a60_TEST FAIL\n");
        return 1;
    }
    std::printf("00a84a60_TEST PASS\n");
    return 0;
}
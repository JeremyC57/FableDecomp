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

static void __fastcall IterSwap(SKeyedList* a, SKeyedList* b)   // 0x00A82B10
{
    SKeyedList tmp = *a;
    *a = *b;
    *b = tmp;
}

// the row: SGI __unguarded_partition(first, last, pivot, comp)
static SKeyedList* __fastcall UnguardedPartition(SKeyedList* first, SKeyedList* last, SKeyedList pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        IterSwap(first, last);
        ++first;
    }
}

int main()
{
    static const int keys[9] = { 4, 8, 1, 9, 3, 7, 2, 6, 5 };
    {
        SKeyedList a[9];
        Fill(a, keys, 9);
        int nodes = g_nodes;
        u64 sum = KeySum(a, 9);
        SKeyedList* cut = UnguardedPartition(a, a + 9, SKeyedList(5), CKeyPairCompareLess());
        Check(cut > a && cut < a + 9);
        for (SKeyedList* p = a; p != cut; ++p)
            Check(p->first <= 5);
        for (SKeyedList* q = cut; q != a + 9; ++q)
            Check(q->first >= 5);
        Check(AllConsistent(a, 9));
        Check(KeySum(a, 9) == sum);
        Check(g_nodes == nodes);
    }
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a83cc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a83cc0_TEST PASS\n");
    return 0;
}
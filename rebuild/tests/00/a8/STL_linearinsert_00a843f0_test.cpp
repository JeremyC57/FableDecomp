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

static SKeyedList* __fastcall CopyBackward(SKeyedList* first, SKeyedList* last, SKeyedList* result)   // 0x00A821C0
{
    while (first != last)
        *--result = *--last;
    return result;
}
static void __fastcall UnguardedLinearInsert(SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp)   // 0x00A81E20
{
    SKeyedList* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

// the row: SGI __linear_insert(first, last, value, comp)
static void __fastcall LinearInsert(SKeyedList* first, SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp)
{
    if (comp(value, *first)) {
        CopyBackward(first, last, last + 1);
        *first = value;
    } else
        UnguardedLinearInsert(last, value, comp);
}

int main()
{
    static const int mid[5] = { 1, 3, 5, 7, 4 };
    static const int low[5] = { 2, 3, 5, 7, 0 };
    {
        SKeyedList a[5];
        Fill(a, mid, 5);
        int nodes = g_nodes;
        LinearInsert(a, a + 4, SKeyedList(a[4]), CKeyPairCompareLess());
        Check(a[0].first == 1 && a[1].first == 3 && a[2].first == 4 && a[3].first == 5 && a[4].first == 7);
        Check(AllConsistent(a, 5));
        Check(g_nodes == nodes);
        SKeyedList b[5];
        Fill(b, low, 5);
        nodes = g_nodes;
        LinearInsert(b, b + 4, SKeyedList(b[4]), CKeyPairCompareLess());
        Check(b[0].first == 0 && b[1].first == 2 && b[2].first == 3 && b[3].first == 5 && b[4].first == 7);
        Check(AllConsistent(b, 5));
        Check(g_nodes == nodes);
    }
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a843f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a843f0_TEST PASS\n");
    return 0;
}
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

struct SKeyedVector {                                   // STLport vector<SKeyedList>: start / finish / end of storage
    SKeyedList* start;
    SKeyedList* finish;
    SKeyedList* eos;
};

// the row: vector<SKeyedList>::_M_insert_overflow(pos, x, __false_type, fill_len, atend)
static void InsertOverflow(SKeyedVector& v, SKeyedList* pos, const SKeyedList& x, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = (unsigned int)(v.finish - v.start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    SKeyedList* newStart = len ? (SKeyedList*)malloc(len * sizeof(SKeyedList)) : 0;
    SKeyedList* newFinish = newStart;
    for (SKeyedList* p = v.start; p != pos; ++p, ++newFinish)
        new (newFinish) SKeyedList(*p);
    if (fillLen == 1) {
        new (newFinish) SKeyedList(x);
        ++newFinish;
    } else
        for (unsigned int n = fillLen; n > 0; --n, ++newFinish)
            new (newFinish) SKeyedList(x);
    if (!atEnd)
        for (SKeyedList* q = pos; q != v.finish; ++q, ++newFinish)
            new (newFinish) SKeyedList(*q);
    for (SKeyedList* d = v.start; d != v.finish; ++d)
        d->~SKeyedList();
    if (v.start)
        free(v.start);
    v.start = newStart;
    v.finish = newFinish;
    v.eos = newStart + len;
}

int main()
{
    {
        SKeyedVector v = { 0, 0, 0 };
        SKeyedList one(1), two(2), three(3), nine(9);
        InsertOverflow(v, v.finish, one, 1, true);
        Check(v.finish - v.start == 1 && v.eos - v.start == 1);
        InsertOverflow(v, v.finish, two, 1, true);
        Check(v.finish - v.start == 2 && v.eos - v.start == 2);
        InsertOverflow(v, v.finish, three, 1, true);
        Check(v.finish - v.start == 3 && v.eos - v.start == 4);
        InsertOverflow(v, v.start + 1, nine, 2, false);      // [1, 9, 9, 2, 3], capacity 3 + 3
        Check(v.finish - v.start == 5 && v.eos - v.start == 6);
        Check(v.start[0].first == 1 && v.start[1].first == 9 && v.start[2].first == 9 && v.start[3].first == 2 && v.start[4].first == 3);
        Check(AllConsistent(v.start, 5));
        for (SKeyedList* d = v.start; d != v.finish; ++d)
            d->~SKeyedList();
        free(v.start);
    }
    Check(g_nodes == 0);

    if (g_fail) {
        std::printf("00a84c20_TEST FAIL\n");
        return 1;
    }
    std::printf("00a84c20_TEST PASS\n");
    return 0;
}
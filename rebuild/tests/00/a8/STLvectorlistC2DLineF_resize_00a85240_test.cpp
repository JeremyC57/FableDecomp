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

static int g_blocks = 0;
template <class T> class TVec {                         // STLport vector<T>: start / finish / end of storage
public:
    T* start;
    T* finish;
    T* eos;
    TVec() : start(0), finish(0), eos(0) { }
    ~TVec() { Destroy(start, finish); Free(start); }
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    T& operator[](unsigned int i) { return start[i]; }
    const T& operator[](unsigned int i) const { return start[i]; }
    static void Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
    static void Free(T* p) { if (p) { --g_blocks; free(p); } }
    void InsertOverflow(T* pos, const T& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        T* newStart = (T*)malloc(len * sizeof(T));
        ++g_blocks;
        T* newFinish = newStart;
        for (T* p = start; p != pos; ++p, ++newFinish)
            new (newFinish) T(*p);
        for (unsigned int n = fillLen; n > 0; --n, ++newFinish)
            new (newFinish) T(x);
        if (!atEnd)
            for (T* q = pos; q != finish; ++q, ++newFinish)
                new (newFinish) T(*q);
        Destroy(start, finish);
        Free(start);
        start = newStart;
        finish = newFinish;
        eos = newStart + len;
    }
    void push_back(const T& x)
    {
        if (finish != eos) {
            new (finish) T(x);
            ++finish;
        } else
            InsertOverflow(finish, x, 1, true);
    }
    void swap(TVec& o)
    {
        T* t = start; start = o.start; o.start = t;
        t = finish; finish = o.finish; o.finish = t;
        t = eos; eos = o.eos; o.eos = t;
    }
private:
    TVec(const TVec&);
    TVec& operator=(const TVec&);
};

// vector<list<C2DLineF> >::erase(first, last): copy the tail down, destroy the leftovers, return first (0x00A84090)
static TList* EraseLists(TVec<TList>& v, TList* first, TList* last)
{
    TList* i = first;
    for (TList* s = last; s != v.finish; ++s, ++i)
        *i = *s;
    for (TList* d = i; d != v.finish; ++d)
        d->~TList();
    v.finish = i;
    return first;
}
static void FillLists(TVec<TList>& v, int count)
{
    for (int k = 0; k < count; ++k) {
        TList l;
        for (int m = 0; m <= k; ++m)
            l.PushBack(k * 100 + m);
        v.push_back(l);
    }
}
static bool ListIs(const TList& l, int k) { return l.Count() == k + 1 && l.Front() == k * 100; }

// vector<list<C2DLineF> >::_M_fill_insert(pos, n, x) at the end (0x00A84690)
static void FillInsertLists(TVec<TList>& v, unsigned int n, const TList& x)
{
    for (; n > 0; --n)
        v.push_back(x);
}
// the row: resize(n) = resize(n, list()) -- erase the tail or append copies of an empty list
static void ResizeLists(TVec<TList>& v, unsigned int newSize)
{
    TList x;
    if (newSize < v.size())
        EraseLists(v, v.start + newSize, v.finish);
    else
        FillInsertLists(v, newSize - v.size(), x);
}

int main()
{
    {
        TVec<TList> v;
        FillLists(v, 2);
        ResizeLists(v, 5);
        Check(v.size() == 5);
        Check(ListIs(v[0], 0) && ListIs(v[1], 1));
        Check(v[2].Count() == 0 && v[3].Count() == 0 && v[4].Count() == 0);
        ResizeLists(v, 5);
        Check(v.size() == 5);
        ResizeLists(v, 1);
        Check(v.size() == 1 && ListIs(v[0], 0));
        Check(g_nodes == 1 + 1);
        ResizeLists(v, 0);
        Check(v.size() == 0 && g_nodes == 0);
    }
    Check(g_nodes == 0 && g_blocks == 0);

    if (g_fail) {
        std::printf("00a85240_TEST FAIL\n");
        return 1;
    }
    std::printf("00a85240_TEST PASS\n");
    return 0;
}
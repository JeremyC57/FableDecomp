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

struct SNavigationLayer {                               // CNavQuadTree::CNavigationLayer stand-in (member-wise copy)
    int   NodeCount;
    TList LineLists;
    u64   SwitchKey;
    bool  Dirty;
};
// the row: vector<CNavigationLayer>::push_back -- construct in place, else grow through _M_insert_overflow
static void PushLayer(TVec<SNavigationLayer>& v, const SNavigationLayer& x) { v.push_back(x); }

int main()
{
    static const unsigned int caps[5] = { 1, 2, 4, 4, 8 };
    {
        TVec<SNavigationLayer> v;
        for (int k = 0; k < 5; ++k) {
            SNavigationLayer layer;
            layer.NodeCount = k;
            layer.SwitchKey = (u64)k << 40;
            layer.Dirty = (k & 1) != 0;
            for (int m = 0; m < k; ++m)
                layer.LineLists.PushBack(k * 10 + m);
            PushLayer(v, layer);
            Check(v.size() == (unsigned int)(k + 1) && v.capacity() == caps[k]);
        }
        for (unsigned int j = 0; j < 5; ++j) {
            Check(v[j].NodeCount == (int)j && v[j].SwitchKey == ((u64)j << 40));
            Check(v[j].Dirty == ((j & 1) != 0));
            Check(v[j].LineLists.Count() == (int)j);
        }
        Check(v[3].LineLists.Front() == 30);
    }
    Check(g_nodes == 0 && g_blocks == 0);

    if (g_fail) {
        std::printf("00a85e40_TEST FAIL\n");
        return 1;
    }
    std::printf("00a85e40_TEST PASS\n");
    return 0;
}
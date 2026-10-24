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

struct CANavQuadTreeNode { int id; };
struct SNodePair {                                      // pair<unsigned __int64, CANavQuadTreeNode*>
    u64                first;
    CANavQuadTreeNode* second;
    SNodePair(const u64& a, CANavQuadTreeNode* const& b) : first(a), second(b) { }
};
class CNodeMap : public TVec<SNodePair> {               // CVectorMap<unsigned __int64, CANavQuadTreeNode*, ...>
public:
    bool Compare;
    bool Dirty;
    CNodeMap() : Dirty(false) { }
    // the row: append the pair (unsorted) and mark the map dirty
    void Insert(const u64& key, CANavQuadTreeNode* const& value)
    {
        push_back(SNodePair(key, value));
        Dirty = true;
    }
};

int main()
{
    static CANavQuadTreeNode nodes[5];
    static const unsigned int caps[5] = { 1, 2, 4, 4, 8 };
    {
        CNodeMap m;
        for (int k = 0; k < 5; ++k) {
            m.Dirty = false;
            u64 key = ((u64)(5 - k) << 32) | (u64)k;
            CANavQuadTreeNode* node = &nodes[k];
            m.Insert(key, node);
            Check(m.Dirty);
            Check(m.size() == (unsigned int)(k + 1) && m.capacity() == caps[k]);
        }
        for (unsigned int j = 0; j < 5; ++j) {
            Check(m[j].first == ((((u64)(5 - j)) << 32) | (u64)j));
            Check(m[j].second == &nodes[j]);
        }
        Check(g_blocks == 1);
    }
    Check(g_blocks == 0);

    if (g_fail) {
        std::printf("00a837e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a837e0_TEST PASS\n");
    return 0;
}
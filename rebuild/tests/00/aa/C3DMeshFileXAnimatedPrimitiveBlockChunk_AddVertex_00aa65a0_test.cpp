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

struct C3DIndexedBoneWeight { unsigned long BoneIndex; unsigned long Weight; };
class C3DMeshFileXAnimatedPrimitiveBlockChunk {
public:
    unsigned long               BonesPerVertex;
    TVec<unsigned long>         VertexList;
    TVec<C3DIndexedBoneWeight>  BoneWeights;
    // the row: record the vertex, then append each of its bone weights
    void AddVertex(unsigned long vertex, const TVec<C3DIndexedBoneWeight>& weights)
    {
        VertexList.push_back(vertex);
        for (unsigned long i = 0; i < weights.size(); ++i)
            BoneWeights.push_back(weights[i]);
    }
};
int main()
{
    {
        C3DMeshFileXAnimatedPrimitiveBlockChunk c;
        TVec<C3DIndexedBoneWeight> w;
        for (unsigned long k = 0; k < 3; ++k) {
            C3DIndexedBoneWeight bw = { k + 1, 100 * (k + 1) };
            w.push_back(bw);
        }
        c.AddVertex(7, w);
        TVec<C3DIndexedBoneWeight> none;
        c.AddVertex(9, none);
        c.AddVertex(11, w);
        Check(c.VertexList.size() == 3 && c.VertexList[0] == 7 && c.VertexList[1] == 9 && c.VertexList[2] == 11);
        Check(c.BoneWeights.size() == 6);
        for (unsigned int j = 0; j < 6; ++j)
            Check(c.BoneWeights[j].BoneIndex == j % 3 + 1 && c.BoneWeights[j].Weight == 100 * (j % 3 + 1));
    }
    Check(g_blocks == 0);

    if (g_fail) {
        std::printf("00aa65a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00aa65a0_TEST PASS\n");
    return 0;
}
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

struct C3DVector { float X, Y, Z; };
class C3DMeshFileXUVTangentChunk {
public:
    TVec<C3DVector> UTangents;
    TVec<C3DVector> VTangents;
    // the row: append one tangent pair
    void AddTangent(const C3DVector& uTangent, const C3DVector& vTangent)
    {
        UTangents.push_back(uTangent);
        VTangents.push_back(vTangent);
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    {
        C3DMeshFileXUVTangentChunk c;
        for (int k = 0; k < 3; ++k) {
            C3DVector u = { (float)k, 0.0f, 1.0f };
            C3DVector v = { 0.0f, (float)k, -1.0f };
            c.AddTangent(u, v);
        }
        Check(c.UTangents.size() == 3 && c.VTangents.size() == 3);
        for (unsigned int j = 0; j < 3; ++j) {
            Check(Near(c.UTangents[j].X, (float)j) && Near(c.UTangents[j].Z, 1.0f));
            Check(Near(c.VTangents[j].Y, (float)j) && Near(c.VTangents[j].Z, -1.0f));
        }
        c.AddTangent(c.UTangents[0], c.UTangents[2]);
        Check(c.UTangents.size() == 4 && Near(c.UTangents[3].X, 0.0f) && Near(c.VTangents[3].X, 2.0f));
    }
    Check(g_blocks == 0);

    if (g_fail) {
        std::printf("00aa64e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00aa64e0_TEST PASS\n");
    return 0;
}
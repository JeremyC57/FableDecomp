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

template <class T> struct SPodVector { T* start; T* finish; T* eos; };
static int g_allocs = 0;
static void* Allocate(unsigned int n, unsigned int size) { if (!n) return 0; ++g_allocs; return malloc(n * size); }

// the row: vector<unsigned short>::vector(const vector&) -- exact-size block, memmove of the elements
static void CopyConstruct(SPodVector<unsigned short>& v, const SPodVector<unsigned short>& x)
{
    unsigned int n = (unsigned int)(x.finish - x.start);
    v.start = (unsigned short*)Allocate(n, sizeof(unsigned short));
    v.finish = v.start;
    v.eos = v.start + n;
    unsigned int bytes = (unsigned int)((const char*)x.finish - (const char*)x.start);
    v.finish = (x.finish == x.start) ? v.start : (unsigned short*)((char*)memmove(v.start, x.start, bytes) + bytes);
}
int main()
{
    unsigned short src[7] = { 1, 2, 3, 400, 5, 60000, 7 };
    SPodVector<unsigned short> x = { src, src + 7, src + 7 };
    SPodVector<unsigned short> v;
    CopyConstruct(v, x);
    Check(v.start != src && v.finish - v.start == 7 && v.eos - v.start == 7);
    for (int k = 0; k < 7; ++k)
        Check(v.start[k] == src[k]);
    free(v.start);
    SPodVector<unsigned short> e = { 0, 0, 0 };
    SPodVector<unsigned short> c;
    CopyConstruct(c, e);
    Check(c.start == 0 && c.finish == 0 && c.eos == 0 && g_allocs == 1);

    if (g_fail) {
        std::printf("00a87090_TEST FAIL\n");
        return 1;
    }
    std::printf("00a87090_TEST PASS\n");
    return 0;
}
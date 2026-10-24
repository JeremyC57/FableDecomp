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

// the row: vector<unsigned short>::vector(n) -- allocate n, zero-fill, size == capacity == n
static void ConstructN(SPodVector<unsigned short>& v, unsigned int n)
{
    v.start = (unsigned short*)Allocate(n, sizeof(unsigned short));
    v.finish = v.start;
    v.eos = v.start + n;
    unsigned short* cur = v.start;
    for (unsigned int k = n; k > 0; --k, ++cur)
        *cur = 0;
    v.finish = cur;
}
int main()
{
    SPodVector<unsigned short> v;
    ConstructN(v, 9);
    Check(v.finish - v.start == 9 && v.eos - v.start == 9 && g_allocs == 1);
    for (int k = 0; k < 9; ++k)
        Check(v.start[k] == 0);
    free(v.start);
    SPodVector<unsigned short> e;
    ConstructN(e, 0);
    Check(e.start == 0 && e.finish == 0 && e.eos == 0 && g_allocs == 1);

    if (g_fail) {
        std::printf("00a8d750_TEST FAIL\n");
        return 1;
    }
    std::printf("00a8d750_TEST PASS\n");
    return 0;
}
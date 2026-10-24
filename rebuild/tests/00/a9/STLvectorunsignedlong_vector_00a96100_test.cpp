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

// the row: vector<unsigned long>::vector(n) -- allocate n, zero-fill, size == capacity == n
static void ConstructN(SPodVector<unsigned long>& v, unsigned int n)
{
    v.start = (unsigned long*)Allocate(n, sizeof(unsigned long));
    v.finish = v.start;
    v.eos = v.start + n;
    unsigned long* cur = v.start;
    for (unsigned int k = n; k > 0; --k, ++cur)
        *cur = 0;
    v.finish = cur;
}
int main()
{
    SPodVector<unsigned long> v;
    ConstructN(v, 17);
    Check(v.finish - v.start == 17 && v.eos - v.start == 17 && g_allocs == 1);
    for (int k = 0; k < 17; ++k)
        Check(v.start[k] == 0);
    free(v.start);
    SPodVector<unsigned long> e;
    ConstructN(e, 0);
    Check(e.start == 0 && e.finish == 0 && e.eos == 0 && g_allocs == 1);

    if (g_fail) {
        std::printf("00a96100_TEST FAIL\n");
        return 1;
    }
    std::printf("00a96100_TEST PASS\n");
    return 0;
}
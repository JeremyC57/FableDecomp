// Behaviour test of std::copy_backward(CCharString* first, CCharString* last, CCharString* result) (retail 0x005E0FE0)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

class CCharString { public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

__forceinline CCharString* __copy_backward(CCharString* first, CCharString* last, CCharString* result,
                                    const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

CCharString* __fastcall copy_backward(CCharString* first, CCharString* last, CCharString* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

// ---- test bodies of the retail callees ----
static int g_assigns = 0;
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
int main()
{
    static int tags[5];
    CCharString a[5];
    for (int i = 0; i < 5; ++i)
        a[i].PStringData = &tags[i];
    CCharString* r = copy_backward(a, a + 3, a + 5);        // overlapping move up by two
    bool ok = r == a + 2 && g_assigns == 3 && a[0].PStringData == &tags[0] && a[1].PStringData == &tags[1]
           && a[2].PStringData == &tags[0] && a[3].PStringData == &tags[1] && a[4].PStringData == &tags[2];
    r = copy_backward(a, a, a + 5);                         // empty range
    ok = ok && r == a + 5 && g_assigns == 3;
    if (!ok) { std::printf("005e0fe0_TEST FAIL\n"); return 1; }
    std::printf("005e0fe0_TEST PASS\n");
    return 0;
}
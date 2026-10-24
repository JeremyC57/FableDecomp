// Behaviour test for 0x008ED510 Copy_ToNameInserter4_008ed510: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SDefEntry4 {                          // 4 bytes
    long          DefIndex;                  // +0x00
};

struct CCharStringVector;

struct CDefNameInserter4 {                   // output iterator over a vector<CCharString>, 4 bytes
    CCharStringVector* Container;            // +0x00
    explicit CDefNameInserter4(CCharStringVector& container) : Container(&container) { }
    CDefNameInserter4& operator=(const SDefEntry4& entry);     // 0x008ED440
    CDefNameInserter4& operator*() { return *this; }
    CDefNameInserter4& operator++() { return *this; }
};

CDefNameInserter4 __fastcall Copy_ToNameInserter4_008ed510(const SDefEntry4* first, const SDefEntry4* last, CDefNameInserter4 result)
{
    for (; first != last; ++result, ++first)
        *result = *first;
    return result;
}

struct CCharStringVector { long names[8]; int count; };
CDefNameInserter4& CDefNameInserter4::operator=(const SDefEntry4& entry) { Container->names[Container->count++] = entry.DefIndex; return *this; }

typedef SDefEntry4 T;
#define KEY(E) (E).DefIndex
#define INIT(E) (void)0
#define CALL(f, l, r) Copy_ToNameInserter4_008ed510(f, l, r, tag, 0)

int main()
{
    int i;
    
    SDefEntry4 e[3];
    for (i = 0; i < 3; ++i) e[i].DefIndex = 10 * (i + 1);
    CCharStringVector v;
    v.count = 0;
    CDefNameInserter4 it(v);
    CDefNameInserter4 out = Copy_ToNameInserter4_008ed510(e, e + 3, it);
    CHECK(out.Container == &v && v.count == 3 && v.names[0] == 10 && v.names[1] == 20 && v.names[2] == 30);
    out = Copy_ToNameInserter4_008ed510(e, e, it);
    CHECK(out.Container == &v && v.count == 3);
    if (g_fail) { std::printf("008ed510_TEST FAIL\n"); return 1; }
    std::printf("008ed510_TEST PASS\n");
    return 0;
}
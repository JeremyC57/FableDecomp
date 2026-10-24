// Behaviour test for 0x008ED4D0 Copy_ToNameInserter8_008ed4d0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SDefEntry8 {                          // 8 bytes
    long          DefIndex;                  // +0x00 (read by the inserter)
    unsigned long Extra;                     // +0x04
};

struct CCharStringVector;

struct CDefNameInserter8 {                   // output iterator over a vector<CCharString>, 4 bytes
    CCharStringVector* Container;            // +0x00
    explicit CDefNameInserter8(CCharStringVector& container) : Container(&container) { }
    CDefNameInserter8& operator=(const SDefEntry8& entry);     // 0x008ED3B0
    CDefNameInserter8& operator*() { return *this; }
    CDefNameInserter8& operator++() { return *this; }
};

CDefNameInserter8 __fastcall Copy_ToNameInserter8_008ed4d0(const SDefEntry8* first, const SDefEntry8* last, CDefNameInserter8 result)
{
    for (; first != last; ++result, ++first)
        *result = *first;
    return result;
}

struct CCharStringVector { long names[8]; int count; };
CDefNameInserter8& CDefNameInserter8::operator=(const SDefEntry8& entry) { Container->names[Container->count++] = entry.DefIndex; return *this; }

typedef SDefEntry8 T;
#define KEY(E) (E).DefIndex
#define INIT(E) (void)0
#define CALL(f, l, r) Copy_ToNameInserter8_008ed4d0(f, l, r, tag, 0)

int main()
{
    int i;
    
    SDefEntry8 e[3];
    for (i = 0; i < 3; ++i) e[i].DefIndex = 10 * (i + 1);
    CCharStringVector v;
    v.count = 0;
    CDefNameInserter8 it(v);
    CDefNameInserter8 out = Copy_ToNameInserter8_008ed4d0(e, e + 3, it);
    CHECK(out.Container == &v && v.count == 3 && v.names[0] == 10 && v.names[1] == 20 && v.names[2] == 30);
    out = Copy_ToNameInserter8_008ed4d0(e, e, it);
    CHECK(out.Container == &v && v.count == 3);
    if (g_fail) { std::printf("008ed4d0_TEST FAIL\n"); return 1; }
    std::printf("008ed4d0_TEST PASS\n");
    return 0;
}
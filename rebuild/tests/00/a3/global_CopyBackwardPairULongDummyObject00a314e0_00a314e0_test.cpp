// Behaviour test for 0x00A314E0 CopyBackward_PairULongDummyObject_00a314e0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject { public:                // 0x34 bytes
    CMatrix3x4 Transform;              // +0x00
    long       BoneIndex;              // +0x30
};
struct PairULongDummyObject {          // pair<unsigned long, C3DDummyObject>, 0x38 bytes
    unsigned long  first;              // +0x00
    C3DDummyObject second;             // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairULongDummyObject* __fastcall CopyBackward_PairULongDummyObject_00a314e0(PairULongDummyObject* first, PairULongDummyObject* last, PairULongDummyObject* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}


typedef PairULongDummyObject T;
#define KEY(E) (E).first
#define INIT(E) (E).second.BoneIndex = 7
#define CALL(f, l, r) CopyBackward_PairULongDummyObject_00a314e0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(a[3].second.BoneIndex == 7);
    if (g_fail) { std::printf("00a314e0_TEST FAIL\n"); return 1; }
    std::printf("00a314e0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00A31060 CopyBackward_DummyObjectEntry_00a31060: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject { public:                      // Ego_r.pdb, 0x34 bytes
    CMatrix3x4 Transform;                    // +0x00
    long       BoneIndex;                    // +0x30
};

struct SDummyObjectEntry {                   // 0x40 bytes
    C3DDummyObject Object;                   // +0x00
    CCharString    Name;                     // +0x34
    long           Index;                    // +0x38
    bool           Flag;                     // +0x3C
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SDummyObjectEntry* __fastcall CopyBackward_DummyObjectEntry_00a31060(SDummyObjectEntry* first, SDummyObjectEntry* last, SDummyObjectEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_strAssign = 0;
const CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }

typedef SDummyObjectEntry T;
#define KEY(E) (E).Index
#define INIT(E) (E).Name.m_pData = 0; (E).Object.BoneIndex = (E).Flag = false
#define CALL(f, l, r) CopyBackward_DummyObjectEntry_00a31060(f, l, r, tag, 0)

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
    CHECK(g_strAssign == 3);
    if (g_fail) { std::printf("00a31060_TEST FAIL\n"); return 1; }
    std::printf("00a31060_TEST PASS\n");
    return 0;
}
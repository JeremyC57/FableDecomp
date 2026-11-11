// Behaviour test for 0x00613390 CopyBackward_UIMiniMapMarker_00613390: a copy of the source model with working stand-ins for the out-of-line callees.
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

class C2DVector { public:
    float X, Y;
};

struct CUIMiniMapMarker {                    // CTCInventoryMap::CUIMiniMapMarker, 0x1C bytes
    long        Region;                      // +0x00
    CCharString UIDefName;                   // +0x04
    C2DVector   Position;                    // +0x08
    long        DefNameLength;               // +0x10
    long        GraphicIndex;                // +0x14
    float       Angle;                       // +0x18
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CUIMiniMapMarker* __fastcall CopyBackward_UIMiniMapMarker_00613390(CUIMiniMapMarker* first, CUIMiniMapMarker* last, CUIMiniMapMarker* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_strAssign = 0;
const CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }

typedef CUIMiniMapMarker T;
#define KEY(E) (E).Region
#define INIT(E) (E).UIDefName.m_pData = &(E); (E).Angle = 1.5f
#define CALL(f, l, r) CopyBackward_UIMiniMapMarker_00613390(f, l, r, tag, 0)

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
    CHECK(g_strAssign == 3 && a[3].UIDefName.m_pData == &a[2] && a[3].Angle == 1.5f);
    if (g_fail) { std::printf("00613390_TEST FAIL\n"); return 1; }
    std::printf("00613390_TEST PASS\n");
    return 0;
}
// behaviour test for 0x00613e30 (std::__uninitialized_fill_n over CUIMiniMapMarker)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
struct CCharString {
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
struct C2DVector {
    float X;                                    // +0x00
    float Y;                                    // +0x04
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
struct CTCInventoryMap {
    struct CUIMiniMapMarker {                   // 28 bytes
        long        Region;                     // +0x00
        CCharString UIDefName;                  // +0x04
        C2DVector   Position;                   // +0x08
        long        DefNameLength;              // +0x10
        long        GraphicIndex;               // +0x14
        float       Angle;                      // +0x18
    };
};
typedef CTCInventoryMap::CUIMiniMapMarker CUIMiniMapMarker;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CUIMiniMapMarker* __fastcall __uninitialized_fill_n(CUIMiniMapMarker*, unsigned int, const CUIMiniMapMarker&, const __false_type&);
}

int main()
{
    static double raw[64];
    CUIMiniMapMarker* p = (CUIMiniMapMarker*)raw;
    p[2].Region = -1;
    static double xs[16];
    CUIMiniMapMarker& x = *(CUIMiniMapMarker*)xs;
    static char block;
    x.Region = 4; x.UIDefName.m_pStringData = &block; x.Position.X = 1.0f; x.Position.Y = 2.0f; x.DefNameLength = 3; x.GraphicIndex = 5; x.Angle = 0.5f;
    __false_type tag;
    CUIMiniMapMarker* e = std::__uninitialized_fill_n(p, 2u, x, tag);
    int ok = e == p + 2 && g_stringCopies == 2 && p[1].Region == 4 && p[0].UIDefName.m_pStringData == &block && p[1].Position.X == 1.0f
          && p[0].GraphicIndex == 5 && p[1].Angle == 0.5f && p[2].Region == -1;
    if (!ok) { printf("00613e30_TEST FAIL\n"); return 1; }
    printf("00613e30_TEST PASS\n");
    return 0;
}
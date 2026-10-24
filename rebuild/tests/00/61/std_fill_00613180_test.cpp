// behaviour test for 0x00613180 (std::fill over CUIMiniMapMarker)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
struct CCharString {
    void* m_pStringData;                        // +0x00
    CCharString& operator=(const CCharString& other) { m_pStringData = other.m_pStringData; ++g_stringAssigns; return *this; }
};
struct C2DVector {
    float X;                                    // +0x00
    float Y;                                    // +0x04
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

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CUIMiniMapMarker*, CUIMiniMapMarker*, const CUIMiniMapMarker&);
}

int main()
{
    static char a, b;
    CUIMiniMapMarker v[3];
    for (int i = 0; i < 3; ++i) { v[i].Region = 0; v[i].UIDefName.m_pStringData = &a; v[i].Angle = 0.0f; v[i].GraphicIndex = 0; }
    CUIMiniMapMarker val; val.Region = 4; val.UIDefName.m_pStringData = &b; val.Position.X = 1.0f; val.Position.Y = 2.0f;
    val.DefNameLength = 3; val.GraphicIndex = 5; val.Angle = 0.75f;
    std::fill(v, v + 2, val);
    int ok = g_stringAssigns == 2 && v[0].Region == 4 && v[0].UIDefName.m_pStringData == &b && v[1].Position.Y == 2.0f
          && v[1].DefNameLength == 3 && v[1].GraphicIndex == 5 && v[0].Angle == 0.75f && v[2].Region == 0 && v[2].UIDefName.m_pStringData == &a;
    if (!ok) { printf("00613180_TEST FAIL\n"); return 1; }
    printf("00613180_TEST PASS\n");
    return 0;
}
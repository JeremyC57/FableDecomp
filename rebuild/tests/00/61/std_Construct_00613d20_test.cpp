#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N00613d20 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }           // user memberwise copy
};
struct CUIMiniMapMarker {                        // CTCInventoryMap::CUIMiniMapMarker (PDB, sizeof 0x1C)
    long        Region;                          // +0x00
    CCharString UIDefName;                       // +0x04
    C2DVector   Position;                        // +0x08
    long        DefNameLength;                   // +0x10
    long        GraphicIndex;                    // +0x14
    float       Angle;                           // +0x18
    CUIMiniMapMarker() { }
    __forceinline CUIMiniMapMarker(const CUIMiniMapMarker& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : Region(o.Region), UIDefName(o.UIDefName), Position(o.Position), DefNameLength(o.DefNameLength),
          GraphicIndex(o.GraphicIndex), Angle(o.Angle) { }
};

void __fastcall _Construct(CUIMiniMapMarker* p, const CUIMiniMapMarker& value)
{
    if (p != 0)
        new (p) CUIMiniMapMarker(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00613d20;
    CUIMiniMapMarker src;
    src.Region = 3;
    src.UIDefName.m_pData = reinterpret_cast<void*>(17);
    src.Position.X = 10.0f;
    src.Position.Y = -4.5f;
    src.DefNameLength = 6;
    src.GraphicIndex = 21;
    src.Angle = 0.75f;
    CUIMiniMapMarker* slot = reinterpret_cast<CUIMiniMapMarker*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->Region == src.Region)) ok = false;
    if (!(slot->UIDefName.m_pData == src.UIDefName.m_pData)) ok = false;
    if (!(slot->Position.X == src.Position.X)) ok = false;
    if (!(slot->Position.Y == src.Position.Y)) ok = false;
    if (!(slot->DefNameLength == src.DefNameLength)) ok = false;
    if (!(slot->GraphicIndex == src.GraphicIndex)) ok = false;
    if (!(slot->Angle == src.Angle)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("00613d20_TEST FAIL\n"); return 1; }
    std::printf("00613d20_TEST PASS\n");
    return 0;
}
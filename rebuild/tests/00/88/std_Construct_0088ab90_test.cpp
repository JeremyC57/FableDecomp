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
namespace N0088ab90 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user memberwise copy: fixed-offset dword moves, no block copy
};
struct C3DHelperPoint {                          // PDB sizeof 0x10
    C3DVector Position;                          // +0x00
    long      BoneIndex;                         // +0x0C
};
struct CNamedHelperPoint {                       // std::pair<CCharString, C3DHelperPoint>
    CCharString    first;                        // +0x00
    C3DHelperPoint second;                       // +0x04
};

void __fastcall _Construct(CNamedHelperPoint* p, const CNamedHelperPoint& value)
{
    if (p != 0)
        new (p) CNamedHelperPoint(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0088ab90;
    CNamedHelperPoint src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.Position.X = 1.5f;
    src.second.Position.Y = -2.0f;
    src.second.Position.Z = 3.25f;
    src.second.BoneIndex = 12;
    CNamedHelperPoint* slot = reinterpret_cast<CNamedHelperPoint*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.Position.X == src.second.Position.X)) ok = false;
    if (!(slot->second.Position.Y == src.second.Position.Y)) ok = false;
    if (!(slot->second.Position.Z == src.second.Position.Z)) ok = false;
    if (!(slot->second.BoneIndex == src.second.BoneIndex)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("0088ab90_TEST FAIL\n"); return 1; }
    std::printf("0088ab90_TEST PASS\n");
    return 0;
}
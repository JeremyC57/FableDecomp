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
namespace N0077e250 {
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
struct CNamedVector {                            // std::pair<CCharString, C3DVector>
    CCharString first;                           // +0x00
    C3DVector   second;                          // +0x04
};

void __fastcall _Construct(CNamedVector* p, const CNamedVector& value)
{
    if (p != 0)
        new (p) CNamedVector(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0077e250;
    CNamedVector src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.X = 1.5f;
    src.second.Y = -2.0f;
    src.second.Z = 3.25f;
    CNamedVector* slot = reinterpret_cast<CNamedVector*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.X == src.second.X)) ok = false;
    if (!(slot->second.Y == src.second.Y)) ok = false;
    if (!(slot->second.Z == src.second.Z)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("0077e250_TEST FAIL\n"); return 1; }
    std::printf("0077e250_TEST PASS\n");
    return 0;
}
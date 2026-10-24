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
namespace N00ad15c0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct COptimisedBone {                          // QuadricOptimiserInternals::COptimisedBone (PDB, sizeof 0x10)
    CCharString BoneName;                        // +0x00
    long        Parent;                          // +0x04
    bool        Remapped;                        // +0x08
    long        NewIndex;                        // +0x0C
};

void __fastcall _Construct(COptimisedBone* p, const COptimisedBone& value)
{
    if (p != 0)
        new (p) COptimisedBone(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00ad15c0;
    COptimisedBone src;
    src.BoneName.m_pData = reinterpret_cast<void*>(17);
    src.Parent = 4;
    src.Remapped = true;
    src.NewIndex = 9;
    COptimisedBone* slot = reinterpret_cast<COptimisedBone*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->BoneName.m_pData == src.BoneName.m_pData)) ok = false;
    if (!(slot->Parent == src.Parent)) ok = false;
    if (!(slot->Remapped == src.Remapped)) ok = false;
    if (!(slot->NewIndex == src.NewIndex)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("00ad15c0_TEST FAIL\n"); return 1; }
    std::printf("00ad15c0_TEST PASS\n");
    return 0;
}
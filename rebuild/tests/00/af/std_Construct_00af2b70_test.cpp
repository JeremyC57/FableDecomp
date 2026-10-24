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
namespace N00af2b70 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CPSCBase;
typedef CPSCBase* (__cdecl *PSCAllocFunc)();
enum EPSCType { PSC_TYPE_NONE };
struct CRegisteredPSCInfo {                      // NParticleEngine::CRegisteredPSCInfo (PDB, sizeof 0xC)
    PSCAllocFunc AllocFunc;                      // +0x00
    EPSCType     ComponentType;                  // +0x04
    CCharString  GUIName;                        // +0x08
};
struct CNamedPSCInfo {                           // std::pair<CCharString, CRegisteredPSCInfo>
    CCharString        first;                    // +0x00
    CRegisteredPSCInfo second;                   // +0x04
};

void __fastcall _Construct(CNamedPSCInfo* p, const CNamedPSCInfo& value)
{
    if (p != 0)
        new (p) CNamedPSCInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00af2b70;
    CNamedPSCInfo src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.AllocFunc = reinterpret_cast<PSCAllocFunc>(0x60);
    src.second.ComponentType = (EPSCType)5;
    src.second.GUIName.m_pData = reinterpret_cast<void*>(19);
    CNamedPSCInfo* slot = reinterpret_cast<CNamedPSCInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.AllocFunc == src.second.AllocFunc)) ok = false;
    if (!(slot->second.ComponentType == src.second.ComponentType)) ok = false;
    if (!(slot->second.GUIName.m_pData == src.second.GUIName.m_pData)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("00af2b70_TEST FAIL\n"); return 1; }
    std::printf("00af2b70_TEST PASS\n");
    return 0;
}
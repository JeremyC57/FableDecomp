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
namespace N009e5f60 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CSymbolInfo {                             // CSymbolMap::CSymbolInfo (PDB, sizeof 0xC)
    CCharString GroupName;                       // +0x00
    CCharString EntryName;                       // +0x04
    long        Val;                             // +0x08
};

void __fastcall _Construct(CSymbolInfo* p, const CSymbolInfo& value)
{
    if (p != 0)
        new (p) CSymbolInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N009e5f60;
    CSymbolInfo src;
    src.GroupName.m_pData = reinterpret_cast<void*>(17);
    src.EntryName.m_pData = reinterpret_cast<void*>(34);
    src.Val = -7;
    CSymbolInfo* slot = reinterpret_cast<CSymbolInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->GroupName.m_pData == src.GroupName.m_pData)) ok = false;
    if (!(slot->EntryName.m_pData == src.EntryName.m_pData)) ok = false;
    if (!(slot->Val == src.Val)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("009e5f60_TEST FAIL\n"); return 1; }
    std::printf("009e5f60_TEST PASS\n");
    return 0;
}
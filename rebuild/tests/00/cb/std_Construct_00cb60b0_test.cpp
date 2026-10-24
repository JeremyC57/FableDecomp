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
namespace N00cb60b0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CScriptBase; struct CScriptDataBase; struct CGameScriptInterfaceBase;
typedef CScriptBase* (__cdecl *ScriptAllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
typedef CScriptDataBase* (__cdecl *ScriptDataAllocFunc)();
struct CScriptInfo {                             // PDB sizeof 0x14
    ScriptAllocFunc     AllocFunc;               // +0x00
    ScriptDataAllocFunc MasterScriptDataAlloc;   // +0x04
    CCharString         Name;                    // +0x08
    long                ID;                      // +0x0C
    bool                MasterScript;            // +0x10
    CScriptInfo() { }
    __forceinline CScriptInfo(const CScriptInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : AllocFunc(o.AllocFunc), MasterScriptDataAlloc(o.MasterScriptDataAlloc), Name(o.Name), ID(o.ID), MasterScript(o.MasterScript) { }
};
struct CNamedScriptInfo {                        // std::pair<CCharString, CScriptInfo>
    CCharString first;                           // +0x00
    CScriptInfo second;                          // +0x04
    CNamedScriptInfo() { }
    __forceinline CNamedScriptInfo(const CNamedScriptInfo& o) : first(o.first), second(o.second) { }   // pair copy, forced inline (VC7.1 otherwise calls it)
};

void __fastcall _Construct(CNamedScriptInfo* p, const CNamedScriptInfo& value)
{
    if (p != 0)
        new (p) CNamedScriptInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00cb60b0;
    CNamedScriptInfo src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.AllocFunc = reinterpret_cast<ScriptAllocFunc>(0x60);
    src.second.MasterScriptDataAlloc = reinterpret_cast<ScriptDataAllocFunc>(0x70);
    src.second.Name.m_pData = reinterpret_cast<void*>(18);
    src.second.ID = 5;
    src.second.MasterScript = true;
    CNamedScriptInfo* slot = reinterpret_cast<CNamedScriptInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.AllocFunc == src.second.AllocFunc)) ok = false;
    if (!(slot->second.MasterScriptDataAlloc == src.second.MasterScriptDataAlloc)) ok = false;
    if (!(slot->second.Name.m_pData == src.second.Name.m_pData)) ok = false;
    if (!(slot->second.ID == src.second.ID)) ok = false;
    if (!(slot->second.MasterScript == src.second.MasterScript)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("00cb60b0_TEST FAIL\n"); return 1; }
    std::printf("00cb60b0_TEST PASS\n");
    return 0;
}
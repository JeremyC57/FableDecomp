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
namespace N004b8160 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CPersistInfo {                            // NQuestManager::CRuntimeQuestPersistence::CPersistInfo (PDB, sizeof 8)
    CCharString QuestName;                       // +0x00
    CCharString PersistScript;                   // +0x04
};
struct CNamedPersistInfo {                       // std::pair<CCharString, CPersistInfo>
    CCharString  first;                          // +0x00
    CPersistInfo second;                         // +0x04
};

void __fastcall _Construct(CNamedPersistInfo* p, const CNamedPersistInfo& value)
{
    if (p != 0)
        new (p) CNamedPersistInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N004b8160;
    CNamedPersistInfo src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.QuestName.m_pData = reinterpret_cast<void*>(18);
    src.second.PersistScript.m_pData = reinterpret_cast<void*>(19);
    CNamedPersistInfo* slot = reinterpret_cast<CNamedPersistInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 3 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.QuestName.m_pData == src.second.QuestName.m_pData)) ok = false;
    if (!(slot->second.PersistScript.m_pData == src.second.PersistScript.m_pData)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 3 || g_links != 0) ok = false;
    if (!ok) { std::printf("004b8160_TEST FAIL\n"); return 1; }
    std::printf("004b8160_TEST PASS\n");
    return 0;
}
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
namespace N004b8130 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CScriptInfo;
struct CActivateQuestInfo {                      // CQuestManager::CActivateQuestInfo (PDB, sizeof 0xC)
    CCharString  QuestName;                      // +0x00
    CScriptInfo* PScriptInfo;                    // +0x04
    bool         GameScript;                     // +0x08
    bool         LoadResources;                  // +0x09
};

void __fastcall _Construct(CActivateQuestInfo* p, const CActivateQuestInfo& value)
{
    if (p != 0)
        new (p) CActivateQuestInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N004b8130;
    CActivateQuestInfo src;
    src.QuestName.m_pData = reinterpret_cast<void*>(17);
    src.PScriptInfo = reinterpret_cast<CScriptInfo*>(0x50);
    src.GameScript = true;
    src.LoadResources = true;
    CActivateQuestInfo* slot = reinterpret_cast<CActivateQuestInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->QuestName.m_pData == src.QuestName.m_pData)) ok = false;
    if (!(slot->PScriptInfo == src.PScriptInfo)) ok = false;
    if (!(slot->GameScript == src.GameScript)) ok = false;
    if (!(slot->LoadResources == src.LoadResources)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("004b8130_TEST FAIL\n"); return 1; }
    std::printf("004b8130_TEST PASS\n");
    return 0;
}
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
namespace N004b82f0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CQuestCard {                              // CQuestManager::CQuestCard (PDB, sizeof 0x10)
    CCharString DefName;                         // +0x00
    CCharString ScriptName;                      // +0x04
    bool        Replayable;                      // +0x08
    long        FeatIndex;                       // +0x0C
};

void __fastcall _Construct(CQuestCard* p, const CQuestCard& value)
{
    if (p != 0)
        new (p) CQuestCard(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N004b82f0;
    CQuestCard src;
    src.DefName.m_pData = reinterpret_cast<void*>(17);
    src.ScriptName.m_pData = reinterpret_cast<void*>(18);
    src.Replayable = true;
    src.FeatIndex = 33;
    CQuestCard* slot = reinterpret_cast<CQuestCard*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->DefName.m_pData == src.DefName.m_pData)) ok = false;
    if (!(slot->ScriptName.m_pData == src.ScriptName.m_pData)) ok = false;
    if (!(slot->Replayable == src.Replayable)) ok = false;
    if (!(slot->FeatIndex == src.FeatIndex)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("004b82f0_TEST FAIL\n"); return 1; }
    std::printf("004b82f0_TEST PASS\n");
    return 0;
}
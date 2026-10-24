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
namespace N0047b590 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CQuestObjectiveDesc {                     // CHeroLogBook::CQuestObjectiveDesc (PDB, sizeof 0xC)
    unsigned long SubObjectiveID;                // +0x00
    CCharString   TextDescription;               // +0x04
    bool          QuestObjectiveCompleted;       // +0x08
};

void __fastcall _Construct(CQuestObjectiveDesc* p, const CQuestObjectiveDesc& value)
{
    if (p != 0)
        new (p) CQuestObjectiveDesc(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0047b590;
    CQuestObjectiveDesc src;
    src.SubObjectiveID = 5;
    src.TextDescription.m_pData = reinterpret_cast<void*>(17);
    src.QuestObjectiveCompleted = true;
    CQuestObjectiveDesc* slot = reinterpret_cast<CQuestObjectiveDesc*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->SubObjectiveID == src.SubObjectiveID)) ok = false;
    if (!(slot->TextDescription.m_pData == src.TextDescription.m_pData)) ok = false;
    if (!(slot->QuestObjectiveCompleted == src.QuestObjectiveCompleted)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("0047b590_TEST FAIL\n"); return 1; }
    std::printf("0047b590_TEST PASS\n");
    return 0;
}
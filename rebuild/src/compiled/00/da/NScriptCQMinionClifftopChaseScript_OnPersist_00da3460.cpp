// NScript::CQ_MinionClifftopChaseScript::OnPersist(CPersistContext& context) -- saves/loads the script's QuestStartScreened and MinionCount by name (default false / 0L)
// Leaf: ?OnPersist@CQ_MinionClifftopChaseScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA3460, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_MinionClifftopChaseScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012D7158 (stored by 0x00DA30B0); the two literals name exactly the Ego_r.pdb members QuestStartScreened / MinionCount of
//   CQ_MinionClifftopChaseScript (the only Ego_r class with both; Ego_r +0x5F / +0x7C, retail +0x4B / +0x68: the retail quest-script base is 0x14 bytes smaller), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CQ_MinionClifftopChaseScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00da3460" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<float> 0x00410620 (FableWin.pdb
//   ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z), here bool then long.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: each default is a temporary in a dead stack slot (the first in the `push ecx` slot, the second in the context
//   argument slot after the context moves to edi).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // bool 0x004045C0, long 0x00410BE0, float 0x00410620
};
class CGameScriptInterfaceBase;
class CScriptDataBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_MinionClifftopChaseScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _pad48[0x3];
    bool   QuestStartScreened;                // +0x4B
    unsigned char _pad4c[0x1C];
    long   MinionCount;                       // +0x68
};

void CQ_MinionClifftopChaseScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreened", QuestStartScreened, false);
    context.Transfer("MinionCount", MinionCount, 0L);
}
}
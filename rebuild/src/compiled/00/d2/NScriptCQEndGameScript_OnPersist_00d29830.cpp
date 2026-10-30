// NScript::CQ_EndGameScript::OnPersist(CPersistContext& context) -- saves/loads the script's UndeadBar and QuestStartScreened by name (default 0.0f / false)
// Leaf: ?OnPersist@CQ_EndGameScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D29830, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_EndGameScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012CBA58 (stored by 0x00D27370); the two literals name exactly the Ego_r.pdb members UndeadBar / QuestStartScreened of
//   CQ_EndGameScript (the only Ego_r class with both; Ego_r +0x5C / +0x65, retail +0x48 / +0x51: the retail quest-script base is 0x14 bytes smaller), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CQ_EndGameScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00d29830" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<float> 0x00410620 (FableWin.pdb
//   ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z), here float then bool.
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
class CQ_EndGameScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    float  UndeadBar;                         // +0x48
    unsigned char _pad4c[0x5];
    bool   QuestStartScreened;                // +0x51
};

void CQ_EndGameScript::OnPersist(CPersistContext& context)
{
    context.Transfer("UndeadBar", UndeadBar, 0.0f);
    context.Transfer("QuestStartScreened", QuestStartScreened, false);
}
}
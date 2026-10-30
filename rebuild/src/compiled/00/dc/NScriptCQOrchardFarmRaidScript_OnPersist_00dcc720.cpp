// NScript::CQ_OrchardFarmRaidScript::OnPersist(CPersistContext& context) -- saves/loads the 2 members ShownCombatMultiplierTutorial / HeroMetWhisperBeforeFarm by name (defaults false / false)
// Leaf: ?OnPersist@CQ_OrchardFarmRaidScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCC720, 73 bytes (+ int3 padding to 80), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_OrchardFarmRaidScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012DAF78 (stored by 0x00DCC040); the literals ShownCombatMultiplierTutorial / HeroMetWhisperBeforeFarm name exactly the Ego_r.pdb members of CQ_OrchardFarmRaidScript (the only Ego_r class with both; retail quest-script members sit 0x18 below Ego_r, offsets asserted), and the Ego_r/FableWin.pdb name sets hold ?OnPersist@CQ_OrchardFarmRaidScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00dcc720" is propagated / a placeholder (family J014: six two-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<bool> 0x004045C0, Transfer<bool> 0x004045C0.
// Shape: context in edi, this in esi; each default is a temporary in a dead stack slot (the first in the `push ecx` slot,
//   the second in the context argument slot); the second member's address is `add esi, N` in place. Model: agent952 H036
//   (the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 shape).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CGameScriptInterfaceBase;
class CScriptDataBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _pad48[0x40];
    bool           HeroMetWhisperBeforeFarm;          // +0x88 (Ego_r +0xA0)
    bool           ShownCombatMultiplierTutorial;     // +0x89 (Ego_r +0xA1)
};

void CQ_OrchardFarmRaidScript::OnPersist(CPersistContext& context)
{
    context.Transfer("ShownCombatMultiplierTutorial", ShownCombatMultiplierTutorial, false);
    context.Transfer("HeroMetWhisperBeforeFarm", HeroMetWhisperBeforeFarm, false);
}
}

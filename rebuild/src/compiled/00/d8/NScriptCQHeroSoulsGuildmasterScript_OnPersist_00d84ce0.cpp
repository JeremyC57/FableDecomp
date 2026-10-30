// NScript::CQ_HeroSoulsGuildmasterScript::OnPersist(CPersistContext& context) -- saves/loads the 4 members PlayedGuildMasterCS / PlayedBriarCS / PlayedGuildCS / GiveWarningCutscene by name with zero defaults
// Leaf: ?OnPersist@CQ_HeroSoulsGuildmasterScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D84CE0, 119 bytes, thiscall, `ret 4`.
// Identity (proven): NScript::CQ_HeroSoulsGuildmasterScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012D4804 (stored by 0x00D84B50); the 4 literals name exactly the Ego_r.pdb
//   members of CQ_HeroSoulsGuildmasterScript (the only Ego_r class with all of them; retail quest-script members sit 0x14 below Ego_r (the retail CScriptBase is 0x40 bytes, Ego_r 0x54), offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?OnPersist@CQ_HeroSoulsGuildmasterScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00d84ce0" is propagated / a placeholder (family H038: seven four-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<bool> 0x004045C0.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: context in edi, this in esi, the zero in ebx; each default is a temporary in a dead stack slot (the first in the
//   `push ecx` slot, the rest in the context argument slot); the last member's address is `add esi, N` in place.
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
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    bool           PlayedBriarCS;                     // +0x48 (Ego_r +0x5C)
    bool           PlayedGuildMasterCS;               // +0x49 (Ego_r +0x5D)
    bool           PlayedGuildCS;                     // +0x4A (Ego_r +0x5E)
    unsigned char _pad4b[0x1];
    bool           GiveWarningCutscene;               // +0x4C (Ego_r +0x60)
};

void CQ_HeroSoulsGuildmasterScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PlayedGuildMasterCS", PlayedGuildMasterCS, false);
    context.Transfer("PlayedBriarCS", PlayedBriarCS, false);
    context.Transfer("PlayedGuildCS", PlayedGuildCS, false);
    context.Transfer("GiveWarningCutscene", GiveWarningCutscene, false);
}
}

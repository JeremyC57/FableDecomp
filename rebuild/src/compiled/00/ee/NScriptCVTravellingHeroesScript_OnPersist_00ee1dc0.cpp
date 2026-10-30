// NScript::CV_TravellingHeroesScript::OnPersist(CPersistContext& context) -- saves/loads the 4 members WhisperCutscenePlayed / ThunderSceneStarted / MazeCutsceneDone / SnowTrollSceneDone by name with zero defaults
// Leaf: ?OnPersist@CV_TravellingHeroesScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE1DC0, 119 bytes, thiscall, `ret 4`.
// Identity (proven): NScript::CV_TravellingHeroesScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012F407C (stored by 0x00EDEBA0); the 4 literals name exactly the Ego_r.pdb
//   members of CV_TravellingHeroesScript (the only Ego_r class with all of them; retail quest-script members sit 0x14 below Ego_r (the retail CScriptBase is 0x40 bytes, Ego_r 0x54), offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?OnPersist@CV_TravellingHeroesScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00ee1dc0" is propagated / a placeholder (family H038: seven four-member persist bodies).
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
class CV_TravellingHeroesScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _pad48[0x31];
    bool           ThunderSceneStarted;               // +0x79 (Ego_r +0x8D)
    unsigned char _pad7a[0x1];
    bool           MazeCutsceneDone;                  // +0x7B (Ego_r +0x8F)
    bool           WhisperCutscenePlayed;             // +0x7C (Ego_r +0x90)
    bool           SnowTrollSceneDone;                // +0x7D (Ego_r +0x91)
};

void CV_TravellingHeroesScript::OnPersist(CPersistContext& context)
{
    context.Transfer("WhisperCutscenePlayed", WhisperCutscenePlayed, false);
    context.Transfer("ThunderSceneStarted", ThunderSceneStarted, false);
    context.Transfer("MazeCutsceneDone", MazeCutsceneDone, false);
    context.Transfer("SnowTrollSceneDone", SnowTrollSceneDone, false);
}
}

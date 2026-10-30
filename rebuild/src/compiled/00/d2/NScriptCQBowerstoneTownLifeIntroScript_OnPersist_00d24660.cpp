// CQ_BowerstoneTownLifeIntroScript::OnPersist(CPersistContext& context) -- saves/loads the script flags ArenaFinished, DoneGuardSpeech and PoshGuardComplete by name (default false)
// Leaf: ?OnPersist@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D24660, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CQ_BowerstoneTownLifeIntroScript::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012CB52C (a quest-script vtable: dtor, RegisterMain, Main, Init, OnPersist); the three literals name exactly the Ego_r.pdb members ArenaFinished / DoneGuardSpeech / PoshGuardComplete of NScript::CQ_BowerstoneTownLifeIntroScript (Ego_r
//   +92 / +93 / +94, retail +0x48 / +0x49 / +0x4A: the retail quest-script base is 0x14 bytes smaller); FableWin.pdb name set holds
//   ?OnPersist@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
//   "_gapscan::sub_<addr>").
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb
//   ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound at that address).
// Shape: each `false` default is a bool temporary in a dead stack slot (the first in the `push ecx` slot, the next two in the
//   context argument slot after the context moves to edi).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (already NScript). Same bytes.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <bool>: 0x004045C0
};
class CGameScriptInterfaceBase;
class CScriptDataBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_BowerstoneTownLifeIntroScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    virtual void OnPersist(CPersistContext& context);
    bool ArenaFinished;                                // +0x48
    bool DoneGuardSpeech;                              // +0x49
    bool PoshGuardComplete;                            // +0x4A
};

void CQ_BowerstoneTownLifeIntroScript::OnPersist(CPersistContext& context)
{
    context.Transfer("ArenaFinished", ArenaFinished, false);
    context.Transfer("DoneGuardSpeech", DoneGuardSpeech, false);
    context.Transfer("PoshGuardComplete", PoshGuardComplete, false);
}
}
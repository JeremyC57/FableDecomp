// NScript::CQ_BowerstoneTownLifeIntroScript::Main() -- the Bowerstone town-life intro quest main: bind the two intro guards, post them, close the region exit, wait for the guard speech, remove the intro guard and fade in, wait for the arena and the posh guard, remove the posh-gate guard and fade in, reopen the region exit, then wait until both flags are set and Bowerstone Slums is unloaded and deactivate Q_BowerstoneTownLifeIntro
// Leaf: ?Main@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D23DC0, 1079 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012CB52C (stored by the landed Alloc 0x00D253B0; the landed
//   RegisterMain 0x00D23D20 spawns it); Ego_r.pdb name set ?Main@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ. The bound Allocs
//   are the landed CBowerstoneIntroGuard::Alloc 0x00D24210 and CBowerstoneIntroGuardToPosh::Alloc 0x00D24330 (binding-name literal =
//   class name less its `C`). The FSE Lua port (refs/script_recovery/seed_corpus/sources/BowerstoneTownLifeIntro) gives the same
//   call order; retail reuses one CScriptThing local for both guards and never re-fetches the region exit.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseClass() 0x0099A2D0 (the expanded default CScriptThing ctor),
//   CCountedPointer<CScriptThing>::operator= 0x00704580 (the expanded CScriptThing::operator=), ~CScriptThing 0x004AA840,
//   CScriptThing::IsAlive 0x004AB130 (called at the last two sites, expanded over PImp at the first two);
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 72 GetThingWithScriptName,
//   108 RemoveThing, 281 DeactivateQuestLater, 374 FadeScreenIn, 567 SetRegionExitAsActive.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptThing;
template <> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);   // 0x00704580
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BowerstoneTownLifeIntroScript.h"
#include "engine/CEntityScriptBinding.h"

namespace NScript {
#define FABLE_BTLI_ENTITY(Name) \
    class CQ_BowerstoneTownLifeIntroScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BowerstoneTownLifeIntroScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_BTLI_ENTITY(CBowerstoneIntroGuard)          // Alloc 0x00D24210
FABLE_BTLI_ENTITY(CBowerstoneIntroGuardToPosh)    // Alloc 0x00D24330
#undef FABLE_BTLI_ENTITY
}

inline CScriptThing::CScriptThing() { }
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }

void NScript::CQ_BowerstoneTownLifeIntroScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BowerstoneTownLifeIntroScript>("BowerstoneIntroGuard", this, &CBowerstoneIntroGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BowerstoneTownLifeIntroScript>("BowerstoneIntroGuardToPosh", this, &CBowerstoneIntroGuardToPosh::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CScriptThing RegionExit = Game->GetThingWithScriptName("BowerstoneIntroRegionExit");
    if (RegionExit.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetRegionExitAsActive(RegionExit, false);
    }
    CScriptThing Guard;
    while (!DoneGuardSpeech) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Guard = Game->GetThingWithScriptName("BowerstoneIntroGuard");
    if (Guard.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Guard, false, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    Game->FadeScreenIn();
    while (!ArenaFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!PoshGuardComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Guard = Game->GetThingWithScriptName("BowerstoneIntroGuardToPosh");
    if (Guard.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Guard, false, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    Game->FadeScreenIn();
    if (RegionExit.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetRegionExitAsActive(RegionExit, true);
    }
    while (!PoshGuardComplete || !DoneGuardSpeech || Game->IsRegionLoaded("BowerstoneSlums")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater("Q_BowerstoneTownLifeIntro", 0);
}
// NScript::CQ_OrchardFarmRaidScript::COrchardFarmWhisper::Init() -- put Whisper in the enemy-guard faction when the hero is on team 1, else with the bandits; she cannot be killed, is aware of the hero, cannot be damaged and has no boss phase changes
// Leaf: ?Init@COrchardFarmWhisper@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF000, 144 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DB3E0, which the entity Alloc 0x00DCEF50 (`NScript::CQ_OrchardFarmRaidScript::COrchardFarmWhisper::Alloc`) stores; ?Init@COrchardFarmWhisper@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dcf000" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (an if/else of two whole EntitySetInFaction calls: VC7.1 hoists the shared `push -1` and tail-merges everything after the pushed literal); interface slots 601 EntitySetInFaction, 517 EntitySetAsKillable, 70 GetHero, 637 EntitySetAsAwareOfThing, 516 EntitySetAsDamageable, 521 EntitySetAllowBossPhaseChanges.
// Data: "FACTION_GUARDS_ENEMY" @ 0x012D4F1C, "FACTION_BANDITS" @ 0x0125FE68.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent HeroTeam +0x78 (Ego_r.pdb +0x90: -0x14, -4 for the STLport CrateList vector).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript {
public:
    unsigned char _pad000[0x78];
    long HeroTeam;                                      // +0x78 (Ego_r.pdb +0x90)
    class COrchardFarmWhisper;
};
class CQ_OrchardFarmRaidScript::COrchardFarmWhisper : public CEntityScriptBase {
public:
    CQ_OrchardFarmRaidScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CQ_OrchardFarmRaidScript::COrchardFarmWhisper::Init()
{
    if (ParentClass->HeroTeam == 1)
        Game->EntitySetInFaction(Me, "FACTION_GUARDS_ENEMY");
    else
        Game->EntitySetInFaction(Me, "FACTION_BANDITS");
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsAwareOfThing(Me, Game->GetHero());
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAllowBossPhaseChanges(Me, false);
}
// NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Init() -- the trader to rescue: unpushable, no information, scared, neutral faction, allowed to follow the hero; health-bar index cleared
// Leaf: ?Init@CTraderToRescue@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFB1B0, 155 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012DEC68, which the landed
//   NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Alloc 0x00DFB050 stores (site 0x00DFB086); Ego_r.pdb has ?Init@CTraderToRescue@CQ_TraderConflictGoodScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTraderToRescue (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 844 SetIsPushableByHero (Me by value: CBaseClass then CScriptThing vtable, PImp add-ref),
//   360 SetThingHasInformation, 496 EntitySetAsScared, 601 EntitySetInFaction ("FACTION_NEUTRAL" temporary), 235
//   EntitySetAsAllowedToFollowHero; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictGoodScript {
public:
    class CTraderToRescue;
};
class CQ_TraderConflictGoodScript::CTraderToRescue : public CEntityScriptBase {
public:
    CQ_TraderConflictGoodScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long BarIndex;                                      // +0x1c
private:
    virtual void Init();
};

void CQ_TraderConflictGoodScript::CTraderToRescue::Init()
{
    Game->SetIsPushableByHero(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    Game->EntitySetAsScared(Me, true);
    Game->EntitySetInFaction(Me, "FACTION_NEUTRAL");
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    BarIndex = 0;
}
}
// CAIStateGroup_EatAtTable::Update() -- claim a seat (else stop); while the creature belongs to a village, take the village's eating
//   duration plus up to 45 random seconds; put the creature in the table's occupant list, walk to the seat (else stop), sit down with
//   the SIT_AT_TABLE_INTO animation into a new tavern-stool sitting mode and, once seated, mark the seat used and eat each food item
//   the table puts in front of the seat (idling while there is none) until eating or idling ends; then terminate.
// Leaf: ?Update@CAIStateGroup_EatAtTable@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096CD70, 578 bytes (+ padding), thiscall, esp frame 0xBC, plain `ret`. Identity:
//   Ego_r.pdb ?Update@CAIStateGroup_EatAtTable@@MAEXXZ (CAIStateGroupBase slot 12); the same-TU neighbours are the landed
//   WalkTo 0x0096C880, Eat 0x0096C9A0 and OnReplace 0x0096CFC0.
// Callees: CAIStateGroup_EatAtTable::GetSeat 0x0096CB40 / WalkTo 0x0096C880 / Eat 0x0096C9A0, CAIStateGroupBase::Terminate 0x008FCF40 /
//   GetPWorldSeed 0x008FCFD0 / AIAction_PerformAction 0x0097D8B0 / AIAction_DoNothing 0x0097E200, CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound const 0x004365B0 (PeekTC<const CTCVillageMember>) / non-const 0x0040F020 (GetTC<CTCTavernTable>), CTCVillageMember::
//   GetPVillage 0x007EE540, GFFloatRandom(float, unsigned long*) 0x004978A0 (__fastcall), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThingCreatureBase::GetAnimFromName 0x00662A00, operator new
//   0x00BFEA1A, NCreatureMode::CModeSitTavernStool(CThing&) 0x0094B530, CCountedPointer<NCreatureMode::CModeSitBase>(T*) 0x0094B3A0 /
//   ~CCountedPointer 0x008AB320 (same-TU COMDATs, called), CCreatureAction_SitDown 0x008AB9D0 / ~ 0x008ABAA0, CTCTavernTable::
//   SetSeatState 0x007B8530 / GetFood 0x007B85C0. Literal "SIT_AT_TABLE_INTO" 0x01293060; 45.0f pushed as an immediate.
// Layout: Ego_r.pdb CAIStateGroup_EatAtTable (no containers): Table +0x14 (CIntelligentPointer<CThing>), SeatIndex +0x1C,
//   SeatActionPointIndex +0x20, SeatPos +0x24, SeatRHSet +0x30, Duration +0x48 (no shared header: local derived class on the shared
//   CAIStateGroupBase). CTCVillage::PDef +0x94 (CVillageDef::EatDuration +0x90), CTCTavernTable::Occupants +0x10.
// Shape: the presence bit is materialised into a stack byte before the branch (the `const bool&` HasTC); the tavern-table pointer is
//   uninitialised (the absent-component path reads its stale frame word, 0x0096CE59); the eating duration goes through the inline
//   by-value CTCVillage::GetPDef (add-ref copy, release after the read); the sit mode's counted pointer is built and destroyed by the
//   called same-TU COMDATs with visible bodies (packs into the dead key / name slot and is copied into the by-value argument without
//   re-reading Info, as 0x008A8C60).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODESITTAVERNSTOOL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCTavernTable.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/GFFloatRandom.h"

class CAIStateGroup_EatAtTable : public CAIStateGroupBase {
protected:
    virtual void Update();                                               // slot 12
    bool WalkTo(C3DVector pos, CRightHandedSet rh_set);                 // 0x0096C880
    bool Eat(CThing* food);                                              // 0x0096C9A0
    bool GetSeat();                                                      // 0x0096CB40
    CIntelligentPointer<CThing> Table;                                   // +0x14
    long SeatIndex;                                                      // +0x1c
    long SeatActionPointIndex;                                           // +0x20
    C3DVector SeatPos;                                                   // +0x24
    CRightHandedSet SeatRHSet;                                           // +0x30
    float Duration;                                                      // +0x48
};

void CAIStateGroup_EatAtTable::Update()
{
    if (!GetSeat()) {
        Terminate();
        return;
    }
    const CTCVillageMember* member;
    if (GetCreature().PeekTC(&member) && member->GetPVillage()) {
        Duration = (float)member->GetPVillage()->GetPDef()->EatDuration;
        Duration += GFFloatRandom(45.0f, GetPWorldSeed());
    }
    CTCTavernTable* table;
    Table.Get()->GetTC(&table);
    table->Occupants[SeatIndex] = &GetCreature();
    if (!WalkTo(SeatPos, SeatRHSet)) {
        Terminate();
        return;
    }
    const CAnimationEntry& anim = GetCreature().GetAnimFromName("SIT_AT_TABLE_INTO", 0);
    CCountedPointer<NCreatureMode::CModeSitBase> sit_mode(new NCreatureMode::CModeSitTavernStool(GetCreature()));
    bool failed = AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim, sit_mode)) != AI_ACTION_FINISHED;
    if (!failed) {
        table->SetSeatState(SeatIndex, true);
        for (;;) {
            CThingObject* food = table->GetFood(SeatIndex);
            if (food) {
                if (!Eat(food))
                    break;
            } else if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS)
                break;
        }
    }
    Terminate();
}
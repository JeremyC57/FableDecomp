// CAIStateGroup_CreatureIdleUnderground::OnActivate() -- unless the creature is already in the underground mode (type 0x1e), make it go underground
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Leaf: ?OnActivate@CAIStateGroup_CreatureIdleUnderground@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009208E0, length 124 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x31 =
// CTCCreatureModeManager: bitset word 1 bit 17 -> `test dword ptr [eax+24h],20000h`), the mode manager's current-mode
// getter @ 0x0073A6E0 (the manifest's CPixelShader::CountReferences label is propagated), the mode's virtual slot 1
// (+0x04, its type), CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*) @ 0x008B8110,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, the action base destructor @ 0x0062E900
// (the manifest's ~CTCLightningOrb label is propagated).
// Shape: the landed GetInterface out-parameter template inside the `if`; the action is a TEMPORARY inside the
// call (the landed 0x00933BB0 shape): the creature read into a named local (esi) is both the constructor
// argument and the `this` of SetCurrentAction, the constructor's returned `this` is pushed, the base destructor
// runs after the call. Frame 0x118 = the lookup key (+0) and the 0x114-byte retail action (+4; the PDB's
// 300-byte layout is wider).
// Model: local (no engine header for this class).
// Name fix (agentNF6 2026-11-10): CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 is ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ (Ego_r.pdb / FableWin.pdb; engine/CTCCreatureModeManager.h): const, returns a reference to the nested CTCCreatureModeManager::CModeBase.
// Port (NF93, q11261 / q11262): the local N_0062e900::CCreatureActionBase / CCreatureAction_GoUnderground / CThingAICreature (GetInterface) /
//   CTCCreatureModeManager / CAIBrain / CAIStateGroupBase stand-ins replaced by the shared headers (the landed 0x009210A0 BeetleToggleHover
//   recipe: GetCreature() + GetTC) with the NF93 opt-in CCREATUREACTION_GOUNDERGROUND_CLASS: SetCurrentAction 0x006644F0 (by reference),
//   the ctor 0x008B8110 and ~CCombatActionBase 0x0062E900 now bind the PDB spellings; 0x1e is NCreatureMode::MODE_UNDERGROUND.
//   CAIStateGroup_CreatureIdleUnderground has no shared header: it stays a row-local class on the shared CAIStateGroupBase (RTTI base).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_CLASS
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_GoUnderground.h"

class CAIStateGroup_CreatureIdleUnderground : public CAIStateGroupBase {   // RTTI : CAIStateGroupBase (no shared header)
public:
    virtual void OnActivate();
};

void CAIStateGroup_CreatureIdleUnderground::OnActivate()
{
    CTCCreatureModeManager* pmodes;
    if (GetCreature().GetTC(&pmodes)) {
        if (pmodes->GetCurrentMode().GetType() != NCreatureMode::MODE_UNDERGROUND) {
            CThingAICreature& creature = GetCreature();
            creature.SetCurrentAction(CCreatureAction_GoUnderground(creature));
        }
    }
}

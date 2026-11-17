// CCreatureAction_ScorpionPrepareTailPlunge::FinishAction() -- when the owner's flag bit 3 (+0x6c) is set, put the scorpion king into a new CModeScorpionKingVulnerable aimed at the action's target through the owner's mode manager; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_ScorpionPrepareTailPlunge@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4990, length 237 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0xb8 bytes), CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing*)
// @ 0x008D8120 (vtable 0x0128582C, mode name "VULNERABLE"), CCountedPointer<CModeScorpionKingVulnerable>::CCountedPointer(T*)
// @ 0x008C5980 (out of line, per-type deleter 0x008C57F0), CModeScorpionKingVulnerable::SetTarget(CThing*) @ 0x008D73E0,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, operator delete @ 0x00BFE9BC (the count block),
// CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the target is fetched first (ebx), then the UNCHECKED out-parameter GetInterface (the absent path reads the never
// written out pointer from the lookup key's frame word, which the counted-pointer local reuses afterwards); the counted
// pointer is converted into a CCountedPointer<CModeBase> built in the argument area (inline converting copy, add-ref) and
// released inline afterwards (block {refs, __fastcall destroy, object}, freed with operator delete). `this` is spilled to the
// frame across the block (register pressure).
// Layout: the owner (CIntelligentPointer<CThingCreatureBase>) at +8, the target at +0xa8 (retail CCreatureActionBase is 0xa8).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"
#include "engine/CCreatureAction_ScorpionPrepareTailPlunge.h"

void CCreatureAction_ScorpionPrepareTailPlunge::FinishAction()
{
    if (PCreature.Get()->GetTypeFlags() & 8) {
        CThingCreatureBase* target = static_cast<CThingCreatureBase*>(PTarget.Get());
        CTCCreatureModeManager* modeManager;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
        CCountedPointer<NCreatureMode::CModeScorpionKingVulnerable> mode(new NCreatureMode::CModeScorpionKingVulnerable(*PCreature.Get()));
        mode->SetTarget(*target);
        modeManager->AddMode(mode);
    }
    CCombatActionBase::FinishAction();
}

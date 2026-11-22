// ?A0x6cb5dcbb::CCreatureAction_ScreamerIntoAggressive::IsToFaceTargetAtStart() const -- returns true (the class header's inline override)
// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_ScreamerIntoAggressive@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096DBA0, 3 bytes (mov al,1 ; ret). Identity: slot 74 of vtable 0x01296F14 (slot 73 is 0x0096DBB0).
// Names (agentNF252, 2026-11-22, CODEXP27 L101 / E3): was the gap-scan stand-in ?Get@T@@QAE_NXZ (catalog vfunc_74). FableWin.pdb S_PUB32
//   ?IsToFaceTargetAtStart@CCreatureAction_ScreamerIntoAggressive@?A0x4c815337@@UBE_NXZ (retail hash ?A0x6cb5dcbb); slot 74 in engine/CCombatActionBase.h's order, which
//   the landed 0x0096DF50 vtable COMDAT reproduces (vtcheck.py).
// Namespace: the class is TU-local (anonymous namespace) in the original CAIStateGroup_ScreamerPursueTarget TU: retail RTTI
//   .?AVCCreatureAction_ScreamerIntoAggressive@?A0x6cb5dcbb@@ (retail type descriptor; rtti-vtables.tsv), Ego_r.pdb / FableWin.pdb ?A0x4c815337
//   (their own builds). This row emits its own per-TU hash (EXTRA_RULES 2026-11-20: the hash is stripped when counting spellings), so the
//   Leaf line stops at ?A0x and the full spelling is written out on the Names line with the retail hash.
// Prelude: the #define / #include block of the landed 0x0096DF50 CAIStateGroup_ScreamerPursueTarget::Update (its object emits this
//   class's vtable 0x01296F14 at HEAD from the same headers), so this row's vtable COMDAT has the same slots as that copy (vtcheck.py).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SCREAMER_VTABLE_PASS27
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_ScreamerPursueTarget.h"
#include "engine/CAIBrain.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAdaptThingCompareToSTLComparitor.h"
#include "engine/CCreatureAction_ScreamerIntoAggressive.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CTCStealth.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCreatureMode.h"
#include "engine/GFSqr.h"
#include "engine/GFRandom.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable with its scalar deleting destructor and
// the inline virtuals (this row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol);
// never called.
CCreatureActionBase* EmitScreamerIntoAggressiveVirtuals(CThingCreatureBase& creature, CThing& target)
{
    return new CCreatureAction_ScreamerIntoAggressive(creature, target);
}
}

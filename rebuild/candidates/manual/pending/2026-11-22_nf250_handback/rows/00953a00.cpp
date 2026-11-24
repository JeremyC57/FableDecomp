// CCreatureAction_DragonFireBlastForward::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_DragonFireBlastForward expanded,
//   then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCCreatureAction_DragonFireBlastForward@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00953A00, 30 bytes. Identity: slot 0 of vtable 0x012940E4, RTTI .?AVCCreatureAction_DragonFireBlastForward@@ (manifest rtti-vtables.tsv; the
//   catalog name agrees).
// Callees: CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase (out of line; the class adds no member), operator
//   delete 0x00BFE9BC. Model: the shared engine/CCreatureAction_DragonFireBlastForward.h (FABLE_RETAIL_JACKDRAGON_HOVER_PASS27) over
//   engine/CCreatureAction_DragonFireBlastBase.h (FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE).
// Names (agentNF250, 2026-11-22, CODEXP27 L96): was a free __fastcall stand-in (no Leaf line) over a row-local model; now the member the
//   compiler emits for the shared header class, so the leaf is the ??_G spelling (no landed code referencer: the address is reached only
//   through the vtable slot).
// Prelude: the #define / #include block of the landed 0x00951FB0 (its object emits this class's vtable at HEAD), so this row's vtable
//   COMDAT has the same slots as that copy (vtcheck.py).
#define FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_JACKDRAGON_HOT_CTORS_PASS27
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATEGROUND
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATESTRAFE_PASS26
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CSoundPair.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CCreatureAction_DragonFireBlastBase.h"
#include "engine/CCreatureAction_DragonFireBlastForward.h"
#include "engine/CCreatureAction_DragonFireBlastLeft.h"
#include "engine/CCreatureAction_DragonFireBeamInto.h"
#include "engine/CCreatureAction_DragonHoverOutOf.h"
#include "engine/NCreatureMode_CModeDragonHover.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/ThingAlive.h"
namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable and its scalar deleting destructor (the
// row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol); never called.
CCreatureActionBase* EmitDragonFireBlastForwardDeletingDestructor(CThingCreatureBase& creature, CThing& target)
{
    return new CCreatureAction_DragonFireBlastForward(creature, target);
}
}

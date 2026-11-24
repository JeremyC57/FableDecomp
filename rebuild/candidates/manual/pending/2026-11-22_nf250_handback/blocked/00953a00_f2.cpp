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
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBOSS_STRIKES
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_ALLOWREACTIONS
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CCreatureAction_DragonFireBlastBase.h"
#include "engine/CCreatureAction_DragonFireBlastForward.h"
namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable and its scalar deleting destructor (the
// row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol); never called.
CCreatureActionBase* EmitDragonFireBlastForwardDeletingDestructor(CThingCreatureBase& creature, CThing& target)
{
    return new CCreatureAction_DragonFireBlastForward(creature, target);
}
}

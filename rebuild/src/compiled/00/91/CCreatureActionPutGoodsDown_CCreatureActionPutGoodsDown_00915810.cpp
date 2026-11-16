// CCreatureAction_PutGoodsDown::CCreatureAction_PutGoodsDown(CThingCreatureBase& creature, const CCharString& anim_name) -- base
// CCreatureActionBase(creature, -1.0, 0.0, a random animation of the put-down name, zeroed CSoundPair, 0, 0), own vtable (0x0128FAD4)
// Leaf: ??0CCreatureAction_PutGoodsDown@@QAE@AAVCThingCreatureBase@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915810, length 79 bytes, `ret 8`.
// Callees: CCreatureActionBase::GetRandomAnimFromName(const CThingCreatureBase&, const CCharString&) @ 0x006924B0 (static __fastcall: ecx creature,
//   edx name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long)
//   @ 0x00693B30.
// Shape: the same argument order as 0x00842A20 with 0.0f pushed as the zero immediate.
// NF39 (2026-11-16): moved off its local CCreatureActionBase / CSoundPair / class models (pointer + `long kind` arguments) onto the shared
//   engine/CCreatureAction_PutGoodsDown.h (opt-in FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN, which names 0x00915810), engine/CCreatureActionBase.h
//   (ANIMCTOR: the base constructor and GetRandomAnimFromName) and engine/CSoundPair.h. Evidence: Ego_r.pdb + FableWin.pdb
//   ??0CCreatureAction_PutGoodsDown@@QAE@AAVCThingCreatureBase@@ABVCCharString@@@Z (`ret 8`; PlaceGoods 0x00915B60 passes goods_names[crate_index],
//   already bound).
// Defines: the landed 0x00915B60 PlaceGoods preamble + ANIMCTOR.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUP_MOVEGOODSBASE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTION_PUTGOODSDOWN
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCCRATESTACK
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#include "engine/C3DVector.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCrateStack.h"
#include "engine/CTCExclusiveUse.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PutGoodsDown.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CAIStateGroup_MoveGoodsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

CCreatureAction_PutGoodsDown::CCreatureAction_PutGoodsDown(CThingCreatureBase& creature, const CCharString& anim_name)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, anim_name), CSoundPair(), 0, 0)
{
}

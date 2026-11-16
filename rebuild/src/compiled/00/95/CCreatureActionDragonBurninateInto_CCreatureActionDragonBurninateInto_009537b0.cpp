// CCreatureAction_DragonBurninateInto::CCreatureAction_DragonBurninateInto(CThingCreatureBase& creature, std::vector<C3DVector>& movement_points, C3DVector& previous_point, float burn_start, float burn_stop, C3DVector& flame_start_pos, C3DVector& flame_end_pos) -- base CCombatActionBase(creature, 250, the creature's "SWORD_SWING" sound pair), then the flight path, burn window and flame end points
// Leaf: ??0CCreatureAction_DragonBurninateInto@@QAE@AAVCThingCreatureBase@@AAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@MM22@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009537B0, length 208 bytes, `ret 1Ch`. Vtable 0x01294374.
// Identity: Ego_r.pdb / FableWin.pdb ??0CCreatureAction_DragonBurninateInto@@QAE@AAVCThingCreatureBase@@AAV?$vector@..@std@@AAVC3DVector@@MM22@Z
//   (engine/CCreatureAction_DragonBurninateInto.h retail branch); the probed caller binds it.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("SWORD_SWING" @ 0x0127AC80), CCombatActionBase::GetSoundPair 0x008561A0
//   (`__fastcall`, CSoundPair by hidden pointer), CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&)
//   0x00858030, std::vector<C3DVector> copy constructor 0x006F0810.
// Shape: the literal is an implicit CCharString temporary built before the base arguments and destroyed after the base call;
//   init-list members in declaration order (C3DVector's memberwise user copy ctor; floats copied as dwords).
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
#define FABLE_RETAIL_CCREATUREACTION_DRAGONBURNINATEINTO
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_DragonBurninateInto.h"

CCreatureAction_DragonBurninateInto::CCreatureAction_DragonBurninateInto(CThingCreatureBase& creature, std::vector<C3DVector>& movement_points,
    C3DVector& previous_point, float burn_start, float burn_stop, C3DVector& flame_start_pos, C3DVector& flame_end_pos)
    : CCombatActionBase(creature, 250, GetSoundPair(creature, "SWORD_SWING"))
    , PreviousPoint(previous_point)
    , MovementPoints(movement_points)
    , BurnStart(burn_start)
    , BurnStop(burn_stop)
    , FlameStartPos(flame_start_pos)
    , FlameEndPos(flame_end_pos)
{
}

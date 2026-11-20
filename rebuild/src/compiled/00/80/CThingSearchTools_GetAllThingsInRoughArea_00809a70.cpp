// 00809a70 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind5<CThingFilter_IsNot, CThingFilter_IsTargetable, `anonymous namespace'::CThingFilter_IsInTargetingRangeAndFOV, CThingFilter_IsInSameVolumeContextAsThing, `anonymous namespace'::CThingFilter_IsValidTargetForPlayerCreature> >(..) const -- the player's targeting candidates under the area
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind5@VCThingFilter_IsNot@@VCThingFilter_IsTargetable@@VCThingFilter_IsInTargetingRangeAndFOV@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00809A70, thiscall `ret 0Ch`.
// Identity: Ego_r.pdb 0x00802FA0 + FableWin.pdb name this instance (anonymous-namespace filters of the CTCTargetingPlayer source file:
//   the Leaf is cut at ?A0x); the landed caller CTCTargetingPlayer::UpdateTargetList 0x00804020 binds it. The body calls CThingFilter_IsNot
//   0x006A76A0, CThingFilter_IsTargetable 0x006A84F0, CThingFilter_IsInTargetingRangeAndFOV 0x008090E0, CThingFilter_IsInSameVolumeContextAsThing
//   0x006A8C60 and CThingFilter_IsValidTargetForPlayerCreature 0x00808E40 in the Bind5 order; push_back grows through vector<CThing*>::
//   _M_insert_overflow 0x00436AC0.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the
//   filters / Filters / a CThingSearchTools_Calls stand-in) onto the shared headers as an explicit instantiation of the engine/CThingSearchTools.h
//   template, with the define / include set of its landed caller 0x00804020 (the same retail TU; the caller's Flags /d2inlT68 are its own
//   inline budget and are not used here) plus the K016 vector opt-ins STLP_VECTOR_OVERFLOW_OUTOFLINE / CLIPAREA / ROUGHAREA_THISEND /
//   ROUGHAREA_TYPEDLOCAL (the 0x0080A550 form). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_RAW_NOT_EQUAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTargetingDef.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CWorld.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCStockItem.h"
#include "engine/TargetingPlayerSupport.h"
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::vector<CThing*>,CFilter_Bind5<CThingFilter_IsNot,CThingFilter_IsTargetable,CThingFilter_IsInTargetingRangeAndFOV,CThingFilter_IsInSameVolumeContextAsThing,CThingFilter_IsValidTargetForPlayerCreature> >(const C2DBoxI&,const CFilter_Bind5<CThingFilter_IsNot,CThingFilter_IsTargetable,CThingFilter_IsInTargetingRangeAndFOV,CThingFilter_IsInSameVolumeContextAsThing,CThingFilter_IsValidTargetForPlayerCreature>&,std::vector<CThing*>&) const;

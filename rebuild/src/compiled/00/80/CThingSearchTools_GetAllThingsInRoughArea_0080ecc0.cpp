// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::list<CThingCreatureBase*>, CFilter_Bind5<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_In2DRange, std::unary_negate<CThingFilter_In2DRange>, `anonymous namespace'::CThingFilter_IsAffectedByExplosions> >(const C2DBoxI& area, const CFilter_Bind5<...>& filter_func, std::list<CThingCreatureBase*>& things) const -- append every creature in the mapwho cells under the area (each thing once per search id) that is neither the excluded thing nor in the exclusion list, lies inside the outer 2D range but outside the inner one, and is affected by explosions; the scratch map list is cleared
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$list@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@V?$CFilter_Bind5@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_In2DRange@@V?$unary_negate@VCThingFilter_In2DRange@@@std@@VCThingFilter_IsAffectedByExplosions@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080ECC0, 643 bytes, thiscall `ret 8`; one caller 0x0080C830 (no catalog row).
// Identity (the landed row bound a CThingSearchTools_Calls stand-in): Ego_r.pdb 0x00809E30 + FableWin.pdb 0x0276D800 name ??$GetAllThingsInRoughArea@VCThingCreatureBase@@
//   V?$list@PAVCThingCreatureBase@@...V?$CFilter_Bind5@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_In2DRange@@V?$unary_negate@VCThingFilter_In2DRange@@@std@@
//   VCThingFilter_IsAffectedByExplosions@?A0x41ace303@@@@@CThingSearchTools@@QBE...; the body calls CThingFilter_IsNot 0x006A76A0, CThingFilter_IsNotList 0x006A7EB0,
//   CThingFilter_In2DRange 0x006A76B0 (plain, then negated) and 0x0080E480 in that Bind5 order (0x0080E480, the fifth filter, also called at 0x0080EBBF in an uncatalogued function: the anonymous-namespace
//   CThingFilter_IsAffectedByExplosions::operator(), FableWin.pdb 0x0276F810; its own row carries the IsAffectedByObstructions label: NAME_FIXES).
// NF110 (round 10, q11280 / q11283): ported off its local model (RetailVector / ThingList / CThing / the thing map / local filters and a Filters struct /
//   a CThingSearchTools_Calls stand-in) onto the shared headers as an explicit instantiation of the engine/CThingSearchTools.h template (the std::list
//   ROUGHAREA_EGOD body of its TU-mates 0x0080E670 / 0x0080E8E0) on the define / include set of the NF107 port 0x00809A70 (engine/TargetingPlayerSupport.h's CFilter_Bind5
//   needs FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST and its include chain). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
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
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
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
#include "engine/CThingFilter_IsNotList.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/TargetingPlayerSupport.h"
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"
// Kept from the landed row (no shared header for this class), moved into the anonymous namespace as both PDBs spell it (the TU-mate 0x0080E670 keeps
//   CThingFilter_IsEvadeExplosion there the same way):
namespace { class CThingFilter_IsAffectedByExplosions { public:bool operator()(const CThing&)const;}; } // 0x0080e480
template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::list<CThingCreatureBase*>,CFilter_Bind5<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_In2DRange,std::unary_negate<CThingFilter_In2DRange>,CThingFilter_IsAffectedByExplosions> >(const C2DBoxI&,const CFilter_Bind5<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_In2DRange,std::unary_negate<CThingFilter_In2DRange>,CThingFilter_IsAffectedByExplosions>&,std::list<CThingCreatureBase*>&) const;

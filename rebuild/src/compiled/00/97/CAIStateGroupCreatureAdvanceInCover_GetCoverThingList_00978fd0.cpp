// CAIStateGroup_CreatureAdvanceInCover::GetCoverThingList(CThingVector&) -- collect unoccupied cover closer to the enemy than the present cover (or creature).
// Leaf: ?GetCoverThingList@CAIStateGroup_CreatureAdvanceInCover@@UAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 0x00978FD0, 287 bytes excluding padding. Shared AdvanceInCover/UseCoverBase/Base/Brain headers audited.
// UseCoverBase exposes PCoverThing at the retail unaligned +0x1f; the inherited enemy handle PTargetThing is at +0x14.
// Search 0x009793D0 audited: calls the four bound predicates, then appends matching things to the supplied vector. No predicate or callback is omitted.
// NF137 (2026-11-20, q4994 / q5140): moved onto the retail classes (CAIStateGroup_CreatureAdvanceInCover.h _ISRUNNABLELEAF + the overlay nested
//   opt-in _GETCOVERTHINGLIST, on the UseCoverBase / EnemyCreatureBase retail modes of the landed 0x009790F0 block). PeekThingSearchTools 0x008FD000
//   now binds the PDB ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (engine/CAIStateGroupBase.h _PEEK), was the
//   AdvanceInCoverMethods pointer stand-in; the search 0x009793D0 now binds its self row's PDB instance
//   GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind4<CThingFilter_CanBeUsedAsCover, unary_negate<CThingFilter_IsOccupiableOccupied>,
//   CThingFilter_IsNot, CThingFilter_In2DRange> > (QBE; was CThingSearchToolsRetail::FindInRoughArea), the box conversion 0x00A55A30
//   ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ (as 0x00969E70; was C2DBox::ToInteger), GetPos 0x004C73D0 the PDB CThing spelling. Own leaf: the PDB
//   public override (Ego_r.pdb 0x0099DD10 + FableWin.pdb 0x02C155C0; was ?GetCoverThingList@AdvanceInCoverMethods@@QAEXAAUCThingVector@@@Z).
//   The filters are the shared CFilter_Bind4 / CThingFilter_IsNot / CThingFilter_In2DRange / CThingFilter_IsOccupiableOccupied (CThingFirePointFilters.h)
//   and std::unary_negate; CThingFilter_CanBeUsedAsCover has no shared header and stays row-local (the shape of landed 0x009793D0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_GETCOVERTHINGLIST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include <math.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFirePointFilters.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"

class CThingFilter_CanBeUsedAsCover { public: CThingFilter_CanBeUsedAsCover() {} bool operator()(const CThing&) const; }; // 0x006a7ab0 (no shared header; as 0x009793D0)

void CAIStateGroup_CreatureAdvanceInCover::GetCoverThingList(std::vector<CThing*>& output)
{
 CThing* current=PCoverThing.Get();
 if(!current) current=&GetCreature();
 const C3DVector& enemy=PTargetThing.Get()->GetPos();
 const C3DVector& start=current->GetPos();
 float dx=start.X-enemy.X,dy=start.Y-enemy.Y;
 float radius=(float)sqrt(dx*dx+dy*dy);
 C2DVector center(PTargetThing.Get()->GetPos());
 PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxF(center,radius).ToC2DBoxI(),
  CFilter_Bind4<CThingFilter_CanBeUsedAsCover,std::unary_negate<CThingFilter_IsOccupiableOccupied>,CThingFilter_IsNot,CThingFilter_In2DRange>(
   CThingFilter_CanBeUsedAsCover(),std::unary_negate<CThingFilter_IsOccupiableOccupied>(CThingFilter_IsOccupiableOccupied()),CThingFilter_IsNot(*current),
   CThingFilter_In2DRange(PTargetThing.Get()->GetPos(),radius)),output);
}

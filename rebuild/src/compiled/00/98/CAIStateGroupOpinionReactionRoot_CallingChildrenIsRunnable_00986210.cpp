// CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable() -- runnable unless the creature is itself a child: search the rough square of its rounded-up sight radius for another child creature within the sight radius of the creature and nearer to the thing reacted to than the creature is
// Leaf: ?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Flags: none
// Retail-asm: 0x0098628A `fadd` + 0x0098628D `fistp dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper GFFloatToLongCeil
//   (engine/GFFloatToLongCeil.h; FableWin calls ?GFFloatToLongCeil@@YIJM@Z at 0x02C770D3) on the sight radius. This row carries no asm
//   of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00986210, 461 bytes, plain `ret` (ebp frame).
// Shape: FableWin.pdb ?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (FableWin 0x02C76EB0) with the Ego_d.pdb
//   locals (ptc_perception, ret_pcreature, scan_area): the child test, the out pointer cleared, GetTC<const CTCPerceiveThing> (its
//   debug ASSERT is not in retail), the scan box C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), GFFloatToLongCeil(GetSightRadius())),
//   then GetPFirstThingInRoughArea over GFFilter_Bind4(IsNot(creature), IsCreatureType(HUMAN_CHILD), In2DRange(creature, sight),
//   In2DRange(reacted-to, distance creature -> reacted-to)), the result tested against null.
// Callees: CThingCreatureBase::GetCreatureType 0x00662000, CVectorMap::LowerBound 0x0040F020 (GetTC expanded), CTCPerceiveThing::
//   GetSightRadius 0x007448F0 (x2), CThing::GetPos 0x004C73D0, _ftol2 0x00BFEA70 (ToC2DCoordI), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CAIStateGroupBase::PeekThingSearchTools 0x008FD000, GetPFirstThingInRoughArea<CFilter_Bind4<..>, const
//   CThingAICreature> 0x009871C0 (landed).
// Bracket: `(ptc_perception->GetSightRadius())`, the second argument of the first In2DRange (the creature's sight filter), fixes
//   0x00986374..0x0098637A (retail loads the spilled sight radius between the position's Y load and Y store). Measured: brackets on
//   the GetPos() argument, on GetCreature() or on the whole In2DRange temporary leave the radius load after the Z load (461v461).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_CALLINGCHILDREN
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsCreatureType.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

// Same-TU template body: CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind4<..>, const CThingAICreature> (retail 0x009871C0,
// the landed row's text; retail's object holds this instance). Without it visible the frame is 0x68 v 0x5c.
template<class Filter,class T>
bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area,const Filter& filter_func,T** thing) const
{
    unsigned long flags=T::StatGetTypeFlags();
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if((loop_thing->GetTypeFlags()&flags)==flags) {
                                T* tthing=static_cast<T*>(loop_thing);
                                if(filter_func(*tthing)) {
                                    *thing=tthing;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return false;
}

bool CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable()
{
    if (GetCreature().GetCreatureType() != HUMAN_CHILD) {
        const CTCPerceiveThing* ptc_perception;
        const CThingAICreature* ret_pcreature = NULL;
        GetCreature().GetTC(&ptc_perception);
        C2DBoxI scan_area(GetCreature().GetPos().ToC2DCoordI(), GFFloatToLongCeil(ptc_perception->GetSightRadius()));
        PeekThingSearchTools().GetPFirstThingInRoughArea(scan_area,
            GFFilter_Bind4(CThingFilter_IsNot(GetCreature()), CThingFilter_IsCreatureType(HUMAN_CHILD),
                           CThingFilter_In2DRange(GetCreature().GetPos(), (ptc_perception->GetSightRadius())),
                           CThingFilter_In2DRange(ThingReactingTo->GetPos(), (GetCreature().GetPos() - ThingReactingTo->GetPos()).GetMagnitude())),
            &ret_pcreature);
        return ret_pcreature != NULL;
    }
    return false;
}
// CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind4<CThingFilter_IsNot, CThingFilter_IsCreatureType, CThingFilter_In2DRange, CThingFilter_In2DRange>, const CThingAICreature>(const C2DBoxI& area, const CFilter_Bind4<...>& filter_func, const CThingAICreature** thing) const -- the first AI creature in the mapwho cells under the area (each thing once per search id) that is not the excluded thing, is of the creature type and lies in both 2D ranges, through the out pointer (true; the scratch map list is left as is); else the scratch list is cleared and false
// Leaf: ??$GetPFirstThingInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009871C0, 614 bytes, thiscall `ret 0xc`; one caller 0x009863C0 (CAIStateGroup_OpinionReaction...).
// Names: Ego_r.pdb 0x009A7EB0 / FableWin.pdb 0x02C7F290 (ai_state_group_opinion_reaction_behaviours.obj)
//   ??$GetPFirstThingInRoughArea@V?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_IsCreatureType@@VCThingFilter_In2DRange@@V3@@@
//   $$CBVCThingAICreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind4@...@@PAPBVCThingAICreature@@@Z.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsNot::operator()
//   0x006A76A0, CThingCreatureBase::GetCreatureType 0x00662000, CThingFilter_In2DRange::operator() 0x006A76B0 (x2), memmove 0x00BFEAE6.
// Shape: Ego_d.pdb's own record of this instance (debug length 1409; and of its GetPFirstThingInRoughArea siblings): parameters area /
//   filter_func / thing, function-scope `flags`, scopes it, local_area / thing_map, c1, clipped_area, cy, pcell, cx, ploop_node,
//   pmapwho, loop_thing, tthing -- the shared ROUGHAREA_EGOD GetAllThingsInRoughArea text (inline GetMapwhoArea, GetWidth() /
//   GetHeight() empty test) with the first hit returned through `thing`. Written in the row over the decl-only FIRSTROUGH opt-in.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsCreatureType.h"
#include "engine/CThingFilter_In2DRange.h"
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
template bool CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsCreatureType,CThingFilter_In2DRange,CThingFilter_In2DRange>,const CThingAICreature>(const C2DBoxI&,const CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsCreatureType,CThingFilter_In2DRange,CThingFilter_In2DRange>&,const CThingAICreature**) const;
// CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>(const C2DBoxI& area, const CThingFilter_IsNot& filter_func, const FrameUpdateThing& proc) const -- runs the (anonymous namespace) frame-update functor on every thing in the mapwho cells under the area (each thing once per search id) that is not the excluded thing; true when any call returned true
// Leaf: ??$ForEachInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CE800, 535 bytes (incl. padding), thiscall `ret 0xc`; one caller 0x006CC983.
// Names: Ego_r.pdb 0x007029E0 / FableWin.pdb 0x0247FAE0 ??$ForEachInRoughArea@VCThing@@VCThingFilter_IsNot@@UFrameUpdateThing@?A0xd4ba0483@@@
//   CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_IsNot@@ABUFrameUpdateThing@?A0xd4ba0483@@@Z (const member, `const Proc&`,
//   FrameUpdateThing a struct in an anonymous namespace); the shared FOREACHINROUGHAREA + CONSTPROC declaration.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsNot::operator() 0x006A76A0, (anonymous namespace)::FrameUpdateThing::operator() 0x006CE730 (landed), memmove 0x00BFEAE6
//   (AreaMaps.clear()). Statics NoLookups / NoThingsScanned / SearchID 0x013B89A8.., NThingMap::MaxLayers 0x013BA848, LayerInfo 0x0137DAD8.
// Shape: Ego_d.pdb has no record of this instance; the body is the GetAllThingsInRoughArea text Ego_d.pdb records for
//   cm_helper_functions.obj (shared FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD: function-scope `flags`, a const_iterator over
//   AreaMaps, thing_map / local_area / c1 / clipped_area through the inline GetMapwhoArea, cy / pcell / cx / ploop_node / pmapwho /
//   loop_thing / tthing), calling the functor after the filter, with the empty cell-box test written on the edges (0x006CE8B6
//   `cmp ebp,edx ; je`, as the shared FOREACHINROUGHAREA_BODY): the GetWidth() / GetHeight() test computes a width (551v535). This
//   shape also gives retail's level-loop exit reload order (this before the iterator, 0x006CE9C1), which the parked local model and the
//   shared FOREACHINROUGHAREA_BODY (named `maps` reference) did not.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
class CSteeringAgent;
namespace {
struct FrameUpdateThing {
    std::vector<const CSteeringAgent*>& Agents;
    FrameUpdateThing(std::vector<const CSteeringAgent*>& agents) : Agents(agents) {}
    bool operator()(const CThing& thing) const;            // 0x006CE730
};
}
template<class T,class Filter,class Proc>
bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area,const Filter& filter_func,const Proc& proc) const
{
    bool result=false;
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
            if(clipped_area.TLX==clipped_area.BRX || clipped_area.TLY==clipped_area.BRY)
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
                                if(filter_func(*tthing) && proc(*tthing))
                                    result=true;
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return result;
}
template bool CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, FrameUpdateThing>(const C2DBoxI& area,
    const CThingFilter_IsNot& filter, const FrameUpdateThing& proc) const;
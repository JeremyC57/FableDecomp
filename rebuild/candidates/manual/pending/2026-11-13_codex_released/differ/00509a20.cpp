// CWorldMap::GetMapNavigationAreaUpdateInfo(long,const C2DBoxI&,const CCharString&,CNavigationAreaUpdate&) const -- collect map navigation geometry and topology weights.
// Leaf: ?GetMapNavigationAreaUpdateInfo@CWorldMap@@QBEXJABVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaUpdate@@@Z
// Grade: DIFFER(3188v3120) -- frame 0x12C vs 0x128 and the switchable-blockage line copy (std::copy/back_inserter growth path) expand differently (+68 bytes); 12 shapes (line/blockage declaration order, insert, assignment, direct fetch, counter placement, inline index, inline prefix length, width local, 64-bit mask literal, total-layers local, iterator-reference thing, layer reference) leave or worsen it. Graded in Codex's header context (FABLE_RETAIL_NAVIGATION_UPDATE / C2DVECTOR_UI / QUEST_COMPLETION and FABLE_STLPORT_* per-feature macros = header debt)
// Flags: /Gr
#define FABLE_RETAIL_NAVIGATION_UPDATE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_STLPORT_UI_ACTIONS
#define FABLE_RETAIL_QUEST_COMPLETION
#include <string.h>
#include "rebuild_abi.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CNavigationAreaUpdate.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCGraphicAppearance.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CStringParser.h"
#include "engine/CMap.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_OverlappingArea.h"
#include "engine/CThingFilter_HasNavigationInformation.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
#include "stlport/navigation_algorithms.h"
extern const CCharString NAVIGATION_TRANSITION_POINT;
extern const CCharString ACTION_POINT;
CCharString __fastcall operator+(const CCharString&,const char*);

#pragma auto_inline(off)
C2DBoxI CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long map_number,const C2DBoxI& area)const{
 const CMapInfo& map=Maps[map_number];
 C2DBoxI clipped_area=area;C2DBoxI clip_area=map.Area;
 clipped_area.ClipTo(clip_area);
 clipped_area.TLX-=map.Area.TLX;clipped_area.TLY-=map.Area.TLY;
 clipped_area.BRX-=map.Area.TLX;clipped_area.BRY-=map.Area.TLY;
 return clipped_area;
}

template<> void CThingSearchTools::GetAllThings<CThingPhysical,std::list<const CThingPhysical*>,CFilter_Bind3<CThingFilter_OverlappingArea,CThingFilter_HasNavigationInformation,CThingFilter_IsInQuestOrNullQuest> >(const CFilter_Bind3<CThingFilter_OverlappingArea,CThingFilter_HasNavigationInformation,CThingFilter_IsInQuestOrNullQuest>& filter,std::list<const CThingPhysical*>& container)const {
    const std::list<CClass*>& things=PeekClassList();
    for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
        const CThing* thing=static_cast<const CThing*>(*it);
        if(thing->IsIncludedInThingScans() && (thing->GetTypeFlags()&CThingPhysical::StatGetTypeFlags())==CThingPhysical::StatGetTypeFlags()) {
            const CThingPhysical* physical=static_cast<const CThingPhysical*>(thing);
            if(filter(*physical))container.push_back(physical);
        }
    }
}
namespace std {
template<>
CNavigationTransitionPoint* __fastcall __uninitialized_copy(CNavigationTransitionPoint* first,CNavigationTransitionPoint* last,CNavigationTransitionPoint* result,const __false_type&) {
    CNavigationTransitionPoint* current=result;
    for(;first!=last;++first,++current)_Construct(&*current,*first);
    return current;
}
template<>
back_insert_iterator<list<C2DLineF> > __fastcall __copy(C2DLineF* first,C2DLineF* last,back_insert_iterator<list<C2DLineF> > result,const random_access_iterator_tag&,int*) {
    for(int n=last-first;n>0;--n) { *result=*first;++first;++result; }
    return result;
}
template<> template<>
void list<C2DLineF>::_M_insert_dispatch<list<C2DLineF>::const_iterator>(iterator position,const_iterator first,const_iterator last,const __false_type&) {
    for(;first!=last;++first) insert(position,*first);
}
template<>
CNavigationAreaUpdate::CSwitchableBlockage* __fastcall __uninitialized_copy(CNavigationAreaUpdate::CSwitchableBlockage* first,CNavigationAreaUpdate::CSwitchableBlockage* last,CNavigationAreaUpdate::CSwitchableBlockage* result,const __false_type&) {
    CNavigationAreaUpdate::CSwitchableBlockage* current=result;
    for(;first!=last;++first,++current)_Construct(&*current,*first);
    return current;
}
}
#pragma auto_inline(on)

void CWorldMap::GetMapNavigationAreaUpdateInfo(long map_no,const C2DBoxI& area,const CCharString& section_name,CNavigationAreaUpdate& area_info)const {
    const CThingSearchTools& search_tools=World->PeekThingSearchTools();
    std::list<const CThingPhysical*> pthing_list;
    search_tools.GetAllThings<CThingPhysical>(GFFilter_Bind3(CThingFilter_OverlappingArea(area),CThingFilter_HasNavigationInformation(),CThingFilter_IsInQuestOrNullQuest(section_name)),pthing_list);
    area_info.NavigationLayers.clear();
    for(std::list<const CThingPhysical*>::const_iterator i_list=pthing_list.begin();i_list!=pthing_list.end();++i_list) {
        const CThingPhysical* p_thing=*i_list;
        long base_nav_layer=GetNavigationLayerAt(p_thing->GetPos(),p_thing);
        long no_of_nav_layers=p_thing->GetNoNavCollisionLineLists();
        if(base_nav_layer+no_of_nav_layers>area_info.NavigationLayers.size())area_info.NavigationLayers.resize(base_nav_layer+no_of_nav_layers);
        if(p_thing->HasTC(TCI_SWITCHABLE_NAVIGATION)) {
            for(long nav_layer=0;nav_layer<no_of_nav_layers;++nav_layer) {
                CNavigationAreaUpdate::CSwitchableBlockage switchable_blockage;
                switchable_blockage.UniqueID=p_thing->GetUniqueID()&((((unsigned __int64)1)<<40)-1);
                std::vector<C2DLineF> line_list;
                p_thing->GetNavCollisionLineList(line_list,nav_layer);
                std::copy(line_list.begin(),line_list.end(),std::back_inserter(switchable_blockage.LineList));
                area_info.NavigationLayers[base_nav_layer+nav_layer].SwitchableBlockages.push_back(switchable_blockage);
            }
        } else {
            for(long nav_layer=0;nav_layer<no_of_nav_layers;++nav_layer)p_thing->GetNavCollisionLineList(area_info.NavigationLayers[base_nav_layer+nav_layer].LineList,nav_layer);
        }
        std::vector<CCharString> action_point_names;
        p_thing->GetTCGraphicAppearance()->GetAllDummyObjectNames(action_point_names);
        long action_point_index=0;
        for(std::vector<CCharString>::const_iterator name_it=action_point_names.begin();name_it!=action_point_names.end();++name_it) {
            const CCharString& name=*name_it;
            if(name.GetLength()>=12 && name.GetSubString(0,12)=="ACTION_POINT") {
                C3DVector pos;
                CRightHandedSet rhset;
                long index_to_use=name==ACTION_POINT?action_point_index:0;
                if(p_thing->GetTCGraphicAppearance()->GetDummyObject(name,index_to_use,0,pos,rhset,1.0f)) {
                    if(area.ContainsPoint(pos.ToC2DCoordI()))
                        area_info.ActionPoints.push_back(CNavigationPosition(C2DVector(pos.X,pos.Y),GetNavigationLayerAt(pos)));
                }
                ++action_point_index;
            }
        }
        if(p_thing->GetPhysicsTC()->GetPhysicsMeshInfo() && !p_thing->HasTC(TCI_SWITCHABLE_NAVIGATION)) {
            CPhysicsMeshObject object=p_thing->GetPhysicsTC()->GetPhysicsMeshObject();
            std::vector<CCharString> helper_points;
            object.GetAllHelperPointNames(helper_points);
            CCharString last_trans_point_name;
            long index_into_trans_points=0;
            long length_of_trans_point_name=NAVIGATION_TRANSITION_POINT.GetLength();
            for(std::vector<CCharString>::const_iterator tp_it=helper_points.begin();tp_it!=helper_points.end();++tp_it) {
                const CCharString& trans_point_name=*tp_it;
                if(trans_point_name.GetLength()>=length_of_trans_point_name && trans_point_name.GetSubString(0,length_of_trans_point_name)==NAVIGATION_TRANSITION_POINT) {
                    if(trans_point_name!=last_trans_point_name) {last_trans_point_name=trans_point_name;index_into_trans_points=0;}else ++index_into_trans_points;
                    C2DVector helper_pos=object.GetHelperPointPositionStatic(trans_point_name,index_into_trans_points);
                    if(area.ContainsPoint(helper_pos.ToC2DCoordI())) {
                        if(trans_point_name==NAVIGATION_TRANSITION_POINT) {
                            area_info.TransitionPoints.push_back(CNavigationTransitionPoint(helper_pos,0,1));
                        } else {
                            CStringParser parser(trans_point_name);
                            parser.SkipPastString(NAVIGATION_TRANSITION_POINT+"_");
                            long layer1=parser.ReadAsInteger()-1;
                            parser.SkipPastString("_");
                            long layer2=parser.ReadAsInteger()-1;
                            area_info.TransitionPoints.push_back(CNavigationTransitionPoint(helper_pos,layer1,layer2));
                        }
                    }
                }
            }
        }
    }
    area_info.TopologyWeights.resize((area.BRX-area.TLX)*(area.BRY-area.TLY));
    C2DBoxI local_box=ClipAndConvertWorldMapAreaToLocalArea(map_no,area);
    const CMap& map=PeekMap(map_no);
    for(long mapy=local_box.TLY,areay=0;mapy<local_box.BRY;++mapy,++areay) {
        for(long mapx=local_box.TLX,areax=0;mapx<local_box.BRX;++mapx,++areax) {
            if(map.IsPassableAt(C2DCoordI(mapx,mapy))) {
                if(map.IsPrefNavAt(C2DCoordI(mapx,mapy)))area_info.TopologyWeights[areay*(area.BRX-area.TLX)+areax]=0;
                else area_info.TopologyWeights[areay*(area.BRX-area.TLX)+areax]=128;
            }else area_info.TopologyWeights[areay*(area.BRX-area.TLX)+areax]=255;
        }
    }
}

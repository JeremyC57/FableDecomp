// CWorldMap::GetMapNavigationAreaUpdateInfo(long,const C2DBoxI&,const CCharString&,CNavigationAreaUpdate&) const -- collect map navigation geometry and topology weights.
// Leaf: ?GetMapNavigationAreaUpdateInfo@CWorldMap@@QBEXJABVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaUpdate@@@Z
// Grade: DIFFER(3272v3120) 152 hunks (B24S1 port onto today's headers + hdr/ overlay) -- call sequence = retail except the vector<CSwitchableBlockage>::push_back overflow path (retail calls the implicit copy ctor 0x005199E0 there, ours expands it); frame 0x13C v 0x128 (+20), prologue zeroes an extra dword
// Flags: /Gr
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DBOXI_CONTAINSPOINT_BODY
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_HELPERPOINTS
#define FABLE_RETAIL_CNAVIGATIONTRANSITIONPOINT_CTOR
#define FABLE_RETAIL_C2DVECTOR_TOC2DCOORDI
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECTNAMES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGPHYSICAL_NAVCOLLISIONLINES
#define FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS
#define FABLE_RETAIL_CWORLDMAP_HEIGHTAREA
#define FABLE_RETAIL_CTHINGFILTER_ISINQUESTORNULLQUEST_INLINE_CTOR
#define FABLE_RETAIL_STLP_COPY_GENERIC_INLINE
#include <string.h>
#include "fable_string.h"
#include "rebuild_abi.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CNavigationAreaUpdate.h"
#include "engine/CNavigationPosition.h"
#include "engine/CNavQuadTree.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CStringParser.h"
#include "engine/CMap.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_OverlappingArea.h"
#include "engine/CThingFilter_HasNavigationInformation.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
extern const CCharString NAVIGATION_TRANSITION_POINT;
extern const CCharString ACTION_POINT;
CCharString __fastcall operator+(const CCharString&,const char*);

#pragma auto_inline(off)
namespace std {
template<>
CNavigationTransitionPoint* __fastcall __uninitialized_copy(CNavigationTransitionPoint* first,CNavigationTransitionPoint* last,CNavigationTransitionPoint* result,const __false_type&) {
    CNavigationTransitionPoint* current=result;
    for(;first!=last;++first,++current)_Construct(&*current,*first);
    return current;
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

// FableWin.pdb ?GFGetMapThingUIDFromThingUID@@YI_K_K@Z (FableWin 0x01C903B0, defined after this function there), inline in retail.
inline unsigned __int64 __fastcall GFGetMapThingUIDFromThingUID(unsigned __int64 uid) { return uid & 0xFFFFFFFFFFui64; }

void CWorldMap::GetMapNavigationAreaUpdateInfo(long map_no,const C2DBoxI& area,const CCharString& section_name,CNavigationAreaUpdate& area_info)const {
    const CThingSearchTools& search_tools=World.PeekThingSearchTools();
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
                switchable_blockage.UniqueID=GFGetMapThingUIDFromThingUID(p_thing->GetUniqueID());
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
                        area_info.ActionPoints.push_back(CNavigationPosition(C2DVector(pos.X,pos.Y),GetNavigationLayerAt(pos,0)));
                }
                ++action_point_index;
            }
        }
        if(p_thing->GetPhysicsTC()->HasPhysicsMeshObject() && !p_thing->HasTC(TCI_SWITCHABLE_NAVIGATION)) {
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
    area_info.TopologyWeights.resize(area.GetWidth()*area.GetHeight());
    C2DBoxI local_box=ClipAndConvertWorldMapAreaToLocalArea(map_no,area);
    const CMap& map=PeekMap(map_no);
    for(long mapy=local_box.TLY,areay=0;mapy<local_box.BRY;++mapy,++areay) {
        for(long mapx=local_box.TLX,areax=0;mapx<local_box.BRX;++mapx,++areax) {
            if(map.IsPassableAt(C2DCoordI(mapx,mapy))) {
                if(map.IsPrefNavAt(C2DCoordI(mapx,mapy)))area_info.TopologyWeights[areay*area.GetWidth()+areax]=0;
                else area_info.TopologyWeights[areay*area.GetWidth()+areax]=128;
            }else area_info.TopologyWeights[areay*area.GetWidth()+areax]=255;
        }
    }
}

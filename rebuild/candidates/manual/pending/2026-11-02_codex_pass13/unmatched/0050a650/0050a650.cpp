// CWorldMap::GetMapNavigationAreaInit -- collect detailed areas and navigation seeds.
// Leaf: ?GetMapNavigationAreaInit@CWorldMap@@IBEXJPBVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaInit@@@Z
// Grade: DIFFER(2233v2233) -- four native bytes differ at final empty-loop edge and vector cleanup register.
#define FABLE_RETAIL_REGION_CONNECTIVITY_TYPES
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHING_NAVIGATION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_NAVIGATION_HELPERS
#define FABLE_RETAIL_C2DBOXF
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingObject.h"
#include "engine/CThingVillage.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CNavigationAreaInit.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
#include "engine/CThingFilter_OverlappingArea.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InArea.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Bind3.h"
class CThingBuilding; class CThingObject; class CThingVillage; class CThingAICreature;
class CTCDNavigationSeed; class CTCDRegionExit;
inline C2DVector C2DCoordI::ToC2DCoordF()const{return C2DVector((float)X,(float)Y);}
inline C2DVector::C2DVector(const C3DVector& p):X(p.X),Y(p.Y){}
void CWorldMap::GetMapNavigationAreaInit(long map_no,const C2DBoxI* pworld_box,const CCharString& section_name,CNavigationAreaInit& area_init) const
{
    C2DBoxI box;
    if(!pworld_box) { box=Maps[map_no].Area; pworld_box=&box; }
    GetMapNavigationAreaUpdateInfo(map_no,*pworld_box,section_name,area_init);
    std::list<const CThingPhysical*> phigh_detail_list;
    const CThingSearchTools& search_tools=World.PeekThingSearchTools();
    search_tools.GetAllLeafThings<CThingBuilding>(GFFilter_Bind2(CThingFilter_IsInQuestOrNullQuest(section_name),CThingFilter_OverlappingArea(*pworld_box)),phigh_detail_list);
    search_tools.GetAllLeafThings<CThingObject>(GFFilter_Bind2(CThingFilter_IsInQuestOrNullQuest(section_name),CThingFilter_OverlappingArea(*pworld_box)),phigh_detail_list);
    for(std::list<const CThingPhysical*>::const_iterator i_blist=phigh_detail_list.begin();i_blist!=phigh_detail_list.end();++i_blist) {
        const CThingPhysical* p_thing=*i_blist;
        if(p_thing->UseHighDetailQuadTree() && p_thing->GetPhysicsTC()) {
            C3DVector centre_pos;
            float radius=0;
            p_thing->GetPhysicsTC()->GetBoundingInfo(centre_pos,radius);
            C2DVector tl_pos(centre_pos.X-radius,centre_pos.Y-radius);
            if(tl_pos.X<pworld_box->TLX) tl_pos.X=(float)pworld_box->TLX;
            if(tl_pos.Y<pworld_box->TLY) tl_pos.Y=(float)pworld_box->TLY;
            C2DVector br_pos(centre_pos.X+radius,centre_pos.Y+radius);
            if(br_pos.X>pworld_box->BRX) br_pos.X=(float)pworld_box->BRX;
            if(br_pos.Y>pworld_box->BRY) br_pos.Y=(float)pworld_box->BRY;
            area_init.DetailedAreas.push_back(C2DBoxF(tl_pos,br_pos));
        }
    }
    area_init.WorldArea=*pworld_box;
    {
    std::list<const CThingPhysical*> nthing_list;
    search_tools.GetAllLeafThings<CThingVillage>(GFFilter_Bind2(CThingFilter_IsInQuestOrNullQuest(section_name),CThingFilter_OverlappingArea(*pworld_box)),nthing_list);
    for(std::list<const CThingPhysical*>::const_iterator i_list=nthing_list.begin();i_list!=nthing_list.end();++i_list) {
        const CThingPhysical* p_thing=*i_list;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    nthing_list.clear();
    search_tools.GetAllLeafThings<CThingAICreature>(GFFilter_Bind2(CThingFilter_IsInQuestOrNullQuest(section_name),CThingFilter_OverlappingArea(*pworld_box)),nthing_list);
    for(std::list<const CThingPhysical*>::const_iterator i_list=nthing_list.begin();i_list!=nthing_list.end();++i_list) {
        const CThingPhysical* p_thing=*i_list;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    {
    std::vector<const CTCCreatureGenerator*> creature_generators;
    search_tools.GetAllTCsInRoughArea<CTCCreatureGenerator>(*pworld_box,CThingFilter_IsInQuestOrNullQuest(section_name),creature_generators);
    for(std::vector<const CTCCreatureGenerator*>::const_iterator creat_gen_it=creature_generators.begin();creat_gen_it!=creature_generators.end();++creat_gen_it) {
        const CTCCreatureGenerator* ptc_generator=*creat_gen_it;
        const CThing& thing=ptc_generator->GetParentThing();
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(thing.GetPos()),GetNavigationLayerAt(thing.GetPos(),0)));
    }
    {
    std::vector<const CThing*> navigation_seeds;
    search_tools.GetAllTCDsInRoughArea<CTCDNavigationSeed>(*pworld_box,CThingFilter_IsInQuestOrNullQuest(section_name),navigation_seeds);
    for(std::vector<const CThing*>::const_iterator nav_seed_it=navigation_seeds.begin();nav_seed_it!=navigation_seeds.end();++nav_seed_it) {
        const CThing* p_thing=*nav_seed_it;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    {
    C2DBoxF area_in_floating_point(pworld_box->GetTLPos().ToC2DCoordF(),pworld_box->GetBRPos().ToC2DCoordF());
    std::vector<const CThing*> region_entrances;
    search_tools.GetAllThings<CThing>(GFFilter_Bind3(CThingFilter_IsInQuestOrNullQuest(section_name),CThingFilter_HasTC(TCI_REGION_ENTRANCE),CThingFilter_InArea(area_in_floating_point)),region_entrances);
    for(std::vector<const CThing*>::const_iterator entrance_it=region_entrances.begin();entrance_it!=region_entrances.end();++entrance_it) {
        const CThing* p_thing=*entrance_it;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    {
    std::vector<const CThing*> region_exits;
    search_tools.GetAllTCDsInRoughArea<CTCDRegionExit>(*pworld_box,CThingFilter_IsInQuestOrNullQuest(section_name),region_exits);
    for(std::vector<const CThing*>::const_iterator exit_it=region_exits.begin();exit_it!=region_exits.end();++exit_it) {
        const CThing* p_thing=*exit_it;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    }
    }
    }
    }
    }
}

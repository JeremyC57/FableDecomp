// CWorldMap::GetMapNavigationAreaInit(long, const C2DBoxI*, const CCharString&, CNavigationAreaInit&) -- collect detailed areas and navigation seeds.
// Grade: DIFFER(2233v2233) -- 6 lines / 3 instructions (unchanged by chunk 856's 100 grades): the final region_exits vector's free reloads _M_start into ebx (retail eax), so the empty-loop edge skips the reload (je 0x0050AE63 vs retail 0x0050AE5F); every other byte and all 18 relocations agree
// Next idea: a register-colouring tie, not a spelling: the dtor's _M_start temporary takes the loop iterator's register and the empty edge then threads past the reload; chunk 856 tried last-loop body / iterator / while / predeclared / reversed / postfix forms, every scope-block removal, all-loop spellings and the vector-base dtor shapes (DIAG overlay: one null test, if-free) -- next compare with a landed row that frees a vector right after an iterator loop over it
// Leaf: ?GetMapNavigationAreaInit@CWorldMap@@IBEXJPBVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050A650, 2233 bytes, thiscall, `ret 10h`, frame 0x7c. Leaf from Ego_r.pdb (0x004DF950) and FableWin.pdb
//   (0x01C905D0): protected const (IBE). Retry of chunk 850's SRC_FAIL; body = the Codex pass-13 attempt-8 body (pending/2026-11-02_codex_pass13).
// Fixes in chunk 853 (on top of chunk 850's n2): header opt-ins C2DCoordI TOFLOAT (reordered), CThingSearchTools TCSINROUGHAREA / LEAFBODY /
//   ALLTHINGSBODY, CNavigationPosition CTORS, StatGetType for CThingVillage / CThingAICreature; FABLE_RETAIL_C2DBOXF_CORNERS (the corner
//   ctor was an out-of-line call at both sites; retail inlines it: 2284 -> 2252 bytes); FABLE_RETAIL_C2DBOXF_COPY (new nested opt-in: the
//   authored copy ctor, FableWin ??0C2DBoxF@@QAE@ABV0@@Z 0x01A664E0, lets the CThingFilter_InArea copy take the four fild results directly:
//   frame 0x80 -> 0x7c, 2252 -> 2233 bytes); FABLE_RETAIL_CARRAY (Maps[map_no] through begin(): retail's ecx/edx order).
// Callees (all relocations paired with retail, none bound elsewhere): GetMapNavigationAreaUpdateInfo 0x00509A20, GetAllLeafThings<Building /
//   Object / Village / AICreature> 0x005163B0 / 0x00516530 / 0x005166B0 / 0x00516830, GetAllTCsInRoughArea<CTCCreatureGenerator> 0x00518340,
//   GetAllTCDsInRoughArea<CTCDNavigationSeed / CTCDRegionExit> 0x005185F0 / 0x00518900, GetAllThings<CThing> 0x00518850, the two
//   _M_insert_overflow 0x00517BA0 (CNavigationPosition) / 0x00517CB0 (C2DBoxF), CThing::GetPos 0x004C73D0, GetNavigationLayerAt 0x0063E590,
//   CTCPhysicsBase::GetBoundingInfo 0x006AF640, CCharString copy 0x0099EC30 / dtor 0x0099EAE0, malloc 0x00BFEA0E / free 0x00BFEA14;
//   virtual CThingPhysical slot 57 (+0xe4) UseHighDetailQuadTree (descriptive slot view below).
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
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
#define FABLE_RETAIL_CTHINGVILLAGE_STATGETTYPE
#define FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS
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



#include "engine/CThingFilter_HasTC.h"

#include "engine/CFilter_Bind2.h"

#include "engine/CNavigationPosition.h"
#include "engine/C2DBoxF.h"
// Row-local models (no shared header yet; FableWin.pdb ctor signatures): the three filters, CFilter_Bind3, the navigation area records.
class CThingFilter_IsInQuestOrNullQuest {
public:
    CThingFilter_IsInQuestOrNullQuest(const CCharString& name) : QuestSectionName(name) { }
    bool operator()(const CThing& thing) const;                          // 0x006A7C60
    CCharString QuestSectionName;                                        // +0x00
};
class CThingFilter_OverlappingArea {
public:
    CThingFilter_OverlappingArea(const C2DBoxI& area) : Area(&area) { }
    bool operator()(const CThing& thing) const;
    const C2DBoxI* Area;                                                 // +0x00
};
class CThingFilter_InArea {
public:
    CThingFilter_InArea(const C2DBoxF& area) : Area(area) { }
    bool operator()(const CThing& thing) const;                          // 0x006A7990
    C2DBoxF Area;                                                        // +0x00
};
template<class A,class B,class C> class CFilter_Bind3 {
public:
    CFilter_Bind3(const A& a,const B& b,const C& c) : F1(a), F2(b), F3(c) { }
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing) && F3(thing); }
    const A& F1;
    const B& F2;
    const C& F3;
};
template<class A,class B,class C> CFilter_Bind3<A,B,C> __fastcall GFFilter_Bind3(const A& a,const B& b,const C& c) { return CFilter_Bind3<A,B,C>(a,b,c); }
class CNavigationAreaUpdate {
public:
    unsigned char _containers[0x30];   // NavigationLayers, TransitionPoints, ActionPoints, TopologyWeights (STLport vectors)
};
class CNavigationAreaInit : public CNavigationAreaUpdate {
public:
    C2DBoxI WorldArea;                                                   // +0x30
    long MapIndex;                                                       // +0x40
    std::vector<C2DBoxF> DetailedAreas;                                  // +0x44
    std::vector<CNavigationPosition> NavRegionSeeds;                     // +0x50
};
class CThingPhysicalView {                         // descriptive view: CThingPhysical virtual slot 57 (+0xe4), UseHighDetailQuadTree
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual bool UseHighDetailQuadTree() const;   // slot 57 (Ego_r ?UseHighDetailQuadTree@CThingObject@@UBE_NXZ / CThingBuilding)
};
class CThingBuilding; class CThingObject; class CThingVillage; class CThingAICreature;
class CTCDNavigationSeed; class CTCDRegionExit;
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
        if(reinterpret_cast<const CThingPhysicalView*>(p_thing)->UseHighDetailQuadTree() && p_thing->GetPhysicsTC()) {
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
    for(std::vector<const CThing*>::const_iterator exit_it=region_exits.begin(),exit_end=region_exits.end();exit_it!=exit_end;++exit_it) {
        const CThing* p_thing=*exit_it;
        area_init.NavRegionSeeds.push_back(CNavigationPosition(C2DVector(p_thing->GetPos()),GetNavigationLayerAt(p_thing->GetPos(),0)));
    }
    }
    }
    }
    }
    }
}

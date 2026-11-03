#include <vector>
#include <list>
#include <string>
#include <cassert>
#include <cstdio>
#include <cmath>
class CCharString {public:std::string text;CCharString(){}CCharString(const char* p):text(p){}};
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class C2DVector {public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C3DVector&p):X(p.X),Y(p.Y){}};
class C2DCoordI {public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}C2DVector ToC2DCoordF()const{return C2DVector((float)X,(float)Y);}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(){}C2DBoxF(const C2DVector&a,const C2DVector&b):TLX(a.X),TLY(a.Y),BRX(b.X),BRY(b.Y){}};
class CNavigationPosition {public:C2DVector Position;long Layer;CNavigationPosition(const C2DVector&p,long l):Position(p),Layer(l){}};
class CNavigationAreaInit {public:C2DBoxI WorldArea;long MapIndex;std::vector<C2DBoxF>DetailedAreas;std::vector<CNavigationPosition>NavRegionSeeds;};
static C2DBoxI expected_box;
static std::string expected_section;
static std::vector<int> events;
static bool same_box(const C2DBoxI&a,const C2DBoxI&b){return a.TLX==b.TLX&&a.TLY==b.TLY&&a.BRX==b.BRX&&a.BRY==b.BRY;}
class CTCPhysicsBase {public:C3DVector centre;float radius;mutable int bounds_calls;CTCPhysicsBase():centre(0,0,0),radius(0),bounds_calls(0){}void GetBoundingInfo(C3DVector& c,float&r)const{++bounds_calls;c=centre;r=radius;}};
class CThing {public:C3DVector position;mutable int pos_calls;CThing():position(0,0,0),pos_calls(0){}const C3DVector&GetPos()const{++pos_calls;return position;}};
class CThingPhysical:public CThing {public:bool detail;CTCPhysicsBase*physics;mutable int detail_calls;CThingPhysical():detail(false),physics(0),detail_calls(0){}bool UseHighDetailQuadTree()const{++detail_calls;return detail;}const CTCPhysicsBase*GetPhysicsTC()const{return physics;}};
class CThingBuilding {public:enum{Id=0};};class CThingObject {public:enum{Id=1};};class CThingVillage {public:enum{Id=2};};class CThingAICreature {public:enum{Id=3};};
class CTCCreatureGenerator {public:const CThing*parent;CTCCreatureGenerator():parent(0){}const CThing&GetParentThing()const{return *parent;}};
class CTCDNavigationSeed {public:enum{Id=0};};class CTCDRegionExit {public:enum{Id=1};};
enum ETCInterfaceType {TCI_REGION_ENTRANCE=99};
class CThingFilter_IsInQuestOrNullQuest {public:CCharString section;CThingFilter_IsInQuestOrNullQuest(const CCharString&s):section(s){assert(s.text==expected_section);}};
class CThingFilter_OverlappingArea {public:const C2DBoxI& area;CThingFilter_OverlappingArea(const C2DBoxI&a):area(a){assert(same_box(a,expected_box));}};
class CThingFilter_HasTC {public:ETCInterfaceType type;CThingFilter_HasTC(ETCInterfaceType t):type(t){assert(t==TCI_REGION_ENTRANCE);}};
class CThingFilter_InArea {public:C2DBoxF area;CThingFilter_InArea(const C2DBoxF&a):area(a){assert(a.TLX==(float)expected_box.TLX&&a.TLY==(float)expected_box.TLY&&a.BRX==(float)expected_box.BRX&&a.BRY==(float)expected_box.BRY);}};
template<class A,class B>class CFilter_Bind2 {public:const A&a;const B&b;CFilter_Bind2(const A&x,const B&y):a(x),b(y){}};
template<class A,class B>CFilter_Bind2<A,B> GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
template<class A,class B,class C>class CFilter_Bind3 {public:const A&a;const B&b;const C&c;CFilter_Bind3(const A&x,const B&y,const C&z):a(x),b(y),c(z){}};
template<class A,class B,class C>CFilter_Bind3<A,B,C> GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
class CThingSearchTools {public:
 std::vector<const CThingPhysical*>leaves[4];std::vector<const CTCCreatureGenerator*>generators;std::vector<const CThing*>tags[2],entrances;
 template<class T,class Container,class Filter>void GetAllLeafThings(const Filter&,Container&out)const{events.push_back(10+T::Id);for(unsigned i=0;i<leaves[T::Id].size();++i)out.push_back(leaves[T::Id][i]);}
 template<class T,class Filter,class Container>void GetAllTCsInRoughArea(const C2DBoxI&box,const Filter&,Container&out)const{assert(same_box(box,expected_box));events.push_back(14);for(unsigned i=0;i<generators.size();++i)out.push_back(generators[i]);}
 template<class T,class Filter,class Container>void GetAllTCDsInRoughArea(const C2DBoxI&box,const Filter&,Container&out)const{assert(same_box(box,expected_box));events.push_back(15+T::Id*2);for(unsigned i=0;i<tags[T::Id].size();++i)out.push_back(tags[T::Id][i]);}
 template<class T,class Container,class Filter>void GetAllThings(const Filter&,Container&out)const{events.push_back(16);for(unsigned i=0;i<entrances.size();++i)out.push_back(entrances[i]);}
};
class CWorld {public:CThingSearchTools search;const CThingSearchTools&PeekThingSearchTools()const{return search;}};
class CMapInfo {public:C2DBoxI Area;};
class CWorldMap {public:CWorld&World;std::vector<CMapInfo>Maps;mutable int update_calls,layer_calls;CWorldMap(CWorld&w):World(w),update_calls(0),layer_calls(0){}
 void GetMapNavigationAreaUpdateInfo(long n,const C2DBoxI&b,const CCharString&s,CNavigationAreaInit&a)const{assert(events.empty());events.push_back(1);++update_calls;assert(same_box(b,expected_box)&&s.text==expected_section);a.MapIndex=n;}
 long GetNavigationLayerAt(const C3DVector&p,const CThing*t)const{assert(t==0);++layer_calls;return (long)p.Z;}
 void GetMapNavigationAreaInit(long,const C2DBoxI*,const CCharString&,CNavigationAreaInit&)const;
};

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

static C2DBoxF expected_detail(const CTCPhysicsBase& p,const C2DBoxI& b) {
 C2DBoxF out;out.TLX=p.centre.X-p.radius;out.TLY=p.centre.Y-p.radius;out.BRX=p.centre.X+p.radius;out.BRY=p.centre.Y+p.radius;
 if(out.TLX<(float)b.TLX)out.TLX=(float)b.TLX;if(out.TLY<(float)b.TLY)out.TLY=(float)b.TLY;
 if(out.BRX>(float)b.BRX)out.BRX=(float)b.BRX;if(out.BRY>(float)b.BRY)out.BRY=(float)b.BRY;return out;
}
static bool same_detail(const C2DBoxF&a,const C2DBoxF&b){return a.TLX==b.TLX&&a.TLY==b.TLY&&a.BRX==b.BRX&&a.BRY==b.BRY;}
static void scenario(unsigned mask,bool explicit_box,bool prefix,int section_id,int detail_mode) {
 events.clear();expected_section=section_id==0?"":section_id==1?"quest/one":"Quest TWO";
 CWorld world;CWorldMap map(world);map.Maps.resize(2);map.Maps[0].Area=C2DBoxI(100,100,200,200);map.Maps[1].Area=C2DBoxI(-10,-6,10,12);
 C2DBoxI supplied(-4,-3,8,9);expected_box=explicit_box?supplied:map.Maps[1].Area;C2DBoxI before=supplied;
 CTCPhysicsBase physics[6];CThingPhysical detail[6];const float x[6]={0,-12,12,100,-5,8},y[6]={0,-9,13,100,8,-3},r[6]={2,4,5,1,0,3};
 std::vector<C2DBoxF>wanted_details;
 CNavigationAreaInit out;std::vector<CNavigationPosition>wanted_seeds;
 if(prefix){C2DBoxF p(C2DVector(-100,-99),C2DVector(-98,-97));out.DetailedAreas.push_back(p);wanted_details.push_back(p);CNavigationPosition n(C2DVector(-99,-88),777);out.NavRegionSeeds.push_back(n);wanted_seeds.push_back(n);}
 for(int i=0;i<6;++i){physics[i].centre=C3DVector(x[i],y[i],(float)i);physics[i].radius=r[i];detail[i].detail=detail_mode==1||detail_mode==3||(detail_mode==2&&i%2==0);detail[i].physics=detail_mode==3&&i%2==0?0:&physics[i];world.search.leaves[i<3?0:1].push_back(&detail[i]);if(detail[i].detail&&detail[i].physics)wanted_details.push_back(expected_detail(physics[i],expected_box));}
 CThingPhysical seeds[6][3];CTCCreatureGenerator generators[3];int seed_count=0;
 for(int group=0;group<6;++group){
  for(int j=0;j<3;++j){int position_group=group==4?3:group;seeds[group][j].position=C3DVector((float)(position_group*3+j-8),(float)(j*2-group),(float)(group-j));}
  if(!(mask&(1u<<group)))continue;
  for(int j=0;j<3;++j){CThingPhysical* p=&seeds[group][j];wanted_seeds.push_back(CNavigationPosition(C2DVector(p->position), (long)p->position.Z));++seed_count;
   if(group<2)world.search.leaves[group+2].push_back(p);
   else if(group==2){generators[j].parent=p;world.search.generators.push_back(&generators[j]);}
   else if(group==3)world.search.tags[0].push_back(p);
   else if(group==4)world.search.entrances.push_back(p);
   else world.search.tags[1].push_back(p);
  }
 }
 CCharString section(expected_section.c_str());map.GetMapNavigationAreaInit(1,explicit_box?&supplied:0,section,out);
 assert(map.update_calls==1&&map.layer_calls==seed_count&&out.MapIndex==1);assert(same_box(out.WorldArea,expected_box)&&same_box(supplied,before));assert(same_box(map.Maps[1].Area,C2DBoxI(-10,-6,10,12)));assert(section.text==expected_section);
 assert(events.size()==9);assert(events[0]==1);for(int i=1;i<9;++i)assert(events[i]==9+i);
 assert(out.DetailedAreas.size()==wanted_details.size());for(unsigned i=0;i<wanted_details.size();++i)assert(same_detail(out.DetailedAreas[i],wanted_details[i]));
 assert(out.NavRegionSeeds.size()==wanted_seeds.size());for(unsigned i=0;i<wanted_seeds.size();++i){assert(out.NavRegionSeeds[i].Position.X==wanted_seeds[i].Position.X&&out.NavRegionSeeds[i].Position.Y==wanted_seeds[i].Position.Y&&out.NavRegionSeeds[i].Layer==wanted_seeds[i].Layer);}
 for(int i=0;i<6;++i){assert(detail[i].detail_calls==1);assert(physics[i].bounds_calls==(detail[i].detail&&detail[i].physics?1:0));}
 for(int g=0;g<6;++g)for(int j=0;j<3;++j)assert(seeds[g][j].pos_calls==((mask&(1u<<g))?2:0));
}
int main(){int count=0;for(unsigned mask=0;mask<64;++mask)for(int box=0;box<2;++box)for(int prefix=0;prefix<2;++prefix)for(int section=0;section<3;++section)for(int detail=0;detail<4;++detail){scenario(mask,box!=0,prefix!=0,section,detail);++count;}std::fprintf(stderr,"SCENARIOS %d\n",count);assert(count==3072);std::puts("0050a650_TEST PASS");return 0;}

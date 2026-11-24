#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
namespace CombatSequenceTest {
void check(bool ok){if(!ok)exit(1);}
struct CThing {};
struct CThingCreatureBase : CThing {
    C3DVector p; bool player,knocked;
    CThingCreatureBase():p(0,0,1),player(false),knocked(false){}
    const C3DVector& GetPos()const{return p;}
    bool IsPlayerCharacter()const{return player;}
};
namespace NCreatureMode { bool IsKnockedDown(const CThingCreatureBase& d){return d.knocked;} }
struct CCombatSequenceBase {
    bool melee,into,boast,prefer,valid,zone;
    mutable int zones,prefs;
    CCombatSequenceBase():melee(false),into(false),boast(false),prefer(false),valid(true),zone(true),zones(0),prefs(0){}
    bool IsMeleeSequence()const{return melee;}
    bool IsBreakIntoMeleeSequence()const{return into;}
    bool IsBoastSequence()const{return boast;}
    bool IsValid()const{return valid;}
    bool IsZoneValid()const{++zones;return zone;}
    bool IsToPreferSequenceWhenValid()const{++prefs;return prefer;}
};
template<class T>struct CCountedPointer{
    T* p;explicit CCountedPointer(T* q):p(q){}
    T* Get()const{return p;} T& operator*()const{return *p;}
};
struct CCombatantInfo{
    CThingCreatureBase* Parent;
    struct Attack{std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;long CurrentSequence;Attack():CurrentSequence(-1){}} AttackerInfo;
    struct Defend{std::vector<int> Attackers;const CThingCreatureBase* PLastAttacker;Defend():PLastAttacker(0){}} DefenderInfo;
};
struct CThingObject:CThing{};struct CThingBuilding:CThing{};struct CThingFilter_None{};
struct Scene {
    bool los;float normal;int search,casts,cameraRequests,outer,due,randoms;
    CThingObject object;CThingBuilding building;
    Scene():los(true),normal(0),search(0),casts(0),cameraRequests(0),outer(0),due(0),randoms(0){}
} *scene;
template<class T>const CThing* found();
template<>const CThing* found<CThingObject>(){check(scene->search==0);return &scene->object;}
template<>const CThing* found<CThingBuilding>(){check(scene->search==1);return &scene->building;}
struct Search{
 template<class T>void GetAllThingsInRoughArea(const C2DBoxI& area,const CThingFilter_None&,std::vector<const CThing*>& things)const{
    check(area.TLX==0&&area.TLY==-2&&area.BRX==4&&area.BRY==2);
    things.push_back(found<T>());++scene->search;
 }
};
namespace NCameraModes {struct CTools{
 enum Mode{TEST_THINGS};
 bool IsLineOfSight(const std::vector<const CThing*>& things,const C3DVector& from,const C3DVector& to,C3DVector&,C3DVector& normal,float&,const CThing**,Mode mode,bool extra)const{
  check(things.size()==2&&things[0]==&scene->object&&things[1]==&scene->building);
  check(mode==TEST_THINGS&&!extra&&from.X==0&&to.X==4&&from.Z==2&&to.Z==2);
  ++scene->casts;normal.Z=scene->normal;return scene->los;
 }
};}
struct CGameCameraManager{NCameraModes::CTools tools;const NCameraModes::CTools& GetCameraTools()const{return tools;}};
struct Camera{CGameCameraManager manager;bool GetCameraManager(const CGameCameraManager** out)const{check(*out==0);*out=&manager;++scene->cameraRequests;return true;}};
struct World{Camera cam;const Camera* GetGameCamera()const{return &cam;}long GetWorldSeed()const{return 77;}};
float GFCos(float turn){check(turn==1.0f/6.0f);return 0.5f;}
long GFRandom(long size,long seed){check(size>0&&size%100==0&&seed==77);++scene->randoms;return size-1;}
struct CCombatManager{
 bool EnableAINonAttackMode,may; World world;Search search;
 CCombatManager():EnableAINonAttackMode(false),may(true){}
 const World& GetWorld()const{return world;} const Search& PeekThingSearchTools()const{return search;}
 bool IsDefenderDueForAttack(const CCombatantInfo&)const{++scene->due;return may;}
 bool IsCreatureOnOuterRing(const CThingCreatureBase&)const{++scene->outer;return false;}
 long PickBestSequenceFromIndexList(const std::vector<long>& index_list)const;
 long GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo&,const CCombatantInfo&)const;
};
FABLE_FORCEINLINE long CCombatManager::PickBestSequenceFromIndexList(const std::vector<long>& index_list) const
{
    long size=index_list.size();
    long random=GFRandom(size*100,GetWorld().GetWorldSeed());
    random/=100;
    long index=index_list[random];
    return index;
}
long CCombatManager::GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker_info,const CCombatantInfo& defender_info) const
{
    const CThingCreatureBase& attacker_creature = *attacker_info.Parent;
    const CThingCreatureBase& defender_creature = *defender_info.Parent;
    const std::vector<CCountedPointer<CCombatSequenceBase> >& all_possible_sequences = attacker_info.AttackerInfo.CombatSequences;
    const CCombatSequenceBase* pold_sequence = 0;
    if (attacker_info.AttackerInfo.CurrentSequence >= 0)
        pold_sequence = all_possible_sequences[attacker_info.AttackerInfo.CurrentSequence].Get();
    std::vector<long> correct_zone_is_valid, correct_zone_melee, correct_zone_break_into_melee;
    std::vector<long> correct_zone_boast, correct_zone_outerring, correct_zone_non_melee;
    bool attacker_on_outer_ring = IsCreatureOnOuterRing(attacker_creature);
    bool can_attack = IsDefenderDueForAttack(defender_info);
    if (EnableAINonAttackMode) can_attack = false;
    long c1 = 0;
    for (std::vector<CCountedPointer<CCombatSequenceBase> >::const_iterator it=all_possible_sequences.begin(); it!=all_possible_sequences.end(); ++it, ++c1) {
        CCombatSequenceBase& possible_sequence=**it;
        bool melee_seq=possible_sequence.IsMeleeSequence();
        bool break_into_seq=possible_sequence.IsBreakIntoMeleeSequence();
        if ((melee_seq || break_into_seq) && !can_attack) continue;
        if (!possible_sequence.IsValid() || !possible_sequence.IsZoneValid()) continue;
        if(possible_sequence.IsToPreferSequenceWhenValid()) correct_zone_is_valid.push_back(c1);
        else if(melee_seq) correct_zone_melee.push_back(c1);
        else if(break_into_seq) correct_zone_break_into_melee.push_back(c1);
        else if(possible_sequence.IsBoastSequence()) correct_zone_boast.push_back(c1);
        else correct_zone_non_melee.push_back(c1);
    }
    if(can_attack && defender_creature.IsPlayerCharacter() && (!correct_zone_melee.empty() || !correct_zone_break_into_melee.empty())) {
        C2DBoxI area(((attacker_creature.GetPos()+defender_creature.GetPos())/2.0f).ToC2DCoordI(),
                     (long)((attacker_creature.GetPos()-defender_creature.GetPos()).GetAccurateMagnitude()/2.0f));
        std::vector<const CThing*> things;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(area,CThingFilter_None(),things);
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingBuilding>(area,CThingFilter_None(),things);
        const CGameCameraManager* pcam_man=0;
        GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
        C3DVector from_pos=attacker_creature.GetPos()+C3DVector(0,0,1);
        C3DVector to_pos=defender_creature.GetPos();
        to_pos.Z=from_pos.Z;
        C3DVector coll_pos,coll_normal;
        float cam_rad;
        const CThing* phit_thing;
        if(!pcam_man->GetCameraTools().IsLineOfSight(things,from_pos,to_pos,coll_pos,coll_normal,cam_rad,&phit_thing,NCameraModes::CTools::TEST_THINGS,false)) {
            float cos_theta=coll_normal.Z;
            if((float)fabs(cos_theta)<GFCos(1.0f/6.0f)) can_attack=false;
        }
    }
    bool break_into_melee=false;
    if(pold_sequence && pold_sequence->IsBreakIntoMeleeSequence()) break_into_melee=true;
    bool target_can_be_mocked=NCreatureMode::IsKnockedDown(defender_creature);
    if(pold_sequence && pold_sequence->IsMeleeSequence() && defender_info.DefenderInfo.Attackers.size()>1) can_attack=false;
    long chosen_index=-1;
    if(!correct_zone_is_valid.empty()) chosen_index=PickBestSequenceFromIndexList(correct_zone_is_valid);
    else if(!correct_zone_boast.empty() && target_can_be_mocked) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_boast);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_break_into_melee.empty() && defender_info.DefenderInfo.PLastAttacker!=&attacker_creature && can_attack) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_break_into_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_melee.empty() && (can_attack || break_into_melee)) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_non_melee.empty()) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_non_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    return chosen_index;
}

struct Trial {
 Scene state; CThingCreatureBase att,def;CCombatantInfo a,d;CCombatManager game; CCombatSequenceBase seq[7];
 Trial(){a.Parent=&att;d.Parent=&def;def.p=C3DVector(4,0,1);scene=&state;}
 void add(int n){for(int i=0;i<n;++i)a.AttackerInfo.CombatSequences.push_back(CCountedPointer<CCombatSequenceBase>(&seq[i]));}
 void run(long expected,bool camera){check(game.GetBestSequenceIndexAttackerOnDefender(a,d)==expected);check(state.outer==1&&state.due==1&&state.randoms==(expected>=0?1:0));check(state.casts==int(camera)&&state.cameraRequests==int(camera)&&state.search==2*int(camera));}
};
void cases(){
 {Trial t;t.run(-1,false);}
 {Trial t;t.add(4);t.seq[0].valid=false;t.seq[1].zone=false;t.seq[2].prefer=true;t.seq[3].prefer=true;t.run(3,false);check(t.seq[0].zones==0&&t.seq[1].prefs==0&&t.seq[2].prefs==1);}
 for(int player=0;player<2;++player)for(int due=0;due<2;++due)for(int disabled=0;disabled<2;++disabled)
 for(int history=0;history<3;++history)for(int same=0;same<2;++same)for(int crowd=0;crowd<2;++crowd)
 for(int collision=0;collision<4;++collision)for(int knock=0;knock<2;++knock){
  Trial t;t.add(4);t.seq[0].melee=true;t.seq[1].into=true;t.seq[2].boast=true;
  t.def.player=player!=0;t.def.knocked=knock!=0;t.game.may=due!=0;t.game.EnableAINonAttackMode=disabled!=0;
  t.a.AttackerInfo.CurrentSequence=history-1;t.d.DefenderInfo.PLastAttacker=same?&t.att:&t.def;
  t.d.DefenderInfo.Attackers.assign(crowd?2:1,0);t.state.los=collision==0;t.state.normal=collision==1?0.0f:collision==2?-0.5f:1.0f;
  bool eligible=due&&!disabled;bool camera=player&&eligible;
  bool attack=eligible&&!(camera&&collision==1)&&!(history==1&&crowd);
  long expected=knock?2:eligible&&attack&&!same?1:eligible&&(attack||history==2)?0:3;
  t.run(expected,camera);check(t.seq[0].zones==int(eligible)&&t.seq[1].zones==int(eligible));
 }
 {Trial t;t.def.player=true;t.seq[0].boast=true;t.add(1);t.run(-1,false);}
 {Trial t;t.add(3);t.seq[0].melee=true;t.seq[0].into=true;t.seq[1].boast=true;t.seq[2].prefer=true;t.def.knocked=true;t.run(2,false);}
}
}
int main(){CombatSequenceTest::cases();puts("006eae20_TEST PASS");return 0;}
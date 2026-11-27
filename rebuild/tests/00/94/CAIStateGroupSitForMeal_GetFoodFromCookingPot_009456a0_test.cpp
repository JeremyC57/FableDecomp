#include <cstdio>
#include <cstdlib>
#include <cstring>
enum EAIActionReturn {AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS};
enum {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2,AI_MOVEMENT_WALKING=3};
struct CCharString {const char* str; CCharString(const char* s):str(s){}};
struct C3DVector{float x;C3DVector(float a=0):x(a){} C3DVector operator+(const C3DVector& b)const{return C3DVector(x+b.x);}};
struct CRightHandedSet{C3DVector forward;const C3DVector& GetForward()const{return forward;}};
struct CThing;struct CTCSingleUserActionPoints{int freeAt,search;bool GetFreeActionPoint(CThing&,C3DVector& p,CRightHandedSet& rhs,long& index){if(++search!=freeAt)return false;p.x=7;rhs.forward.x=2;index=12;return true;}};
struct Village {bool meal;bool IsEatTime()const{return meal;}};
struct CTCVillageMember {Village* village;Village* GetPVillage()const{return village;}};
struct CThingBuilding{bool alive,inside;bool IsPointInside(const C3DVector& pos)const{if(pos.x!=3)abort();return inside;}};
struct CThing {bool alive;CThingBuilding* home;CTCSingleUserActionPoints* pt;CTCVillageMember* member;C3DVector pos;
 CThing():alive(true),home(0),pt(0),member(0),pos(3){}CThingBuilding* GetHomeBuilding()const{return home;}const C3DVector& GetPos()const{return pos;}
 bool GetTC(CTCSingleUserActionPoints** p){*p=pt;return pt!=0;}bool PeekTC(const CTCVillageMember** p)const{*p=member;return member!=0;}};
template<class T>struct CIntelligentPointer{T* p;CIntelligentPointer(T* x=0):p(x){}T* operator->()const{return p;}};
template<class T>bool IS_THING_ALIVE(const T& obj){return obj.p&&obj.p->alive;}template<class T>bool IS_THING_DEAD(const T& obj){return !IS_THING_ALIVE(obj);}
struct DefManager{long GetDefGlobalIndexFromName(const CCharString& n)const{if(strcmp(n.str,"OBJECT_EATING_BOWL_NONSE"))abort();return 31;}};
struct CSoundPair{long sound;CSoundPair(long s):sound(s){}};
struct CCreatureAction_GetItemOutOfPocket{CCreatureAction_GetItemOutOfPocket(CThing&,long idx){if(idx!=31)abort();}};
struct CCreatureAction_PlayAnimation{CCreatureAction_PlayAnimation(CThing&,const CCharString& n,CSoundPair s,bool a,bool b,bool c,long d,long e){if(strcmp(n.str,"ST_FILL_ITEM")||s.sound||a||!b||c||d||e)abort();}};
unsigned long GFRandom(unsigned long n,unsigned long* s){if(n!=100)abort();return *s;}
class CAIStateGroup_SitForMeal{
public:CThing creature;CIntelligentPointer<CThing> PPot;long ActionPointIndex;bool HoldingBowl;unsigned long seed;DefManager definitions;
 int prox,moves,look,pocket,fill,failAt;float proximity;
 CAIStateGroup_SitForMeal():ActionPointIndex(99),HoldingBowl(false),seed(0),prox(0),moves(0),look(0),pocket(0),fill(0),failAt(0),proximity(-1){}
 CThing& GetCreature(){return creature;}const CThing& GetCreature()const{return creature;}unsigned long* GetPWorldSeed(){return &seed;}const DefManager& PeekDefinitionManager()const{return definitions;}
 bool IsMealTime()const;bool GetFoodFromCookingPot();
 EAIActionReturn AIAction_MoveToThingProximity(int r,CIntelligentPointer<CThing> t,float f,bool& nav,int m,bool a,bool b,bool c,bool d,float e){if(r!=2||t.p!=PPot.p||m!=3||!a||!b||!c||!d||e)abort();nav=true;++prox;proximity=f;return failAt==1?AI_ACTION_FAILED:(prox%2?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED);}
 EAIActionReturn AIAction_MoveToPos(int r,C3DVector p,bool& nav,int m,bool a,bool b,const CThing* q,bool c,float f){if(r!=1||p.x!=7||m!=3||!a||!b||q||!c||f)abort();nav=true;++moves;return failAt==2?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_LookAtPos(int r,C3DVector p,int frames,float f){if(r!=1||p.x!=9||frames!=17||f!=0.0027777f)abort();++look;return failAt==3?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_GetItemOutOfPocket&,bool w){if(w)abort();++pocket;return failAt==4?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_PlayAnimation&,bool w){if(w)abort();++fill;return failAt==5?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
};
inline bool CAIStateGroup_SitForMeal::IsMealTime() const
{
    const CTCVillageMember* member;
    if (GetCreature().PeekTC(&member) && member->GetPVillage() && member->GetPVillage()->IsEatTime())
        return true;
    return false;
}

namespace { CCharString NAME_OF_BITCHIN_BOWL("OBJECT_EATING_BOWL_NONSE"); }
bool CAIStateGroup_SitForMeal::GetFoodFromCookingPot()
{
    CIntelligentPointer<const CThingBuilding> p_home_building(GetCreature().GetHomeBuilding());
    if (IS_THING_DEAD(PPot) || IS_THING_DEAD(p_home_building)) return false;
    if (!p_home_building->IsPointInside(PPot->GetPos())) p_home_building=0;
    ActionPointIndex=-1;
    bool ready_to_eat=false;
    bool has_nearby_wait_pos=false;
    C3DVector wait_pos=PPot->GetPos();
    C3DVector action_pos,look_pos;
    float proximity=GFRandom(100,GetPWorldSeed())*0.05f+2.0f;
    EAIActionReturn ret;
    do {
        CTCSingleUserActionPoints* ptc_pot=0;
        if (!(IS_THING_ALIVE(PPot) && PPot->GetTC(&ptc_pot))) return false;
        CRightHandedSet rhs;
        if (ptc_pot->GetFreeActionPoint(GetCreature(),action_pos,rhs,ActionPointIndex)) {
            ready_to_eat=true;
            look_pos=action_pos+rhs.GetForward();
        }
        if (!ready_to_eat) {
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PPot,proximity,nav,AI_MOVEMENT_WALKING,true,true,true,true,0.0f);
            if (ret!=AI_ACTION_FINISHED && ret!=AI_ACTION_IN_PROGRESS) return false;
        }
    } while (!ready_to_eat && IsMealTime());
    if (!ready_to_eat) return false;
    bool nav;
    ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,action_pos,nav,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
    if (ret!=AI_ACTION_FINISHED) return false;
    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,look_pos,17,0.0027777f);
    if (ret!=AI_ACTION_FINISHED) return false;
    long bowl_def_index=PeekDefinitionManager().GetDefGlobalIndexFromName(NAME_OF_BITCHIN_BOWL);
    ret=AIAction_PerformActionAndWait(CCreatureAction_GetItemOutOfPocket(GetCreature(),bowl_def_index),false);
    if (ret!=AI_ACTION_FINISHED) return false;
    HoldingBowl=true;
    ret=AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"ST_FILL_ITEM",CSoundPair(0),false,true,false,0,0),false);
    if (ret!=AI_ACTION_FINISHED) return false;
    return true;
}

int main(){
 for(int flags=0;flags<256;++flags)for(int freeAt=1;freeAt<4;++freeAt)for(int fail=0;fail<6;++fail){
  CAIStateGroup_SitForMeal g;CThing pot;CThingBuilding house;CTCSingleUserActionPoints points;CTCVillageMember member;Village village;
  if(flags&1)g.PPot.p=&pot;pot.alive=flags&2;if(flags&4)g.creature.home=&house;house.alive=flags&8;house.inside=flags&16;
  if(flags&32)pot.pt=&points;if(flags&64)g.creature.member=&member;member.village=&village;village.meal=flags&128;
  points.freeAt=freeAt;points.search=0;g.failAt=fail;g.seed=(flags*7+freeAt)%100;
  bool started=(flags&15)==15,canSearch=started&&(flags&32);int attempts=0,waits=0;bool ready=false;
  if(canSearch){while(++attempts!=freeAt){++waits;if(fail==1||!(flags&64)||!(flags&128))break;}ready=attempts==freeAt;}
  bool got= g.GetFoodFromCookingPot();bool expected=ready&&fail!=2&&fail!=3&&fail!=4&&fail!=5;
  if(got!=expected||g.prox!=waits||points.search!=attempts||g.ActionPointIndex!=(ready?12:(started?-1:99)))return 1;
  if(g.moves!=int(ready)||g.look!=int(ready&&fail!=2)||g.pocket!=int(ready&&fail!=2&&fail!=3))return 2;
  bool bowl=ready&&fail!=2&&fail!=3&&fail!=4;if(g.fill!=int(bowl)||g.HoldingBowl!=bowl)return 3;
  float expected_proximity=2.0f+g.seed*0.05f;
  if(waits && std::memcmp(&g.proximity, &expected_proximity, sizeof(expected_proximity)) != 0)return 4;
 }
 puts("009456a0_TEST PASS");return 0;
}
#include <stdio.h>
#include <limits.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
struct CTCBuyableHouse {
    int visits;long day;
    void MovedInNotification(long d){++visits;day=d;}
};
struct CThingBuilding {
    bool alive,has;
    CTCBuyableHouse house;
    bool GetTC(CTCBuyableHouse** ptr){if(has)*ptr=&house;return has;}
};
template<class T> struct CIntelligentPointer {
    T* item;T* Get()const{return item;}T* operator->()const{return item;}T& operator*()const{return *item;}
    void operator=(T* p){item=p;}
};
static bool IS_THING_ALIVE(const CIntelligentPointer<CThingBuilding>& b){return b.Get() && b->alive;}
struct CVillagePeopleDef {float ProbabilityOfHomsexualityPerDialogueEnabledMan;};
template<class T> struct CDefPointer {T* value;CDefPointer():value(0){}T* operator->()const{return value;}};
struct Parent {
    bool has;CVillagePeopleDef definition;
    Parent& PeekBaseDef(){return *this;}
    bool GetSubDef(CDefPointer<const CVillagePeopleDef>* p){if(has)p->value=&definition;return has;}
};
struct CThingAICreatureInit {CThingBuilding* HomeBuilding;CThingBuilding* WorkBuilding;bool EnableCreatureAutoPlacing;};
struct CThingAICreature{
    bool follow;float probability;long def,player;C3DVector position;CThingAICreatureInit init;
    static CThingAICreature* Create(long d,const C3DVector& p,long n,const CThingAICreatureInit& in){
        static CThingAICreature c;c.def=d;c.player=n;c.position=p;c.init=in;c.follow=false;c.probability=-100;return &c;}
    void SetAsAllowedToFollowHero(bool f){follow=f;}
    void AutoSetupSexuality(float p){probability=p;}
};
struct CTCVillage{
    long existing;int checked,guard_calls;CThingAICreature* added;Parent parent;
    long GetNextGuardTypeAndIncrement(){++guard_calls;return 80;}
    long GetNumberOfGayVillagers(){++checked;return existing;}
    void AddThingToVillage(CThingAICreature& c){added=&c;}
    Parent& GetParentThing(){return parent;}
};
struct CTCDummyVillager{
    bool Guard;long DefType,DayToBeRecreated,FrameToBeRecreated;
    CIntelligentPointer<CThingBuilding> HomeBuilding,WorkBuilding;
    CIntelligentPointer<CThingAICreature> Creature;
    CThingAICreature* CreateVillager(CTCVillage&,const C3DVector&,bool);
};
CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage& village,const C3DVector& pos,bool stop_autoplace)
{
    if (Guard) DefType=village.GetNextGuardTypeAndIncrement();
    CThingAICreatureInit init;
    init.HomeBuilding=HomeBuilding.Get();
    init.WorkBuilding=WorkBuilding.Get();
    init.EnableCreatureAutoPlacing=!stop_autoplace;
    Creature=CThingAICreature::Create(DefType,pos,4,init);
    village.AddThingToVillage(*Creature);
    Creature->SetAsAllowedToFollowHero(true);
    float prob_of_gay_per_dialogue_enabled_man=0.0f;
    CDefPointer<const CVillagePeopleDef> pvillage_people_def;
    if (village.GetParentThing().PeekBaseDef().GetSubDef(&pvillage_people_def))
        prob_of_gay_per_dialogue_enabled_man=pvillage_people_def->ProbabilityOfHomsexualityPerDialogueEnabledMan;
    if (prob_of_gay_per_dialogue_enabled_man>0 && !(village.GetNumberOfGayVillagers()>0))
        prob_of_gay_per_dialogue_enabled_man=1.0f;
    Creature->AutoSetupSexuality(prob_of_gay_per_dialogue_enabled_man);
    CTCBuyableHouse* buyable_house;
    if (IS_THING_ALIVE(HomeBuilding) && HomeBuilding->GetTC(&buyable_house))
        buyable_house->MovedInNotification(DayToBeRecreated);
    if (IS_THING_ALIVE(WorkBuilding) && WorkBuilding->GetTC(&buyable_house))
        buyable_house->MovedInNotification(DayToBeRecreated);
    DayToBeRecreated=INT_MAX;
    FrameToBeRecreated=INT_MAX;
    return Creature.Get();
}

int main(){
 const float probabilities[]={-0.25f,0.0f,0.5f};
 for(int bits=0;bits<8;++bits)for(int h=0;h<4;++h)for(int w=0;w<4;++w)
 for(int n=-1;n<=1;++n)for(int j=0;j<3;++j){
    CThingBuilding buildings[2];CTCDummyVillager v;CTCVillage village;
    int states[]={h,w};for(int k=0;k<2;++k){
       buildings[k].alive=states[k]>=2;buildings[k].has=states[k]==3;
       buildings[k].house.visits=0;buildings[k].house.day=0;}
    v.Guard=(bits&1)!=0;v.DefType=11;v.DayToBeRecreated=9;v.FrameToBeRecreated=14;
    v.HomeBuilding=h?buildings:0;v.WorkBuilding=w?buildings+1:0;v.Creature=0;
    village.guard_calls=village.checked=0;village.existing=n;village.added=0;
    village.parent.has=(bits&4)!=0;village.parent.definition.ProbabilityOfHomsexualityPerDialogueEnabledMan=probabilities[j];
    C3DVector pos(5,-3,0.5f);bool stop=(bits&2)!=0;
    CThingAICreature* c=v.CreateVillager(village,pos,stop);
    if(!c || c!=village.added || c!=v.Creature.Get() || c->player!=4 || !c->follow ||
       c->init.HomeBuilding!=v.HomeBuilding.Get() || c->init.WorkBuilding!=v.WorkBuilding.Get() ||
       c->init.EnableCreatureAutoPlacing==stop || c->position.X!=5 || c->position.Y!=-3 || c->position.Z!=0.5f)return 1;
    if(v.DefType!=(v.Guard?80:11) || c->def!=v.DefType || village.guard_calls!=v.Guard)return 2;
    float prob=village.parent.has?probabilities[j]:0.0f;
    if(village.checked!=(prob>0) || c->probability!=(prob>0 && n<=0?1.0f:prob))return 3;
    for(int u=0;u<2;++u)if(buildings[u].house.visits!=(states[u]==3) ||
       (states[u]==3 && buildings[u].house.day!=9))return 4;
    if(v.DayToBeRecreated!=INT_MAX || v.FrameToBeRecreated!=INT_MAX)return 5;
 }
 puts("0072f750_TEST PASS");return 0;
}
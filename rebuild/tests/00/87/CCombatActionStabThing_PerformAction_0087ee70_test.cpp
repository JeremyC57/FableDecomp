#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"
static int failures;
#define Check(x) do { if(!(x)) {printf("failed %d\n",__LINE__);exit(1);} } while(0)
struct C3DVector {float x,y,z;};
class CCharString {public:const char* value; CCharString(const char* s=""):value(s){} CCharString& operator=(const char* s){value=s;return *this;}};
template<class T> class CIntelligentPointer {
public:T* p;T* Get() const{return p;}T* operator->() const{return p;}T& operator*()const{return *p;}
};
class CThing; class CThingAICreature;
class CAnimFilterBase {public:virtual ~CAnimFilterBase(){}};
class CAnimFilterNoGroup:public CAnimFilterBase {};
class CAnimationEntry {public:long BankIndex;};
static CAnimationEntry entry;
static unsigned bits,tests,plays,added,rendered,events,actions,posts,type;
static const char* selected;
static CAnimFilterBase own;
class CAnimationSet {
public:bool AnimExists(const CCharString& name,const CAnimFilterBase& filter) const {
 ++tests;
 if(&filter==&own)return strcmp(name.value,"STABBED_ALREADY_DEAD")==0 ? (bits&2)!=0 : (bits&4)!=0;
 Check(strcmp(name.value,"STABBED_TO_DEAD")==0);
 return tests==1 ? (bits&1)!=0 : (bits&8)!=0;
}
const CAnimationEntry& GetRandomAnim(const CCharString& name,unsigned long& seed,const CAnimFilterBase& filter,long variant)const {
 Check(seed==123 && variant==-1);
 Check((&filter==&own)==bool(bits&6));selected=name.value;return entry;
}
};
class CAppearanceDef {public:CAnimationSet Animation;};
static CAppearanceDef appearance;
static bool have_def;
template<class T> class CDefPointer {public:T* p;CDefPointer():p(0){}T* operator->()const{return p;}};
class CThingBaseDef {public:bool GetSubDef(CDefPointer<const CAppearanceDef>* p)const {p->p=&appearance;return have_def;}};
class CFaction {};
class CTCBase {};
class CTCEnemy:public CTCBase {public:enum{slot=0};const CFaction* faction;const CFaction* GetPFaction()const{return faction;}};
class CTCAnimatingObject:public CTCBase {public:enum{slot=1};void PlayAnimation(long i,long p,bool l){Check(i==77 && !p && !l);++plays;}};
static CTCAnimatingObject animation;
class CTCHeroOpinionDeedLog:public CTCBase {public:enum{slot=2};long PostDeed(EOpinionDeedType,const C3DVector&,const CThing*,CThingAICreature*,EOpinionPostDeedType,bool,bool,const CFaction*,bool,long);};
namespace NEntityEvents {class CEventKillLocked {public:const CThing* killer;CEventKillLocked(const CThing* p):killer(p){}};}
class CTCEntityEvents:public CTCBase {public:enum{slot=3};void AddEvent(NEntityEvents::CEventKillLocked*);};
class CThing {
public:bool dead;unsigned flags,kind;CTCBase* components[4];CThingBaseDef def;C3DVector pos;
 CThing():dead(false),flags(0),kind(0) {memset(components,0,sizeof components);}
 unsigned GetTypeFlags()const{return flags;}unsigned GetType()const{return kind;}
 template<class T> bool GetTC(T** p) {if(!components[T::slot])return false;*p=static_cast<T*>(components[T::slot]);return true;}
 template<class T> bool PeekTC(T** p)const {if(!components[T::slot])return false;*p=static_cast<T*>(components[T::slot]);return true;}
 CThingBaseDef* GetPBaseDef(){return &def;}const C3DVector& GetPos()const{return pos;}
 CTCBase* AddTC(const char* name,bool b,const void* ptr){Check(!strcmp(name,"CTCAnimatingObject")&&!b&&!ptr);++added;return &animation;}
 void SetAsRenderedThisFrame(unsigned p){Check(p==2);++rendered;}
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& ptr){return ptr.Get() && !ptr->dead;}
class CThingDeadCreature:public CThing {public:CAnimFilterBase& GetAnimationFilter()const{return own;}};
class CCreatureAction_StabbedToDeath;
class CThingCreatureBase:public CThing {public:bool SetCurrentAction(const CCreatureAction_StabbedToDeath&);};
class CCreatureAction_StabbedToDeath {public:CThingCreatureBase& who;CCreatureAction_StabbedToDeath(CThingCreatureBase& v):who(v){}};
class CWorld {public:unsigned long seed;unsigned long* GetWorldSeed(){return &seed;}};
static CWorld world;
class CCombatAction_StabThing {public:CIntelligentPointer<CThing> PTarget;CIntelligentPointer<CThingCreatureBase> PCreature;CWorld& GetWorld(){return world;}void PerformAction();};
static CThing* active;static CThingCreatureBase owner;
static CFaction faction;static bool has_enemy,has_faction;
void CTCEntityEvents::AddEvent(NEntityEvents::CEventKillLocked* e){Check(e && e->killer==&owner);++events;delete e;}
bool CThingCreatureBase::SetCurrentAction(const CCreatureAction_StabbedToDeath& e){Check(&e.who==this && this==active);++actions;return true;}
long CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType d,const C3DVector& p,const CThing* t,CThingAICreature* c,EOpinionPostDeedType m,bool b,bool b2,const CFaction* f,bool special,long v){
 Check(&p==&owner.pos && t==active && !c && m==OPINION_POST_DEED_TYPE_ALL && !b && !b2 && !v);
 Check(f==(has_enemy && has_faction ? &faction:0));Check(d==(posts?OPINION_DEED_TYPE_KILL_BAD_GUY:OPINION_DEED_TYPE_EXPRESSION_EVIL_LAUGH));
 Check(special==(posts==1));++posts;return 0;
}
void CCombatAction_StabThing::PerformAction()
{
    if (!IS_THING_ALIVE(PTarget))
        return;
    if (PTarget->GetTypeFlags() & 8) {
        CThingCreatureBase& target_creature = static_cast<CThingCreatureBase&>(*PTarget);
        CTCEntityEvents* ptc_ent;
        target_creature.GetTC(&ptc_ent);
        ptc_ent->AddEvent(new NEntityEvents::CEventKillLocked(PCreature.Get()));
        target_creature.SetCurrentAction(CCreatureAction_StabbedToDeath(target_creature));
    } else if (PTarget->GetType() == 13) {
        CDefPointer<const CAppearanceDef> pappearance_def;
        if (PTarget->GetPBaseDef()->GetSubDef(&pappearance_def) &&
                pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", CAnimFilterNoGroup())) {
            CCharString animName;
            CThingDeadCreature* p_dead_creature = static_cast<CThingDeadCreature*>(PTarget.Get());
            CAnimFilterNoGroup filter_no_group;
            const CAnimFilterBase* pfilter = &filter_no_group;
            if (pappearance_def->Animation.AnimExists("STABBED_ALREADY_DEAD", p_dead_creature->GetAnimationFilter())) {
                animName = "STABBED_ALREADY_DEAD";
                pfilter = &p_dead_creature->GetAnimationFilter();
            } else if (pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", p_dead_creature->GetAnimationFilter())) {
                animName = "STABBED_TO_DEAD";
                pfilter = &p_dead_creature->GetAnimationFilter();
            } else if (pappearance_def->Animation.AnimExists("STABBED_TO_DEAD", CAnimFilterNoGroup())) {
                animName = "STABBED_ALREADY_DEAD";
            } else {
                animName = "STABBED_TO_DEAD";
            }
            const CAnimationEntry& anim_entry = pappearance_def->Animation.GetRandomAnim(animName,*GetWorld().GetWorldSeed(),*pfilter,-1);
            CTCAnimatingObject* ptc_anim_obj;
            if (!PTarget->GetTC(&ptc_anim_obj))
                ptc_anim_obj = static_cast<CTCAnimatingObject*>(PTarget->AddTC("CTCAnimatingObject",false,0));
            ptc_anim_obj->PlayAnimation(anim_entry.BankIndex,0,false);
            PTarget->SetAsRenderedThisFrame(2);
        }
    }
    CTCHeroOpinionDeedLog* ptc_deedlog;
    if (PCreature->GetTC(&ptc_deedlog)) {
        const CTCEnemy* ptc_enemy;
        if (PTarget->PeekTC(&ptc_enemy) && ptc_enemy->GetPFaction()) {
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_EXPRESSION_EVIL_LAUGH,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,ptc_enemy->GetPFaction(),false,0);
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_KILL_BAD_GUY,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,ptc_enemy->GetPFaction(),true,0);
        } else
            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_EXPRESSION_EVIL_LAUGH,PCreature->GetPos(),PTarget.Get(),0,OPINION_POST_DEED_TYPE_ALL,false,false,0,false,0);
    }
}
int main(){
 CCombatAction_StabThing action;CThingCreatureBase victim; CThingDeadCreature corpse;CThing other;
 CTCEnemy enemy;CTCHeroOpinionDeedLog deeds;CTCEntityEvents event_manager;
 world.seed=123;entry.BankIndex=77;action.PCreature.p=&owner;
 for(type=0;type<5;++type) for(bits=0;bits<32;++bits) for(unsigned config=0;config<16;++config) {
  have_def=(config&1)!=0;has_enemy=(config&2)!=0;has_faction=(config&4)!=0;
  active=type==0?0:type==1?&other:type==2?static_cast<CThing*>(&victim):type==3?static_cast<CThing*>(&corpse):&other;
  action.PTarget.p=active;enemy.faction=has_faction?&faction:0;owner.components[2]=(config&8)?&deeds:0;
  if(active){active->dead=type==1;active->flags=type==2?8:0;active->kind=type==3?13:0;
   active->components[0]=has_enemy?&enemy:0;active->components[1]=(bits&16)?&animation:0;active->components[3]=&event_manager;}
  tests=plays=added=rendered=events=actions=posts=0;selected=0;
  action.PerformAction();
  bool play= type==3 && have_def && (bits&1);
  Check(plays==play && rendered==play && added==(play && !(bits&16)));
  Check(actions==(type==2) && events==(type==2));
  Check(posts==((type>=2 && (config&8))?1+(has_enemy && has_faction):0));
  if(play) Check(!strcmp(selected,(bits&2)||(!(bits&4)&&(bits&8))?"STABBED_ALREADY_DEAD":"STABBED_TO_DEAD"));
  else Check(!selected);
 }
 puts("0087ee70_TEST PASS");
}
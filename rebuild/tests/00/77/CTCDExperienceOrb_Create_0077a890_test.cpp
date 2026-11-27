#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
namespace std { template<class T> const T& (max)(const T& a,const T& b){return a<b?b:a;} }
#define FABLE_FASTCALL __fastcall
static void check(bool b){if(!b)exit(2);}
class CTCDExperienceOrb;
struct C3DVector{float x,y,z;C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}void RotateXY(float a){float xx=x; x=xx*(float)cos(a)-y*(float)sin(a);y=xx*(float)sin(a)+y*(float)cos(a);}};
struct CTCPhysicsBase{C3DVector velocity;int writes;CTCPhysicsBase():writes(0){}void SetVelocity(const C3DVector& v){velocity=v;++writes;}};
struct Stats{long ExperienceWorth;};
struct CCreatureDef{Stats stats;const Stats& GetStatsDef()const{return stats;}};
template<class T>class CDefPointer{public:T* p;CDefPointer():p(0){}T* operator->()const{return p;}};
struct Def{CCreatureDef* def;bool GetSubDef(CDefPointer<const CCreatureDef>* p)const{p->p=def;return def!=0;}};
struct CThing{
 unsigned long flags;bool dead;Def def;CTCDExperienceOrb* orb;C3DVector pos;CTCPhysicsBase physics;
 CThing():flags(0),dead(false),orb(0){def.def=0;}
 unsigned long GetTypeFlags()const{return flags;}
 const Def& PeekBaseDef()const{return def;}
 const C3DVector& GetPos()const{return pos;}
 CTCPhysicsBase* GetPhysicsTC(){return &physics;}
 bool GetTC(CTCDExperienceOrb** out){*out=orb;return orb!=0;}
};
struct CThingCreatureBase:public CThing{CThingCreatureBase* attacker;CThingCreatureBase():attacker(0){}const CThingCreatureBase* GetPLastAttackedByCreature()const{return attacker;}};
template<class A,class B>B* GFDebugDynamicCast(A* a,B*){return static_cast<B*>(a);}
template<class T>bool IS_THING_ALIVE(const T& p){return p&&!p->dead;}
static float multiplier;static int multipliers,definitions,creations;static unsigned long seed;
namespace NHeroExperience{float GetWeaponModifiedExperienceMultiplierForThing(const CThing&){++multipliers;return multiplier;}}
class CGameDefinitionManager{public:static CGameDefinitionManager& Get(){static CGameDefinitionManager s;return s;}long GetDefGlobalIndexFromName(const char* name){check(strcmp(name,"THING_EXPERIENCE_ORB")==0);++definitions;return 78;}};
static float GFFloatRandom(float p,unsigned long* s){check(p==1&&s==&seed);++seed;return seed*0.125f;}
struct ExperienceDef{std::map<long,float> ParticleScalesPerExperiencePoints;};static ExperienceDef effects;
class CTCDExperienceOrb{
public:long points;CTCDExperienceOrb():points(-1){}
 static void FABLE_FASTCALL Create(CThing&);static CThing* FABLE_FASTCALL Create(long,const C3DVector&);
 void GetExperienceOrbsToCreateForTotalExperience(long,std::vector<long>&)const;
 const ExperienceDef& GetDef()const{return effects;} void SetExperiencePoints(long p){points=p;}
 unsigned long* GetPWorldSeed()const{return &seed;}
};
static CThing orbs[50];static CTCDExperienceOrb drivers[50];static int failed,dead_at;
CThing* FABLE_FASTCALL CTCDExperienceOrb::Create(long index,const C3DVector& pos){
 check(index==78&&pos.x==1&&pos.y==2&&pos.z==3);int n=creations++;check(n<50);if(n==failed)return 0;
 orbs[n].dead=n==dead_at;orbs[n].orb=&drivers[n];return &orbs[n];
}
void CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience(long experience, std::vector<long>& orbs_to_create_values) const
{
    while (experience > 0) {
        std::map<long, float>::const_iterator lower_it = GetDef().ParticleScalesPerExperiencePoints.lower_bound(experience);
        if (lower_it != GetDef().ParticleScalesPerExperiencePoints.begin())
            --lower_it;
        long threshold_experience = (std::max)(lower_it->first, 1L);
        orbs_to_create_values.push_back(threshold_experience);
        experience -= threshold_experience;
    }
}
namespace {
const C3DVector VELOCITY_TO_START_MOVING_AT(0.3f, 0.0f, 0.4f);
}

void FABLE_FASTCALL CTCDExperienceOrb::Create(CThing& creator)
{
    float weapon_based_multiplier = 1.0f;
    if (creator.GetTypeFlags() & 8) {
        CThingCreatureBase& creature_creator = *GFDebugDynamicCast(&creator, (CThingCreatureBase*)0);
        const CThingCreatureBase* pthing_killed_by = creature_creator.GetPLastAttackedByCreature();
        if (IS_THING_ALIVE(pthing_killed_by))
            weapon_based_multiplier = NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(*pthing_killed_by);
    }
    long experience_worth;
    CDefPointer<const CCreatureDef> pcreature_def;
    if (creator.PeekBaseDef().GetSubDef(&pcreature_def))
        experience_worth = pcreature_def->GetStatsDef().ExperienceWorth;
    else
        experience_worth = 1;
    experience_worth = (long)(experience_worth * weapon_based_multiplier);
    if (experience_worth) {
        long experience_orb_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("THING_EXPERIENCE_ORB");
        std::vector<CThing*> orb_list;
        std::vector<long> orbs_to_create_values;
        CThing* pexperience_orb = Create(experience_orb_def_index, creator.GetPos());
        if (IS_THING_ALIVE(pexperience_orb)) {
            orb_list.push_back(pexperience_orb);
            CTCDExperienceOrb* ptcd_experience_orb;
            pexperience_orb->GetTC(&ptcd_experience_orb);
            ptcd_experience_orb->GetExperienceOrbsToCreateForTotalExperience(experience_worth, orbs_to_create_values);
        }
        for (long c1 = 0; c1 < orbs_to_create_values.size(); ++c1) {
            if (IS_THING_ALIVE(pexperience_orb)) {
                CTCDExperienceOrb* ptcd_experience_orb;
                pexperience_orb->GetTC(&ptcd_experience_orb);
                ptcd_experience_orb->SetExperiencePoints(orbs_to_create_values[c1]);
                C3DVector initial_velocity = VELOCITY_TO_START_MOVING_AT;
                initial_velocity.RotateXY(GFFloatRandom(1.0f, ptcd_experience_orb->GetPWorldSeed()));
                pexperience_orb->GetPhysicsTC()->SetVelocity(initial_velocity);
            }
            pexperience_orb = 0;
            if (c1 + 1 < orbs_to_create_values.size()) {
                pexperience_orb = Create(experience_orb_def_index, creator.GetPos());
                orb_list.push_back(pexperience_orb);
            }
        }
    }
}
int main(){
 effects.ParticleScalesPerExperiencePoints[1]=1;effects.ParticleScalesPerExperiencePoints[4]=1;effects.ParticleScalesPerExperiencePoints[10]=1;
 for(int c=0;c<64;++c){
  for(int i=0;i<50;++i){orbs[i]=CThing();drivers[i]=CTCDExperienceOrb();}
  CThingCreatureBase creator,attacker;creator.pos=C3DVector(1,2,3);creator.flags=(c&1)?8:0;creator.attacker=(c&2)?&attacker:0;attacker.dead=(c&4)!=0;
  CCreatureDef def;def.stats.ExperienceWorth=(c%5==0)?0:(c%5==1)?1:(c%5==2)?9:(c%5==3)?15:24;
  creator.def.def=(c&8)?&def:0;multiplier=(c&16)?1.5f:0.5f;failed=(c%7==0)?0:(c%7==1)?1:-1;dead_at=c%9==0?0:c%9==1?2:-1;
  bool applies=creator.flags&&creator.attacker&&!attacker.dead;int xp=(int)((creator.def.def?def.stats.ExperienceWorth:1)*(applies?multiplier:1.f));
  multipliers=definitions=creations=0;seed=0;CTCDExperienceOrb::Create(creator);
  check(multipliers==(applies?1:0)&&definitions==(xp?1:0));
  std::vector<long> values;int left=xp;
  while(left>0){int v=left>10?10:left>4?4:1;values.push_back(v);left-=v;}
  int count=xp?(failed==0||dead_at==0?1:(int)values.size()):0;
  check(creations==count);unsigned n=0;
  for(int j=0;j<count;++j){
   if(j==failed||j==dead_at){check(drivers[j].points==-1&&orbs[j].physics.writes==0);continue;}
   ++n;check(drivers[j].points==values[j]&&orbs[j].physics.writes==1);
   C3DVector v=orbs[j].physics.velocity;check(fabs(v.x-0.3*cos(n*.125))<1e-6&&fabs(v.y-0.3*sin(n*.125))<1e-6&&v.z==0.4f);
  }
  check(seed==n);
 }
 puts("0077a890_TEST PASS");
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
static unsigned caseId;static int iter;
#define Check(x) do {if(!(x)){printf("failed %d case %u step %d\n",__LINE__,caseId,iter);exit(1);}}while(0)
struct C2DCoordI{long X,Y;C2DCoordI(long a,long b):X(a),Y(b){}};
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}
 C3DVector& operator*=(float f){X*=f;Y*=f;Z*=f;return *this;}
 void Normalise(){float m=(float)sqrt(X*X+Y*Y+Z*Z);if(m)*this*=1.f/m;}
 void RotateXY(float r){double v=6.283185307179586*r;float x=X;X=(float)(cos(v)*x-sin(v)*Y);Y=(float)(sin(v)*x+cos(v)*Y);++iter;}
};
static void same(const C3DVector&a,const C3DVector&b){Check(fabs(a.X-b.X)<0.001&&fabs(a.Y-b.Y)<0.001&&fabs(a.Z-b.Z)<0.001);}
struct CRightHandedSet{C3DVector up,forward;CRightHandedSet(const C3DVector&a,const C3DVector&b):up(a),forward(b){}};
struct CTCCreatureNavigation{bool IsAbleToNavigateFromPosToPos(const C3DVector&,const C3DVector&,float r)const{Check(r==0);return !((caseId+iter)%5==1);}};
struct CWorld{unsigned long seed;CWorld& GetWorldMap(){return *this;}bool IsPosInMap(const C2DCoordI&)const{return (caseId+iter)%7!=2;}bool IsPosPassable(const C2DCoordI&)const{return (caseId+iter)%4!=3;}unsigned long*GetWorldSeed(){return &seed;}static long GetFrame(){return 900+caseId;}};
struct CThingAICreatureInit{const char* ScriptName;CThingAICreatureInit():ScriptName(""){}};
template<class T>struct CDefPointer{T* p;CDefPointer():p(0){}T* operator->()const{return p;}};
struct Arr{long v[5],n;long size()const{return n;}long operator[](long k)const{Check(k>=0&&k<n);return v[k];}};
struct CSummonDef{long SummoneeEffect,CreatureToSummon,CreatureFamilyToSummon;float SummonRadius;};
struct CCreatureGenerationFamilyDef{Arr Creatures;};struct CThingCreatureDef{long DefaultOwner;};
static CCreatureGenerationFamilyDef family;static CThingCreatureDef creatureDef;
static int familyQueries,creatureQueries,randomCalls,effects,lastIndex[8],createdOwner[8];static C3DVector createdPos[8],effectPos[8];
unsigned long GFRandom(unsigned long n,unsigned long* seed){++randomCalls;return ((*seed)++%n);}
struct CGameDefinitionManager{static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}
 void GetDef(long k,CDefPointer<const CCreatureGenerationFamilyDef>*p){Check(k==12);++familyQueries;p->p=&family;}
 void GetDef(long k,CDefPointer<const CThingCreatureDef>*p){lastIndex[iter]=k;++creatureQueries;creatureDef.DefaultOwner=k+10;p->p=&creatureDef;}
};
struct CThingAICreature;
struct CCreatureAction_OnGenerate{CThingAICreature* owner;CCreatureAction_OnGenerate(CThingAICreature&c):owner(&c){}};
struct CThingAICreature{int actions,turns;C3DVector facing;bool dead;CThingAICreature():actions(0),turns(0),dead(false){}
 static CThingAICreature* Create(long n,const C3DVector& pos,long owner,const CThingAICreatureInit& init);
 CThingAICreature*GetPhysicsTC(){return this;}void SetFacingAnglesFromRHSet(const CRightHandedSet&r){same(r.up,C3DVector(0,0,1));facing=r.forward;++turns;}
 void SetCurrentAction(const CCreatureAction_OnGenerate&a){Check(a.owner==this);++actions;}
};
static CThingAICreature creatures[8];
CThingAICreature* CThingAICreature::Create(long n,const C3DVector& p,long owner,const CThingAICreatureInit&init){Check(lastIndex[iter]==n&&!strcmp(init.ScriptName,"SUMMONED_CREATURE"));createdPos[iter]=p;createdOwner[iter]=owner;creatures[iter].dead=(caseId+iter)%9==4;return (caseId+iter)%11==1?0:&creatures[iter];}
bool IS_THING_ALIVE(CThingAICreature*c){return c&&!c->dead;}
struct CTCDParticleEmitter{static void Create(long e,const C3DVector& pos,bool b){Check(e==6&&!b);effectPos[iter]=pos;++effects;}};
struct PtrList{CThingAICreature* item[8];long n;PtrList():n(0){}void push_back(CThingAICreature*c){Check(n<8);item[n++]=c;}};
struct CTCAISummon{
 CWorld w;C3DVector pos;CSummonDef def;const CTCCreatureNavigation* nav;PtrList MySummonedCreatures;static PtrList AllSummonedCreatures;long LastSummonTick,count;
 CWorld& GetWorld(){return w;}CTCAISummon&GetParentThing(){return *this;}const C3DVector&GetPos(){return pos;}
 bool PeekTC(const CTCCreatureNavigation**p){*p=nav;return nav!=0;}const CSummonDef&GetSummonDef(){return def;}
 long GetNumberCanSummon(){return count;}long SummonCreatures(const C3DVector* summonCentre);
};PtrList CTCAISummon::AllSummonedCreatures;
long CTCAISummon::SummonCreatures(const C3DVector* summonCentre)
{
    long numberToSummon=GetNumberCanSummon();
    long numberSummoned=0;
    if(numberToSummon>0){
        long summoneeEffect=GetSummonDef().SummoneeEffect;
        C3DVector offset(GetSummonDef().SummonRadius,0,0);
        float rotAngle=1.f/numberToSummon;
        C3DVector centre=summonCentre?*summonCentre:GetParentThing().GetPos();
        const CTCCreatureNavigation* pTCNavigate=0;
        GetParentThing().PeekTC(&pTCNavigate);
        if(false){}else for(long i=0;i<numberToSummon;++i){
            C3DVector pos=centre+offset;
            C2DCoordI coord=pos.ToC2DCoordI();
            if(GetWorld().GetWorldMap().IsPosInMap(coord)&&GetWorld().GetWorldMap().IsPosPassable(coord)&&
                (!pTCNavigate||pTCNavigate->IsAbleToNavigateFromPosToPos(centre,pos,0.f))){
                CThingAICreatureInit init;
                init.ScriptName="SUMMONED_CREATURE";
                long summonDefIndex=GetSummonDef().CreatureToSummon;
                if(GetSummonDef().CreatureFamilyToSummon>0){
                    CDefPointer<const CCreatureGenerationFamilyDef> pcf_def;
                    CGameDefinitionManager::Get().GetDef(GetSummonDef().CreatureFamilyToSummon,&pcf_def);
                    long num_cret_fam=pcf_def->Creatures.size();
                    if(num_cret_fam)summonDefIndex=pcf_def->Creatures[GFRandom(num_cret_fam,GetWorld().GetWorldSeed())];
                }
                CDefPointer<const CThingCreatureDef> def;
                CGameDefinitionManager::Get().GetDef(summonDefIndex,&def);
                CThingAICreature* pCreature=CThingAICreature::Create(summonDefIndex,pos,def->DefaultOwner,init);
                if(IS_THING_ALIVE(pCreature)){
                    C3DVector facing=offset;
                    if(summonCentre)facing*=-1.f;
                    facing.Normalise();
                    pCreature->GetPhysicsTC()->SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0,0,1),facing));
                    pCreature->SetCurrentAction(CCreatureAction_OnGenerate(*pCreature));
                    MySummonedCreatures.push_back(pCreature);
                    AllSummonedCreatures.push_back(pCreature);
                    ++numberSummoned;
                    if(summoneeEffect>0)CTCDParticleEmitter::Create(summoneeEffect,pos,false);
                }
            }
            offset.RotateXY(rotAngle);
        }
        LastSummonTick=CWorld::GetFrame();
    }
    return numberSummoned;
}
int main(){
 for(caseId=0;caseId<4096;++caseId){
  CTCAISummon a;CTCCreatureNavigation nav;C3DVector external(17,-3,7);
  const C3DVector* centre=(caseId&1)?&external:0;a.pos=C3DVector(-8,12,2);
  a.nav=(caseId&2)?&nav:0;a.count=(long)((caseId/13)%8)-1;a.LastSummonTick=-8;
  a.def.SummonRadius=2.f+(caseId%4);a.def.SummoneeEffect=(caseId&4)?6:-1;
  a.def.CreatureToSummon=7;a.def.CreatureFamilyToSummon=(caseId&8)?12:0;
  family.Creatures.n=(caseId/5)%5;for(long k=0;k<family.Creatures.n;++k)family.Creatures.v[k]=15+k;
  a.w.seed=caseId+3;unsigned long expectSeed=a.w.seed;iter=familyQueries=creatureQueries=randomCalls=effects=0;
  CTCAISummon::AllSummonedCreatures.n=0;
  for(int j=0;j<8;++j){createdOwner[j]=-1;lastIndex[j]=-1;creatures[j]=CThingAICreature();}
  long result=a.SummonCreatures(centre);long n=0,queries=0,group=0,rng=0,fx=0;
  for(int i=0;i<a.count;++i){
   bool eligible=(caseId+i)%7!=2&&(caseId+i)%4!=3&&(!a.nav||(caseId+i)%5!=1);
   if(!eligible){Check(lastIndex[i]==-1&&!creatures[i].actions);continue;}
   ++queries;long def=7;if(a.def.CreatureFamilyToSummon){++group;if(family.Creatures.n){def=15+expectSeed++%family.Creatures.n;++rng;}}
   Check(lastIndex[i]==def&&createdOwner[i]==def+10);
   double th=6.283185307179586*i/a.count;C3DVector offset((float)cos(th)*a.def.SummonRadius,(float)sin(th)*a.def.SummonRadius,0);
   C3DVector goal=(centre?*centre:a.pos)+offset;same(createdPos[i],goal);
   bool alive=(caseId+i)%9!=4&&(caseId+i)%11!=1;Check(creatures[i].turns==int(alive)&&creatures[i].actions==int(alive));
   if(!alive)continue;
   Check(a.MySummonedCreatures.item[n]==&creatures[i]&&a.AllSummonedCreatures.item[n]==&creatures[i]);++n;
   float sign=centre?-1.f:1.f;same(creatures[i].facing,C3DVector(sign*(float)cos(th),sign*(float)sin(th),0));
   if(a.def.SummoneeEffect>0){same(effectPos[i],goal);++fx;}
  }
  Check(result==n&&a.MySummonedCreatures.n==n&&a.AllSummonedCreatures.n==n);
  Check(a.LastSummonTick==(a.count>0?CWorld::GetFrame():-8)&&iter==(a.count>0?a.count:0));
  Check(queries==creatureQueries&&group==familyQueries&&rng==randomCalls&&fx==effects&&expectSeed==a.w.seed);
 }
 puts("007fe040_TEST PASS");return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <list>
#include <vector>
static unsigned seed;
static unsigned pick(unsigned n,unsigned m){unsigned v=seed+n*0x9e3779b9u;v^=v>>16;v*=2246822519u;v^=v>>13;return v%m;}
static void check(bool b){if(!b)abort();}
enum Ev{TARGET,HERO,INIT,GRAPHIC,STATICMESH,SETG,DRAW,UNDEAD,TRIGGER,KILL,RANDOM,CREATE,ATTACH,SOUND};
static int trace[32],len;static void log(Ev e){check(len<32);trace[len++]=e;}
static int graphic;static bool DrawCreatureGeneratorDebug;
struct CTCTargeted {void SetTargetable(bool b){check(!b);log(TARGET);}} target;
struct CTCDegradableBase{bool IsFullyDegraded()const{return pick(4,2)!=0;}} degrade;
struct CAppearanceDef{long Graphic;} appearance={122};
enum ENullDef{NULL_DEF};
template<class T> struct CDefPointer{T* p;CDefPointer(ENullDef=NULL_DEF):p(0){}const T* Get()const{return p;}const T* operator->()const{check(p!=0);return p;}};
struct BaseDef{void GetSubDef(CDefPointer<const CAppearanceDef>* p){p->p=&appearance;}} basedef;
struct Graphic{void SetMainGraphic(long g){check(g==122);log(SETG);}} graphicTC;
struct CThing;struct Vec{float x;};
struct CTCDParticleEmitter{static CThing* Create(long,const Vec&,bool);void AttachToThing(CThing&,long,const char*,long,float);};
struct CTCSoundPlayer{void PlayAttachedCriteria(const char*s,bool y,bool b,bool c,long d){check(!strcmp("buzz",s)&&y&&!b&&!c&&d==-1);log(SOUND);}} sound;
static CTCDParticleEmitter particleTC;
struct CThing{bool dead;Vec pos;bool PeekTC(const CTCDegradableBase**p){*p=&degrade;return pick(3,2)!=0;}bool GetTC(CTCTargeted**p){*p=&target;return pick(5,2)!=0;}
 bool GetTC(CTCSoundPlayer**p){*p=&sound;return pick(24,2)!=0;}bool GetTC(CTCDParticleEmitter**p){*p=&particleTC;return true;}
 Graphic* GetTCGraphicAppearance(){return graphic?&graphicTC:0;}void AddTC(const char*n,bool b,void*v){check(!b&&!v);if(!strcmp(n,"CTCGraphicAppearance")){graphic=1;log(GRAPHIC);}else{check(!strcmp(n,"CTCGraphicAppearanceStaticMesh"));log(STATICMESH);}}
 BaseDef& PeekBaseDef(){return basedef;}void SetAsDrawable(bool b){check(b);log(DRAW);}
 const Vec& GetPos()const{return pos;}void Kill(bool b){check(b);dead=true;log(KILL);}
} parent,previous,newParticle,other;
struct CThingCreatureBase:CThing{};struct CThingPlayerCreature:CThingCreatureBase{} hero,otherHero;
template<class T>struct CIntelligentPointer{T*p;CIntelligentPointer(T*t=0):p(t){}bool operator==(const CIntelligentPointer& q)const{return p==q.p;}bool operator!=(int v)const{check(v==0);return p!=0;}T*operator->()const{return p;}};
static bool IS_THING_ALIVE(const CThing* p){return p&&!p->dead;}template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return IS_THING_ALIVE(p.p);}
struct Player{CThingPlayerCreature* GetPPlayerCharacter(){return pick(6,2)?&hero:0;}} player;
struct World{bool IsCreatureGenerationEnabled(CThing&t){check(&t==&parent);return pick(1,4)!=0;}Player* PeekMyPlayer(){return &player;}long GetFrame(){return 897;}} world;
struct Criteria{const char* s;bool operator!=(const char*t) const{return strcmp(s,t)!=0;}operator const char*()const{return s;}};
struct CCreatureGeneratorDef{std::vector<long> RandomEffects;Criteria RandomSoundCriteria;long SecondsBetweenEffects,SecondsBetweenSoundEffects;} def;
static unsigned long worldSeed;static unsigned long GFRandom(unsigned long range,unsigned long* s){check(s==&worldSeed&&range==def.RandomEffects.size()&&range);log(RANDOM);return pick(21,range);}
struct CMainGameComponent{static long GetConstantFPS(){return 24;}};
CThing* CTCDParticleEmitter::Create(long effect,const Vec& pos,bool flag){check(!flag&&&pos==&parent.pos&&effect==def.RandomEffects[pick(21,def.RandomEffects.size())]);log(CREATE);return pick(22,2)?&newParticle:0;}
void CTCDParticleEmitter::AttachToThing(CThing& thing,long at,const char* n,long i,float x){check(&thing==&parent&&at==32&&!n[0]&&!i&&!x);log(ATTACH);}
struct CTCCreatureGenerator {
 bool Destroyed,ReadyToInitialise,Initialised,UndeadTriggered;int RegionCreationCount;
 std::list<CIntelligentPointer<CThing> > CreatedCreatures;std::vector<CIntelligentPointer<CThingCreatureBase> > CreatureTriggerers;
 CDefPointer<const CCreatureGeneratorDef> PDef;CIntelligentPointer<CThing> PRandomParticle;long FramesUntilRandomEffect,FramesUntilRandomSoundEffect;
 World& PeekWorld(){return world;}World& GetWorld(){return world;}CThing& GetParentThing(){return parent;}unsigned long*GetPWorldSeed(){return &worldSeed;}
 void AddCreatureTriggerer(CThingCreatureBase&h){check(&h==&hero);CreatureTriggerers.push_back(&h);log(HERO);}
 void InitialiseCreatureGenerator(bool f){check(!f);Initialised=pick(10,2)!=0;log(INIT);}
 bool IsUndeadGenerator()const{return UndeadTriggered;}void UpdateUndead(){log(UNDEAD);}void UpdateCreatureTrigger(long f){check(f==897);log(TRIGGER);}
 void FrameUpdate();
};
void CTCCreatureGenerator::FrameUpdate()
{
    if (!PeekWorld().IsCreatureGenerationEnabled(GetParentThing()))
        return;
    if (!Destroyed) {
        const CTCDegradableBase* ptc_degradable;
        if (GetParentThing().PeekTC(&ptc_degradable) && ptc_degradable->IsFullyDegraded()) {
            Destroyed = true;
            CTCTargeted* ptc_targeted;
            if (GetParentThing().GetTC(&ptc_targeted)) ptc_targeted->SetTargetable(false);
        }
    }
    if (Destroyed)
        return;
    CThingPlayerCreature* phero = GetWorld().PeekMyPlayer()->GetPPlayerCharacter();
    if (IS_THING_ALIVE(phero))
        AddCreatureTriggerer(*phero);
    if (ReadyToInitialise && !Initialised)
        InitialiseCreatureGenerator(false);
    if (DrawCreatureGeneratorDebug && GetParentThing().GetTCGraphicAppearance() == 0) {
        GetParentThing().AddTC("CTCGraphicAppearance", false, 0);
        GetParentThing().AddTC("CTCGraphicAppearanceStaticMesh", false, 0);
        CDefPointer<const CAppearanceDef> pappearance_def(NULL_DEF);
        GetParentThing().PeekBaseDef().GetSubDef(&pappearance_def);
        GetParentThing().GetTCGraphicAppearance()->SetMainGraphic(pappearance_def->Graphic);
        GetParentThing().SetAsDrawable(true);
    }
    long creatureCount = CreatedCreatures.size();
    CreatedCreatures.remove(CIntelligentPointer<CThing>(0));
    CreatureTriggerers.erase(std::remove(CreatureTriggerers.begin(), CreatureTriggerers.end(), CIntelligentPointer<CThingCreatureBase>(0)),
                             CreatureTriggerers.end());
    RegionCreationCount += CreatedCreatures.size() - creatureCount;
    if (IsUndeadGenerator())
        UpdateUndead();
    if (Initialised)
        UpdateCreatureTrigger(GetWorld().GetFrame());
    if (PDef.Get()) {
        if (!PDef->RandomEffects.empty() && --FramesUntilRandomEffect <= 0) {
            if (IS_THING_ALIVE(PRandomParticle))
                PRandomParticle->Kill(true);
            long random_index = GFRandom(PDef->RandomEffects.size(), GetPWorldSeed());
            PRandomParticle = CTCDParticleEmitter::Create(PDef->RandomEffects[random_index], GetParentThing().GetPos(), false);
            if (PRandomParticle != 0) {
                CTCDParticleEmitter* ptcdParticleEmitter;
                PRandomParticle->GetTC(&ptcdParticleEmitter);
                ptcdParticleEmitter->AttachToThing(GetParentThing(), 0x20, "", 0, 0.0f);
            }
            FramesUntilRandomEffect = PDef->SecondsBetweenEffects * CMainGameComponent::GetConstantFPS();
        }
        if (PDef->RandomSoundCriteria != "" && --FramesUntilRandomSoundEffect <= 0) {
            CTCSoundPlayer* ptc_sound_player;
            if (GetParentThing().GetTC(&ptc_sound_player))
                ptc_sound_player->PlayAttachedCriteria(PDef->RandomSoundCriteria, true, false, false, -1);
            FramesUntilRandomSoundEffect = PDef->SecondsBetweenSoundEffects * CMainGameComponent::GetConstantFPS();
        }
    }
}
int main(){
 for(seed=0;seed<65536;++seed){
  CTCCreatureGenerator x;len=0;x.Destroyed=pick(2,3)==0;x.Initialised=pick(8,2)!=0;x.ReadyToInitialise=pick(9,2)!=0;x.UndeadTriggered=pick(11,2)!=0;
  hero.dead=pick(7,2)!=0;DrawCreatureGeneratorDebug=pick(12,2)!=0;graphic=pick(13,2);bool needsGraphic=DrawCreatureGeneratorDebug&&!graphic;
  x.PDef.p=pick(14,4)?&def:0;def.RandomEffects.clear();for(unsigned j=0;j<pick(15,5);++j)def.RandomEffects.push_back(19*j+2);
  def.RandomSoundCriteria.s=pick(16,2)?"buzz":"";def.SecondsBetweenEffects=3;def.SecondsBetweenSoundEffects=4;
  long et=(long)pick(17,5)-1,st=(long)pick(18,5)-1;x.FramesUntilRandomEffect=et;x.FramesUntilRandomSoundEffect=st;
  CThing* old=pick(19,2)?&previous:0;previous.dead=pick(20,2)!=0;x.PRandomParticle=old;bool alive=old&&!old->dead;
  int removed=0,aliveCreated=0,aliveTriggers=0;for(unsigned k=0;k<pick(25,7);++k){CThing*v=pick(26+k,2)?&other:0;x.CreatedCreatures.push_back(v);if(v)++aliveCreated;else ++removed;}
  for(unsigned m=0;m<pick(40,8);++m){CThingCreatureBase*v=pick(41+m,2)?&otherHero:0;x.CreatureTriggerers.push_back(v);if(v)++aliveTriggers;}
  int before=x.CreatedCreatures.size(),beforeTriggers=x.CreatureTriggerers.size();x.RegionCreationCount=90;
  int w[32],n=0;bool active=pick(1,4)!=0;bool destroyed=x.Destroyed;bool init=x.Initialised;
  if(active&&!destroyed&&pick(3,2)&&pick(4,2)){destroyed=true;if(pick(5,2))w[n++]=TARGET;}
  active=active&&!destroyed;
  if(active){
   if(pick(6,2)&&!hero.dead){w[n++]=HERO;++aliveTriggers;}
   if(x.ReadyToInitialise&&!init){w[n++]=INIT;init=pick(10,2)!=0;}
   if(needsGraphic){w[n++]=GRAPHIC;w[n++]=STATICMESH;w[n++]=SETG;w[n++]=DRAW;}
   if(x.UndeadTriggered)w[n++]=UNDEAD;if(init)w[n++]=TRIGGER;
   if(x.PDef.p&&!def.RandomEffects.empty()&&--et<=0){if(alive)w[n++]=KILL;w[n++]=RANDOM;w[n++]=CREATE;old=pick(22,2)?&newParticle:0;if(old)w[n++]=ATTACH;et=72;}
   if(x.PDef.p&&def.RandomSoundCriteria.s[0]&&--st<=0){if(pick(24,2))w[n++]=SOUND;st=96;}
  }
  x.FrameUpdate();check(len==n);for(int i=0;i<n;++i)check(w[i]==trace[i]);
  check(x.Destroyed==destroyed&&x.Initialised==init&&x.PRandomParticle.p==old&&x.FramesUntilRandomEffect==et&&x.FramesUntilRandomSoundEffect==st);
  check(x.RegionCreationCount==(active?90-removed:90));check(x.CreatedCreatures.size()==(unsigned)(active?aliveCreated:before));check(x.CreatureTriggerers.size()==(unsigned)(active?aliveTriggers:beforeTriggers));
  if(active){for(std::list<CIntelligentPointer<CThing> >::iterator it=x.CreatedCreatures.begin();it!=x.CreatedCreatures.end();++it)check(it->p==&other);
   for(unsigned z=0;z<x.CreatureTriggerers.size();++z)check(x.CreatureTriggerers[z].p==&otherHero||(z+1==x.CreatureTriggerers.size()&&x.CreatureTriggerers[z].p==&hero));}
 }
 puts("0074c130_TEST PASS");return 0;
}
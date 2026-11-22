// The Briar Rose activation body runs against deterministic mode, world, and effect doubles.
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector&operator+=(const C3DVector&v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}C3DVector operator-(const C3DVector& v) const { return C3DVector(X-v.X,Y-v.Y,Z-v.Z); } C3DVector& operator/=(float n) { X/=n;Y/=n;Z/=n;return *this; }void AccurateNormalise(){float l=(float)sqrt(X*X+Y*Y+Z*Z);if(l){X/=l;Y/=l;Z/=l;}}};
typedef std::string CCharString;
struct CRightHandedSet{C3DVector direction;CRightHandedSet(const C3DVector&,const C3DVector&d):direction(d){}};
struct CTCPhysicsBase{C3DVector position,direction;int turns;CTCPhysicsBase():turns(0){}void ResetPosition(const C3DVector&v){position=v;}void ResetRHSet(const CRightHandedSet&s){++turns;direction=s.direction;}};
struct CThing;struct CThingMarker{};
struct CThingFilter_HasScriptName{CThingFilter_HasScriptName(const CCharString&){}};
template<class T>struct CIntelligentPointer{T*p;CIntelligentPointer(T*v=0):p(v){}T& operator*()const{return *p;} T* Get()const{return p;}};
template<class T>struct CDefPointer{T*p;CDefPointer(T*v=0):p(v){}T*operator->()const{return p;}bool IsNull()const{return !p;}};
const int NULL_DEF=0;
struct CBriarRoseDef{float SecondsToChargeDrainLife;int NumberOfHitsToKillImposters;};
struct CSpecialEffectsDef{bool body,sword;bool GetEffectFromString(const CCharString&s,long&n)const{if(s=="DRAIN_CHARGE_BODY"){n=1;return body;}n=2;return sword;}};
struct BaseDef{CBriarRoseDef*mode;CSpecialEffectsDef*fx;bool GetSubDef(CDefPointer<const CBriarRoseDef>*p){p->p=mode;return mode!=0;}bool GetSubDef(CDefPointer<const CSpecialEffectsDef>*p){p->p=fx;return fx!=0;}};
struct CTCBase {};
struct CTCAbsorbHits : CTCBase { int HitsRemaining; CTCAbsorbHits():HitsRemaining(0){} void AddHitsRemaining(long hits) { HitsRemaining += hits; } };
struct CTCDParticleEmitter{CThing*attached;int slot;CTCDParticleEmitter():attached(0),slot(0){}static CThing*Create(int,const C3DVector&,bool);void AttachToThing(CThing&t,int s,const CCharString&,int,float){attached=&t;slot=s;}};
struct CTCCarrying{CThing*item;CThing*GetThingInPrimarySlot() const {return item;}};
struct CThing{int ThingType;bool dead;CTCPhysicsBase body,*PhysicsTC;BaseDef defs;CTCDParticleEmitter fx;CTCCarrying*carrying;
 CThing():ThingType(0),dead(false),PhysicsTC(&body),carrying(0){defs.mode=0;defs.fx=0;}BaseDef*GetPBaseDef(){return &defs;}const C3DVector&GetPos(){return body.position;} CTCPhysicsBase* GetPhysicsTC() {return PhysicsTC;} int GetType() const { return ThingType; }
 void GetTCDriver(CTCDParticleEmitter**p){*p=&fx;}bool PeekTC(const CTCCarrying**p){*p=carrying;return carrying!=0;}};
static bool IS_THING_ALIVE(CThing*t){return t&&!t->dead;}
struct CThingAICreatureInit{CCharString ScriptName;};
typedef CThing CThingCreatureBase;
struct CCreatureAction_BriarRoseConcentrateInto{CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase&,CThingCreatureBase&,bool){}};
struct CThingAICreature:CThing{bool suspended,accept;int actions;CTCAbsorbHits absorb;CThingAICreature():suspended(false),accept(true),actions(0){}
 static int StatGetType(){return 1;}
 void SuspendAI(bool f,void*){suspended=f;}static CThingAICreature*Create(long,const C3DVector&,int,const CThingAICreatureInit&);
 CTCBase*AddTC(const CCharString&,bool,int){return accept?&absorb:0;}void SetCurrentAction(const CCreatureAction_BriarRoseConcentrateInto&){++actions;}};
static CThing*effects[3];static CThingAICreature*nextImposter;
CThing*CTCDParticleEmitter::Create(int i,const C3DVector&,bool){return effects[i];}
CThingAICreature*CThingAICreature::Create(long,const C3DVector&pos,int,const CThingAICreatureInit&init){if(init.ScriptName!="BRIAR_ROSE_IMPOSTER")return 0;if(nextImposter)nextImposter->body.position=pos;return nextImposter;}
struct CGameDefinitionManager{static CGameDefinitionManager&Get(){static CGameDefinitionManager m;return m;}long GetDefGlobalIndexFromName(const CCharString&){return 1;}};
struct CMainGameComponent { static long GetConstantFPS() { return 4; } };
static unsigned long GFRandom(unsigned long n, unsigned long* seed){ return n ? *seed % n : 0; }
struct World{std::vector<CThing*>markers;unsigned long seed;World&PeekThingSearchTools(){return *this;}unsigned long*GetWorldSeed(){return &seed;}
 template<class T>void GetAllThings(const CThingFilter_HasScriptName&,std::vector<CThing*>&out){out=markers;}};
namespace NCreatureMode{
struct CModeStandBase{int calls;CModeStandBase():calls(0){}void OnActivate(){++calls;}};
struct CModeBriarRoseConcentrate:CModeStandBase{CThing*ParentThing,*PTarget;CDefPointer<const CBriarRoseDef>PDef;CIntelligentPointer<CThing>PBodyEffect,PSwordEffect;int Timer;bool Imposter;std::vector<CIntelligentPointer<CThing> > Imposters;World world;World& GetWorld(){return world;} CThing& GetParentThing(){return *ParentThing;}void OnActivate();};
}

void NCreatureMode::CModeBriarRoseConcentrate::OnActivate()
{
    CModeStandBase::OnActivate();
    float seconds_to_charge = 15.0f;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&PDef))
        seconds_to_charge = PDef->SecondsToChargeDrainLife;
    Timer = (long)(CMainGameComponent::GetConstantFPS() * seconds_to_charge);

    if (GetParentThing().GetType() == CThingAICreature::StatGetType()) {
        CThingAICreature* p_ai_creature = static_cast<CThingAICreature*>(&GetParentThing());
        p_ai_creature->SuspendAI(true, this);
    }

    if (!Imposter) {
        std::vector<CThing*> markers;
        GetWorld().PeekThingSearchTools().GetAllThings<CThingMarker>(CThingFilter_HasScriptName("TeleportMarker"), markers);

        if (!markers.empty()) {
            long num_markers = markers.size();
            Imposters.reserve(num_markers);
            long briar_marker_index = GFRandom(num_markers, GetWorld().GetWorldSeed());
            GetParentThing().GetPhysicsTC()->ResetPosition(markers[briar_marker_index]->GetPos());

            long imposter_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_RIVAL_HERO_BRIAR_ROSE_IMPOSTER");
            CThingAICreatureInit init;
            init.ScriptName = "BRIAR_ROSE_IMPOSTER";

            C3DVector arena_centre(0.0f, 0.0f, 0.0f);
            for (long c1 = 0; c1 != num_markers; ++c1) {
                CThing* pthing = markers[c1];
                const C3DVector& pos = pthing->GetPos();
                arena_centre += pos;
                if (c1 != briar_marker_index) {
                    CThingAICreature* pimposter = CThingAICreature::Create(imposter_def_index, pos, 4, init);
                    if (IS_THING_ALIVE(pimposter)) {
                        if (!PDef.IsNull() && PDef->NumberOfHitsToKillImposters > 1) {
                            long num_absorb_hits = PDef->NumberOfHitsToKillImposters - 1;
                            CTCBase* ptc_base = pimposter->AddTC("CTCAbsorbHits", true, 0);
                            if (ptc_base) {
                                CTCAbsorbHits* ptc_absorb = static_cast<CTCAbsorbHits*>(ptc_base);
                                ptc_absorb->AddHitsRemaining(num_absorb_hits);
                            }
                        }
                        pimposter->SetCurrentAction(CCreatureAction_BriarRoseConcentrateInto(*pimposter, *PTarget, true));
                        Imposters.push_back(CIntelligentPointer<CThing>(pimposter));
                    }
                }
            }
            arena_centre /= num_markers;

            {
                C3DVector line_to_centre = arena_centre - GetParentThing().GetPos();
                line_to_centre.Z = 0.0f;
                line_to_centre.AccurateNormalise();
                GetParentThing().GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), line_to_centre));
            }

            for (std::vector<CIntelligentPointer<CThing> >::iterator it = Imposters.begin(); it != Imposters.end(); ++it) {
                CThing* pimposter = it->Get();
                C3DVector line_to_centre = arena_centre - pimposter->GetPos();
                line_to_centre.Z = 0.0f;
                line_to_centre.AccurateNormalise();
                pimposter->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), line_to_centre));
            }
        }
    }

    CDefPointer<const CSpecialEffectsDef> pdef;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&pdef)) {
        long fx_index = 0;
        if (pdef->GetEffectFromString("DRAIN_CHARGE_BODY", fx_index)) {
            CThing* pparticle = CTCDParticleEmitter::Create(fx_index, GetParentThing().GetPos(), false);
            if (IS_THING_ALIVE(pparticle)) {
                CTCDParticleEmitter* ptc_particle;
                pparticle->GetTCDriver(&ptc_particle);
                ptc_particle->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
                PBodyEffect = CIntelligentPointer<CThing>(pparticle);
            }
        }
        if (pdef->GetEffectFromString("DRAIN_CHARGE_SWORD", fx_index)) {
            CThing* pparticle = CTCDParticleEmitter::Create(fx_index, GetParentThing().GetPos(), false);
            if (IS_THING_ALIVE(pparticle)) {
                CTCDParticleEmitter* ptc_particle;
                pparticle->GetTCDriver(&ptc_particle);
                const CTCCarrying* ptc_carrying;
                if (GetParentThing().PeekTC(&ptc_carrying)) {
                    CThing* psword = ptc_carrying->GetThingInPrimarySlot();
                    if (IS_THING_ALIVE(psword))
                        ptc_particle->AttachToThing(*psword, 10, "WEAPON_FOCUS_01", 0, 0.0f);
                }
                PSwordEffect = CIntelligentPointer<CThing>(pparticle);
            }
        }
    }
}

int main(){
 for(int i=0;i<96;++i){NCreatureMode::CModeBriarRoseConcentrate m;CThingAICreature hero,copy;CThing marker1,marker2,body,sword,weapon;CTCCarrying carrying;carrying.item=(i%5)?&weapon:0;weapon.dead=i%7==0;hero.carrying=i%3?&carrying:0;hero.ThingType=i%2;
  CBriarRoseDef mode={3,((i%4)?3:1)};CSpecialEffectsDef fx={i%4!=1,i%4!=2};hero.defs.fx=i%8?&fx:0;hero.defs.mode=i%3?&mode:0;
  m.world.seed = (i%2); m.ParentThing=&hero;m.PTarget=&weapon;m.Imposter=i%5==1;marker2.body.position=C3DVector(4,0,2);if(i%6)m.world.markers.push_back(&marker1);if(i%6>=2)m.world.markers.push_back(&marker2);
  nextImposter=i%7?&copy:0;copy.dead=i%9==0;copy.accept=i%4!=2;effects[1]=i%11?&body:0;effects[2]=i%13?&sword:0;body.dead=i%7==0;sword.dead=i%3==0;
  m.OnActivate();bool imp=!m.Imposter&&m.world.markers.size()==2&&nextImposter&&!copy.dead;bool hasBody=hero.defs.fx&&fx.body&&effects[1]&&!body.dead,hasSword=hero.defs.fx&&fx.sword&&effects[2]&&!sword.dead;
  if(m.calls!=1||hero.suspended!=(hero.ThingType==1)||m.Timer!=(hero.defs.mode?12:60)||m.Imposters.size()!=(int)imp||copy.actions!=(int)imp)return 1;
  if(copy.absorb.HitsRemaining!=(imp&&hero.defs.mode&&copy.accept?mode.NumberOfHitsToKillImposters-1:0))return 2;
  if(hero.body.turns!=(!m.Imposter&&!m.world.markers.empty())||copy.body.turns!=imp)return 3;
  if(imp) {
   const float direction = (m.world.seed % 2) ? -1.0f : 1.0f;
   if(hero.body.direction.X!=direction || copy.body.direction.X!=-direction) return 4;
  }
  if((m.PBodyEffect.p!=0)!=hasBody||(m.PSwordEffect.p!=0)!=hasSword||body.fx.attached!=(hasBody?&hero:0))return 5;
  if(sword.fx.attached!=(hasSword&&hero.carrying&&IS_THING_ALIVE(carrying.item)?&weapon:0))return 6;
 }
 puts("008e8060_TEST PASS");return 0;
}
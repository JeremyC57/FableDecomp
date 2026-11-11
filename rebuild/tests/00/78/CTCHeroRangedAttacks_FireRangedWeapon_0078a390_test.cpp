#include <vector>
#include <cstdio>
#include <cstdlib>
static void check(bool b){if(!b){std::puts("FAIL");std::exit(1);}}
struct E{int k,v;E(int a,int b=0):k(a),v(b){}bool operator==(const E&o)const{return k==o.k&&v==o.v;}};static std::vector<E> trace;
class C3DVector{ public:float X,Y,Z;};class CThing;class CCreatureActionBase;class CTCWeapon;class CTCCarrying;class CTCTargetingBase;class CTCHeroRangedAttacks;
enum EPlayerMode{MODE_NORMAL=0,MODE_FIRST=25};enum EMessageEventType{MSG_FIRE=67};
class CPlayer{ public:EPlayerMode mode;EPlayerMode GetCurrentMode()const{return mode;}};
class CThing{ public:int id;bool dead;CPlayer player;C3DVector pos;CTCCarrying*carrying;CTCWeapon*weapon;CTCTargetingBase*targeting;bool IsDead()const{return dead;}CPlayer&GetPlayer(){return player;}const C3DVector&GetPos()const{return pos;}bool GetTC(CTCCarrying**);bool PeekTC(const CTCWeapon**)const;bool PeekTC(const CTCTargetingBase**)const;};
class CThingCreatureBase:public CThing{ public:bool accept;bool SetCurrentAction(const CCreatureActionBase&);};
class CTCCarrying{ public:CThing*item;CThing*GetThingInPrimarySlot()const{return item;}};
class CTCWeapon{ public:bool projectile,loaded;bool IsProjectileWeapon()const{return projectile;}bool IsProjectileWeaponLoaded()const{return loaded;}};
class CTCTargetingBase{ public:CThing*target;CThing*GetPTargetedThing()const{return target;}};
bool CThing::GetTC(CTCCarrying**o){*o=carrying;return carrying!=0;}bool CThing::PeekTC(const CTCWeapon**o)const{*o=weapon;return weapon!=0;}bool CThing::PeekTC(const CTCTargetingBase**o)const{*o=targeting;return targeting!=0;}
template<class T>bool IS_THING_ALIVE(const T&t){return t!=0&&!t->IsDead();}
class CHeroCombatDef{ public:float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;};class CMeleeCombatAbilityDef{ public:};
template<class T>class CDefPointer{ public:T*p;CDefPointer():p(0){}CDefPointer(T*q):p(q){}T*operator->()const{return p;}};
class CMainGameComponent{ public:static long fps;static long GetConstantFPS(){return fps;}};long CMainGameComponent::fps;
class CMessageEventManager{ public:void AddMessage(EMessageEventType,unsigned long,const C3DVector&,CThing*,CThing*,float,float,long);};class CWorld{ public:CMessageEventManager messages;CMessageEventManager&GetMessageEventManager(){return messages;}};
struct Shot{CThingCreatureBase*creature;const CThing*target;float damage,accuracy,buildup;bool decap;long delay,count;};static Shot shot;static int actionCount,performCount,messageCount,worldCount;static bool callback,validTarget;static CTCHeroRangedAttacks*active;
class CCreatureActionBase{ public:};
class CCreatureAction_FireMissileWeapon:public CCreatureActionBase{ public:CCreatureAction_FireMissileWeapon(CThingCreatureBase&c,const CThing*t,float d,float a,float b,bool cut,CDefPointer<const CMeleeCombatAbilityDef>p,long delay,long count){check(p.p==0);shot.creature=&c;shot.target=t;shot.damage=d;shot.accuracy=a;shot.buildup=b;shot.decap=cut;shot.delay=delay;shot.count=count;++actionCount;trace.push_back(E(2));}~CCreatureAction_FireMissileWeapon(){trace.push_back(E(7));}void PerformAction(){++performCount;trace.push_back(E(4));}};
class CTCHeroRangedAttacks{ public:CThing*parent;CWorld*world;CThingCreatureBase*PParentCreature;CDefPointer<const CHeroCombatDef>PHeroCombatDef;bool FireQueued;long AimingTimer;float DamageMultiplier,Accuracy;bool Decapitate;static float AccuracyOverride;CThing&GetParentThing(){return *parent;}CWorld&GetWorld()const{++worldCount;trace.push_back(E(5));if(callback){CMainGameComponent::fps*=2;parent->pos.X+=8.0f;}return *world;}float GetShotBuildupTime()const{double time=AimingTimer;return float(time/CMainGameComponent::GetConstantFPS());}void FireRangedWeapon();};float CTCHeroRangedAttacks::AccuracyOverride;
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&){trace.push_back(E(3));if(callback)CMainGameComponent::fps*=2;return accept;}
static unsigned long messageFrames;static float messageX;
void CMessageEventManager::AddMessage(EMessageEventType t,unsigned long frames,const C3DVector&p,CThing*a,CThing*b,float f,float g,long l){check(t==MSG_FIRE&&a==0&&b==0&&f==0&&g==0&&l==0);++messageCount;messageFrames=frames;messageX=p.X;trace.push_back(E(6));}
namespace NCreatureMode{static bool IsValidRangedWeaponTargetForShooter(const CThing&c,const CThing&t,float angle){check(c.id==1&&t.id==3&&angle==128.0f);trace.push_back(E(1));if(callback){active->DamageMultiplier=16.0f;active->Accuracy=16.0f;CTCHeroRangedAttacks::AccuracyOverride=16.0f;}return validTarget;}}
void CTCHeroRangedAttacks::FireRangedWeapon()
{
 if(FireQueued&&AimingTimer>=0) {
  CTCCarrying* ptc_carrying;
  GetParentThing().GetTC(&ptc_carrying);
  CThing* pcarried_thing=ptc_carrying->GetThingInPrimarySlot();
  const CTCWeapon* ptc_weapon;
  if(IS_THING_ALIVE(pcarried_thing)&&pcarried_thing->PeekTC(&ptc_weapon)&&ptc_weapon->IsProjectileWeapon()&&ptc_weapon->IsProjectileWeaponLoaded()) {
   const CTCTargetingBase* ptc_targeting;
   GetParentThing().PeekTC(&ptc_targeting);
   CThing* ptarget=ptc_targeting->GetPTargetedThing();
   float augmented_damage_multiplier=DamageMultiplier;
   float accuracy_to_use=AccuracyOverride>=0.0f?AccuracyOverride:Accuracy;
   if(GetParentThing().GetPlayer().GetCurrentMode()==(EPlayerMode)25)accuracy_to_use=100.0f;
   CDefPointer<const CMeleeCombatAbilityDef> pdef;
   if(IS_THING_ALIVE(ptarget)&&!NCreatureMode::IsValidRangedWeaponTargetForShooter(GetParentThing(),*ptarget,PHeroCombatDef->MaxAngleToTurnToShootAtTargetInThirdPersonBowMode))ptarget=0;
   CCreatureAction_FireMissileWeapon action_bloke(*PParentCreature,ptarget,augmented_damage_multiplier,accuracy_to_use,GetShotBuildupTime(),Decapitate,pdef,100,4);
   if(!PParentCreature->SetCurrentAction(action_bloke))action_bloke.PerformAction();
   CThing& thing=GetParentThing();
   unsigned long frames=CMainGameComponent::GetConstantFPS();
   CMessageEventManager& messages=GetWorld().GetMessageEventManager();
   messages.AddMessage((EMessageEventType)67,frames,thing.GetPos(),0,0,0.0f,0.0f,0);
   FireQueued=false;
  }
 }
}

int main(){unsigned long cases=0;
 for(int queued=0;queued<2;++queued)for(int aim=0;aim<3;++aim)for(int carried=0;carried<4;++carried)for(int projectile=0;projectile<2;++projectile)for(int loaded=0;loaded<2;++loaded)for(int target=0;target<3;++target)for(int valid=0;valid<2;++valid)for(int over=0;over<3;++over)for(int mode=0;mode<2;++mode)for(int accept=0;accept<2;++accept)for(int decap=0;decap<2;++decap)for(int fps=0;fps<2;++fps)for(int cb=0;cb<2;++cb){
 CThingCreatureBase hero;hero.id=1;hero.dead=false;hero.player.mode=mode?MODE_FIRST:MODE_NORMAL;hero.accept=accept!=0;hero.pos.X=32.0f;hero.pos.Y=64.0f;hero.pos.Z=128.0f;
 CThing weapon,mark;weapon.id=2;weapon.dead=carried==2;mark.id=3;mark.dead=target==2;CTCWeapon wt;wt.projectile=projectile!=0;wt.loaded=loaded!=0;weapon.weapon=carried==3?0:&wt;CTCCarrying carrying;carrying.item=carried?&weapon:0;hero.carrying=&carrying;CTCTargetingBase targeting;targeting.target=target?&mark:0;hero.targeting=&targeting;CHeroCombatDef def;def.MaxAngleToTurnToShootAtTargetInThirdPersonBowMode=128.0f;CWorld world;
 CTCHeroRangedAttacks a;a.parent=&hero;a.world=&world;a.PParentCreature=&hero;a.PHeroCombatDef.p=&def;a.FireQueued=queued!=0;a.AimingTimer=aim==0?-1:aim==1?0:64;a.DamageMultiplier=2.0f;a.Accuracy=64.0f;a.Decapitate=decap!=0;CTCHeroRangedAttacks::AccuracyOverride=over==0?-1.0f:over==1?0.0f:128.0f;active=&a;CMainGameComponent::fps=fps?32:16;long initialFPS=CMainGameComponent::fps;callback=cb!=0;validTarget=valid!=0;actionCount=performCount=messageCount=worldCount=0;trace.clear();a.FireRangedWeapon();
 bool fires=queued&&aim>0&&carried==1&&projectile&&loaded;std::vector<E>expected;if(fires){if(target==1)expected.push_back(E(1));expected.push_back(E(2));expected.push_back(E(3));if(!accept)expected.push_back(E(4));expected.push_back(E(5));expected.push_back(E(6));expected.push_back(E(7));check(shot.creature==&hero&&shot.target==(target==1&&!valid?0:target?&mark:0));check(shot.damage==2.0f&&shot.accuracy==(mode?100.0f:over==0?64.0f:over==1?0.0f:128.0f));check(shot.buildup==(aim==1?0.0f:float(64/initialFPS)));check(shot.decap==(decap!=0)&&shot.delay==100&&shot.count==4);check(messageFrames==unsigned(initialFPS*(cb?2:1)));check(messageX==(cb?40.0f:32.0f));}
 check(trace==expected);check(actionCount==int(fires)&&messageCount==int(fires)&&worldCount==int(fires)&&performCount==int(fires&&!accept));check(a.FireQueued==bool(queued&&!fires));check(CMainGameComponent::fps==initialFPS*(fires&&cb?4:1));++cases;
 }
 check(cases==55296);std::puts("0078a390_TEST PASS");return 0;
}
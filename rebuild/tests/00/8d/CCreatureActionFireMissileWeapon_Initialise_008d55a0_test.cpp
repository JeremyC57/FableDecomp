#include <cstdio>
#include <cstdlib>
#include <cmath>
static void check(bool ok){if(!ok)std::abort();}
static int targetGets,creatureGets,players,firstQueries,positions,normalizes,orientations,carryingQueries,weaponQueries,primaryQueries,unloads,transitions,bases;
static const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector{ public:
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 bool IsNonZero()const{float squared=X*X;squared+=Z*Z;squared+=Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 C3DVector& AccurateNormalise(){++normalizes;float length=(float)std::sqrt((double)(X*X+Y*Y+Z*Z));X/=length;Y/=length;Z/=length;return *this;}
};
class CRightHandedSet{ public:C3DVector Up,Forward;CRightHandedSet(const C3DVector& up,const C3DVector& forward):Up(up),Forward(forward){}};
class CPlayer{ public:bool z,first;bool IsZTargeting()const{return z;}bool IsFirstPersonTargeting()const{++firstQueries;return first;}};
class CAnimationEntry{ public:long transition;long GetTransitionTime()const{++transitions;return transition;}};
template<class T>class CCountedPointer{ public:T* Data;bool IsNull()const{return Data==0;}T* operator->()const{return Data;}};
class CThing;class CTCWeapon;class CTCCarrying;class CTCPhysicsBase;class CCreatureAction_FireMissileWeapon;
static CCreatureAction_FireMissileWeapon* active;static bool expire,swapAnim;static CAnimationEntry replacement;static long expectedDelay;
class CTCPhysicsBase{ public:C3DVector last;void SetRHSet(const CRightHandedSet&);};
class CThing{ public:
 bool dead,hasCarry,hasWeapon;CPlayer* player;C3DVector pos;CTCPhysicsBase* physics;CTCCarrying* carrying;CTCWeapon* weapon;
 bool IsDead()const{return dead;}
 const CPlayer& PeekPlayer()const{++players;check(player!=0);return *player;}
 const C3DVector& GetPos()const{++positions;return pos;}
 CTCPhysicsBase* GetPhysicsTC(){return physics;}
 bool GetTC(CTCCarrying** out){++carryingQueries;if(!hasCarry)return false;*out=carrying;return true;}
 bool GetTC(CTCWeapon** out){++weaponQueries;if(!hasWeapon)return false;*out=weapon;return true;}
};
template<class T>class CIntelligentPointer{ public:
 T* pointer;bool target;T* Get()const{if(target)++targetGets;else ++creatureGets;return pointer;}
 T* operator->()const{return Get();}bool operator!=(int)const{return Get()!=0;}
};
template<class T>static bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CTCCarrying{ public:CThing* held;CThing* GetThingInPrimarySlot()const{++primaryQueries;return held;}};
class CTCWeapon{ public:void SetProjectileWeaponLoaded(bool,CThing*);};
class CCreatureActionBase{ public:
 CIntelligentPointer<CThing> PCreature;CCountedPointer<CAnimationEntry> PAnimEntry;long DelayTimer;
 void Initialise(){++bases;check(DelayTimer==expectedDelay);}
};
class CCreatureAction_FireMissileWeapon:public CCreatureActionBase{ public:CIntelligentPointer<const CThing> PTarget;void Initialise();};
void CTCPhysicsBase::SetRHSet(const CRightHandedSet& rhs){++orientations;check(rhs.Up.X==0&&rhs.Up.Y==0&&rhs.Up.Z==1&&rhs.Forward.Z==0);last=rhs.Forward;if(expire)active->PCreature.pointer=0;}
void CTCWeapon::SetProjectileWeaponLoaded(bool loaded,CThing* projectile){check(!loaded&&projectile==0);++unloads;if(swapAnim)active->PAnimEntry.Data=&replacement;}
void CCreatureAction_FireMissileWeapon::Initialise()
{
 if(IS_THING_ALIVE(PTarget) && !(PCreature->PeekPlayer().IsZTargeting() || PCreature->PeekPlayer().IsFirstPersonTargeting())) {
  C3DVector line=PTarget->GetPos()-PCreature->GetPos();
  line.Z=0.0f;
  if(line.IsNonZero()) {
   line.AccurateNormalise();
   PCreature->GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f,0.0f,1.0f),line));
  }
 }
 CTCCarrying* ptc_carry;
 if(IS_THING_ALIVE(PCreature) && PCreature->GetTC(&ptc_carry)) {
  CThing* pcarried_thing=ptc_carry->GetThingInPrimarySlot();
  CTCWeapon* ptc_weapon;
  if(IS_THING_ALIVE(pcarried_thing) && pcarried_thing->GetTC(&ptc_weapon))
   ptc_weapon->SetProjectileWeaponLoaded(false,0);
 }
 if(!PAnimEntry.IsNull()) DelayTimer=PAnimEntry->GetTransitionTime();
 CCreatureActionBase::Initialise();
}

int main(){
 const C3DVector lines[]={C3DVector(0,0,8),C3DVector(1,0,2),C3DVector(-1,0,-4),C3DVector(3,4,1),C3DVector(0,-2,0),C3DVector(1.0f/32768.0f,0,0),C3DVector(1.0f/4096.0f,0,0)};int cases=0;
 for(int ts=0;ts<3;++ts)for(int cs=0;cs<3;++cs)for(int flags=0;flags<128;++flags)for(int li=0;li<7;++li){
  if(ts==2&&cs==0)continue;
  targetGets=creatureGets=players=firstQueries=positions=normalizes=orientations=carryingQueries=weaponQueries=primaryQueries=unloads=transitions=bases=0;
  CPlayer player;player.z=(flags&1)!=0;player.first=(flags&2)!=0;CThing creature,target,held;creature.dead=cs==1;target.dead=ts==1;held.dead=(flags&4)!=0;
  creature.player=&player;creature.hasCarry=(flags&8)!=0;held.hasWeapon=(flags&16)!=0;CTCCarrying carry;CTCWeapon weapon;CTCPhysicsBase physics;physics.last=C3DVector(8,8,8);
  creature.physics=&physics;creature.carrying=&carry;held.weapon=&weapon;carry.held=(flags&32)?&held:0;creature.pos=C3DVector(0,0,0);target.pos=lines[li];
  CAnimationEntry anim;anim.transition=16;replacement.transition=64;CCreatureAction_FireMissileWeapon action;active=&action;action.PCreature.pointer=cs==0?0:&creature;action.PCreature.target=false;action.PTarget.pointer=ts==0?0:&target;action.PTarget.target=true;action.DelayTimer=32;action.PAnimEntry.Data=(flags&64)?&anim:0;
  expire=(li&1)!=0;swapAnim=(li&2)!=0;
  bool face=ts==2&&!player.z&&!player.first;double dx=lines[li].X,dy=lines[li].Y;bool rotate=face&&(dx*dx+dy*dy>1.0/100000000.0);bool aliveCreature=cs==2&&!(rotate&&expire);bool gotCarry=aliveCreature&&creature.hasCarry;bool gotHeld=gotCarry&&(flags&32)!=0;bool heldAlive=gotHeld&&!held.dead;bool unload=heldAlive&&held.hasWeapon;
  expectedDelay=unload&&swapAnim?64:((flags&64)?16:32);action.Initialise();
  check(bases==1&&action.DelayTimer==expectedDelay&&players==(ts==2?(player.z?1:2):0)&&firstQueries==(ts==2&&!player.z?1:0));
  check(positions==(face?2:0)&&normalizes==(rotate?1:0)&&orientations==(rotate?1:0));
  check(carryingQueries==(aliveCreature?1:0)&&primaryQueries==(gotCarry?1:0)&&weaponQueries==(heldAlive?1:0)&&unloads==(unload?1:0));
  check(transitions==((unload&&swapAnim)||(flags&64)?1:0));
  check(targetGets==(ts==0?1:2)+(face?1:0));
  int expectedCreatureGets=(ts==2?(player.z?1:2):0)+(face?1:0)+(rotate?1:0)+(cs==0||(rotate&&expire)?1:2)+(aliveCreature?1:0);check(creatureGets==expectedCreatureGets);
  if(rotate){double length=std::sqrt(dx*dx+dy*dy);check(std::fabs(physics.last.X-dx/length)<1.0/1048576.0&&std::fabs(physics.last.Y-dy/length)<1.0/1048576.0);}else check(physics.last.X==8);
  check(creature.pos.X==0&&target.pos.X==lines[li].X);++cases;
 }
 check(cases==7168);std::puts("008d55a0_TEST PASS");return 0;
}
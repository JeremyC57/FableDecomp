#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
static unsigned caseId;
#define Check(x) do{if(!(x)){printf("failed line %d case %u\n",__LINE__,caseId);exit(1);}}while(0)
namespace std {
template<class T> struct vector {
 T cells[4];unsigned count; vector():count(0){} T* begin(){return cells;}
 T* end(){return cells+count;}unsigned size()const{return count;}
 void push_back(const T& v){Check(count<4);cells[count++]=v;}
};
template<class T>T* find(T* first,T* end,const T& value){while(first!=end&&!(*first==value))++first;return first;}
}
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float x)const{return C3DVector(X*x,Y*x,Z*x);}
 C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
 float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}
 float GetSquaredMagnitude()const{return Dot(*this);}
 float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
 void Normalise(){float m=(float)sqrt(GetSquaredMagnitude());if(m){X/=m;Y/=m;Z/=m;}}
};
static bool close(float a,float b){return fabs(a-b)<0.001;}
static void same(const C3DVector& a,const C3DVector& b){Check(close(a.X,b.X)&&close(a.Y,b.Y)&&close(a.Z,b.Z));}
double GFCos(float t){return cos(t*6.2831855);}
float GFSqr(float x){return x*x;}
struct CRightHandedSet {C3DVector up,forward;CRightHandedSet(const C3DVector& a,const C3DVector& b):up(a),forward(b){}};
struct CTCPhysicsBase {
 C3DVector last,face,next,tele;int orientations,steps,teleports;bool use;
 CTCPhysicsBase():face(1,0,0),orientations(0),steps(0),teleports(0),use(false){}
 C3DVector GetFacingVector(){return face;}
 const C3DVector& GetLastPosition(){return last;}
 void SetFacingAnglesFromRHSet(const CRightHandedSet& r){same(r.up,C3DVector(0,0,1));face=r.forward;++orientations;}
 void SetForcedToMovePos(const C3DVector& p,bool b){next=p;use=b;++steps;}
 void SetPosition(const C3DVector& p){tele=p;++teleports;}
};
struct CTCSoundPlayer{int played;CTCSoundPlayer():played(0){}void PlayAttachedCriteria(const char* s,bool a,bool b,bool c,long d){Check(!strcmp(s,"HERO_ABILITY_ASSASSIN_RUSH_COLLISION")&&a&&!b&&!c&&d==-1);++played;}};
struct CThing {bool dead;unsigned flags,type;C3DVector pos;CThing():dead(false),flags(0),type(0){}C3DVector& GetPos(){return pos;}unsigned GetTypeFlags(){return flags;}unsigned GetType(){return type;}};
struct CCreatureActionBase{};
struct CThingCreatureBase:CThing {
 CTCPhysicsBase phys; CTCSoundPlayer* sound;float floor;int finishes,actions,heightAsks;
 CThingCreatureBase():sound(0),floor(0),finishes(0),actions(0),heightAsks(0){}
 CTCPhysicsBase* GetPhysicsTC(){return &phys;}
 float GetHeightZBelowAt(const C3DVector&,bool,bool last){Check(last);++heightAsks;return floor;}
 bool GetTC(CTCSoundPlayer** p){*p=sound;return sound!=0;}
 void FinishCurrentAction(){++finishes;}void SetCurrentAction(CCreatureActionBase&){++actions;}
};
struct CThingAICreature:CThingCreatureBase{int resets;CThingAICreature():resets(0){}void ForceAIReset(){++resets;}};
template<class T>struct Ptr{T* p;Ptr(T* v=0):p(v){}T* Get()const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}bool operator!=(int)const{return p!=0;}bool operator==(const Ptr& v)const{return p==v.p;}};
template<class T>bool IS_THING_ALIVE(const Ptr<T>& p){return p.p&&!p.p->dead;}
template<class S,class T>T* GFDebugDynamicCast(S* s,T*){return static_cast<T*>(s);}
template<class S,class T>T* GFDebugPDynamicCast(S* s,T*){return static_cast<T*>(s);}
struct C2DCoordI{long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
struct CEnvironmentSpellEffectInterpolator{unsigned blurs;C3DVector blur;void* id;CEnvironmentSpellEffectInterpolator():blurs(0),id(0){}void RadialBlurSetCenterWorldPos(void* key,const C3DVector& v){id=key;blur=v;++blurs;}};
struct CWorld{bool pass;int checks,slows;void* key;C2DCoordI cell;CEnvironmentSpellEffectInterpolator interp;
 CWorld():pass(true),checks(0),slows(0),key(0),cell(0,0){}
 CWorld& PeekWorldMap(){return *this;}bool IsPosPassable(C2DCoordI p){cell=p;++checks;return pass;}
 CWorld& GetEnvironment(){return *this;}CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator(){return interp;}
 CWorld& GetBulletTimeManager(){return *this;}void SetBulletTimeSlowdownMultiplier(long m,void* k,float f){Check(m==7&&f==0.5f);key=k;++slows;}
};
struct CMainGameComponent{static long GetConstantFPS(){return 30;}};
struct Def{float RunSpeed,SlowTimeLength,DistFromFloor;long SlowTimeMultiplier,NumPeopleHit[3];};
struct Base {
 CThingAICreature parent;CWorld w;Def* PDef;Ptr<CThing> PTarget,CollidedThing;bool DirectionOverride,IsZTargetting,StartedMoving,FakeCollideWithSomething;
 C3DVector DirectionOverrideVector,PositionStartedFrom;float DashDistance;
 CThingCreatureBase& GetParentThing(){return parent;}CWorld& GetWorld(){return w;}
};
struct CTCBattleCharge:Base {std::vector<Ptr<CThing> > PeopleHit;C3DVector DisplacementVector;long SpellLevel;int hits,finishes;bool finishCollide;
 CTCBattleCharge():hits(0),finishes(0),finishCollide(false){}
 void FinishMove(CThingCreatureBase&,bool hit){++finishes;finishCollide=hit;}void CreateHitEffect(){++hits;}
 void RushForwards();void SetupFacingDirection(C3DVector&);
};
struct CTCAssassinRush:Base {bool UseCollision,StartTrailDelay;C3DVector BehindTargetPos;long SlowTimeLength;void* RadialBlurFadeKey;Ptr<CCreatureActionBase> FinalAction;
 int clean;CTCAssassinRush():StartTrailDelay(false),SlowTimeLength(0),clean(0){}
 C3DVector GetValidTeleportPos(const CTCPhysicsBase* p){Check(p==&parent.phys);return C3DVector(8,2,1);}
 void CleanUpEffects(){++clean;}void FinishMove(CThingCreatureBase&);void RushForwards();void SetupFacingDirection(C3DVector&);
};
const float MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE = 25.0f;

void CTCBattleCharge::SetupFacingDirection(C3DVector& direction)
{
    direction.Normalise();
    if(direction.Dot(C3DVector(0,0,1))>GFCos(60.f/360.f))direction=GetParentThing().GetPhysicsTC()->GetFacingVector();
}
void CTCBattleCharge::RushForwards()
{
    CThingCreatureBase& parent_creature=*GFDebugDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    CTCPhysicsBase& parent_physics=*GetParentThing().GetPhysicsTC();
    C3DVector direction_to_move;
    if(DirectionOverride && !IsZTargetting) direction_to_move=DirectionOverrideVector;
    else if(IS_THING_ALIVE(PTarget)) direction_to_move=PTarget->GetPos()-parent_creature.GetPos();
    else direction_to_move=parent_creature.GetPhysicsTC()->GetFacingVector();
    SetupFacingDirection(direction_to_move);
    parent_physics.SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0,0,1),direction_to_move));
    C3DVector movement_vector=direction_to_move*PDef->RunSpeed;
    float distance_from_start_point_sq=(parent_creature.GetPos()-PositionStartedFrom).GetSquaredMagnitude();
    float distance_travelled_sq=DisplacementVector.GetSquaredMagnitude();
    float sqrdashdistance=GFSqr(DashDistance);
    if(distance_from_start_point_sq<sqrdashdistance && distance_travelled_sq<sqrdashdistance) {
        if(StartedMoving) {
            if(CollidedThing!=0 && (CollidedThing->GetTypeFlags()&8)) {
                if(std::find(PeopleHit.begin(),PeopleHit.end(),CollidedThing)==PeopleHit.end()){
                    CreateHitEffect();
                    PeopleHit.push_back(CollidedThing);
                    if(PeopleHit.size()>=PDef->NumPeopleHit[SpellLevel]) {
                        FinishMove(parent_creature,true);
                        return;
                    }
                }
                C3DVector target_pos=parent_creature.GetPos()+movement_vector;
                float height_z=parent_creature.GetHeightZBelowAt(target_pos,true,true);
                if(height_z<target_pos.Z)target_pos.Z=height_z;
                parent_physics.SetForcedToMovePos(target_pos,true);
                CollidedThing=(CThing*)0;
                DisplacementVector+=target_pos-parent_creature.GetPos();
                return;
            }
            C3DVector lastpos=parent_physics.GetLastPosition();
            C3DVector expected_position=lastpos+movement_vector;
            expected_position.Z=parent_creature.GetHeightZBelowAt(expected_position,false,true);
            C3DVector thingpos=parent_creature.GetPos();
            C3DVector difference=expected_position-thingpos;
            if(FakeCollideWithSomething || difference.GetSquaredMagnitude2D()>GFSqr(PDef->RunSpeed*0.3f) || !GetWorld().PeekWorldMap().IsPosPassable(C2DCoordI((long)expected_position.X,(long)expected_position.Y))) {
                CreateHitEffect();FinishMove(parent_creature,true);return;
            }
        }
        C3DVector target_pos=parent_creature.GetPos()+movement_vector;
        float height_z=parent_creature.GetHeightZBelowAt(target_pos,true,true);
        if(height_z<target_pos.Z) {
            float diff=target_pos.Z-height_z;
            if(diff<MAX_ALLOWED_Z_CHANGE_FOR_RUSH_MOVE)target_pos.Z=height_z;
        }
        parent_physics.SetForcedToMovePos(target_pos,true);
        DisplacementVector+=target_pos-parent_creature.GetPos();
        C3DVector pos=parent_creature.GetPos();
        C3DVector dir=target_pos-pos;
        target_pos+=dir*100.f;
        StartedMoving=true;
    } else {
        bool still_hitting_some_creature=(CollidedThing!=0 && (CollidedThing->GetTypeFlags()&8));
        FinishMove(parent_creature,still_hitting_some_creature);
    }
}

#define BATTLE 1
#define RUSH CTCBattleCharge
#define PASS "007a8700_TEST PASS"
int main(){
 for(caseId=0;caseId<8192;++caseId){
  unsigned n=caseId*2654435761u;
  RUSH a;CThingAICreature target,other;CTCSoundPlayer sound;CCreatureActionBase action;Def d;
  a.PDef=&d;d.RunSpeed=2;d.SlowTimeLength=1.5f;d.SlowTimeMultiplier=7;d.DistFromFloor=1.25f;
  d.NumPeopleHit[0]=1;d.NumPeopleHit[1]=2;d.NumPeopleHit[2]=3;
  a.DirectionOverride=((n&1)!=0);a.IsZTargetting=((n&2)!=0);a.StartedMoving=((n&4)!=0);
  a.FakeCollideWithSomething=((n&8)!=0);a.w.pass=((n&16)!=0);
  a.parent.pos=C3DVector(3,7,8);a.parent.type=(n&32)?2:0;
  a.parent.floor=(n&64)?-20.f:(n&128)?12.f:4.f;a.parent.sound=(n&256)?&sound:0;
  a.DirectionOverrideVector=(n&512)?C3DVector(0,0,1):C3DVector(0,2,0);
  target.pos=a.parent.pos+C3DVector(0,-3,0);target.dead=((n&1024)!=0);
  a.PTarget=(n&2048)?&target:0;other.dead=((n&4096)!=0);other.flags=(n&8192)?8:0;other.type=(n&16384)?1:0;
  a.CollidedThing=(n&32768)?&other:0;other.pos=C3DVector(5,3,4);
  a.PositionStartedFrom=a.parent.pos+C3DVector((n&65536)?4.f:0.f,0,0);a.DashDistance=4;
  a.parent.phys.face=C3DVector(1,0,0);
  C3DVector dir=a.DirectionOverride&&!a.IsZTargetting ? ((n&512)?C3DVector(1,0,0):C3DVector(0,1,0))
                 : a.PTarget.Get()&&!target.dead ? C3DVector(0,-1,0):C3DVector(1,0,0);
  C3DVector step=dir*2.f;C3DVector next=a.parent.pos+step;
  a.parent.phys.last=(n&131072)?a.parent.pos-step:a.parent.pos;
  bool started=a.StartedMoving;bool inRange=(n&65536)==0, physicalHit=(n&32768)&&(n&8192);
  bool failedProbe=a.FakeCollideWithSomething || !(n&131072) || !a.w.pass;
  bool checkMap=!a.FakeCollideWithSomething&&(n&131072);
#if BATTLE
  a.SpellLevel=(n>>19)%3;
  a.DisplacementVector=C3DVector((n&262144)?4.f:0,0,0);
  inRange=inRange&&!(n&262144);C3DVector old=a.DisplacementVector;
  if(n&8388608)a.PeopleHit.push_back(&target);
  if(n&16777216)a.PeopleHit.push_back(&other);
  size_t prev=a.PeopleHit.size();
  bool collide=inRange&&started&&physicalHit;
  bool newVictim=collide&&!(n&16777216);
  bool limit=newVictim&&(prev+1>=(unsigned)d.NumPeopleHit[a.SpellLevel]);
  bool probe=inRange&&started&&!physicalHit;
  bool done=!inRange||limit||(probe&&failedProbe);
  a.RushForwards();
  Check(a.parent.phys.orientations==1);same(a.parent.phys.face,dir);
  Check(a.finishes==int(done)&&a.w.checks==int(probe&&checkMap));
  Check(!done||a.finishCollide==(inRange||physicalHit));
  Check(a.hits==int(newVictim||probe&&failedProbe)&&a.PeopleHit.size()==prev+int(newVictim));
  Check(a.parent.phys.steps==int(!done)&&a.StartedMoving==(started||!done));
  if(!done){if(next.Z>a.parent.floor&&(collide||next.Z-a.parent.floor<25.f))next.Z=a.parent.floor;
    same(a.parent.phys.next,next);same(a.DisplacementVector,old+(next-a.parent.pos));Check(a.parent.phys.use);
    if(collide)Check(!a.CollidedThing.Get());
  }else same(a.DisplacementVector,old);
#else
  a.UseCollision=((n&262144)!=0);a.RadialBlurFadeKey=&a;a.FinalAction=(n&524288)?&action:0;
  bool strike=inRange&&started&&physicalHit&&!other.dead&&a.UseCollision;
  bool probe=inRange&&started&&!strike;
  bool done=!inRange||strike||(probe&&failedProbe);
  a.RushForwards();
  Check(a.parent.phys.orientations==1+int(strike)&&a.parent.phys.teleports==int(strike));
  Check(a.parent.finishes==int(done)&&a.clean==int(done)&&a.StartTrailDelay==done);
  Check(a.parent.actions==int(done&&a.FinalAction.Get())&&a.w.checks==int(probe&&checkMap));
  Check(sound.played==int(probe&&failedProbe&&a.parent.sound)&&a.parent.phys.steps==int(!done));
  Check(a.w.slows==int(strike&&a.parent.type==2)&&a.SlowTimeLength==45*a.w.slows);
  Check(other.resets==int(strike&&other.type==1)&&a.w.interp.blurs==int(!done));
  if(strike){same(a.BehindTargetPos,C3DVector(8,2,1));same(a.parent.phys.tele,a.BehindTargetPos);same(a.parent.phys.face,other.pos-a.BehindTargetPos);}
  else same(a.parent.phys.face,dir);
  if(!done){if(next.Z>a.parent.floor&&next.Z-a.parent.floor<25.f)next.Z=a.parent.floor;
      if(!a.UseCollision&&next.Z<a.parent.floor)next.Z=a.parent.floor;
      same(a.parent.phys.next,next);Check(a.parent.phys.use==a.UseCollision);Check(a.w.interp.id==&a);
      same(a.w.interp.blur,a.parent.pos+C3DVector(0,0,d.DistFromFloor));
  }
  Check(a.StartedMoving==(started||!done));
#endif
 }
 puts(PASS);return 0;
}
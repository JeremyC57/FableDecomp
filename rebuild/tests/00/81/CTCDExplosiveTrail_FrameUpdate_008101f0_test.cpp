#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <new>
#include <list>
#include <vector>
#include <utility>
struct C2DCoordI { int x,y; C2DCoordI(int a,int b):x(a),y(b){} };
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float k)const{return C3DVector(k*X,k*Y,k*Z);}
 C3DVector CrossWithUp()const{return C3DVector(Y,-X,0);}
 float Dot(const C3DVector&v)const{return X*v.X+Y*v.Y+Z*v.Z;}
 C3DVector Cross(const C3DVector&v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
 float GetSquaredMagnitude()const{return Dot(*this);}
 bool IsNonZero()const{return GetSquaredMagnitude()>0.00000001f;}
 void AccurateNormalise(){float n=(float)sqrt(GetSquaredMagnitude());if(n){X/=n;Y/=n;Z/=n;}}
 void Rotate(const C3DVector&,float turns){double r=turns*6.283185307179586;float x=X;X=(float)(x*cos(r)-Y*sin(r));Y=(float)(x*sin(r)+Y*cos(r));}
 C2DCoordI ToC2DCoordI()const{return C2DCoordI((int)X,(int)Y);}
};
static float GFArcCos(double x){return (float)acos(x)*0.15915494f;}
const float DELTA_NOTIONAL_ZERO=.0001f;struct CMainGameComponent{static long GetConstantFPS(){return 4;}};
struct CPhysics{C3DVector last,forward;CPhysics():forward(1,0,0){} void GetVelocity(C3DVector*v){*v=forward;}C3DVector GetFacingVector()const{return forward;}const C3DVector&GetLastPosition()const{return last;}};
struct CThing;
struct CTCTrapBase{bool armed,done;int immune,trigger,resets;CTCTrapBase():armed(false),done(false),immune(0),trigger(0),resets(0){}void SetThingAsImmune(CThing&){++immune;}void ManualTrigger(){++trigger;}bool IsClosed()const{return done;}bool IsOpen()const{return armed;}void ManualReset(){++resets;}};
struct CThing{unsigned m_presentBits3,Flags91;int Region,kills;CPhysics*Physics;C3DVector pos;CTCTrapBase trap;
 CThing():m_presentBits3(0),Flags91(0),Region(3),kills(0),Physics(0){}const C3DVector&GetPos(){return pos;}
 bool IsDead()const{return (Flags91&1)!=0;}unsigned long GetPlayerNumber()const{return Region;}const C3DVector&GetPos()const{return pos;}const CPhysics*GetPhysicsTC()const{return Physics;}CPhysics*GetPhysicsTC(){return Physics;}bool GetTC(int,CTCTrapBase**dst){if((m_presentBits3 & 0x20000000)==0)return false;*dst=&trap;return true;}void Kill(bool){Flags91|=1;++kills;}};
template<class T>class CIntelligentPointer{public:T*p; CIntelligentPointer(T*v=0):p(v){}T*Get()const{return p;}T*operator->()const{return p;}};
template<class T>bool IS_THING_ALIVE(T*p){return p&&!p->IsDead();}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return IS_THING_ALIVE(p.Get());}
typedef std::list<std::pair<CIntelligentPointer<CThing>,long> > TrapList;
const int TCI_TRAP=125;
struct CWorldMap{float height,ground;bool pass;CWorldMap():height(0),ground(0),pass(true){}
 float GetHeightZBelowPos(const C3DVector&){return height;}float GetGroundSizeZAt(const C3DVector&){return ground;}bool IsPosPassable(const C3DVector&){return pass;}};
struct CWorld{CWorldMap map;unsigned long seed;CWorldMap&GetWorldMap(){return map;}unsigned long*GetWorldSeed(){return &seed;}};
static CThing*created;static int creations,rotations;static C3DVector spawn;
struct CThingObject{static CThing*Create(long,const C3DVector&p,long,const void*,int,const char*){++creations;spawn=p;if(created)created->pos=p;return created;}};
namespace NExplosiveTrailHelpers {static void RandomiseThingOrientationAroundDirection(CThing&,const C3DVector&){++rotations;}}
static float GFFloatRandom(float bound,unsigned long*){return bound*.5f;}
struct CExplosiveTrailDef{float SecondsBetweenTraps,DistanceBetweenTraps,SecondsForTrapPersistance,SecondsTrailLifetime,DistanceForLateralDeviation,DegreesMaxTurnTowardsTargetPerTrap,FractionOfLifetimeToTurnTowardsTarget,SecondsIcicleAttackTargetPositionPredictionTime;int TrapDefIndex;};
struct CTCDExplosiveTrail{CThing*ParentThing;CIntelligentPointer<const CThing>PTarget;long Timer;bool Terminated;TrapList ComponentTraps;std::vector<CIntelligentPointer<CThing> >ThingsToIgnore;const CExplosiveTrailDef*PDef;CWorld world;CWorld&GetWorld(){return world;}CThing&GetParentThing(){return *ParentThing;}void FrameUpdate();};

template<class T, class U> static void GFLimitUpper(T& value, const U& limit) { if (value > limit) value = limit; }
static double GFTanh(double value) { return tanh(value); }
void CTCDExplosiveTrail::FrameUpdate()
{
    const float NUM_SECS_BETWEEN_TRAPS = PDef->SecondsBetweenTraps;
    const float TRAP_SPACING = PDef->DistanceBetweenTraps;
    const float NUM_SECS_FOR_TRAP_PERSIST = PDef->SecondsForTrapPersistance;
    const float NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST = PDef->SecondsTrailLifetime;
    const float TRAP_DEVIATION_AMOUNT = PDef->DistanceForLateralDeviation;
    ++Timer;
    if (Timer > CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST || Terminated) {
        if (ComponentTraps.empty())
            GetParentThing().Kill(true);
    } else {
        long trap_create_period = (long)(CMainGameComponent::GetConstantFPS() * NUM_SECS_BETWEEN_TRAPS);
        if (Timer % trap_create_period == 0) {
            long trap_index = Timer / trap_create_period;
            C3DVector base_pos = GetParentThing().GetPos();
            C3DVector fire_line = GetParentThing().GetPhysicsTC()->GetFacingVector();
            long num_traps = ComponentTraps.size();
            if (num_traps > 0) {
                TrapList::reverse_iterator it = ComponentTraps.rbegin();
                CThing* plast_trap = it->first.Get();
                if (IS_THING_ALIVE(plast_trap))
                    base_pos = plast_trap->GetPos();
                if (num_traps > 1) {
                    CThing* pbefore_last_trap = (++it)->first.Get();
                    if (IS_THING_ALIVE(pbefore_last_trap)) {
                        fire_line = base_pos - pbefore_last_trap->GetPos();
                    }
                }
            }
            fire_line.Z = 0.0f;
            if (fire_line.IsNonZero()) fire_line.AccurateNormalise();
            const float FRACTION_OF_LIFE_TO_CHASE = PDef->FractionOfLifetimeToTurnTowardsTarget;
            const float DEGREES_MAX_TURN_PER_TRAP = PDef->DegreesMaxTurnTowardsTargetPerTrap;
            const float SECONDS_TARGET_POS_PREDICT = PDef->SecondsIcicleAttackTargetPositionPredictionTime;
            if (IS_THING_ALIVE(PTarget) && Timer <= CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST * FRACTION_OF_LIFE_TO_CHASE) {
                const C3DVector& cur_target_pos = PTarget->GetPos();
                C3DVector target_vel = cur_target_pos - PTarget->GetPhysicsTC()->GetLastPosition();
                C3DVector target_pos = cur_target_pos + target_vel * SECONDS_TARGET_POS_PREDICT;
                C3DVector line_to_target_pos = target_pos - base_pos;
                line_to_target_pos.Z = 0.0f;
                if (line_to_target_pos.IsNonZero())
                    line_to_target_pos.AccurateNormalise();
                C3DVector start_to_target_pos = target_pos - GetParentThing().GetPos();
                start_to_target_pos.Z = 0.0f;
                if (start_to_target_pos.IsNonZero()) start_to_target_pos.AccurateNormalise();
                if (start_to_target_pos.Dot(line_to_target_pos) > 0.0f) {
                    float cos_thet = line_to_target_pos.Dot(fire_line);
                    if (cos_thet < -1.0f) cos_thet = -1.0f;
                    else if (cos_thet > 1.0f)
                    cos_thet = 1.0f;
                    float theta = GFArcCos(cos_thet);
                    if (line_to_target_pos.Cross(fire_line).Z < 0.0f) theta *= -1.0f;
                    GFLimitUpper(theta, DEGREES_MAX_TURN_PER_TRAP / 360.0f);
                    fire_line.Rotate(C3DVector(0.0f, 0.0f, 1.0f), theta);
                }
            }
            C3DVector perp_line = fire_line.CrossWithUp();
            float trap_deviation_mag_max = TRAP_DEVIATION_AMOUNT * GFTanh(trap_index / (TRAP_SPACING * 5.0f));
            float trap_dev_mag = (GFFloatRandom(trap_deviation_mag_max, GetWorld().GetWorldSeed()) * 2.0f) - trap_deviation_mag_max;
            C3DVector create_pos = base_pos + fire_line * TRAP_SPACING + perp_line * trap_dev_mag;
            CWorldMap& wmap = GetWorld().GetWorldMap();
            float height = wmap.GetHeightZBelowPos(create_pos + C3DVector(0.0f,0.0f,10.0f));
            float ground_height = wmap.GetGroundSizeZAt(create_pos + C3DVector(0.0f,0.0f,10.0f));
            if (height - ground_height > DELTA_NOTIONAL_ZERO)
                Terminated = true;
            else if (!wmap.IsPosPassable(create_pos))
            Terminated = true;
            if (!Terminated) {
                create_pos.Z = ground_height;
                if (PDef->TrapDefIndex > 0) {
                    CThing* pthing = CThingObject::Create(PDef->TrapDefIndex, create_pos, GetParentThing().GetPlayerNumber(), 0, 0, "");
                    if (pthing) {
                        NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection(*pthing, fire_line);
                        CTCTrapBase* ptc_trap;
                        if (pthing->GetTC(TCI_TRAP, &ptc_trap)) {
                            for (std::vector<CIntelligentPointer<CThing> >::iterator it = ThingsToIgnore.begin(); it != ThingsToIgnore.end(); ++it) {
                                if (IS_THING_ALIVE(*it))
                                    ptc_trap->SetThingAsImmune(*it->Get());
                            }
                            ptc_trap->ManualTrigger();
                        }
                        ComponentTraps.push_back(std::make_pair(CIntelligentPointer<CThing>(pthing), -1));
                    }
                }
            }
        }
    }
    TrapList::iterator it = ComponentTraps.begin();
    while (it != ComponentTraps.end()) {
        CThing* pthing = it->first.Get();
        long& timer = it->second;
        CTCTrapBase* ptc_trap;
        if (IS_THING_ALIVE(pthing) && pthing->GetTC(TCI_TRAP, &ptc_trap)) {
            if (ptc_trap->IsClosed()) {
                pthing->Kill(true);
                it = ComponentTraps.erase(it);
            } else {
                if (ptc_trap->IsOpen()) {
                    if (timer < 0)
                        timer = (long)(CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_TRAP_PERSIST);
                    else if (timer == 0)
                    ptc_trap->ManualReset();
                    else
                        --timer;
                }
                ++it;
            }
        } else {
            it = ComponentTraps.erase(it);
        }
    }
}
int main(){
 for(int i=0;i<24;++i){
  CPhysics physics;CThing parent,target,trap,other,dead;CIntelligentPointer<CThing> immune[2];immune[0].p=&other;immune[1].p=&dead;dead.Flags91=1;parent.Physics=target.Physics=&physics;
  CTCDExplosiveTrail trail;CExplosiveTrailDef def={.5f,2,1,8,0,90,1,0,2};trap.m_presentBits3=0x20000000;trap.trap.armed=true;
  trail.ThingsToIgnore.assign(immune,immune+2);trail.Timer=1;trail.ParentThing=&parent;trail.Terminated=false;trail.PDef=&def;created=&trap;creations=rotations=0;
  if(i==1)trail.Timer=0;
  if(i==2)trail.Timer=32;
  if(i==3)trail.Terminated=true;
  if(i==4)def.TrapDefIndex=0;
  if(i==5)trail.world.map.height=1;
  if(i==6)trail.world.map.pass=false;
  if(i==7)created=0;
  if(i==8)trap.m_presentBits3=0;
  if(i>=9&&i<=14){trail.PTarget.p=&target;target.pos=C3DVector(0,(float)(i-11),0);if(i==12)target.Flags91=1;if(i==13)def.FractionOfLifetimeToTurnTowardsTarget=0;if(i==14)physics.forward=C3DVector();}
  if(i>=15){trail.Terminated=true;trail.ComponentTraps.push_back(std::make_pair(CIntelligentPointer<CThing>(&trap),i==15?-1:i==16?0:3));if(i==18)trap.trap.armed=false;if(i==19)trap.trap.done=true;if(i==20)trap.Flags91=1;if(i==21)trap.m_presentBits3=0;if(i==22)trail.ComponentTraps.front().first.p=0;if(i==23){trail.Terminated=false;trail.ComponentTraps.push_back(std::make_pair(CIntelligentPointer<CThing>(&dead),0));}}
  trail.FrameUpdate();TrapList::iterator n=trail.ComponentTraps.begin();bool empty=n==trail.ComponentTraps.end();
  if(i<=14){bool spawnAttempt=i==0||i>=7;if(creations!=(int)spawnAttempt||rotations!=(int)(spawnAttempt&&i!=7))return 1;
   if(parent.kills!=((i==2||i==3)?1:0)||trail.Terminated!=(i==3||i==5||i==6))return 2;
   if(i==0&&(spawn.X!=2||spawn.Y!=0||empty||n->second!=4||trap.trap.trigger!=1||trap.trap.immune!=1))return 3;
   if(i==8&&!empty)return 4;
   if((i==9||i==10)&&fabs(spawn.Y-2)>0.00001)return 5;
   if((i==12||i==13)&&spawn.X!=2)return 6;
  }else{if(i==15&&(empty||n->second!=4))return 7;if(i==16&&trap.trap.resets!=1)return 8;if(i==17&&(empty||n->second!=2))return 9;
   if(i==18&&(empty||n->second!=3))return 10;if(i>=19&&i<=22&&(!empty||trap.kills!=(i==19)))return 11;if(i==23&&creations!=1)return 12;}
 }
 // The original strict expiry test keeps the final eligible frame alive.
 {
   CPhysics physics; CThing parent, trap; parent.Physics = &physics;
   CTCDExplosiveTrail trail; CExplosiveTrailDef def={.5f,2,1,8,0,90,1,0,2};
   trail.Timer=31; trail.ParentThing=&parent; trail.Terminated=false; trail.PDef=&def;
   created=&trap; creations=rotations=0; trail.FrameUpdate();
   if (trail.Timer!=32 || parent.kills!=0 || creations!=1) return 13;
 }
 puts("008101f0_TEST PASS");return 0;
}
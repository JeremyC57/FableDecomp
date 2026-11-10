#include <cstdio>
#include <cstdlib>
#include <cmath>
static void check(bool value){if(!value)std::abort();}
struct Creature {bool dead;int calls;bool IsDead()const{return dead;}bool IsInMelee(){++calls;return true;}};
template<class T>bool IS_THING_ALIVE(const T& t){return t!=0 && !t->IsDead();}
static float BODY_ANGULAR_ACCELERATION_XY=2.0f,BODY_ANGULAR_VELOCITY_MAX_XY=4.0f;
static long GFGetSign(float value){return value<0.0f?-1:1;}
static float GET_ROUNDED_ANGLE(float angle){if(angle<0||angle>=1){angle=(float)std::fmod((double)angle,1.0);if(angle<0)angle+=1;}return angle;}
class CTCLook { public:
 Creature* PParentCreature;long BodyTurnState,LookFlags;float BodyVelocityXY,BodyVelocityYZ,BodyAngleXY;
 bool turning;float target,delta,headDegrees,recorded;int queries,settings,implements;
 bool GetTurningAnglesAndCheckForTurningBody(float& a,float& b){++queries;a=target;b=delta;return turning;}
 bool IsLookTypeBodyOnly()const{return (LookFlags&1)&&!(LookFlags&2)&&!(LookFlags&4);}
 float GetHeadTurnRangeXY()const{return (headDegrees*0.0027777778f);}
 void SetBodyTurnState(long state){if(BodyTurnState!=state)BodyTurnState=state;}
 void GetBodySettingsSlowInFastOut(float& a,float& v,float acceleration,float speed,float destination)const{
  check(&a==&BodyAngleXY&&&v==&BodyVelocityXY&&acceleration==2.0f&&speed==4.0f);
  CTCLook* self=const_cast<CTCLook*>(this);++self->settings;self->recorded=destination;
 }
 void ImplementBodySettings(){++implements;}
 void SetBodyMoving();
};
void CTCLook::SetBodyMoving()
{
 bool melee=IS_THING_ALIVE(PParentCreature) && PParentCreature->IsInMelee();
 bool dest_reached=true;
 float destination_angle=0.0f;
 float angle_from_north_to_target_xy;
 float rel_angle_difference_xy;
 if(GetTurningAnglesAndCheckForTurningBody(angle_from_north_to_target_xy,rel_angle_difference_xy)) {
  dest_reached=false;
  if(!IsLookTypeBodyOnly() && !(LookFlags&0x80)) {
   destination_angle=angle_from_north_to_target_xy-GetHeadTurnRangeXY()*GFGetSign(rel_angle_difference_xy)*0.25f;
   destination_angle=GET_ROUNDED_ANGLE(destination_angle);
  } else destination_angle=angle_from_north_to_target_xy;
 }
 if(dest_reached) {
  SetBodyTurnState(BodyTurnState|8);
  BodyVelocityXY=0.0f;
  BodyVelocityYZ=0.0f;
 } else {
  GetBodySettingsSlowInFastOut(BodyAngleXY,BodyVelocityXY,BODY_ANGULAR_ACCELERATION_XY,BODY_ANGULAR_VELOCITY_MAX_XY,destination_angle);
  ImplementBodySettings();
 }
}

int main(){
 const long flags[]={0,1,2,4,7,128,129,192};
 const float targets[]={-2.0f,-0.125f,0.0f,0.5f,1.0f,2.125f};
 const float deltas[]={-0.25f,0.0f,0.25f};const float degrees[]={90.0f,180.0f,360.0f};
 int cases=0;
 for(int parent=0;parent<3;++parent)for(int turn=0;turn<2;++turn)for(int f=0;f<8;++f)
 for(int a=0;a<6;++a)for(int d=0;d<3;++d)for(int h=0;h<3;++h){
  Creature creature;creature.dead=parent==2;creature.calls=0;
  CTCLook look;look.PParentCreature=parent?&creature:0;look.BodyTurnState=18;
  look.LookFlags=flags[f];look.BodyVelocityXY=8;look.BodyVelocityYZ=-4;look.BodyAngleXY=0.25f;
  look.turning=turn!=0;look.target=targets[a];look.delta=deltas[d];look.headDegrees=degrees[h];
  look.recorded=-16;look.queries=look.settings=look.implements=0;
  look.SetBodyMoving();check(look.queries==1&&creature.calls==(parent==1?1:0));
  if(turn){
   double expected=targets[a];
   bool bodyOnly=(flags[f]&1)&&!(flags[f]&6);
   if(!bodyOnly&&!(flags[f]&128)){
    expected-=(d==0?-1.0:1.0)*(degrees[h]/360.0)*0.25;
    expected-=std::floor(expected);
   }
   check(look.settings==1&&look.implements==1&&look.recorded==(float)expected);
   check(look.BodyTurnState==18&&look.BodyVelocityXY==8&&look.BodyVelocityYZ==-4);
  }else{
   check(look.settings==0&&look.implements==0&&look.BodyTurnState==26);
   check(look.BodyVelocityXY==0&&look.BodyVelocityYZ==0&&look.recorded==-16);
  }
  check(look.BodyAngleXY==0.25f&&look.LookFlags==flags[f]);++cases;
 }
 check(cases==2592);std::puts("00752e00_TEST PASS");return 0;
}
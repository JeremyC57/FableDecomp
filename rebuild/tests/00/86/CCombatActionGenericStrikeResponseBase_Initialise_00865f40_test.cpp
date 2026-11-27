#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_GENERIC_STRIKE_RESPONSE_PASS29
#include "engine/ECombatAnimationHandedness.h"
#define Check(c) do{if(!(c)){printf("failed %d\n",__LINE__);exit(1);}}while(0)
class C3DVector {
public:
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator-(const C3DVector& b) const {return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
 C3DVector operator-() const{return C3DVector(-X,-Y,-Z);}
 C3DVector operator^(const C3DVector& b)const{return C3DVector(Y*b.Z-Z*b.Y,Z*b.X-X*b.Z,X*b.Y-Y*b.X);}
 void Normalise(){double l=sqrt(X*X+Y*Y+Z*Z);if(l){X=(float)(X/l);Y=(float)(Y/l);Z=(float)(Z/l);}}
 float GetAngleXY() const {double v=atan2(X,Y)/6.283185307179586;return float(v<0?v+1:v);}
};
float GFDotProduct(const C3DVector&a,const C3DVector&b){return a.X*b.X+a.Y*b.Y+a.Z*b.Z;}
double GFCos(float v){return cos(v*6.283185307179586);}
namespace NGlobalConsole{bool GCheatModeEnableRemoveProjectileModeWhenHit;}
class CThing;
static unsigned cancels,resets,removes,drop,initialised,looked,face_calls,modes_removed,player_calls;
static int result;static bool face_hit,active_mode;static float angle;
class CPlayer {public:void RemoveMode(EPlayerMode m){Check(m==PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);++removes;}};
class CTCBase {};
class CTCHeroRangedAttacks:public CTCBase { public:enum{slot=0};void CancelAimingWeapon(){++cancels;}void ResetProjectileWeaponMode(){++resets;} };
class CTCCreatureModeManager:public CTCBase {public:enum{slot=1}; void RemoveMode(unsigned long m){Check(m==(modes_removed?0x36:0xd));++modes_removed;}bool IsModeActive(unsigned long m)const{Check(m==0x36 && initialised);return active_mode;} };
class CTCCreatureItemLevitationBase:public CTCBase {public:enum{slot=2};void DropItems(){Check(!initialised);++drop;}};
class CTCPhysicsBase { public:C3DVector GetFacingVector()const{++looked;return C3DVector(0,1,0);}void SetFacingAngleXY(float a){angle=a;++face_calls;} };
class CThing {public:bool dead;unsigned flags;C3DVector pos;CTCBase* tc[3];CPlayer player;CTCPhysicsBase physics;
 CThing():dead(false),flags(0){memset(tc,0,sizeof tc);}unsigned GetTypeFlags()const{return flags;}
 const C3DVector& GetPos() const{return pos;}CPlayer& GetPlayer(){return player;} CTCPhysicsBase* GetPhysicsTC(){return &physics;}
 template<class T> bool GetTC(T** o){*o=static_cast<T*>(tc[T::slot]);return *o!=0;}
};
class CThingCreatureBase:public CThing {public:ECombatAnimationHandedness hand;ECombatAnimationHandedness GetLastActionHandedness()const{return hand;}bool IsPlayerCharacter() const {++player_calls;return false;}};
template<class T> class CIntelligentPointer {public:T* p;T* Get() const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return p.Get()&&!p->dead;}
namespace NCreatureMode {bool IsFaceHitterOnHit(const CThing&){return face_hit;}}
class CCombatActionBase {public:CIntelligentPointer<CThingCreatureBase> PCreature;long TotalTime;void Initialise(){++initialised;}void SetCombatAnimation(const char* s){
 result=!strcmp(s,"RESPONSE_GENERIC_HIT_BACK")?0:!strcmp(s,"RESPONSE_GENERIC_HIT_LEFT")?1:!strcmp(s,"RESPONSE_GENERIC_HIT_RIGHT")?2:3;Check(result!=3);
}};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {public:CIntelligentPointer<const CThing> PThingThatHit;C3DVector PointOfHit,DustTravelDirection;long FrameForDust;void Initialise();};
void CCombatAction_GenericStrikeResponseBase::Initialise()
{
    PCreature->IsPlayerCharacter();
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);
    }
    C3DVector facing = PCreature->GetPhysicsTC()->GetFacingVector();
    C3DVector to_hit_point = PointOfHit - PCreature->GetPos();
    if (IS_THING_ALIVE(PThingThatHit))
        to_hit_point = PThingThatHit->GetPos() - PCreature->GetPos();
    to_hit_point.Normalise();
    C3DVector travel_direction(0,0,0);
    float dot = GFDotProduct(facing,to_hit_point);
    float min_angle = GFCos(100.0f/360.0f);
    if (dot < min_angle) {
        travel_direction = -to_hit_point;
        SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK");
    } else {
        travel_direction = to_hit_point;
        bool anim_setup = false;
        if (IS_THING_ALIVE(PThingThatHit) && (PThingThatHit->GetTypeFlags() & 8)) {
            const CThingCreatureBase* pcreature = static_cast<const CThingCreatureBase*>(PThingThatHit.Get());
            if (pcreature->GetLastActionHandedness() == HANDED_RIGHT) {
                SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT");
                anim_setup = true;
            } else if (pcreature->GetLastActionHandedness() == HANDED_LEFT) {
                SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT");
                anim_setup = true;
            }
        }
        if (!anim_setup) {
            C3DVector cross_result = facing ^ to_hit_point;
            if (cross_result.Z < 0.0f)
                SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT");
            else
                SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT");
        }
    }
    CTCCreatureModeManager* ptc_cmm;
    if (PCreature->GetTC(&ptc_cmm)) {
        ptc_cmm->RemoveMode(0xd);
        if (NCreatureMode::IsFaceHitterOnHit(*PCreature))
            PCreature->GetPhysicsTC()->SetFacingAngleXY(travel_direction.GetAngleXY());
    }
    CTCCreatureItemLevitationBase* ptc_levitation;
    if (PCreature->GetTC(&ptc_levitation))
        ptc_levitation->DropItems();
    CCombatActionBase::Initialise();
    DustTravelDirection = travel_direction;
    FrameForDust = TotalTime / 2;
    if (ptc_cmm && ptc_cmm->IsModeActive(0x36))
        ptc_cmm->RemoveMode(0x36);
}
int main(){
 CThingCreatureBase subject,hitter;CTCHeroRangedAttacks ranged;CTCCreatureModeManager modes;CTCCreatureItemLevitationBase levitation;
 CCombatAction_GenericStrikeResponseBase action;action.PCreature.p=&subject;
 C3DVector headings[]={C3DVector(0,-1,0),C3DVector(1,1,0),C3DVector(-1,1,0),C3DVector(0,0,1)};
 for(unsigned bits=0;bits<64;++bits) for(unsigned state=0;state<4;++state) for(unsigned dir=0;dir<4;++dir) for(unsigned hand=0;hand<3;++hand) {
  subject.tc[0]=(bits&1)?&ranged:0;subject.tc[1]=(bits&2)?&modes:0;subject.tc[2]=(bits&4)?&levitation:0;
  NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit=(bits&8)!=0;face_hit=(bits&16)!=0;active_mode=(bits&32)!=0;
  action.PThingThatHit.p=state?&hitter:0;hitter.dead=state==1;hitter.flags=state==3?8:0;hitter.hand=(ECombatAnimationHandedness)hand;
  unsigned use=state>=2?(dir+1)%4:dir;action.PointOfHit=headings[dir];hitter.pos=headings[(dir+1)%4];
  action.TotalTime=dir%2?5:-7;cancels=resets=removes=drop=initialised=looked=face_calls=modes_removed=player_calls=0;result=-1;
  action.Initialise();
  int expected=use==0?0:(state==3 && hand!=2)?int(hand+1):use==1?1:2;
  Check(result==expected && action.FrameForDust==action.TotalTime/2);
  Check(cancels==bool(bits&1) && removes==cancels && resets==(cancels && (bits&8)) && drop==bool(bits&4));
  Check(initialised==1 && looked==1 && player_calls==1 && face_calls==((bits&2)&&face_hit) && modes_removed==((bits&2)?1+active_mode:0));
  double l=sqrt(GFDotProduct(headings[use],headings[use]));double sign=use==0?-1:1;
  Check(fabs(action.DustTravelDirection.X-sign*headings[use].X/l)<0.0001 && fabs(action.DustTravelDirection.Y-sign*headings[use].Y/l)<0.0001 && fabs(action.DustTravelDirection.Z-sign*headings[use].Z/l)<0.0001);
  if(face_calls) Check(fabs(angle-action.DustTravelDirection.GetAngleXY())<0.00001);
 }
 puts("00865f40_TEST PASS");
}
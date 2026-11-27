#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine/EPlayerMode.h"
#include "engine/NCreatureMode.h"
#define Check(c) do{if(!(c)){printf("failed %d\n",__LINE__);exit(1);}}while(0)
struct C3DVector {float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
 bool IsNonZero()const{return X!=0||Y!=0||Z!=0;}
 void AccurateNormalise(){float l=(float)sqrt(X*X+Y*Y+Z*Z);X/=l;Y/=l;Z/=l;}
 float GetAngleXY()const{double a=atan2(X,Y)/6.283185307179586;return (float)(a<0?a+1:a);}
};
float GFGetSignedAngleDifference(float a,float b){float d=b-a;while(d>.5f)d-=1;while(d<-.5f)d+=1;return d;}
struct CRightHandedSet {C3DVector up,forward;CRightHandedSet(C3DVector a,C3DVector b):up(a),forward(b){}};
namespace NGlobalConsole {bool GCheatModeEnableRemoveProjectileModeWhenHit;}
unsigned cancels,resets,removed,mode,adds,removes,baseCalls,reorient,faceCalls;
struct CPlayer{void RemoveMode(EPlayerMode m){Check(m==PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);++removed;}};
struct CTCHeroRangedAttacks{void CancelAimingWeapon(){++cancels;}void ResetProjectileWeaponMode(){++resets;}};
struct CCharString {const char* str;CCharString(const char* s):str(s){}};
struct CAnimComponentFollowingOnAnimFilter {CCharString GetNextAnimFilter(){return CCharString("follow");}};
struct CAnimationEntry {CAnimComponentFollowingOnAnimFilter c;void GetComponent(CAnimComponentFollowingOnAnimFilter** p){*p=&c;}};
struct CThing {};
namespace NCreatureMode {struct CModeExplosiveForcePushed {CThing* parent;CThing* explosion;CCharString filter;
 CModeExplosiveForcePushed(CThing& p,CThing& e,const CCharString& f):parent(&p),explosion(&e),filter(f){} };}
static CThing* expectedParent;static CThing* expectedExplosion;
struct CTCCreatureModeManager {
 struct Mode{unsigned GetType(){return mode;}} current;
 Mode& GetCurrentMode(){return current;}
 void AddMode(NCreatureMode::CModeExplosiveForcePushed* p){Check(baseCalls==1&&p->parent==expectedParent&&p->explosion==expectedExplosion&&!strcmp(p->filter.str,"follow"));++adds;delete p;}
 void RemoveMode(unsigned m){Check(m==NCreatureMode::MODE_SLEEP_BED&&adds==1);++removes;}
};
struct CTCPhysicsBase {
 float direction;C3DVector facing,stored;
 C3DVector GetFacingVector()const{return facing;}float GetFacingAngleXY()const{return direction;}
 void ResetRHSet(const CRightHandedSet& r){Check(r.up.X==0&&r.up.Y==0&&r.up.Z==1);stored=r.forward;++faceCalls;}
};
struct CThingCreatureBase: CThing {
 CTCHeroRangedAttacks* ranged;CTCCreatureModeManager modes;CPlayer player;CTCPhysicsBase physics;unsigned type;
 bool GetTC(CTCHeroRangedAttacks** p){*p=ranged;return ranged!=0;}
 bool GetTC(CTCCreatureModeManager** p){*p=&modes;return true;}
 CPlayer& GetPlayer(){return player;} CTCPhysicsBase* GetPhysicsTC(){return &physics;}
 unsigned GetType(){return type;}void SetBodyReorienterActive(bool b){Check(b);++reorient;}
};
struct CCombatActionBase {CThingCreatureBase* PCreature;CAnimationEntry* PAnimEntry;const char* selected;
 void Initialise(){++baseCalls;}void SetCombatAnimation(const char* n){selected=n;}
};
struct CCombatAction_GenericResponseToExplosion:CCombatActionBase {bool FlipFacing;C3DVector HitDir;CThing* PExplosion;void Initialise();};
void CCombatAction_GenericResponseToExplosion::Initialise()
{
    PCreature->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode(PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON);
    }
    HitDir.Z=0;
    if (HitDir.IsNonZero()) HitDir.AccurateNormalise();
    else {
        HitDir=PCreature->GetPhysicsTC()->GetFacingVector();
        HitDir.Z=0;
        if (HitDir.IsNonZero()) HitDir.AccurateNormalise();
        else HitDir=C3DVector(1,0,0);
    }
    C3DVector toExplosion=HitDir * -1.0f;
    float hitAng=GFGetSignedAngleDifference(PCreature->GetPhysicsTC()->GetFacingAngleXY(),toExplosion.GetAngleXY());
    C3DVector newFacingDir;
    if (fabs(hitAng)>0.25f) {
        SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN");
        if (PCreature->GetType()==1) FlipFacing=true;
        newFacingDir=HitDir;
    } else {
        if (hitAng<0) SetCombatAnimation("RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN");
        else SetCombatAnimation("RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN");
        newFacingDir=toExplosion;
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* ptc_mode_manager;
    PCreature->GetTC(&ptc_mode_manager);
    if (ptc_mode_manager->GetCurrentMode().GetType()!=NCreatureMode::MODE_STUNNED)
        PCreature->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0,0,1),newFacingDir));
    CAnimComponentFollowingOnAnimFilter* pfilt_comp;
    PAnimEntry->GetComponent(&pfilt_comp);
    CCharString val(pfilt_comp->GetNextAnimFilter());
    ptc_mode_manager->AddMode(new NCreatureMode::CModeExplosiveForcePushed(*PCreature,*PExplosion,val));
    ptc_mode_manager->RemoveMode(NCreatureMode::MODE_SLEEP_BED);
}
int main(){
 for(unsigned i=0;i<4096;++i){
    CCombatAction_GenericResponseToExplosion a;CThingCreatureBase c;CTCHeroRangedAttacks r;CAnimationEntry ent;CThing explosion;
    cancels=resets=removed=adds=removes=baseCalls=reorient=faceCalls=0;mode=(i&8)?NCreatureMode::MODE_STUNNED:NCreatureMode::MODE_NULL;
    c.type=(i&1);c.ranged=(i&2)?&r:0;NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit=(i&4)!=0;
    c.physics.direction=(i%32)/32.f;a.PCreature=&c;a.PExplosion=&explosion;a.PAnimEntry=&ent;
    expectedParent=&c;expectedExplosion=&explosion;a.FlipFacing=(i&64)!=0;bool initialFlip=a.FlipFacing;
    int dx=(int)((i/11)%3)-1,dy=(int)((i/31)%3)-1,fx=(int)((i/5)%3)-1,fy=(int)((i/13)%3)-1;
    if(i%4==0)dx=dy=0;if(i%16==0)fx=fy=0;
    c.physics.facing=C3DVector((float)fx,(float)fy,10);a.HitDir=C3DVector((float)dx,(float)dy,-6);
    int x=dx,y=dy;if(!x&&!y){x=fx;y=fy;}if(!x&&!y)x=1;
    double length=sqrt(double(x*x+y*y));double vx=x/length,vy=y/length;
    float relative=(float)(atan2(-vx,-vy)/6.283185307179586)-c.physics.direction;
    while(relative>.5f)relative-=1;while(relative<-.5f)relative+=1;
    bool back=fabs(relative)>.25f;
    if(fabs(fabs(relative)-.5f)<.00001)back=true;
    a.Initialise();
    Check(!strcmp(a.selected,back?"RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN":relative<0?"RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN":"RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN"));
    Check(a.FlipFacing==(initialFlip||(back&&c.type==1)));
    Check(cancels==(c.ranged?1:0)&&removed==cancels&&resets==(c.ranged&&(i&4)?1:0));
    Check(reorient==1&&baseCalls==1&&adds==1&&removes==1&&faceCalls==((i&8)?0:1));
    Check(fabs(a.HitDir.X-vx)<.0001&&fabs(a.HitDir.Y-vy)<.0001&&a.HitDir.Z==0);
    if(faceCalls){double sign=back?1:-1;Check(fabs(c.physics.stored.X-sign*vx)<.0001&&fabs(c.physics.stored.Y-sign*vy)<.0001&&c.physics.stored.Z==0);}
 }
 puts("00867200_TEST PASS");return 0;
}
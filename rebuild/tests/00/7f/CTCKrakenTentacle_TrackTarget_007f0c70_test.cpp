#ifndef _MSC_VER
#define FABLE_PORTABLE
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __fastcall
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#endif
#include <cstdio>
#include "rebuild_abi.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#include "engine/C3DVector.h"
#define FABLE_RETAIL_ASSERT(expr) ((void)0)
float C3DVector::GetMagnitudeAndNormalise() {
    const float n=(float)sqrt(X*X+Y*Y+Z*Z);
    if (n>0) {X/=n; Y/=n; Z/=n;}
    return n;
}
static float GFGetSignedAngleDifference(float a,float b) {
    float d=b-a;
    if(d>0.5f) d-=1;
    if(d< -0.5f) d+=1;
    return d;
}
struct PhysicsFixture {
    float facing;
    C3DVector position;
    int turns,moves;
    float GetFacingAngleXY()const {return facing;}
    void SetFacingAngleXY(float a){facing=a; ++turns;}
    void SetPosition(const C3DVector& p){position=p; ++moves;}
};
struct CTCKraken;
struct CThing {
    CTCKraken* component;
    bool alive;
    PhysicsFixture physics;
    const C3DVector& GetPos()const{return physics.position;}
    PhysicsFixture* GetPhysicsTC(){return &physics;}
    bool GetTC(CTCKraken** out){*out=component;return component!=0;}
};
static bool IS_THING_ALIVE(CThing* t){return t && t->alive;}
struct CKrakenDef {float PoolRadius;};
struct CTCKraken {
    CThing *target,*marker;
    CKrakenDef def;
    CThing* GetPTarget()const{return target;}
    CThing* GetPCentreMarker()const{return marker;}
    const CKrakenDef& GetKrakenDef()const{return def;}
};
struct CKrakenTentacleDef {float TentacleStrikeAngleTolerence,TentacleTurnSpeed,TentacleStrikeDistTolerence,TentacleMoveSpeed;};
struct CTCKrakenTentacle {
    CThing *PKraken,*parent;
    bool CanStrike;
    CKrakenTentacleDef def;
    CThing& GetParentThing(){return *parent;}
    const CKrakenTentacleDef& GetTentacleDef()const{return def;}
    void TrackTarget();
};
void CTCKrakenTentacle::TrackTarget()
{
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(PKraken));
    CTCKraken* pTCKraken;
    if (!PKraken->GetTC(&pTCKraken)) {
        FABLE_RETAIL_ASSERT(pTCKraken);
    }
    CThing* pTarg = pTCKraken->GetPTarget();
    if (IS_THING_ALIVE(pTarg)) {
        C3DVector delta = pTarg->GetPos() - GetParentThing().GetPos();
        delta.Z = 0.0f;
        float desiredFace = delta.GetAngleXY();
        float currentFace = GetParentThing().GetPhysicsTC()->GetFacingAngleXY();
        float deltaAng = GFGetSignedAngleDifference(currentFace, desiredFace);
        bool angleInTolerence = false;
        if (fabs(deltaAng) < GetTentacleDef().TentacleStrikeAngleTolerence * (1.0f / 360.0f)) angleInTolerence = true;
        float rotSpeed = GetTentacleDef().TentacleTurnSpeed * (1.0f / 360.0f);
        if (deltaAng < 0.0f) {
            if (-rotSpeed > deltaAng)
                deltaAng = -rotSpeed;
        } else if (rotSpeed < deltaAng) {
            deltaAng = rotSpeed;
        }
        FABLE_RETAIL_ASSERT(pTCKraken->GetPCentreMarker());
        C3DVector desiredPos = pTarg->GetPos() - pTCKraken->GetPCentreMarker()->GetPos();
        desiredPos.Z = 0.0f;
        float desiredPosDelta = desiredPos.GetMagnitudeAndNormalise();
        desiredPos *= desiredPosDelta > pTCKraken->GetKrakenDef().PoolRadius ? pTCKraken->GetKrakenDef().PoolRadius : desiredPosDelta;
        C3DVector tentacleDelta = (desiredPos + pTCKraken->GetPCentreMarker()->GetPos()) - GetParentThing().GetPos();
        tentacleDelta.Z = 0.0f;
        desiredPosDelta = tentacleDelta.GetMagnitudeAndNormalise();
        if (GetTentacleDef().TentacleStrikeDistTolerence < desiredPosDelta) {
            desiredPosDelta = desiredPosDelta > GetTentacleDef().TentacleMoveSpeed ? GetTentacleDef().TentacleMoveSpeed : desiredPosDelta;
            GetParentThing().GetPhysicsTC()->SetPosition(GetParentThing().GetPos() + tentacleDelta * desiredPosDelta);
        } else CanStrike = angleInTolerence;
        GetParentThing().GetPhysicsTC()->SetFacingAngleXY(deltaAng + currentFace);
    }
}
int main() {
 const float facings[]={0.0f,0.125f,0.25f,0.375f,0.5f};
 for(int life=0;life<3;++life)
 for(int f=0;f<5;++f)
 for(int x=0;x<=8;x+=4)
 for(int radius=2;radius<=8;radius+=2)
 for(int tolerance=0;tolerance<=4;tolerance+=2)
 for(int speed=2;speed<=6;speed+=2)
 for(int angular=0;angular<3;++angular)
 for(int prior=0;prior<=1;++prior) {
    CThing parent,owner,marker,target;
    CTCKraken controller;
    CTCKrakenTentacle t;
    owner.component=&controller;controller.target=life?&target:0;
    target.alive=life==2;controller.marker=&marker;controller.def.PoolRadius=(float)radius;
    marker.physics.position=C3DVector(0,0,-8);target.physics.position=C3DVector((float)x,0,9);
    parent.physics.position=C3DVector(0,0,1);parent.physics.facing=facings[f];
    parent.physics.turns=parent.physics.moves=0;
    t.PKraken=&owner;t.parent=&parent;t.CanStrike=prior!=0;
    const float degrees=angular==0?0.0f:(angular==1?44.0f:46.0f);
    t.def.TentacleStrikeAngleTolerence=degrees;t.def.TentacleTurnSpeed=45;
    t.def.TentacleMoveSpeed=(float)speed;t.def.TentacleStrikeDistTolerence=(float)tolerance;
    t.TrackTarget();
    const int distance=x<radius?x:radius;
    const bool move=life==2 && distance>tolerance;
    const float angle=x?0.25f:0.0f;
    const float error=angle-facings[f];
    const float turn=error< -0.125f?-0.125f:(error>0.125f?0.125f:error);
    if(parent.physics.turns!=(life==2)||parent.physics.moves!=int(move))return 1;
    if(fabs(parent.physics.facing-(facings[f]+(life==2?turn:0)))>0.00001)return 2;
    if(t.CanStrike!=(life==2&&!move?fabs(error)*360.0<degrees:prior!=0))return 3;
    if(parent.physics.position.X!=(move?(speed<distance?speed:distance):0)||
       parent.physics.position.Y!=0||parent.physics.position.Z!=1)return 4;
 }
 std::puts("007f0c70_TEST PASS");
 return 0;
}
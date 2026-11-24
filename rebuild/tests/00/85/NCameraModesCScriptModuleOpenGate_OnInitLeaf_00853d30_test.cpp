// Execute the exact camera body using real vector operations and observable dependencies.
#include <stdio.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN
#include "engine/C3DVector.h"
C3DVector& C3DVector::AccurateNormalise(){float scale=1.0f/(float)sqrt(X*X+Y*Y+Z*Z);X*=scale;Y*=scale;Z*=scale;return *this;}
struct Physics {C3DVector facing;C3DVector GetFacingVector()const{return facing;}};
struct Thing {
    bool alive;C3DVector position;Physics physics;
    const C3DVector& GetPos()const{return position;}const Physics* GetPhysicsTC()const{return &physics;}
};
static bool IS_THING_ALIVE(const Thing* thing){return thing&&thing->alive;}
struct Owner {Thing observed;const Thing* GetObservedThing()const{return &observed;}};
struct CScriptModuleOpenGate {
    Thing* PGate;Owner owner;int duration;C3DVector Pos,DeltaPos;
    Owner& GetOwner(){return owner;}int GetDuration()const{return duration;}
    void OnInitLeaf();
};
void CScriptModuleOpenGate::OnInitLeaf()
{
    if (IS_THING_ALIVE(PGate)) {
        C3DVector gate_to_creature = GetOwner().GetObservedThing()->GetPos() - PGate->GetPos();
        gate_to_creature.Z = 0.0f;
        C3DVector axis_y = PGate->GetPhysicsTC()->GetFacingVector();
        axis_y.Z = 0.0f;
        axis_y.AccurateNormalise();
        if (axis_y.Dot(gate_to_creature) < 0.0f) axis_y *= -1.0f;
        C3DVector axis_x = axis_y.CrossWithUp();
        if (axis_x.Dot(gate_to_creature) > 0.0f) axis_x *= -1.0f;
        const float AWAY_FROM_GATE = 1.5f;
        const float OFF_GATE_AXIS = 1.5f;
        const float START_HEIGHT = 0.75f;
        C3DVector offset = axis_y * AWAY_FROM_GATE + axis_x * OFF_GATE_AXIS + C3DVector(0, 0, START_HEIGHT);
        Pos = PGate->GetPos() + offset;
        C3DVector end_pos = PGate->GetPos() + offset * 2.0f;
        DeltaPos = (end_pos - Pos) / (float)GetDuration();
    }
}
int main(){
    for(int life=0;life<3;++life)for(int x=-1;x<=1;++x)for(int y=-1;y<=1;++y)for(int d=1;d<=2;++d){
        Thing gate;gate.alive=life==2;gate.position=C3DVector(4,8,16);gate.physics.facing=C3DVector(0,2,11);
        CScriptModuleOpenGate camera;camera.PGate=life?&gate:0;camera.owner.observed.position=C3DVector(4.0f+x,8.0f+y,32);
        camera.duration=d;camera.Pos=C3DVector(-1,-2,-3);camera.DeltaPos=C3DVector(-4,-5,-6);
        camera.OnInitLeaf();
        if(life<2){
            if(camera.Pos.X!=-1||camera.Pos.Y!=-2||camera.Pos.Z!=-3||camera.DeltaPos.X!=-4||camera.DeltaPos.Y!=-5||camera.DeltaPos.Z!=-6)return 1;
        }else{
            float axis_y=y<0?-1.0f:1.0f;
            float axis_x=axis_y*x>0?-axis_y:axis_y;
            if(camera.Pos.X!=4+1.5f*axis_x||camera.Pos.Y!=8+1.5f*axis_y||camera.Pos.Z!=16.75f)return 2;
            if(camera.DeltaPos.X!=1.5f*axis_x/d||camera.DeltaPos.Y!=1.5f*axis_y/d||camera.DeltaPos.Z!=0.75f/d)return 3;
        }
    }
    puts("00853d30_TEST PASS");return 0;
}
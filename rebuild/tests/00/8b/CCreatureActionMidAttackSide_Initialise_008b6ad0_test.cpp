#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ
#ifndef _MSC_VER
#define __fastcall
#endif
#include "rebuild_abi.h"
#include "engine/C3DVector.h"

// Cases below exercise the sign and zero of the dot product. Powers of two and
// an orthogonal Z displacement keep the choice independent of estimate accuracy.
unsigned long GFastOneOverSqrtTable[128]={0};
class Creature {
public:
    C3DVector pos, facing;
    const C3DVector& GetPos() const { return pos; }
    Creature* GetPhysicsTC() { return this; }
    C3DVector GetFacingVector() const { return facing; }
};
class CCombatActionBase {
public:
    Creature* PCreature;
    Creature* PTarget;
    const char* selected;
    int sequence;
    void SetCombatAnimation(const char* n) { selected=n; sequence=sequence*10+1; }
    void Initialise() { sequence=sequence*10+2; }
};
class CCreatureAction_MidAttackSide : public CCombatActionBase {
public:
    void Initialise();
};

void CCreatureAction_MidAttackSide::Initialise()
{
    C3DVector lineToTarget = PTarget->GetPos() - PCreature->GetPos();
    C3DVector facingDirection = PCreature->GetPhysicsTC()->GetFacingVector();
    lineToTarget.Normalise();
    C3DVector facingDirectionPerpendicular;
    facingDirectionPerpendicular.X = facingDirection.Y;
    facingDirectionPerpendicular.Y = -facingDirection.X;
    facingDirectionPerpendicular.Z = 0.0f;
    if (lineToTarget.Dot(facingDirectionPerpendicular) > 0.0f)
        SetCombatAnimation("STRIKE_MEDIUM_RIGHT_90");
    else
        SetCombatAnimation("STRIKE_MEDIUM_LEFT_90");
    CCombatActionBase::Initialise();
}

int main()
{
    const float cases[][6] = {
        {1,0,0,-1,0,1}, {1,0,0,1,0,0}, {0,1,1,0,0,1}, {0,1,-1,0,0,0},
        {1,0,1,0,0,0}, {0,1,0,0,2,0}, {0,0,2,0,0,0}, {1,0,0,0,0,0}
    };
    for (unsigned i=0; i<sizeof(cases)/sizeof(cases[0]); ++i) {
        Creature source,target;
        source.pos.Initialise(4,8,-2);
        source.facing.Initialise(cases[i][0],cases[i][1],1);
        target.pos.Initialise(4+cases[i][2],8+cases[i][3],-2+cases[i][4]);
        CCreatureAction_MidAttackSide action;
        action.PCreature=&source; action.PTarget=&target;
        action.sequence=0; action.selected=0;
        action.Initialise();
        const char* expected=cases[i][5] ? "STRIKE_MEDIUM_RIGHT_90" : "STRIKE_MEDIUM_LEFT_90";
        if (action.sequence!=12 || strcmp(action.selected,expected)!=0) return 1;
    }
    puts("008b6ad0_TEST PASS");
    return 0;
}
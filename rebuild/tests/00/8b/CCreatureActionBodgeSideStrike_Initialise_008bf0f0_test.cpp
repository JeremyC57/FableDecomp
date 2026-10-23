// Behaviour test for 0x008BF0F0 CCreatureAction_BodgeSideStrike::Initialise: handedness from the side of the facing the target is on.
#include <cstdio>
#include <cstring>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
enum ECombatAnimationHandedness { COMBAT_ANIM_LEFT_HANDED = 0, COMBAT_ANIM_RIGHT_HANDED = 1 };
struct CThingCreatureBase {
    C3DVector pos, facing; int hand;
    const C3DVector& GetPosition_004c73d0() const { return pos; }
    C3DVector GetFacing() const { return facing; }
    void SetLastActionHandedness(ECombatAnimationHandedness h) { hand = h; }
};
struct CBaseIntelligentPointer { CThingCreatureBase* PItem; CThingCreatureBase* GetPItem() const { return PItem; } };
struct CCreatureAction_BodgeSideStrike {
    CBaseIntelligentPointer Owner, Target; const char* anim; int baseInit;
    void SetAnimationName(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        CThingCreatureBase* target = Target.GetPItem();
        C3DVector delta = target->GetPosition_004c73d0() - Owner.GetPItem()->GetPosition_004c73d0();
        C3DVector facing = Owner.GetPItem()->GetFacing();
        facing.Z = 0.0f;
        float side = delta.X * facing.Y - delta.Y * facing.X;
        if (side > 0.0f) Owner.GetPItem()->SetLastActionHandedness(COMBAT_ANIM_RIGHT_HANDED);
        else Owner.GetPItem()->SetLastActionHandedness(COMBAT_ANIM_LEFT_HANDED);
        { CCharString name("STRIKE_MEDIUM_SIDE"); SetAnimationName(name); }
        ++baseInit;
    }
};
int main()
{
    CThingCreatureBase o, t;
    o.pos = C3DVector(0, 0, 0); o.facing = C3DVector(0, 1, 0); o.hand = -1;
    t.pos = C3DVector(-5, 1, 0);          // delta (-5,1): -5*1 - 1*0 = -5 -> left
    CCreatureAction_BodgeSideStrike a; a.Owner.PItem = &o; a.Target.PItem = &t; a.anim = 0; a.baseInit = 0;
    a.Initialise();
    bool ok = o.hand == 0 && a.anim && !std::strcmp(a.anim, "STRIKE_MEDIUM_SIDE") && a.baseInit == 1;
    t.pos = C3DVector(5, 1, 0);           // 5*1 = 5 > 0 -> right
    a.Initialise(); ok = ok && o.hand == 1;
    if (!ok) { std::printf("008bf0f0_TEST FAIL\n"); return 1; }
    std::printf("008bf0f0_TEST PASS\n");
    return 0;
}
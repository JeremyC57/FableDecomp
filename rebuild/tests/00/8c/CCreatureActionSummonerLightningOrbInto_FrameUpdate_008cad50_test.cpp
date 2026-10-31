// Behaviour test for CCreatureAction_SummonerLightningOrbInto::FrameUpdate @ 0x008CAD50: after the base update, with a live
// target the creature's facing turns towards the target in the XY plane by at most 1/36 of a turn per frame (the turn is
// acos(dot) / 2pi), negated when the target lies to the left (cross product Z > 0); a dead or missing target does nothing.
#include <stdio.h>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void AccurateNormalise() { float m = (float)sqrt(X * X + Y * Y + Z * Z); if (m > 0) { X /= m; Y /= m; Z /= m; } }
};
static C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CTest {
    bool HasTarget, TargetDead;
    C3DVector CreaturePos, TargetPos, Facing;
    int baseUpdates, sets;
    float rotatedBy;
    void FrameUpdate()
    {
        ++baseUpdates;
        if (HasTarget && !TargetDead) {
            C3DVector direction(TargetPos.X - CreaturePos.X, TargetPos.Y - CreaturePos.Y, 0.0f);
            direction.AccurateNormalise();
            C3DVector facing = Facing;
            float dot = direction.Z * facing.Z + direction.X * facing.X + direction.Y * facing.Y;
            if (dot < -1.0f)
                dot = -1.0f;
            else if (dot > 1.0f)
                dot = 1.0f;
            float angle = 0.027777778f;
            float turn = (float)fabs(acos(dot) * 0.15915494f);
            if (0.027777778f > turn)
                angle = turn;
            if (CrossProduct(facing, direction).Z > 0.0f)
                angle *= -1.0f;
            rotatedBy = angle;
            ++sets;
        }
    }
};
static CTest Make(bool has, bool dead, float tx, float ty)
{
    CTest t;
    t.HasTarget = has; t.TargetDead = dead;
    t.CreaturePos = C3DVector(5, 5, 0); t.TargetPos = C3DVector(5 + tx, 5 + ty, 9);
    t.Facing = C3DVector(1, 0, 0);
    t.baseUpdates = t.sets = 0; t.rotatedBy = 99.0f;
    return t;
}
int main()
{
    int ok = 1;
    CTest a = Make(true, false, 0, 3); a.FrameUpdate();          // quarter turn left: clamped, negated
    ok &= a.sets == 1 && fabs(a.rotatedBy + 0.027777778f) < 1e-6;
    CTest b = Make(true, false, 0, -3); b.FrameUpdate();         // quarter turn right: clamped, positive
    ok &= fabs(b.rotatedBy - 0.027777778f) < 1e-6;
    CTest c = Make(true, false, (float)cos(0.1), (float)-sin(0.1)); c.FrameUpdate();   // small turn right: unclamped
    ok &= fabs(c.rotatedBy - 0.1f * 0.15915494f) < 1e-4;
    CTest d = Make(true, true, 0, 3); d.FrameUpdate();
    ok &= d.sets == 0 && d.baseUpdates == 1;
    CTest e = Make(false, false, 0, 3); e.FrameUpdate();
    ok &= e.sets == 0 && e.baseUpdates == 1;
    if (!ok) { printf("008cad50_TEST FAIL\n"); return 1; }
    printf("008cad50_TEST PASS\n");
    return 0;
}
#include <stdio.h>
#include <math.h>
// Behaviour copy of CCreatureAction_DuckAndCover::Initialise (0x008B6EE0): normalise the dive direction and turn the physics to face it.
struct Vec { float X, Y, Z; };
static void Normalise(Vec& v) { float l = (float)sqrt((double)(v.X * v.X + v.Y * v.Y + v.Z * v.Z)); v.X /= l; v.Y /= l; v.Z /= l; }
struct Physics { float facing; bool set; void SetFacingAngle(float f) { facing = f; set = true; } };
struct Action {
    const char* anim; bool baseInit; Vec Direction; Physics* physics;
    void Initialise() {
        anim = "DIVE_FOR_COVER"; baseInit = true;
        Normalise(Direction);
        float fraction = (float)atan2((double)Direction.X, (double)Direction.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) { fraction = (float)fmod((double)fraction, 1.0); if (fraction < 0.0f) fraction += 1.0f; }
        physics->SetFacingAngle(fraction);
    }
};
int main()
{
    Physics p = { 0, false };
    Action a; a.anim = 0; a.baseInit = false; a.physics = &p;
    a.Direction.X = -3.0f; a.Direction.Y = 0.0f; a.Direction.Z = 4.0f;   // atan2(-1, 0) = -pi/2 -> -0.25 wraps to 0.75
    a.Initialise();
    bool ok = a.baseInit && p.set && fabs(p.facing - 0.75f) < 1e-4f && fabs(a.Direction.X + 0.6f) < 1e-4f && fabs(a.Direction.Z - 0.8f) < 1e-4f;
    a.Direction.X = 1.0f; a.Direction.Y = 1.0f; a.Direction.Z = 0.0f;    // atan2(1,1) = pi/4 -> 0.125, no wrap
    a.Initialise();
    ok = ok && fabs(p.facing - 0.125f) < 1e-4f;
    printf(ok ? "008b6ee0_TEST PASS\n" : "008b6ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}
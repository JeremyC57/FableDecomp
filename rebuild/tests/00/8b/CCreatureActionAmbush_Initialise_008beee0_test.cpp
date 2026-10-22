#include <stdio.h>
#include <string.h>
#include <math.h>
// Behaviour copy of CCreatureAction_Ambush::Initialise (0x008BEEE0): flatten JumpDest - position to XY, normalise it,
// take the perpendicular side vector (dir.Y, -dir.X), pick the LEFT/RIGHT ambush animation from the sign of the 2D cross
// product with the target's position (negating the side for RIGHT), say the Ambushing speech of the combat dialogue def
// (ref-counted copy released again), face along the side vector (atan2 as a 0..1 turn fraction, fmod-wrapped), then run the
// base Initialise.
struct Vec3 { float X, Y, Z; };
struct DialogueDef { long refCount; long Ambushing; };
struct Action {
    Vec3 JumpDest;
    Vec3 creaturePos;
    Vec3 targetPos;
    DialogueDef* def;
    const char* anim;
    long spokenSpeech;
    int speechCalls;
    float facing;
    bool baseInit;
    void Initialise()
    {
        float dx = JumpDest.X - creaturePos.X;
        float dy = JumpDest.Y - creaturePos.Y;
        float r = 1.0f / (float)sqrt((double)(dx * dx + dy * dy));
        Vec3 dir = { dx * r, dy * r, 0.0f };
        Vec3 side = { dir.Y, -dir.X, 0.0f };
        if (side.X * targetPos.Y - side.Y * targetPos.X > 0.0f) {
            anim = "GENERATE_AMBUSH_LEFT";
        } else {
            anim = "GENERATE_AMBUSH_RIGHT";
            side.X = -side.X; side.Y = -side.Y; side.Z = 0.0f;
        }
        long speech;
        {
            ++def->refCount;                     // CDefPointer copy
            speech = def->Ambushing;
            --def->refCount;                     // released at scope end
        }
        spokenSpeech = speech; ++speechCalls;
        float fraction = (float)atan2((double)side.X, (double)side.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod((double)fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        facing = fraction;
        baseInit = true;
    }
};
static void Reset(Action& a, DialogueDef* d)
{
    a.def = d; a.anim = 0; a.spokenSpeech = -1; a.speechCalls = 0; a.facing = -1.0f; a.baseInit = false;
    a.creaturePos.X = 0.0f; a.creaturePos.Y = 0.0f; a.creaturePos.Z = 0.0f;
    a.JumpDest.X = 0.0f; a.JumpDest.Y = 10.0f; a.JumpDest.Z = 5.0f;    // Z is ignored: dir = (0,1), side = (1,0)
}
int main()
{
    DialogueDef d; d.refCount = 1; d.Ambushing = 42;
    Action a;
    Reset(a, &d);
    a.targetPos.X = -5.0f; a.targetPos.Y = 3.0f; a.targetPos.Z = 0.0f;    // cross = 3 > 0 -> LEFT, atan2(1,0) -> 0.25
    a.Initialise();
    bool ok = strcmp(a.anim, "GENERATE_AMBUSH_LEFT") == 0 && fabs(a.facing - 0.25f) < 1e-4f && a.baseInit &&
              a.spokenSpeech == 42 && a.speechCalls == 1 && d.refCount == 1;
    Reset(a, &d);
    a.targetPos.X = 0.0f; a.targetPos.Y = -3.0f; a.targetPos.Z = 0.0f;    // cross = -3 -> RIGHT, side (-1,0) -> -0.25 wraps to 0.75
    a.Initialise();
    ok = ok && strcmp(a.anim, "GENERATE_AMBUSH_RIGHT") == 0 && fabs(a.facing - 0.75f) < 1e-4f && a.baseInit && d.refCount == 1;
    Reset(a, &d);
    a.creaturePos.X = 3.0f; a.creaturePos.Y = 4.0f;
    a.JumpDest.X = 6.0f; a.JumpDest.Y = 8.0f;                             // dir (0.6,0.8), side (0.8,-0.6)
    a.targetPos.X = 1.0f; a.targetPos.Y = 1.0f;                           // cross = 0.8 + 0.6 > 0 -> LEFT
    a.Initialise();
    float expect = (float)atan2(0.8, -0.6) * 0.15915493667125702f;      // positive, below 1
    ok = ok && strcmp(a.anim, "GENERATE_AMBUSH_LEFT") == 0 && fabs(a.facing - expect) < 1e-4f;
    if (!ok) { printf("008beee0_TEST FAIL\n"); return 1; }
    printf("008beee0_TEST PASS\n");
    return 0;
}
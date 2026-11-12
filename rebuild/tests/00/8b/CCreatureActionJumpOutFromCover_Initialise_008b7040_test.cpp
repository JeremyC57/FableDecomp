// Behaviour test for 0x008B7040 CCreatureAction_JumpOutFromCover::Initialise: the jump direction is normalised (fast 1/sqrt);
// with a live target the creature-to-target direction turned +1/4 decides the side: a positive dot with the jump direction picks
// JUMP_OUT_RIGHT and faces the direction turned -1/4, else JUMP_OUT_LEFT turned +1/4; the facing is atan2(x, y)/(2pi) wrapped into
// [0, 1); the base Initialise always runs.
#include <cstdio>
#include <cmath>
#include <cstring>
struct V { float X, Y, Z; };
static void RotateXY(V& v, float turns)
{
    double a = -turns * 6.283185307179586;
    float x = v.X, y = v.Y;
    v.X = (float)(x * std::cos(a) - y * std::sin(a));
    v.Y = (float)(x * std::sin(a) + y * std::cos(a));
}
static void Normalise(V& v) { float r = 1.0f / std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); v.X *= r; v.Y *= r; v.Z *= r; }
struct Action {
    V JumpDirection; V CreaturePos; V TargetPos; bool TargetAlive; const char* Anim; float Facing; int BaseInit;
    void Initialise()
    {
        Normalise(JumpDirection);
        if (TargetAlive) {
            V toTarget = { TargetPos.X - CreaturePos.X, TargetPos.Y - CreaturePos.Y, TargetPos.Z - CreaturePos.Z };
            Normalise(toTarget);
            V side = toTarget; RotateXY(side, 0.25f);
            V direction = JumpDirection;
            if (side.Z * JumpDirection.Z + side.Y * JumpDirection.Y + side.X * JumpDirection.X > 0.0f) {
                RotateXY(direction, -0.25f); Anim = "JUMP_OUT_RIGHT";
            } else {
                RotateXY(direction, 0.25f); Anim = "JUMP_OUT_LEFT";
            }
            float fraction = (float)std::atan2(direction.X, direction.Y) * 0.15915493667125702f;
            if (fraction < 0.0f || fraction >= 1.0f) {
                fraction = (float)std::fmod((double)fraction, 1.0);
                if (fraction < 0.0f) fraction += 1.0f;
            }
            Facing = fraction;
        }
        ++BaseInit;
    }
};
int main()
{
    Action a = { { 2.0f, 0.0f, 0.0f }, { 0, 0, 0 }, { 0, 5.0f, 0 }, true, 0, -1.0f, 0 };
    a.Initialise();
    bool ok = std::fabs(a.JumpDirection.X - 1.0f) < 1e-5f && a.Anim != 0 && a.BaseInit == 1 && a.Facing >= 0.0f && a.Facing < 1.0f;
    const char* first = a.Anim;
    Action b = { { -2.0f, 0.0f, 0.0f }, { 0, 0, 0 }, { 0, 5.0f, 0 }, true, 0, -1.0f, 0 };
    b.Initialise();
    ok = ok && b.Anim != 0 && std::strcmp(first, b.Anim) != 0;
    Action c = { { 0.0f, 3.0f, 4.0f }, { 0, 0, 0 }, { 1, 1, 0 }, false, 0, -1.0f, 0 };
    c.Initialise();
    ok = ok && c.Anim == 0 && c.Facing == -1.0f && c.BaseInit == 1 && std::fabs(c.JumpDirection.Z - 0.8f) < 1e-5f;
    if (!ok) { std::printf("008b7040_TEST FAIL\n"); return 1; }
    std::printf("008b7040_TEST PASS\n");
    return 0;
}
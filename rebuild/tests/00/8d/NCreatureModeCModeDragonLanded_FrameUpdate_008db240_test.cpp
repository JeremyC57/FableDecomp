// Behaviour test for CModeDragonLanded::FrameUpdate (0x008DB240): head-turn target choice.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
// returns 1 = look at target, 2 = look ahead, 0 = nothing
static int Decide(bool targetAlive, bool hasLook, bool useHeadTurning, V owner, V target)
{
    if (!targetAlive || !hasLook) return 0;
    bool facing = false;
    V d = { target.X - owner.X, target.Y - owner.Y, target.Z - owner.Z };
    float inv = 1.0f / std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
    V dir = { d.X * inv, d.Y * inv, d.Z * inv };
    if (std::sqrt(d.X * d.X + d.Y * d.Y) < 12.5f && (dir.X * 0.0f + dir.Y * -1.0f + dir.Z * 0.0f) > 0.96f)
        facing = true;
    if (useHeadTurning && !facing) return 1;
    return 2;
}

int main()
{
    V o = { 0, 0, 0 };
    V near = { 0.1f, -5.0f, 0.0f };      // close, straight down -Y: facing
    V far = { 0.0f, -50.0f, 0.0f };      // too far
    V side = { 5.0f, 0.0f, 0.0f };       // close but sideways
    int ok = Decide(true, true, true, o, near) == 2 && Decide(true, true, true, o, far) == 1
          && Decide(true, true, true, o, side) == 1 && Decide(true, true, false, o, side) == 2
          && Decide(false, true, true, o, side) == 0 && Decide(true, false, true, o, side) == 0;
    std::printf(ok ? "008db240_TEST PASS\n" : "008db240_TEST FAIL\n");
    return ok ? 0 : 1;
}
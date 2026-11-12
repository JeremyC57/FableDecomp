#include <cstdio>
#include <cmath>
// Behaviour model of CTCKrakenTentacle::ReturnToDesiredPos: dir/dist from current->desired, angle clamp and step clamp.
struct V { float X, Y, Z; };
static float SignedDiff(float from, float to) { float d = to - from; while (d > 0.5f) d -= 1.0f; while (d < -0.5f) d += 1.0f; return d; }
struct Result { V pos; float facing; };
static Result Step(V current, V desired, bool dummyFound, float heightOffset, float krakenFacing, float facingOffset,
                   float facing, float turnSpeed, float moveSpeed, bool immediately)
{
    if (dummyFound) desired.Z += heightOffset;
    V dir = { desired.X - current.X, desired.Y - current.Y, desired.Z - current.Z };
    float dist;
    float squared = dir.Z * dir.Z; squared += dir.Y * dir.Y; squared += dir.X * dir.X;
    if (squared > 1e-4f * 1e-4f) { dist = std::sqrt(squared); dir.X /= dist; dir.Y /= dist; dir.Z /= dist; }
    else dist = 0.0f;
    float krakenAngle = krakenFacing; krakenAngle += facingOffset;
    float angle = SignedDiff(facing, krakenAngle);
    if (!immediately) {
        float turn = turnSpeed * (1.0f / 360.0f);
        if (angle < 0.0f) { if (-turn > angle) angle = -turn; }
        else if (turn < angle) angle = turn;
        if (!(dist < moveSpeed)) dist = moveSpeed;
    }
    Result r; r.pos.X = dir.X * dist + current.X; r.pos.Y = dir.Y * dist + current.Y; r.pos.Z = dir.Z * dist + current.Z;
    r.facing = angle + facing; return r;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    V c = { 0, 0, 0 }, d = { 10, 0, 0 };
    Result r = Step(c, d, true, 2.0f, 0.25f, 0.0f, 0.0f, 36.0f, 1.0f, false);
    // dist clamped to 1 along (10,0,2) normalised; angle clamped to 0.1
    float n = std::sqrt(104.0f);
    if (!Near(r.pos.X, 10.0f / n) || !Near(r.pos.Z, 2.0f / n) || !Near(r.facing, 0.1f)) { std::printf("007f0f40_TEST FAIL\n"); return 1; }
    r = Step(c, d, false, 2.0f, 0.25f, 0.0f, 0.0f, 36.0f, 1.0f, true);
    if (!Near(r.pos.X, 10.0f) || !Near(r.pos.Z, 0.0f) || !Near(r.facing, 0.25f)) { std::printf("007f0f40_TEST FAIL\n"); return 1; }
    r = Step(c, c, false, 0, -0.3f, 0.0f, 0.0f, 36.0f, 1.0f, false);
    if (!Near(r.pos.X, 0.0f) || !Near(r.facing, -0.1f)) { std::printf("007f0f40_TEST FAIL\n"); return 1; }
    std::printf("007f0f40_TEST PASS\n");
    return 0;
}
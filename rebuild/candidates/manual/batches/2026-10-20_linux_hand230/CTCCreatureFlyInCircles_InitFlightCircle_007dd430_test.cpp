// Behaviour test for CTCCreatureFlyInCircles::InitFlightCircle (0x007DD430).
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Physics { V3 pos; V3 up; V3 fwd; };
struct Creature { bool hasTC; bool onMap; Physics phys; };
static bool IsPosInMap(const V3& p) { return p.X >= 0.0f && p.Y >= 0.0f; }
struct Fly {
    Creature* creature; V3 Centre; float Speed; float CurrentRadius; long Mode;
    void InitFlightCircle(const V3& centre, float radius, float height)
    {
        Centre = centre; CurrentRadius = radius; Mode = 1; Speed = 3.0f;
        V3 start = { centre.X + radius, centre.Y, centre.Z + height };
        if (creature->hasTC) creature->onMap = IsPosInMap(start);
        creature->phys.pos = start;
        V3 up = { 0.0f, 0.0f, 1.0f }, fwd = { 0.0f, 1.0f, 0.0f };
        creature->phys.up = up; creature->phys.fwd = fwd;
    }
};

int main()
{
    Creature c = { true, false };
    Fly f = { &c };
    V3 centre = { 10.0f, 20.0f, 5.0f };
    f.InitFlightCircle(centre, 4.0f, 2.5f);
    if (f.Mode != 1 || f.Speed != 3.0f || f.CurrentRadius != 4.0f || f.Centre.Y != 20.0f) { std::printf("007dd430_TEST FAIL\n"); return 1; }
    if (c.phys.pos.X != 14.0f || c.phys.pos.Y != 20.0f || c.phys.pos.Z != 7.5f || !c.onMap) { std::printf("007dd430_TEST FAIL\n"); return 1; }
    if (c.phys.up.Z != 1.0f || c.phys.fwd.Y != 1.0f || c.phys.fwd.Z != 0.0f) { std::printf("007dd430_TEST FAIL\n"); return 1; }
    V3 off = { -30.0f, 0.0f, 0.0f };
    f.InitFlightCircle(off, 4.0f, 0.0f);
    if (c.onMap) { std::printf("007dd430_TEST FAIL\n"); return 1; }
    c.hasTC = false; c.onMap = true; f.InitFlightCircle(off, 1.0f, 0.0f);
    if (!c.onMap) { std::printf("007dd430_TEST FAIL\n"); return 1; }
    std::printf("007dd430_TEST PASS\n");
    return 0;
}

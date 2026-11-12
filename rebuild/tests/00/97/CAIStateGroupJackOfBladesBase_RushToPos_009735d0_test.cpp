// Behaviour test for CAIStateGroup_JackOfBladesBase::RushToPos (0x009735D0): a self-contained model of its logic --
// fast-normalised direction + distance, solid-to-thing component lookup/creation, the rush action with its counted
// final action, and the conditional restore of solidity.
#include <cstdio>
#include <cmath>
static unsigned long GFastOneOverSqrtTable[128];
static void InitTable()
{
    for (int i = 0; i < 128; ++i) {
        float m = 1.0f + (i + 0.5f) / 128.0f;           // mantissa bucket
        float r = 1.0f / std::sqrt(m);
        unsigned long bits = *(unsigned long*)&r;
        GFastOneOverSqrtTable[i] = (bits >> 15) & 0xff;  // only feeds the seed; Newton step refines it
    }
}
static float GFFastOneOverSqrt(float value)
{
    unsigned long bits = *(unsigned long*)&value;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * value) * r * 0.5f;
}
struct V { float X, Y, Z; };
struct Solid { int notSolidCount; Solid() : notSolidCount(0) {} };
struct Creature { V pos; bool hasSolid; Solid solid; Solid* added; };
struct Rush { V dir; float distance; int finalRefs; };
static int g_added = 0;
static int RushToPos(Creature& c, bool targetAlive, const V& p, Rush& out)
{
    Solid* solid = 0;
    V delta = { p.X - c.pos.X, p.Y - c.pos.Y, p.Z - c.pos.Z };
    float r = GFFastOneOverSqrt((delta.Z * delta.Z) + (delta.Y * delta.Y) + (delta.X * delta.X));
    V dir = { delta.X * r, delta.Y * r, delta.Z * r };
    float distance = std::sqrt(delta.X * delta.X + delta.Z * delta.Z + delta.Y * delta.Y);
    if (c.hasSolid) solid = &c.solid; else { ++g_added; c.hasSolid = true; solid = &c.solid; }
    if (solid) ++solid->notSolidCount;
    out.dir = dir; out.distance = distance; out.finalRefs = 2;   // counted pointer: local + by-value copy
    int result = 7;
    if (targetAlive && solid) --solid->notSolidCount;
    return result;
}
int main()
{
    InitTable();
    Creature c = { { 1.0f, 2.0f, 3.0f }, false, Solid(), 0 };
    V target = { 4.0f, 6.0f, 3.0f };
    Rush rush;
    int res = RushToPos(c, true, target, rush);
    if (res != 7 || g_added != 1 || !c.hasSolid || c.solid.notSolidCount != 0) { std::printf("009735d0_TEST FAIL\n"); return 1; }
    if (std::fabs(rush.distance - 5.0f) > 1e-4f) { std::printf("009735d0_TEST FAIL\n"); return 1; }
    if (std::fabs(rush.dir.X - 0.6f) > 0.02f || std::fabs(rush.dir.Y - 0.8f) > 0.02f || std::fabs(rush.dir.Z) > 1e-6f) { std::printf("009735d0_TEST FAIL\n"); return 1; }
    res = RushToPos(c, false, target, rush);                     // dead target: stays non-solid, no second AddTC
    if (g_added != 1 || c.solid.notSolidCount != 1) { std::printf("009735d0_TEST FAIL\n"); return 1; }
    std::printf("009735d0_TEST PASS\n");
    return 0;
}
// Behaviour test for CThingCompare_NearestGroupByMember::operator()<CThing> (0x00934F20), g++ model of the parked source.
#include <cstdio>
struct V { float X, Y, Z; };
struct Thing;
struct Group { Thing* nearest; };
struct Thing { V pos; Group* group; };
static float Dist2(V a, V b) { float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z; return x * x + y * y + z * z; }
static Thing* Compare(V pos, Thing* a, Thing* b)
{
    if (!a) return b;
    if (!b) return a;
    if (!a->group) return b;
    Group* groupA = a->group;
    if (b->group) {
        Thing* nearestA = groupA->nearest;
        Thing* nearestB = groupA->nearest;        // retail asks group A twice (as the parked source)
        if (!nearestA) return b;
        if (nearestB && !(Dist2(nearestA->pos, pos) < Dist2(nearestB->pos, pos))) return b;
    }
    return a;
}
int main()
{
    V o = { 0, 0, 0 };
    Thing m = { { 1, 0, 0 }, 0 };
    Group ga = { &m }, gn = { 0 };
    Thing a = { { 0, 0, 0 }, &ga }, b = { { 0, 0, 0 }, &ga }, c = { { 0, 0, 0 }, 0 }, d = { { 0, 0, 0 }, &gn };
    int ok = Compare(o, 0, &b) == &b && Compare(o, &a, 0) == &a && Compare(o, &c, &b) == &b;
    ok &= Compare(o, &a, &c) == &a;          // b without a group: a wins
    ok &= Compare(o, &a, &b) == &b;          // equal distances: not less, b
    ok &= Compare(o, &d, &b) == &b;          // a's group has no nearest member
    std::printf(ok ? "00934f20_TEST PASS\n" : "00934f20_TEST FAIL\n");
    return ok ? 0 : 1;
}

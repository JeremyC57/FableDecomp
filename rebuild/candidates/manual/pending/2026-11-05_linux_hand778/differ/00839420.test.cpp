// Behaviour test for NThingHelpers::CreateEffectOnHit 0x00839420 (parked): models the hit-position / direction adjustment -- the
// thing-to-striker offset in XY, normalised, scaled by radius * 0.75 from the thing's position (Z kept), the direction swung to
// (-Y, X, -0) for handedness 0 and (Y, -X, 0) for handedness 1 -- and the effect choice (blended pair only when both ids are set,
// else berserk / plain random id, created only when > 0).
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
static bool Near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
static void Adjust(const V& thing, const V& striker, float radius, long hand, V& pos, V& dir)
{
    V off = { striker.X - thing.X, striker.Y - thing.Y, 0.0f };
    float m = off.X * off.X + off.Y * off.Y;
    if (!(m > 0.0001f * 0.0001f)) return;
    float inv = 1.0f / (float)sqrt(m); off.X *= inv; off.Y *= inv;
    V p = { thing.X + off.X * radius * 0.75f, thing.Y + off.Y * radius * 0.75f, pos.Z };
    V d = dir;
    if (hand == 0) { d.X = off.Y * -1.0f; d.Y = -off.X * -1.0f; d.Z = 0.0f * -1.0f; }
    else if (hand == 1) { d.X = off.Y; d.Y = -off.X; d.Z = 0.0f; }
    pos = p; dir = d;
}
static long Choose(bool blended, long a, long b, bool berserk)
{
    if (blended) return (a && b) ? 1000 * a + b : 0;
    long id = berserk ? 7 : 3;
    return id > 0 ? id : 0;
}
int main()
{
    V thing = { 0, 0, 0 }, striker = { 4, 0, 0 }, pos = { 9, 9, 5 }, dir = { 0, 0, 1 };
    Adjust(thing, striker, 2.0f, 0, pos, dir);
    bool ok = Near(pos.X, 1.5f) && Near(pos.Y, 0.0f) && Near(pos.Z, 5.0f) && Near(dir.X, 0.0f) && Near(dir.Y, 1.0f);
    V pos2 = { 1, 1, 1 }, dir2 = { 0, 0, 1 };
    Adjust(thing, striker, 2.0f, 1, pos2, dir2);
    ok = ok && Near(dir2.Y, -1.0f);
    V pos3 = { 1, 1, 1 }, dir3 = { 0, 0, 1 };
    Adjust(thing, thing, 2.0f, 1, pos3, dir3);                           // no offset: unchanged
    ok = ok && Near(pos3.X, 1.0f) && Near(dir3.Z, 1.0f);
    ok = ok && Choose(true, 2, 5, false) == 2005 && Choose(true, 0, 5, false) == 0 && Choose(false, 0, 0, true) == 7;
    printf(ok ? "00839420_TEST PASS\n" : "00839420_TEST FAIL\n");
    return ok ? 0 : 1;
}

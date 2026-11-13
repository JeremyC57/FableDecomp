// Behaviour test for 0x0063B3F0 CWorldMap::CreateLineSegment: the same logic over a plain model of the vector and the map.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
static int g_mapSize = 10;
static bool IsPosInMap(const V3& p) { return p.X >= 0.0f && p.Y >= 0.0f && p.X < (float)g_mapSize && p.Y < (float)g_mapSize; }
static float MaxAbs2D(const V3& a, const V3& b) { float dx = std::fabs(a.X - b.X), dy = std::fabs(a.Y - b.Y); return dx > dy ? dx : dy; }
static bool CreateLineSegment(const V3* from, const V3* to, V3* segStart, V3* segEnd)
{
    bool crossesX = true, crossesY = true, valid = false;
    V3 end = *from;
    long fromX = (long)from->X, toX = (long)to->X;
    if (fromX == toX) crossesX = false;
    long toY = (long)to->Y;
    if ((long)from->Y == toY) crossesY = false;
    float dX = to->X - from->X, dY = to->Y - from->Y, dZ = to->Z - from->Z;
    V3 xHit = {0, 0, 0}, yHit = {0, 0, 0};
    xHit.X = (float)fromX;
    yHit.Y = (float)(long)from->Y;
    if (crossesX) {
        if (dX > 0.0f) xHit.X += 1.0f;
        else if (0.0f == from->X - (float)fromX) { xHit.X -= 1.0f; end.X = xHit.X; }
        float t = std::fabs(xHit.X - from->X), inv = 1.0f / std::fabs(dX);
        xHit.Y = (dY * inv) * t + from->Y; xHit.Z = (dZ * inv) * t + from->Z;
    }
    if (crossesY) {
        if (dY > 0.0f) yHit.Y += 1.0f;
        else if (0.0f == from->Y - (float)(long)from->Y) { yHit.Y -= 1.0f; end.Y = yHit.Y; }
        float t = std::fabs(yHit.Y - from->Y), inv = 1.0f / std::fabs(dY);
        yHit.X = (dX * inv) * t + from->X; yHit.Z = (dZ * inv) * t + from->Z;
    }
    if ((long)end.X == toX) crossesX = false;
    if ((long)end.Y == toY) crossesY = false;
    if (crossesX) {
        if (crossesY) { if (MaxAbs2D(xHit, *from) < MaxAbs2D(yHit, *from)) *segStart = xHit; else *segStart = yHit; }
        else *segStart = xHit;
        valid = true;
    } else if (crossesY) { *segStart = yHit; valid = true; }
    else { *segStart = *to; end = *to; }
    if (valid && !IsPosInMap(*segStart)) { *segStart = *from; end = *from; valid = false; }
    *segEnd = end;
    return valid;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static bool Eq(const V3& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
int main()
{
    int fail = 0; V3 s, e;
    // same cell: segment ends here -> segStart = to, segEnd = to, false
    { V3 f = {1.2f, 1.3f, 0}, t = {1.8f, 1.9f, 2}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (r || !Eq(s, 1.8f, 1.9f, 2) || !Eq(e, 1.8f, 1.9f, 2)) fail |= 1; }
    // crosses X only (+X): hit at x = 2, y/z interpolated
    { V3 f = {1.5f, 1.5f, 0}, t = {3.5f, 1.7f, 4}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (!r || !Eq(s, 2.0f, 1.55f, 1.0f) || !Eq(e, 1.5f, 1.5f, 0)) fail |= 2; }
    // crosses Y only (-Y) from a fractional start: hit at y = 4
    { V3 f = {2.5f, 4.5f, 1}, t = {2.6f, 2.5f, 1}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (!r || !Eq(s, 2.525f, 4.0f, 1) || !Eq(e, 2.5f, 4.5f, 1)) fail |= 4; }
    // crosses both: nearer is X (x = 2 at 0.25 of the way)
    { V3 f = {1.75f, 1.5f, 0}, t = {2.75f, 2.9f, 0}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (!r || !Near(s.X, 2.0f) || !Near(s.Y, 1.85f)) fail |= 8; }
    // crosses both: nearer is Y
    { V3 f = {1.2f, 1.9f, 0}, t = {2.8f, 3.1f, 0}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (!r || !Near(s.Y, 2.0f)) fail |= 16; }
    // start exactly on a block edge, moving -X: steps to x-1, segEnd.X follows
    { V3 f = {3.0f, 1.5f, 0}, t = {1.5f, 1.5f, 0}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (!r || !Eq(s, 2.0f, 1.5f, 0) || !Near(e.X, 2.0f) || !Near(e.Y, 1.5f)) fail |= 32; }
    // crossing off the map: segStart = segEnd = from, false
    { V3 f = {9.5f, 5.5f, 0}, t = {11.5f, 5.5f, 0}; bool r = CreateLineSegment(&f, &t, &s, &e);
      if (r || !Eq(s, 9.5f, 5.5f, 0) || !Eq(e, 9.5f, 5.5f, 0)) fail |= 64; }
    if (fail) { std::printf("0063b3f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("0063b3f0_TEST PASS\n");
    return 0;
}
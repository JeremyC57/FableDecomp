// Grade: DIFFER(836v856) 234 of 271 lines (was 252): the direction is three scalar floats (dx and dz stay on the x87 stack across both branches, dy spilled to a dead argument slot, frame 0x30 as retail) and each hit coordinate is t * (d * inv) (retail's fld d ; fmul inv ; fmul t; the d * inv * t form lets VC7.1 fuse inv*t) -- residue: retail re-converts (long)from->Y for yHit.Y (no CSE; ours reuses the compare's value, (int) cast inert), flag slots (crossesX +12h, crossesY +13h, valid +11h; declaration order inert), dy/floorX dead-slot assignment swapped, and retail cross-jumps the *segStart copies
// CWorldMap::CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const -- step from `from` towards `to` to the nearer of the next whole-block X and Y boundaries; the segment is valid when that crossing lies in the map
// Leaf: ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063B3F0, `ret 0x10`, esp frame.
// Callees: _ftol 0x00BFEA70, CWorldMap::IsPosInMap(const C3DVector&) 0x004FC600. 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: only `this` is passed through (to IsPosInMap); no members are read.
#include <math.h>
struct C3DVector
{
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class CWorldMap
{
public:
    virtual ~CWorldMap();
    bool IsPosInMap(const C3DVector& pos) const;                     // 0x004FC600
protected:
    bool CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const;
};

static __forceinline float MaxAbs2D(const C3DVector& a, const C3DVector& b)
{
    float dx = fabsf(a.X - b.X);
    float dy = fabsf(a.Y - b.Y);
    return dx > dy ? dx : dy;
}

bool CWorldMap::CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const
{
    bool crossesX = true;
    bool crossesY = true;
    bool valid = false;
    C3DVector end = *from;
    long fromX = (long)from->X;
    long toX = (long)to->X;
    if (fromX == toX)
        crossesX = false;
    long toY = (long)to->Y;
    if ((long)from->Y == toY)
        crossesY = false;
    float dx = to->X - from->X;
    float dy = to->Y - from->Y;
    float dz = to->Z - from->Z;
    float floorX = (float)fromX;
    C3DVector xHit;
    xHit.X = floorX;
    C3DVector yHit;
    yHit.Y = (float)(long)from->Y;
    if (crossesX) {
        if (dx > 0.0f)
            xHit.X += 1.0f;
        else if (0.0f == from->X - floorX) {
            xHit.X -= 1.0f;
            end.X = xHit.X;
        }
        float t = fabsf(xHit.X - from->X);
        float inv = 1.0f / fabsf(dx);
        xHit.Y = t * (dy * inv) + from->Y;
        xHit.Z = t * (dz * inv) + from->Z;
    }
    if (crossesY) {
        if (dy > 0.0f)
            yHit.Y += 1.0f;
        else if (0.0f == from->Y - (float)(long)from->Y) {
            yHit.Y -= 1.0f;
            end.Y = yHit.Y;
        }
        float t = fabsf(yHit.Y - from->Y);
        float inv = 1.0f / fabsf(dy);
        yHit.X = t * (dx * inv) + from->X;
        yHit.Z = t * (dz * inv) + from->Z;
    }
    if ((long)end.X == toX)
        crossesX = false;
    if ((long)end.Y == toY)
        crossesY = false;
    if (crossesX) {
        if (crossesY) {
            if (MaxAbs2D(xHit, *from) < MaxAbs2D(yHit, *from))
                *segStart = xHit;
            else
                *segStart = yHit;
        } else {
            *segStart = xHit;
        }
    } else if (crossesY) {
        *segStart = yHit;
    } else {
        *segStart = *to;
        end = *to;
        goto done;
    }
    valid = true;
    if (!IsPosInMap(*segStart)) {
        *segStart = *from;
        end = *from;
        valid = false;
    }
done:
    *segEnd = end;
    return valid;
}

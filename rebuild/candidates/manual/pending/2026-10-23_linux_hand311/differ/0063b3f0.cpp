// Grade: DIFFER(836v856) -- frame packing: retail spills dy/floorX into the dead from/to argument slots and keeps fromX at +14h (flags crossesX +12h, crossesY +13h, valid +11h); ours has an 8-byte larger frame and VC7.1 re-associates inv*t (one fmulp) where retail multiplies (inv*d)*t per axis
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
    C3DVector d(to->X - from->X, to->Y - from->Y, to->Z - from->Z);
    float floorX = (float)fromX;
    C3DVector xHit;
    xHit.X = floorX;
    C3DVector yHit;
    yHit.Y = (float)(long)from->Y;
    if (crossesX) {
        if (d.X > 0.0f)
            xHit.X += 1.0f;
        else if (0.0f == from->X - floorX) {
            xHit.X -= 1.0f;
            end.X = xHit.X;
        }
        float t = fabsf(xHit.X - from->X);
        float inv = 1.0f / fabsf(d.X);
        xHit.Y = d.Y * inv * t + from->Y;
        xHit.Z = d.Z * inv * t + from->Z;
    }
    if (crossesY) {
        if (d.Y > 0.0f)
            yHit.Y += 1.0f;
        else if (0.0f == from->Y - (float)(long)from->Y) {
            yHit.Y -= 1.0f;
            end.Y = yHit.Y;
        }
        float t = fabsf(yHit.Y - from->Y);
        float inv = 1.0f / fabsf(d.Y);
        yHit.X = d.X * inv * t + from->X;
        yHit.Z = d.Z * inv * t + from->Z;
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

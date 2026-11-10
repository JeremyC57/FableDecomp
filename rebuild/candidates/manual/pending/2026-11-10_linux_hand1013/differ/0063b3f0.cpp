// CWorldMap::CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const -- step from `from` towards `to` to the nearer of the next whole-block X and Y boundaries; the segment is valid when that crossing lies in the map
// Grade: DIFFER(836v856) -- retail re-converts (long)from->Y for yHit.Y (a second _ftol; ours reuses the compare's value), frame slots (flags +11h/+12h/+13h, xHit +1Ch, yHit +28h, end +34h; dy / floorX in the from / to argument slots) and the cross-jumped *segStart copies. Inert: declaration order of end/xHit/yHit (all six), operand order of the Y compare, a named fromY, braces
// Leaf: ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063B3F0, 856 bytes, thiscall, `ret 10h`.
// Identity: Ego_r.pdb 0x006E7530 ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z (protected const).
// Callees: _ftol 0x00BFEA70, CWorldMap::IsPosInMap(const C3DVector&) 0x004FC600; constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"
#include <math.h>

static FABLE_FORCEINLINE float MaxAbs2D(const C3DVector& a, const C3DVector& b)
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
    C3DVector end;
    end = *from;
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

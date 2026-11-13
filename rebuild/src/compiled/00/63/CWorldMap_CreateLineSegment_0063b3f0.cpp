// CWorldMap::CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const
// Steps from `from` towards `to` to the nearer whole-block X / Y boundary crossing: segStart = that crossing (or *to when the
// segment ends in this cell, or *from when the crossing is off the map), segEnd = the point identifying the traversed cell;
// returns whether a crossing was found that lies in the map (callers loop while it returns true).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063B3F0, 0x358 (856) bytes, protected const __fastcall, `ret 10h`, esp frame, no SEH.
// Callees: __ftol2 0x00BFEA70 (8 `(long)float` truncations), CWorldMap::IsPosInMap(const C3DVector&) const 0x004FC600
// (separate TU, out of line). Constants: 1.0f 0x0122DED8, 0.0f 0x0122DEDC. No members read; `this` only passed on.
// Shape notes: the Y-cell compare reads from->Y through the FableWin.pdb inline C3DVector::ToC2DVector() (the 2D view keeps
// VC7.1 from reusing that __ftol2 result for yHit.Y, retail 0x0063B489 truncates again); xHit.X is `(float)fromX` twice
// (no float local, so `xHit.X += 1` reloads xHit.X); `valid = true` sits at the end of each crossing branch and the
// IsPosInMap block is gated by `valid &&` (gives retail's frame order valid +11 / crossesX +12 / crossesY +13 and the
// block layout with the both->xHit path falling into the IsPosInMap block).
// MaxAbs2D is a made-up name (no PDB / FSE / EgoCore name found) for the twice-inlined max(|dx|,|dy|) helper.
// Leaf: ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"

static inline float MaxAbs2D(const C3DVector& a, const C3DVector& b)
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
    if ((long)from->ToC2DVector().Y == toY)
        crossesY = false;
    float dX = to->X - from->X;
    float dY = to->Y - from->Y;
    float dZ = to->Z - from->Z;
    C3DVector xHit;
    xHit.X = (float)fromX;
    C3DVector yHit;
    yHit.Y = (float)(long)from->Y;
    if (crossesX) {
        if (dX > 0.0f)
            xHit.X += 1.0f;
        else if (0.0f == from->X - (float)fromX) {
            xHit.X -= 1.0f;
            end.X = xHit.X;
        }
        float t = fabsf(xHit.X - from->X);
        float inv = 1.0f / fabsf(dX);
        xHit.Y = (dY * inv) * t + from->Y;
        xHit.Z = (dZ * inv) * t + from->Z;
    }
    if (crossesY) {
        if (dY > 0.0f)
            yHit.Y += 1.0f;
        else if (0.0f == from->Y - (float)(long)from->Y) {
            yHit.Y -= 1.0f;
            end.Y = yHit.Y;
        }
        float t = fabsf(yHit.Y - from->Y);
        float inv = 1.0f / fabsf(dY);
        yHit.X = (dX * inv) * t + from->X;
        yHit.Z = (dZ * inv) * t + from->Z;
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
        valid = true;
    } else if (crossesY) {
        *segStart = yHit;
        valid = true;
    } else {
        *segStart = *to;
        end = *to;
    }
    if (valid && !IsPosInMap(*segStart)) {
        *segStart = *from;
        end = *from;
        valid = false;
    }
    *segEnd = end;
    return valid;
}
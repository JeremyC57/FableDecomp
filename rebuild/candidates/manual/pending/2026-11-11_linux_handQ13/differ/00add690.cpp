// C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count) -- centre the sphere on the points' axis-aligned box and size it to the farthest point
// Grade: DIFFER(ours v retail) -- DIFFER(646v648) measured 2026-11-11 with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used): now on shared engine/C3DVector.h + C3DBoundingSphere.h (overlay FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD); one residue: the 4x-unrolled scan's remainder loop -- retail computes count-j in ebx then copies it into esi (sub ebx,edi; mov esi,ebx @0x00ADD8C0) and counts esi down, VC7.1 here counts ebx down in place. Next: c2prio on the remainder counter, or an unroller lane gap.
// Q13 (4 grades): named n, signed long j (jge), pointer walk (630v648): the unroller remainder counter (`sub ebx,edi ; mov esi,ebx ; dec esi` v ours `dec ebx`) does not move.
// Leaf: ?FastBuildFromPoints@C3DBoundingSphere@@QAEXPBVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD690, 648 bytes, __thiscall, `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?FastBuildFromPoints@C3DBoundingSphere@@QAEXPBVC3DVector@@J@Z.
// Shape: min / max start at the origin and keep the smaller / larger coordinate per axis (`!(min < p)` tests); the centre is
//   the box midpoint; the radius is the square root of the largest squared distance (VC7.1 unrolls that scan 4x).
// Header overlay: FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD (hdr/engine/C3DBoundingSphere.h, the declaration).
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"

void C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count)
{
    C3DVector minimum(0.0f, 0.0f, 0.0f);
    C3DVector maximum(0.0f, 0.0f, 0.0f);
    long i;
    if (count > 0)
    for (i = 0; i != count; ++i) {
        if (!(minimum.X < points[i].X)) minimum.X = points[i].X;
        if (!(minimum.Y < points[i].Y)) minimum.Y = points[i].Y;
        if (!(minimum.Z < points[i].Z)) minimum.Z = points[i].Z;
        if (!(maximum.X > points[i].X)) maximum.X = points[i].X;
        if (!(maximum.Y > points[i].Y)) maximum.Y = points[i].Y;
        if (!(maximum.Z > points[i].Z)) maximum.Z = points[i].Z;
    }
    Center = C3DVector((maximum.X + minimum.X) * 0.5f, (maximum.Y + minimum.Y) * 0.5f, (maximum.Z + minimum.Z) * 0.5f);
    float best = 0.0f;
    for (unsigned long j = 0; j < (unsigned long)count; ++j) {
        float d = (points[j] - Center).GetSquaredMagnitude();
        if (best < d) best = d;
    }
    Radius = (float)sqrt(best);
}

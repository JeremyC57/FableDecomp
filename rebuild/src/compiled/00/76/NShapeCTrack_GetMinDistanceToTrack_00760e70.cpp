// NShape::CTrack::GetMinDistanceToTrack(const C3DVector& pos, float& distance) const -- smallest distance from pos to the track:
// per segment, the perpendicular distance to the segment's line when pos (flattened to z = 0) lies inside the segment's xy box,
// else the nearer end-point distance; false (distance untouched) when the track has fewer than two points.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760E70, length 603 bytes (`ret 8`). FableWin.pdb / Ego_r binding
// ?GetMinDistanceToTrack@CTrack@NShape@@ABE_NABVC3DVector@@AAM@Z (private const); only caller CTrack::IsPosInShape 0x00761740.
// Callees: C3DVector::AccurateNormalise 0x00A14440 (out of line, other TU). Constants: 0x0122DEDC = 0.0f (perp.Z * offset.Z
// term), the initial best 1e10f is an immediate stored in the dead `pos` argument slot.
// Layout: NShape::CShape::Points (STLport std::vector<C3DVector>) at +0x08 via engine/NShape_CClosedShapeBase.h.
// Shape: the segment box is a C2DBoxF (TLX lives in st0, never homed; TLY/BRX/BRY at [esp+44h..4Ch]) built before the copy of
// the current point; `<` -> test ah,5 jp, `>` -> test ah,41h jne. The perpendicular is built from a C2DVector view of the
// normalised direction (??0C2DVector@@QAE@ABVC3DVector@@@Z): C2DVector is byte-packed, so its X/Y reads are not folded into
// x87 memory operands -- retail loads flatdir.X (`fld [esp+38h] ; fmul st(2)`) and leaves offset.X stale until the
// `fstp st(1)` after fabs. The dot runs Y, X, Z (FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ). The outside arm's flat.Z = 0 folds
// away (prev.Z read unsubtracted); min of the two end distances is `d1 < d2 ? d1 : d2`, the best update is strict `<`.
// Leaf: ?GetMinDistanceToTrack@CTrack@NShape@@ABE_NABVC3DVector@@AAM@Z
// Flags: none
#include <math.h>
#pragma intrinsic(sqrt, fabs)
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/NShape_CClosedShapeBase.h"
namespace NShape {
class CTrack : public CShape {
private:
    bool GetMinDistanceToTrack(const C3DVector& pos, float& distance) const;   // 0x00760E70
};
}

bool NShape::CTrack::GetMinDistanceToTrack(const C3DVector& pos, float& distance) const
{
    if (Points.size() > 1) {
        C3DVector flat(pos.X, pos.Y, 0.0f);
        float best = 1e10f;
        std::vector<C3DVector>::const_iterator it = Points.begin();
        C3DVector prev = *it;
        for (++it; it != Points.end(); ++it) {
            C2DBoxF box(prev.X, prev.Y, prev.X, prev.Y);
            C3DVector cur = *it;
            if (cur.X < prev.X)
                box.TLX = cur.X;
            if (cur.Y < prev.Y)
                box.TLY = cur.Y;
            if (cur.X > prev.X)
                box.BRX = cur.X;
            if (cur.Y > prev.Y)
                box.BRY = cur.Y;
            float d;
            if (flat.X >= box.TLX && flat.X < box.BRX && flat.Y >= box.TLY && flat.Y < box.BRY) {
                C3DVector dir(cur.X - prev.X, cur.Y - prev.Y, 0.0f);
                dir.AccurateNormalise();
                C2DVector flatdir(dir);
                C3DVector perp(flatdir.Y, -flatdir.X, 0.0f);
                d = (float)fabs(perp.Dot(pos - prev));
            } else {
                float d1 = (prev - flat).GetMagnitude();
                float d2 = (cur - flat).GetMagnitude();
                d = d1 < d2 ? d1 : d2;
            }
            if (d < best)
                best = d;
            prev = cur;
        }
        distance = best;
        return true;
    }
    return false;
}
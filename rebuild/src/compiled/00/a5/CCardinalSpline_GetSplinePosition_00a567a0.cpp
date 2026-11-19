// CCardinalSpline::GetSplinePosition(C3DVector& dest, float s) const -- the point at parameter s on the cardinal spline through the control points: none -> origin, one -> that point; a closed spline wraps s into [0, 1), an open one clamps to its end points; the segment is the floored s * segment count, its neighbours (wrapped or repeated at the ends) give the two tangents (scaled by the tension), and the Hermite blend runs over the segment's own parameter range
// Leaf: ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z
// Flags: none
// Retail-asm: 0x00A568CB `fsub` + 0x00A568CE `fistp dword ptr [ebp-8]` -- the engine's inline-assembly floor helper GFFloatToLongFloor
//   (engine/GFFloatToLongFloor.h; its half / one constants in the frame at [ebp-14h] / [ebp-10h], 0x00A568B1). This row carries no asm
//   of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A567A0, 766 bytes, `ret 8` (ebp frame).
// Identity: Ego_r.pdb ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z (0x00BAE530) and FableWin.pdb (0x0324B730), same spelling;
//   the shared header's FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION declaration (landed caller 0x0075D580). Locals = Ego_d.pdb (debug
//   length 2422, one scope): num_points, num_points_sub_one, num_control_pts, num_control_pts_f, from_index, to_index, from_pos, to_pos,
//   previous_from_pos, next_to_pos, from_tan, to_tan, from_index_s, to_index_s, this_s.
// Callees: _CIfmod 0x00BFEB28 (fmod intrinsic, double 1.0 0x01237F00), GFGetHermiteSplinePosition 0x00A56670 (__fastcall). Data: 0.0f
//   0x0122DEDC, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (the unsigned index conversions).
// Inline-budget: two header / TU devices, no flag --
//   (1) GFGetHermiteSplinePosition's body is in this TU, ahead of the row: it is the row's TU-mate (0x00A56670 + 295 bytes ends at
//       0x00A56797, the row starts at 0x00A567A0; the landed 0x00A56670 names this row its only caller) and stays a call (0x00A56A90).
//       Text = the landed 0x00A56670 row. With a declaration only, the tangents' addresses go to an opaque callee and from_tan / to_tan
//       never share the dead previous_from_pos / next_to_pos slots (frame 0x64 v 0x58; probes probe/67a0 vE / vN).
//   (2) FABLE_RETAIL_CARRAY_INDEX_ASSERT (the header calls it INFERRED; a no-op ASSERT arm in CArray::operator[] over the const vector
//       indexer): the from_pos / to_pos copies go through the x87 as retail's (`fld [ecx+eax] ; fstp [ebp-4Ch]`, 0x00A568FC); the default
//       indexer (through the mutable vector one) copies with mov (probe vg v vd: 205 -> 128 differing instructions).
// Ego_d line table (C13 lines, carved; debug length 2422): the statements in the text's order -- num_points_sub_one at line 80 (9 B),
//   right after the size < 2 block and before the EndPointsConnected test (83); the open spline's last point at 96 is 37 B, the
//   `num_points - 1` index (through num_points_sub_one it would be 34 B); both neighbour blocks list the wrap / repeat case first
//   (115 / 132 test, then the EndPointsConnected arm, then the plain neighbour); from_index_s 153, to_index_s 154, this_s 155 BEFORE the
//   tangents 157 / 158 (43 B each) and the call 160 -- written in that order (audit; byte-neutral, RELOCATION_MATCH in both orders).
//   num_control_pts_f is a plain float in Ego_d (type 0x40, not const; byte-neutral). Unreproduced: lines 64 (343 B), 70 (339 B),
//   108 (368 B) and 109 (376 B) have no retail counterpart; not written.
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#include <math.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CCardinalSpline.h"
#include "engine/GFFloatToLongFloor.h"
// Fidelity: same-TU strong definition of 0x00A56670 (retail TU-mate)
void FABLE_FASTCALL GFGetHermiteSplinePosition(C3DVector& result, const C3DVector& v1, const C3DVector& t1,
                                           const C3DVector& v2, const C3DVector& t2, float s)
{
    float s2 = s * s;
    float s3 = s * s2;
    float two_s3 = s3 + s3;
    float three_s2 = s2 * 3.0f;
    float h1 = two_s3 - three_s2 + 1.0f;
    float h2 = three_s2 - two_s3;
    float h3 = s3 - 2.0f * s2 + s;
    float h4 = s3 - s2;
    result = v1 * h1 + v2 * h2 + t1 * h3 + t2 * h4;
}

void CCardinalSpline::GetSplinePosition(C3DVector& dest, float s) const
{
    const long num_points = ControlPoints.size();
    if (num_points < 2)
    {
        if (num_points == 0)
            dest = C3DVector(0.0f, 0.0f, 0.0f);
        else if (num_points == 1)
            dest = ControlPoints[0];
        return;
    }
    // Inferred: FITTED -- num_points_sub_one defined here, used for the open spline's segment count but not for its last point
    //   (retail `lea ecx,[edi-1]` before the EndPointsConnected test, 0x00A56822, and `[ebx + num_points*12 - 0Ch]`, 0x00A5688E);
    //   defined before its first use at the next_to_pos test DIFFER(763v766), the last point through it DIFFER(765v766).
    const long num_points_sub_one = num_points - 1;
    long num_control_pts;
    if (EndPointsConnected)
    {
        if (s < 0.0f)
            s = 1.0f - fmod(s, 1.0f);
        else
            s = fmod(s, 1.0f);
        num_control_pts = num_points;
    }
    else
    {
        if (s <= 0.0f)
        {
            dest = ControlPoints[0];
            return;
        }
        if (s >= 1.0f)
        {
            dest = ControlPoints[num_points - 1];
            return;
        }
        num_control_pts = num_points_sub_one;
    }
    float num_control_pts_f = (float)num_control_pts;
    const unsigned long from_index = GFFloatToLongFloor(num_control_pts_f * s) % num_points;
    unsigned long to_index = (from_index + 1) % num_points;
    C3DVector from_pos = ControlPoints[from_index];
    C3DVector to_pos = ControlPoints[to_index];
    C3DVector previous_from_pos;
    // Inferred: FITTED -- both neighbour tests written for the wrap / repeat case first (retail jumps to the plain neighbour copy, `ja`
    //   0x00A5692F / `jb` 0x00A5698B, and falls into the EndPointsConnected test); `!(from_index > 0)` / `!(to_index < ..)` match too;
    //   `if (from_index > 0) .. else if` DIFFER(766v766), `if (to_index < ..) .. else if` DIFFER(783v766), `from_index == 0` DIFFER(764v766).
    //   The wrapped previous point is indexed `from_index + num_points_sub_one` (`lea ecx,[ebx+edi-1]` 0x00A5693A); `from_index +
    //   num_points - 1` DIFFER(766v766).
    if (from_index <= 0)
    {
        if (EndPointsConnected)
            previous_from_pos = ControlPoints[from_index + num_points_sub_one];
        else
            previous_from_pos = from_pos;
    }
    else
        previous_from_pos = ControlPoints[from_index - 1];
    C3DVector next_to_pos;
    if (to_index >= num_points_sub_one)
    {
        if (EndPointsConnected)
            next_to_pos = ControlPoints[to_index - num_points_sub_one];
        else
            next_to_pos = to_pos;
    }
    else
        next_to_pos = ControlPoints[to_index + 1];
    if (EndPointsConnected && from_index > to_index)
        to_index += num_points;
    float from_index_s = from_index * (1.0f / num_control_pts_f);
    float to_index_s = to_index * (1.0f / num_control_pts_f);
    float this_s = (s - from_index_s) / (to_index_s - from_index_s);
    C3DVector from_tan = (to_pos - previous_from_pos) * Tension;
    C3DVector to_tan = (next_to_pos - from_pos) * Tension;
    GFGetHermiteSplinePosition(dest, from_pos, from_tan, to_pos, to_tan, this_s);
}
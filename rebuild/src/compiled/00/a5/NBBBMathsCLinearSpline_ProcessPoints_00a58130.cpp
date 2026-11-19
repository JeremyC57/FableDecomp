// NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations) -- smooth the control polyline: copy ControlPoints to ProcessedPoints and, smooth_iterations times (when there are at least three points), replace every inner corner by two points cut a quarter of the way along its two edges after rotating each edge a quarter of its angle to the bounding line about the corner's normal
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, no flags). The parked Codex draft (its own per-feature context headers, local
//   _M_insert_overflow under auto_inline(off), /Gr; one scheduling group off) is ported onto the shared headers: engine/
//   NBBBMaths_CLinearSpline.h, stlport/fable_containers.h and engine/C3DVector.h. Locals and scopes are Ego_d.pdb's (smooth_counter,
//   num_pts; new_points, p_first, c1, c3; p_second, p_third, bounding_line, line0, line1, bl_mag, l0_mag, l1_mag, cos_angle_limit0/1,
//   perp, ANGLE_FRACTION, LENGTH_FRACTION, angle_limit0/1, point_12, point_23; c2). new_points is the class's nested
//   TVectorCntr (FableWin.pdb LF_NESTTYPE = vector<C3DVector>), spelled as that vector here: the shared header has no typedef yet.
// Inferred: FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP and FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP are fitted tie-breakers. Both are
//   existing grouping texts with no arithmetic change. Together they move VC7.1's schedule so the first push_back's capacity load /
//   compare (`mov eax,[esp+5Ch] ; cmp esi,eax`, 0x00A584C2) goes before point_12.Z's fld/fadd/fstp. Results: neither 1272v1272 with
//   4 lines differing; ADD alone 2; CROSS alone 4; both RELOCATION_MATCH.
// Inferred: the arc cosines are written `(float)acos(c) * 0.15915493667125702f` (turns), interleaved with the Rotate calls as retail
//   does (0x00A583FF..0x00A5844E). FableWin.pdb's revision computes both first and calls GFArcCos (engine/GFAngles.h, the same body).
//   That spelling grades DIFFER(1272v1272) on the same capacity-load schedule in all 12 grouping combinations tried. It is the
//   inline expansion of the same text either way (`fld ; call _CIacos ; fmul 0.15915494f`).
// Leaf: ?ProcessPoints@CLinearSpline@NBBBMaths@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A58130, 1272 bytes, thiscall `ret 4`. Name: Ego_r.pdb / FableWin.pdb
//   ?ProcessPoints@CLinearSpline@NBBBMaths@@QAEXJ@Z (FableWin 0x0324E0C0); Ego_r donor order GetLength / GetPointWithParam /
//   AddControlPoint / ProcessPoints = retail 0x00A57EF0 / 0x00A57F70 / 0x00A580F0 / this. The catalog label
//   _global::CPSCSplineBase_GenerateCurvePoints is wrong (NAME_FIXES.tsv).
// Callees: vector<C3DVector>::operator= 0x0075B8D0, _CIacos 0x00BFEBA2 (x2), C3DVector::Rotate(const C3DVector&, float) 0x00A13C60 (x2),
//   vector<C3DVector>::_M_insert_overflow 0x0066E370 (x2), vector<C3DVector>::insert 0x007653B0 (x2), operator delete 0x00BFEA14.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt, acos)
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/NBBBMaths_CLinearSpline.h"

void NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations)
{
    ProcessedPoints = ControlPoints;
    long num_pts = ProcessedPoints.size();
    if (num_pts < 3)
        return;
    for (long smooth_counter = 0; smooth_counter != smooth_iterations; ++smooth_counter) {
        std::vector<C3DVector> new_points;
        num_pts = ProcessedPoints.size();
        C3DVector p_first = ProcessedPoints[0];
        for (long c1 = 1; c1 != num_pts - 1; ++c1) {
            C3DVector p_second = ProcessedPoints[c1];
            C3DVector p_third = ProcessedPoints[c1 + 1];
            C3DVector bounding_line = p_third - p_first;
            C3DVector line0 = p_first - p_second;
            C3DVector line1 = p_third - p_second;
            float bl_mag = bounding_line.GetMagnitude();
            float l0_mag = line0.GetMagnitude();
            float l1_mag = line1.GetMagnitude();
            float cos_angle_limit0 = -1.0f * line0.Dot(bounding_line) / (l0_mag * bl_mag);
            float cos_angle_limit1 = line1.Dot(bounding_line) / (l1_mag * bl_mag);
            if (cos_angle_limit0 < -1.0f) cos_angle_limit0 = -1.0f; else if (cos_angle_limit0 > 1.0f) cos_angle_limit0 = 1.0f;
            if (cos_angle_limit1 < -1.0f) cos_angle_limit1 = -1.0f; else if (cos_angle_limit1 > 1.0f) cos_angle_limit1 = 1.0f;
            C3DVector perp = line0.Cross(line1) / (l0_mag * l1_mag);
            const float ANGLE_FRACTION = 0.25f;
            const float LENGTH_FRACTION = 0.25f;
            float angle_limit0 = (float)acos(cos_angle_limit0) * 0.15915493667125702f;
            line0.Rotate(perp, angle_limit0 * ANGLE_FRACTION);
            float angle_limit1 = (float)acos(cos_angle_limit1) * 0.15915493667125702f;
            line1.Rotate(perp, -angle_limit1 * ANGLE_FRACTION);
            line0 *= LENGTH_FRACTION;
            line1 *= LENGTH_FRACTION;
            C3DVector point_12 = line0 + p_second;
            C3DVector point_23 = line1 + p_second;
            new_points.push_back(point_12);
            new_points.push_back(point_23);
            p_first = p_second;
        }
        long c3 = 1;
        for (long c2 = 0; c2 != (long)new_points.size();) {
            ProcessedPoints.insert(ProcessedPoints.begin() + c3, new_points[c2]);
            ++c2;
            c3 += 2;
            ProcessedPoints.insert(ProcessedPoints.begin() + c3, new_points[c2]);
            ++c2;
            ++c3;
        }
    }
}
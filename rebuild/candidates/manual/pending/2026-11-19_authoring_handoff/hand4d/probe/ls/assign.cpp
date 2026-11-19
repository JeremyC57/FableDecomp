// NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations) -- smooth the control polyline: copy ControlPoints to ProcessedPoints and, smooth_iterations times (when there are at least three points), replace every inner corner by two points cut a quarter of the way along its two edges after rotating each edge a quarter of its angle to the bounding line about the corner's normal
// Grade: (in progress)
// Leaf: ?ProcessPoints@CLinearSpline@NBBBMaths@@QAEXJ@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
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

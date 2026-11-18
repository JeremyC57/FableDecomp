// C2DLineF::GetPointOnLine -- Project a point onto a 2D line and report the clamped result.
// Grade: DIFFER(652v654) -- From 00A57210 the optional output tests and temporary x87 values differ. The public const leaf and optional output pointee types agree with both donors.
// Leaf: ?GetPointOnLine@C2DLineF@@QBEMABVC2DVector@@AAV2@MPAV2@PA_N@Z
// Retail: 0x00A571A0, 654 bytes, thiscall.
#define FABLE_RETAIL_C2DLINEF_POINT_ON_LINE_PASS24
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DLineF.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif

float C2DLineF::GetPointOnLine(const C2DVector& point, C2DVector& point_on_line, float radius, C2DVector* point_on_infinite_line, bool* off_start) const
{
    C2DVector along_line = Point2 - Point1;
    float lengthSquared = along_line.X * along_line.X;
    lengthSquared += along_line.Y * along_line.Y;
    float line_length = (float)sqrt(lengthSquared);
    if (line_length == 0.0f) {
        point_on_line = Point1;
        if (point_on_infinite_line)
            *point_on_infinite_line = Point1;
        if (off_start)
            *off_start = false;
        return 0.0f;
    }
    C2DVector u = along_line * (1.0f / line_length);
    float dist = (point - Point1).Dot(u);
    if (off_start)
        *off_start = (dist < 0.0f) ? true : false;
    if (radius * 2.0f > line_length) {
        point_on_line = (Point2 + Point1) * 0.5f;
        if (point_on_infinite_line)
            *point_on_infinite_line = GetPointOnInfiniteLine(point);
        return radius - line_length;
    }
    if (dist < radius) {
        point_on_line = Point1 + u * radius;
        float dist_off_line = radius - dist;
        if (point_on_infinite_line)
            *point_on_infinite_line = Point1 + u * dist;
        return dist_off_line;
    }
    if (line_length - radius < dist) {
        point_on_line = Point2 - u * radius;
        float dist_off_line = dist - (line_length + radius);
        if (point_on_infinite_line)
            *point_on_infinite_line = Point1 + u * dist;
        return dist_off_line;
    }
    point_on_line = Point1 + u * dist;
    if (point_on_infinite_line)
        *point_on_infinite_line = point_on_line;
    return 0.0f;
}

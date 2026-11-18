#include <stdio.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#endif
// 00a571a0 C2DLineF::GetPointOnLine
// Leaf: ?GetPointOnLine@C2DLineF@@QBEMABVC2DVector@@AAV2@MPAV2@PA_N@Z
// Flags: none
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

C2DVector C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const
{
    const C2DVector direction = Point2 - Point1;
    return Point1 + direction * ((point - Point1).Dot(direction) / direction.Dot(direction));
}

int main()
{
    C2DLineF line;
    const float projection[] = {-2,0,1,4,7,8,10};
    const float expected_distance[] = {3,1,0,0,0,-1,1};
    for (int orientation = 0; orientation < 2; ++orientation) {
        line.Point1 = C2DVector(2,-4);
        line.Point2 = orientation ? C2DVector(2,4) : C2DVector(10,-4);
        const C2DVector tangent(orientation ? 0.0f : 1.0f, orientation ? 1.0f : 0.0f);
        const C2DVector side(-tangent.Y, tangent.X);
        for (unsigned int i = 0; i < sizeof(projection)/sizeof(projection[0]); ++i) {
            const C2DVector foot = line.Point1 + tangent * projection[i];
            const C2DVector point = foot + side * 2.0f;
            const float along = projection[i] < 1 ? 1 : (projection[i] > 7 ? 7 : projection[i]);
            const C2DVector expected = line.Point1 + tangent * along;
            for (int flags = 0; flags < 4; ++flags) {
                C2DVector output(19,19), infinite(42,42);
                bool off = projection[i] >= 0;
                float d = line.GetPointOnLine(point,output,1.0f,flags & 1 ? &infinite : 0, flags & 2 ? &off : 0);
                if (d != expected_distance[i] || output.X != expected.X || output.Y != expected.Y) return 1;
                if (flags & 1 ? (infinite.X != foot.X || infinite.Y != foot.Y) : (infinite.X != 42 || infinite.Y != 42)) return 2;
                if ((flags & 2) && off != (projection[i] < 0)) return 3;
                d = line.GetPointOnLine(point,output,5.0f,flags & 1 ? &infinite : 0, flags & 2 ? &off : 0);
                const C2DVector middle = line.Point1 + tangent * 4;
                if (d != -3 || output.X != middle.X || output.Y != middle.Y) return 4;
                if ((flags & 1) && (infinite.X != foot.X || infinite.Y != foot.Y)) return 5;
            }
        }
    }
    line.Point2 = line.Point1;
    for (int flags = 0; flags < 4; ++flags) {
        C2DVector output(7,3), infinite(7,3);
        bool off = true;
        if (line.GetPointOnLine(C2DVector(1,2),output,0,flags & 1 ? &infinite : 0,flags & 2 ? &off : 0) != 0 ||
            output.X != line.Point1.X || output.Y != line.Point1.Y ||
            ((flags & 1) && (infinite.X != output.X || infinite.Y != output.Y)) || ((flags & 2) && off)) return 6;
    }
    puts("00a571a0_TEST PASS");
    return 0;
}

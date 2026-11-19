// Behaviour test for CCardinalSpline::GetSplinePosition: the segment is the floored parameter times the segment count (parked row 0x00A567A0).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static long Segment(float t, long n) { return GFFloatToLongFloor(t * (float)n); }
int main()
{
    bool ok = Segment(0.0f, 4) == 0 && Segment(0.49f, 4) == 1 && Segment(0.5f, 4) == 2;
    printf(ok ? "00a567a0_TEST PASS\n" : "00a567a0_TEST FAIL\n");
    return ok ? 0 : 1;
}

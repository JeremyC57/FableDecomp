// Behaviour test for CEngineWeatherRenderer::BuildBoxPositionMatrix: the camera position is snapped to the box grid with GFFloatToLongFloor (parked row 0x00B51AD0).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static long Snap(float pos, long size) { return GFFloatToLongFloor(pos / (float)size) * size; }
int main()
{
    bool ok = Snap(10.0f, 4) == 8 && Snap(-1.0f, 4) == -4 && Snap(8.0f, 4) == 8;
    printf(ok ? "00b51ad0_TEST PASS\n" : "00b51ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}

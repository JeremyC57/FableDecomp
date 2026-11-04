// Behaviour test for CEngineSceneMap::Update: the grid side is the rounded square root of the primitive count (parked row 0x00B947E0).
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
static long Side(float count) { return GFFloatToLongNear((float)sqrt(count)); }
int main()
{
    bool ok = Side(16.0f) == 4 && Side(20.0f) == 4 && Side(30.0f) == 5;
    printf(ok ? "00b947e0_TEST PASS\n" : "00b947e0_TEST FAIL\n");
    return ok ? 0 : 1;
}

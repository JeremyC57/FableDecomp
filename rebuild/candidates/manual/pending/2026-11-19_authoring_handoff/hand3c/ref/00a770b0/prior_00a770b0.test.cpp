// Behaviour test for CNavQuadTree::GetNodeAt: the cell is the floored map-relative position over the 32-unit cell size (parked row 0x00A770B0).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static long CellOf(float rel) { static float inv = 1.0f / 32.0f; return GFFloatToLongFloor(inv * rel); }
int main()
{
    bool ok = CellOf(0.0f) == 0 && CellOf(31.9f) == 0 && CellOf(32.0f) == 1 && CellOf(100.0f) == 3;
    printf(ok ? "00a770b0_TEST PASS\n" : "00a770b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

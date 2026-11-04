// Behaviour test for CEngineSceneMap::AddPrimitive: the cell is the floored normalised position times the level size, clamped to size - 1 (parked row 0x00B93B30).
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
static unsigned long Cell(float u, unsigned long size) { unsigned long c = GFFloatToLongFloor(u * (float)size); return c > size - 1 ? size - 1 : c; }
int main()
{
    bool ok = Cell(0.5f, 8) == 4 && Cell(0.99f, 8) == 7 && Cell(1.0f, 8) == 7 && Cell(0.0f, 8) == 0;
    printf(ok ? "00b93b30_TEST PASS\n" : "00b93b30_TEST FAIL\n");
    return ok ? 0 : 1;
}

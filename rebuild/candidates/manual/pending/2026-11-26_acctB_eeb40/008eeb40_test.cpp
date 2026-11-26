// Mass::ForceToVel: force * IMass plus the perpendicular lever arm times its dot with the force times AIMass.
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_DOT_GROUP
#include "engine/Mass.h"
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    unsigned char mem[sizeof(Mass)];
    memset(mem, 0, sizeof mem);
    Mass* mass = (Mass*)mem;
    mass->Pos.X = 1.0f;
    mass->Pos.Y = 2.0f;
    mass->IMass = 0.5f;
    mass->AIMass = 0.25f;
    C2DVector pos(4.0f, 6.0f);
    C2DVector force(2.0f, 0.0f);
    C2DVector result = mass->ForceToVel(pos, force);
    // Right(pos - Pos) = Right(3, 4) = (4, -3); dot force = 8; * AIMass = 2; * Right = (8, -6)
    // force * IMass = (1, 0); result = (9, -6)
    check(result.X == 9.0f && result.Y == -6.0f, "lever arm");
    mass->Pos.X = 0.0f;
    mass->Pos.Y = 0.0f;
    mass->IMass = 2.0f;
    mass->AIMass = 0.0f;
    C2DVector origin(0.0f, 0.0f);
    C2DVector push(3.0f, -1.0f);
    result = mass->ForceToVel(origin, push);
    check(result.X == 6.0f && result.Y == -2.0f, "no spin");
    if (!g_fail) printf("008eeb40_TEST PASS\n");
    return g_fail;
}

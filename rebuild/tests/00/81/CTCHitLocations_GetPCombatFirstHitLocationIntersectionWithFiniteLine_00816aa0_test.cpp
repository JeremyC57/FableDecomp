// Behaviour test for 0x00816AA0 (links the REAL row object; NF123 q4687: the old test re-implemented the FableThunk stand-in): the const
// overload must forward this and all five arguments to the non-const overload (0x008168B0, defined here as a recorder) and return its result.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCHitLocations.h"
#include <stdio.h>
static const void* g_self; static const void* g_args[4]; static float g_radius;
NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& a, const C3DVector& b,
    C3DVector& c, C3DVector& d, float r)
{
    g_self = this; g_args[0] = &a; g_args[1] = &b; g_args[2] = &c; g_args[3] = &d; g_radius = r;
    return (NHitLocations::CHitLocation*)0x1234;
}
int main()
{
    unsigned long object[16];
    unsigned long v[4][3];
    const CTCHitLocations* self = (const CTCHitLocations*)object;
    const NHitLocations::CHitLocation* r = self->GetPCombatFirstHitLocationIntersectionWithFiniteLine(*(const C3DVector*)v[0],
        *(const C3DVector*)v[1], *(C3DVector*)v[2], *(C3DVector*)v[3], 2.5f);
    if (r != (const NHitLocations::CHitLocation*)0x1234 || g_self != object || g_args[0] != v[0] || g_args[1] != v[1] || g_args[2] != v[2]
        || g_args[3] != v[3] || g_radius != 2.5f) { printf("B1\n"); return 1; }
    printf("TH_816aa0_OK\n");
    return 0;
}

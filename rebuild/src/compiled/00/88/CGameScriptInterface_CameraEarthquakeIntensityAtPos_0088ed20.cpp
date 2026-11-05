// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

extern const float CGSI_Rumble_Zero;
extern const float CGSI_Rumble_One;
extern const float CGSI_Rumble_Hundred;

extern void __fastcall CGSI_Rumble_Create(
    const C3DVector* position,
    unsigned char localOnly,
    float intensity,
    float radius);

void __fastcall CGameScriptInterface_CameraEarthquakeIntensityAtPos(
    const void*,
    unsigned long,
    const C3DVector& position,
    float intensity,
    float radius)
{
    if (intensity < CGSI_Rumble_Zero)
    {
        intensity = 0.0f;
    }
    else
    {
        if (intensity > CGSI_Rumble_One)
            intensity = 1.0f;
    }

    if (radius < CGSI_Rumble_Zero)
    {
        radius = 0.0f;
    }
    else
    {
        if (radius > CGSI_Rumble_Hundred)
            radius = 100.0f;
    }

    CGSI_Rumble_Create(&position, 0, intensity, radius);
}
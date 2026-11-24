// Leaf: ?CameraEarthquakeIntensityAtPos@CGameScriptInterface@@UBEXABVC3DVector@@MM@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/C3DVector.h"

extern const float CGSI_Rumble_Hundred;

extern void __fastcall CGSI_Rumble_Create(
    const C3DVector* position,
    unsigned char localOnly,
    float intensity,
    float radius);

void CGameScriptInterface::CameraEarthquakeIntensityAtPos(const class C3DVector& position, float intensity, float radius) const
{
    if (intensity < 0.0f)
    {
        intensity = 0.0f;
    }
    else
    {
        if (intensity > 1.0f)
            intensity = 1.0f;
    }

    if (radius < 0.0f)
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
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_CameraEarthquakeIntensityAtPos -> CGameScriptInterface::CameraEarthquakeIntensityAtPos, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.

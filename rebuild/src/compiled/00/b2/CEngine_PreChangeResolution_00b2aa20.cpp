// Leaf: ?PreChangeResolution@CEngine@@UAE_NXZ
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#include "engine/CEngine.h"
bool CEngine::PreChangeResolution()
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CEngine_PreChangeResolution -> CEngine::PreChangeResolution, the RTTI-placed PDB member; class model: engine/CEngine.h under FABLE_RETAIL_CENGINE, FABLE_RETAIL_CENGINE_OVERRIDES (preamble of CEngine_SetEnvironmentalEffectsAlwaysUpdate_00b23b90.cpp).

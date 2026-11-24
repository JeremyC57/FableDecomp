// Leaf: ?PeekSceneFilterFlags@CEngineCamera@@UBEKXZ
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#include "engine/CEngineCamera.h"
unsigned long CEngineCamera::PeekSceneFilterFlags() const
{
    return 1;
}
// Flatten route (flatten_pilot v4, NF324): CEngineCamera_PeekSceneFilterFlags -> CEngineCamera::PeekSceneFilterFlags, the RTTI-placed PDB member; class model: engine/CEngineCamera.h under FABLE_RETAIL_CENGINECAMERA_POSITION (preamble of CEngineCamera_SetGameWindow_00b2fc00.cpp).

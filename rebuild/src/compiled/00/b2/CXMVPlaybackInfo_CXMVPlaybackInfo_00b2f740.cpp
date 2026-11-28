// Flags: 
// Leaf: ?StartNewScene@CEnginePrimitiveOcclusionManager@@QAEXXZ
#define FABLE_RETAIL_ENGINE_LOADSCENE_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CEnginePrimitiveOcclusionManager.h"

void CEnginePrimitiveOcclusionManager::StartNewScene()
{
    NewSceneFlag = true;
}

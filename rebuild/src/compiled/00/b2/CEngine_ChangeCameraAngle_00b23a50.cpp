// Flags: 
// Leaf: ?ChangeCameraAngle@CEngine@@UAEXXZ
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#define FABLE_RETAIL_ENGINE_LOADSCENE_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#include "engine/CEngine.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/NEngineInternal.h"

void CEngine::ChangeCameraAngle()
{
    NEngineInternal::GPPrimitiveOcclusionManager->StartNewScene();
    CameraAngleChanged = true;
}

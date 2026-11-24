// Leaf: ?GetAnimationMemoryPool@CIEngine@@UAEPAVCMovableResourceMemoryPool@@XZ
// Switch route (flatten pilot --switch): the class model is engine/CIEngine.h under FABLE_RETAIL_CIENGINE; preamble copied from the landed precedent CIEngine_AddCustomMorphTexture_00b23460.cpp.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool()
{
    CIEngine* engine = this;
    return 0;
}

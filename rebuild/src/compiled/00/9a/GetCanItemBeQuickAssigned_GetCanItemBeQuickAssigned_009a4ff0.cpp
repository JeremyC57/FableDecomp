// Flags: 
// Leaf: ?IsMouseButton3Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton3Pressed()
{
    return MouseButton3Pressed;
}

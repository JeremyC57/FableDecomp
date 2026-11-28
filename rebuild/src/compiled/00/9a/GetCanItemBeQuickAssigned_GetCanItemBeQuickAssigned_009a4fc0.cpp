// Flags: 
// Leaf: ?IsMouseButton0Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton0Pressed()
{
    return MouseButton0Pressed;
}

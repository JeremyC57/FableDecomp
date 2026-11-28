// Flags: 
// Leaf: ?IsMouseButton4Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton4Pressed()
{
    return MouseButton4Pressed;
}

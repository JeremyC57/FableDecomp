// Flags: 
// Leaf: ?IsMouseButton2Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton2Pressed()
{
    return MouseButton2Pressed;
}

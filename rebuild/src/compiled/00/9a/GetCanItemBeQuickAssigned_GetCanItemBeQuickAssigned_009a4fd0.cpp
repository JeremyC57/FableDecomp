// Flags: 
// Leaf: ?IsMouseButton1Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton1Pressed()
{
    return MouseButton1Pressed;
}

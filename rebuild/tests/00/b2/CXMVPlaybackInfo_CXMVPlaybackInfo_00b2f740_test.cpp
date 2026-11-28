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

#include <cstdio>
#include <cstring>
#include <cassert>
int main() {unsigned long memory[14];unsigned char* b=(unsigned char*)memory;for(int init=0;init<256;++init){std::memset(b,init,56);((CEnginePrimitiveOcclusionManager*)b)->StartNewScene();for(int j=0;j<56;++j)assert(b[j]==(j==0x35?1:init));}std::puts("00b2f740_TEST PASS");return 0;}
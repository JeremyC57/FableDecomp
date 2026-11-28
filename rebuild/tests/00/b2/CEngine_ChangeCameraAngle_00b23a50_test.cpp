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

#include <cstdio>
#include <cstring>
#include <cassert>
namespace NEngineInternal {CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager;}
static void* seen;void CEnginePrimitiveOcclusionManager::StartNewScene(){seen=this;}
int main(){unsigned long storage[sizeof(CEngine)/4],manager[14];NEngineInternal::GPPrimitiveOcclusionManager=(CEnginePrimitiveOcclusionManager*)manager;unsigned char* p=(unsigned char*)storage;for(int i=0;i<256;++i){std::memset(p,i,sizeof(storage));seen=0;((CEngine*)p)->CEngine::ChangeCameraAngle();assert(seen==manager);for(unsigned j=0;j<sizeof(storage);++j)assert(p[j]==(j==0x39?1:i));}std::puts("00b23a50_TEST PASS");return 0;}
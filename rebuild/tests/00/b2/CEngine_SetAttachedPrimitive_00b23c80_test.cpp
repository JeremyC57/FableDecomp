// Leaf: ?SetAttachedPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#define FABLE_RETAIL_ENGINE_ATTACHED_PRIMITIVE_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER
#include "engine/CEngine.h"
#include "engine/CEnginePrimitiveAttachmentManager.h"
#include "engine/NEngineInternal.h"

void CEngine::SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags)
{
    NEngineInternal::GPPrimitiveAttachmentManager->SetAttachedPrimitive(handle, primitive, attachmentInfo, parentHandle, attachIndex, flags);
}

#include <cstdio>
#include <cstring>
#include <cassert>

namespace NEngineInternal {CEnginePrimitiveAttachmentManager* GPPrimitiveAttachmentManager;}
static const void* captured[5];static long lastIndex;static unsigned long lastFlags;
void CEnginePrimitiveAttachmentManager::SetAttachedPrimitive(CEnginePrimitiveHandle& a,const CEnginePrimitive& b,const CEnginePrimitiveAttachmentInfo& c,const CEnginePrimitiveHandle& e,long i,unsigned long f){captured[0]=this;captured[1]=&a;captured[2]=&b;captured[3]=&c;captured[4]=&e;lastIndex=i;lastFlags=f;}
int main(){unsigned long buf[256];NEngineInternal::GPPrimitiveAttachmentManager=(CEnginePrimitiveAttachmentManager*)(buf+8);for(int i=-8;i<9;++i){((CEngine*)buf)->CEngine::SetAttachedPrimitive(*(CEnginePrimitiveHandle*)(buf+1),*(CEnginePrimitive*)(buf+2),*(CEnginePrimitiveAttachmentInfo*)(buf+3),*(CEnginePrimitiveHandle*)(buf+4),i,(unsigned long)~i);for(int j=0;j<5;++j)assert(captured[j]==buf+(j?j:8));assert(lastIndex==i && lastFlags==(unsigned long)~i);}std::puts("00b23c80_TEST PASS");return 0;}

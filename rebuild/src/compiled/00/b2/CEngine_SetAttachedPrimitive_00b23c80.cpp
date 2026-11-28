// Flags: 
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

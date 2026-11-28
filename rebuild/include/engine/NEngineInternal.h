#ifndef FABLE_ENGINE_NENGINEINTERNAL_H
#define FABLE_ENGINE_NENGINEINTERNAL_H

class CEngineCamera;
class CEnginePrimitiveAttachmentManager;
class CEnginePrimitiveOcclusionManager;
class CEngineResourceManager;

namespace NEngineInternal {
extern CEngineCamera* GPCamera;
extern CEnginePrimitiveAttachmentManager* GPPrimitiveAttachmentManager;
extern CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager;
extern CEngineResourceManager* GPResourceManager;
}

#endif

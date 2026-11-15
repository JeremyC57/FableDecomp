#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERTEXT_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERTEXT_H
// Original complete class, both PDBs:44bytes, shaders at0c/14/1c/24.
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
class CEnginePrimitiveManagerText:public CEnginePrimitiveManager {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
private:
    CVertexShader VShader,VShaderMask;
    CPixelShader PShader,PShaderMask;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerText)==44);
#endif

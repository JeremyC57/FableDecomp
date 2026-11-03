#ifndef FABLE_ENGINE_CSHADERRESOURCE_H
#define FABLE_ENGINE_CSHADERRESOURCE_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
// Original Ego_r TPI0xa158; two array shrinkages in VertexShaderInput plus Shaders give native0x3c.
// NativeA5D4D0 writes manager+14, A5D4E0 declaration+34, A5D4F0 format+38.
class CShaderDataBank;class CShaderBase;class CShaderRenderManager;struct _D3DVERTEXELEMENT9;
class CShaderResource : public CBaseClassNonCopyable {
    friend class CShaderRenderManager;
public:
    virtual ~CShaderResource();
private:
    void Initialise(CShaderRenderManager&);
    const CShaderDataBank* ShaderBank;
    CArray<CShaderBase*> Shaders;
    CShaderRenderManager* ShaderRenderManager;
    unsigned char VertexShaderInput[0x1c]; // +18 CVertexShaderInput, CBaseClass plus two12-byte arrays.
    const _D3DVERTEXELEMENT9* VertexShaderDeclaration;
    unsigned char VertexFormat[4]; // +38 original EVertexType.
};
FABLE_STATIC_ASSERT(sizeof(CShaderResource)==0x3c);
#endif

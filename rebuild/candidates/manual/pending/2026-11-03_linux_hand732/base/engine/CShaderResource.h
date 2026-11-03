#ifndef FABLE_ENGINE_CSHADERRESOURCE_H
#define FABLE_ENGINE_CSHADERRESOURCE_H
// Hand-written (chunk 723): CShaderResource, the named-shader factory the engine resource manager owns one of per vertex shader bank
// (CEngineResourceManager::VShaderResources[27]) plus one for pixel shaders (ShaderResourcePixel). Polymorphic (??1 UAE).
// Functions: Ego_r.pdb / FableWin.pdb publics; GetVertexShader / GetPixelShader return the shader handle by value (hidden result
// pointer), retail 0x00A5D5F0 / 0x00A5D720 (callers 0x00BC14E0, 0x00B8B630). Members: Ego_r.pdb names; only the vptr and ShaderBank
// are at proven retail offsets, the rest stay opaque (retail CArray is 0xC where Ego_r's is 0x10).
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
class CShaderDataBank;
class CShaderRenderManager;
class CVertexShaderInput;
struct _D3DVERTEXELEMENT9;
enum EVertexType;                                                        // MS forward enum
class CShaderResource {
public:
    virtual ~CShaderResource();                                          // ??1CShaderResource@@UAE@XZ
    CVertexShader GetVertexShader(const CCharString& name) const;        // 0x00A5D5F0 ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z
    CVertexShader GetVertexShader(unsigned long index) const;            // ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@K@Z
    CPixelShader GetPixelShader(const CCharString& name) const;          // 0x00A5D720 ?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@ABVCCharString@@@Z
    CPixelShader GetPixelShader(unsigned long index) const;              // ?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@K@Z
    void OpenShaderBank(const CShaderDataBank* bank);                    // ?OpenShaderBank@CShaderResource@@QAEXPBVCShaderDataBank@@@Z
    void CloseShaderBank();                                              // ?CloseShaderBank@CShaderResource@@QAEXXZ
    const CShaderDataBank* PeekShaderBank() const;                       // ?PeekShaderBank@CShaderResource@@QBEPBVCShaderDataBank@@XZ
    void SetVertexFormat(EVertexType type);                              // ?SetVertexFormat@CShaderResource@@QAEXW4EVertexType@@@Z
    void SetVertexShaderDeclaration(const _D3DVERTEXELEMENT9* decl);     // ?SetVertexShaderDeclaration@CShaderResource@@QAEXPBU_D3DVERTEXELEMENT9@@@Z
    void SetVertexShaderInput(const CVertexShaderInput& input);          // ?SetVertexShaderInput@CShaderResource@@QAEXABVCVertexShaderInput@@@Z
    void Initialise(CShaderRenderManager& manager);                      // 0x00A5D4D0 ?Initialise@CShaderResource@@QAEXAAVCShaderRenderManager@@@Z (CShaderRenderManager::Initialise 0x0098ACF0)
private:
    CShaderResource();                                                   // ??0CShaderResource@@AAE@XZ
public:
    const CShaderDataBank* ShaderBank;                                   // +0x04
};

#endif // FABLE_ENGINE_CSHADERRESOURCE_H

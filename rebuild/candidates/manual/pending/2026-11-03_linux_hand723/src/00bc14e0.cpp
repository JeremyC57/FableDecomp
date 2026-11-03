// CEnginePrimitiveManagerParticleSpriteGroup::CEnginePrimitiveManagerParticleSpriteGroup(CArray<CEngineScene*>&) -- construct the particle sprite-group manager: fetch the point-light vertex shaders (5 per sprite style), the three displacement vertex shaders and the three pixel shaders by name, then build the vertex/index buffers
// Leaf: ??0CEnginePrimitiveManagerParticleSpriteGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py with FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC14E0, length 2970 bytes (+ int3 padding), __thiscall, `ret 4`, returns `this`.
// Identity: Ego_r.pdb / FableWin.pdb ??0CEnginePrimitiveManagerParticleSpriteGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z; it
//   stores the class vtable 0x012A6140 right after the base ctor CEnginePrimitiveManager::CEnginePrimitiveManager 0x00BACDB0 and
//   ends in ?BuildBuffers@CEnginePrimitiveManagerParticleSpriteGroup@@AAEXXZ 0x00BBF380.
// Members (Ego_r names, retail offsets): CArray<CVertexShader> x3 (+0x0C/+0x18/+0x24, resized to 5 through the out-of-line
//   vector<CVertexShader>::resize(n) 0x00B6CBD0), *Displacement vertex shaders +0x30/+0x38/+0x40 (ctor 0x00A62FB0), pixel shaders
//   +0x48/+0x50/+0x58 (ctor 0x00A5EC00), CVertexBufferWin32 +0x60 (ctor inline: CBaseClassNonCopyable 0x0099A2F0, Length/Type,
//   CVertexCompressionParams unit scale / zero bias, vtable 0x0129BA20, PVertexBuffer/Stride), CIndexBuffer +0x94 (ctor 0x00AC1EC0).
// Shaders: CEngineResourceManager::GetVShaderResource((EShaderBankNo)21) (retail +0x2A68 = VShaderResources[21]) and
//   GetPShaderResource() (+0x2A98), CShaderResource::GetVertexShader / GetPixelShader(const CCharString&) const 0x00A5D5F0 /
//   0x00A5D720 by value; the name literals are the retail strings 0x012A610C .. 0x012A5C74. Inline: CVertexShader::operator=
//   (returns const&), CPixelShader::operator= (void, nulls Data), the temporaries' ~CVertexShader / ~CPixelShader through
//   C*ShaderData::Release (`mov esi,[eax+N] ; dec esi ; mov [eax+N],esi`), ~CShaderBase (vtable 0x0129A030), ~CCharString 0x0099EAE0.
// Levers: (1) CArray index operators through begin() (FABLE_RETAIL_CARRAY, hand-back hdr/engine/CArray.h): the `_M_start[i]` form
//   adds `add esi,8` per element (3008 v 2970); (2) CVertexBufferWin32's own ctor clears PVertexBuffer / Stride after its vtable
//   store (retail order), the base ctor clears Length / Type.
// Headers: hand-back FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP / CENGINERESOURCEMANAGER / CARRAY / CVERTEXSHADER /
//   CVERTEXBUFFERWIN32 branches and the new CEnginePrimitiveManager / CEnginePrimitiveRenderUtil / CShaderResource / CPixelShader /
//   CVertexCompressionParams headers.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"

extern CEngineResourceManager* g_pResourceManager;    // 0x01436E98 (descriptive; no PDB name for this global)

CEnginePrimitiveManagerParticleSpriteGroup::CEnginePrimitiveManagerParticleSpriteGroup(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShaders3DFaceMeSpritePointLights.resize(5);
    VertexShaders3DFaceMeSpritePointLights[0] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_FOG");
    VertexShaders3DFaceMeSpritePointLights[1] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShaders3DFaceMeSpritePointLights[2] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShaders3DFaceMeSpritePointLights[3] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShaders3DFaceMeSpritePointLights[4] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_5POINTLIGHTS_FOG");

    VertexShadersCrossedSpritePointLights.resize(5);
    VertexShadersCrossedSpritePointLights[0] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_FOG");
    VertexShadersCrossedSpritePointLights[1] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersCrossedSpritePointLights[2] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersCrossedSpritePointLights[3] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersCrossedSpritePointLights[4] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_5POINTLIGHTS_FOG");

    VertexShadersBillboardSpritePointLights.resize(5);
    VertexShadersBillboardSpritePointLights[0] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_FOG");
    VertexShadersBillboardSpritePointLights[1] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersBillboardSpritePointLights[2] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersBillboardSpritePointLights[3] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersBillboardSpritePointLights[4] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_5POINTLIGHTS_FOG");

    VertexShadersBillboardSpriteDisplacement = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_BILLBOARD_DISPLACEMENT");
    VertexShadersCrossedSpriteDisplacement = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_CROSSED_DISPLACEMENT");
    VertexShaders3DFaceMeSpriteDisplacement = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("VSHADER_SPRITE_GROUP_3D_FACE_ME_DISPLACEMENT");

    PixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP");
    PixelShaderStipple = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_STIPPLE");
    PixelShaderDisplacement = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_DISPLACEMENT");

    BuildBuffers();
}

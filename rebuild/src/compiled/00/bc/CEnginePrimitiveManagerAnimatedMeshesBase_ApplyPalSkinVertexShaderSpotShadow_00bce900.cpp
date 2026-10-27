// CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderSpotShadow(unsigned long num_point_lights, unsigned long) -- apply the palette-skin spot-shadow vertex shader picked by the point-light count (clamped to 5)
// Leaf: ?ApplyPalSkinVertexShaderSpotShadow@CEnginePrimitiveManagerAnimatedMeshesBase@@AAEXKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCE900, 50 bytes, thiscall; the ApplyVertexShader call is a tail jump that reuses
//   both argument slots (`mov [esp+8],1 ; mov [esp+4],edx ; jmp 0x00988020`), which needs a two-argument caller frame.
// Identity (probable): Ego_r.pdb names three private two-ulong (AAEXKK) members of this family -- ApplyPalSkinVertexShader,
//   ...Shadow and ...SpotShadow; the one-ulong (AAEXK) siblings next door (0x00BCE8A0 +0x2f8 DP3BumpShadow, 0x00BCE940
//   +0x300 EnvSpotShadow) end `call ; ret 4` instead. This body indexes the selector VertexShadersPalSkinSpotShadow
//   (+0x2fc, PDB offset, as in the landed FrameStart 0x00BCEA60), so it is the SpotShadow member; its second argument is
//   unused.
// Callee: ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z @ 0x00988020 (PDB) on g_pShaderRenderManager
//   (0x01436E14). PointLightShaderIndex (+0x20, unsigned long[6]) maps a light count to a shader slot.
#include "engine/CVertexShader.h"                              // 8 bytes (PDB)

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader& shader, bool force);    // 0x00988020
};
extern CShaderRenderManager* g_pShaderRenderManager;                     // 0x01436E14

class CEnginePrimitiveManagerAnimatedMeshesBase {
public:
    unsigned char  _pad000[0x20];
    unsigned long  PointLightShaderIndex[6];                            // +0x020
    unsigned char  _pad038[0x2fc - 0x38];
    CVertexShader* VertexShadersPalSkinSpotShadow;                      // +0x2fc
private:
    void ApplyPalSkinVertexShaderSpotShadow(unsigned long num_point_lights, unsigned long);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderSpotShadow(unsigned long num_point_lights, unsigned long)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    g_pShaderRenderManager->ApplyVertexShader(VertexShadersPalSkinSpotShadow[PointLightShaderIndex[num_point_lights]], true);
}
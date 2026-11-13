# OUTLINE 0x00A079B0  CRenderStateManager::CDeviceState::CDeviceState()  (??0CDeviceState@CRenderStateManager@@QAE@XZ)

## Frame facts
- Prologue: none. `00a079b0 mov eax,ecx ; 00a079b2 xor edx,edx`. No push ebp, no saved callee regs, frame size 0.
- Epilogue: `00a08d05 ret` (c3, __thiscall, 0 arg bytes). Returns this (eax).
- EH states: none (no SEH frame; CStateInfo has no destructor). /GS: none. Calls: none. Jump tables / cold blocks: none.
- Registers: eax = this, edx/dl = 0, ecx = walking element pointer in the 22 arrays.

## Frame map
- No stack slots.

## Source (whole function)
```cpp
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#include "engine/CRenderStateManager.h"

// inline everywhere in release (no Ego_r.pdb copy); FableWin debug body 0x030EC910; same expansion matched in 0x00A05970
inline CRenderStateManager::CStateInfo::CStateInfo()
    : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}

CRenderStateManager::CDeviceState::CDeviceState()
{
    // every CStateInfo member (55 scalars, then 22 arrays of 8) default-constructed in declaration order
}
```
Each CStateInfo expansion = `mov [p+0],edx ; mov [p+4],edx ; mov [p+8],edx ; mov byte [p+0x10],dl ; mov byte [p+0x11],dl`.

## Block map (retail order = declaration order; first store of each member)
Scalars, all through [eax+off] (00a079b4 .. 00a07fdd, straight line):
   0x0000 PolyShadeMode                CStateInfo       first store 00a079b4 via eax
   0x0014 PolyFillMode                 CStateInfo       first store 00a079c2 via eax
   0x0028 ColourWriteEnable            CStateInfo       first store 00a079d1 via eax
   0x003c ZBufferWriteEnable           CStateInfo       first store 00a079e0 via eax
   0x0050 ZBufferEnable                CStateInfo       first store 00a079ef via eax
   0x0064 ZBufferFunc                  CStateInfo       first store 00a079fe via eax
   0x0078 PolyCullMode                 CStateInfo       first store 00a07a0d via eax
   0x008c AlphaTestFunc                CStateInfo       first store 00a07a25 via eax
   0x00a0 AlphaBlendEnable             CStateInfo       first store 00a07a43 via eax
   0x00b4 AlphaTestEnable              CStateInfo       first store 00a07a61 via eax
   0x00c8 AlphaRef                     CStateInfo       first store 00a07a7f via eax
   0x00dc VertexBlendMode              CStateInfo       first store 00a07a9d via eax
   0x00f0 SoftwareVertexProcessing     CStateInfo       first store 00a07abb via eax
   0x0104 SourceAlphaBlendMode         CStateInfo       first store 00a07ad9 via eax
   0x0118 DestAlphaBlendMode           CStateInfo       first store 00a07af7 via eax
   0x012c FogEnable                    CStateInfo       first store 00a07b15 via eax
   0x0140 FogColour                    CStateInfo       first store 00a07b33 via eax
   0x0154 FogStart                     CStateInfo       first store 00a07b51 via eax
   0x0168 FogEnd                       CStateInfo       first store 00a07b6f via eax
   0x017c FogDensity                   CStateInfo       first store 00a07b8d via eax
   0x0190 FogMode                      CStateInfo       first store 00a07bab via eax
   0x01a4 FogVertexMode                CStateInfo       first store 00a07bc9 via eax
   0x01b8 TextureFactor                CStateInfo       first store 00a07be7 via eax
   0x01cc SpecularEnable               CStateInfo       first store 00a07c05 via eax
   0x01e0 AntialiasingOn               CStateInfo       first store 00a07c23 via eax
   0x01f4 DitherEnable                 CStateInfo       first store 00a07c41 via eax
   0x0208 ClippingEnable               CStateInfo       first store 00a07c5f via eax
   0x021c LightingEnable               CStateInfo       first store 00a07c7d via eax
   0x0230 NormalizeNormals             CStateInfo       first store 00a07c9b via eax
   0x0244 AmbientMaterialColourSource  CStateInfo       first store 00a07cb9 via eax
   0x0258 DiffuseMaterialColourSource  CStateInfo       first store 00a07cd7 via eax
   0x026c SpecularMaterialColourSource CStateInfo       first store 00a07cf5 via eax
   0x0280 PointSpriteEnable            CStateInfo       first store 00a07d13 via eax
   0x0294 PointScaleEnable             CStateInfo       first store 00a07d31 via eax
   0x02a8 PointSize                    CStateInfo       first store 00a07d4f via eax
   0x02bc PointSizeMin                 CStateInfo       first store 00a07d6d via eax
   0x02d0 PointSizeMax                 CStateInfo       first store 00a07d8b via eax
   0x02e4 PointScaleConstantFactor     CStateInfo       first store 00a07da9 via eax
   0x02f8 PointScaleDistanceFactor     CStateInfo       first store 00a07dc7 via eax
   0x030c PointScaleDistanceSqrFactor  CStateInfo       first store 00a07de5 via eax
   0x0320 ShadowFunc                   CStateInfo       first store 00a07e03 via eax
   0x0334 SolidOffsetEnable            CStateInfo       first store 00a07e21 via eax
   0x0348 DepthOffset                  CStateInfo       first store 00a07e3f via eax
   0x035c DepthOffsetSlope             CStateInfo       first store 00a07e5d via eax
   0x0370 StencilEnable                CStateInfo       first store 00a07e7b via eax
   0x0384 StencilOpFail                CStateInfo       first store 00a07e99 via eax
   0x0398 StencilOpZFail               CStateInfo       first store 00a07eb7 via eax
   0x03ac StencilOpPass                CStateInfo       first store 00a07ed5 via eax
   0x03c0 StencilFunc                  CStateInfo       first store 00a07ef3 via eax
   0x03d4 StencilRef                   CStateInfo       first store 00a07f11 via eax
   0x03e8 StencilMask                  CStateInfo       first store 00a07f2f via eax
   0x03fc StencilWriteMask             CStateInfo       first store 00a07f4d via eax
   0x0410 StippleEnable                CStateInfo       first store 00a07f6b via eax
   0x0424 BlendOpMode                  CStateInfo       first store 00a07f89 via eax
   0x0438 BlendColour                  CStateInfo       first store 00a07fa7 via eax

Arrays (CStateInfo[8]; element loop fully unrolled by VC7.1: optional peeled element 0 via [eax+base], `lea ecx,[eax+base]`,
then `add ecx,0x14` walks with +/- displacements; trailing `add ecx,0x14` often dead before the next `lea`):
   0x044c TextureCoordWrapMode         CStateInfo[8]    first store 00a07fc5 via eax; lea ecx @ 00a07fe3
   0x04ec TextureMaxAnisotropy         CStateInfo[8]    first store 00a08062 via eax; lea ecx @ 00a08080
   0x058c TextureMipmapLODBias         CStateInfo[8]    first store 00a08104 via eax; lea ecx @ 00a08122
   0x062c TextureMipMapMode            CStateInfo[8]    first store 00a0819f via eax; lea ecx @ 00a081c0
   0x06cc TextureUVModeU               CStateInfo[8]    first store 00a0824d via ecx; lea ecx @ 00a08244
   0x076c TextureUVModeV               CStateInfo[8]    first store 00a082d6 via eax; lea ecx @ 00a082f4
   0x080c TextureUVModeW               CStateInfo[8]    first store 00a08378 via ecx; lea ecx @ 00a08372
   0x08ac TextureFilterModeMin         CStateInfo[8]    first store 00a08404 via eax; lea ecx @ 00a08422
   0x094c TextureFilterModeMag         CStateInfo[8]    first store 00a084a0 via eax; lea ecx @ 00a084c1
   0x09ec TextureOperation             CStateInfo[8]    first store 00a0854b via ecx; lea ecx @ 00a08545
   0x0a8c TextureAlphaOperation        CStateInfo[8]    first store 00a085d5 via eax; lea ecx @ 00a085f3
   0x0b2c TextureArgument1             CStateInfo[8]    first store 00a08679 via ecx; lea ecx @ 00a08673
   0x0bcc TextureArgument2             CStateInfo[8]    first store 00a08705 via eax; lea ecx @ 00a08723
   0x0c6c TextureAlphaArgument1        CStateInfo[8]    first store 00a0879f via eax; lea ecx @ 00a087c0
   0x0d0c TextureAlphaArgument2        CStateInfo[8]    first store 00a0884a via ecx; lea ecx @ 00a08844
   0x0dac TextureCoordIndex            CStateInfo[8]    first store 00a088d2 via eax; lea ecx @ 00a088f3
   0x0e4c TextureTransformMode         CStateInfo[8]    first store 00a0897b via ecx; lea ecx @ 00a08975
   0x0eec TextureBorderColour          CStateInfo[8]    first store 00a08a06 via eax; lea ecx @ 00a08a24
   0x0f8c TextureBumpEnvMat00          CStateInfo[8]    first store 00a08aa1 via eax; lea ecx @ 00a08ac2
   0x102c TextureBumpEnvMat01          CStateInfo[8]    first store 00a08b4c via ecx; lea ecx @ 00a08b46
   0x10cc TextureBumpEnvMat10          CStateInfo[8]    first store 00a08bd2 via eax; lea ecx @ 00a08bf3
   0x116c TextureBumpEnvMat11          CStateInfo[8]    first store 00a08c80 via ecx; lea ecx @ 00a08c77

Dead trailing `add ecx,0x14` (scheduler/unroller artefacts, no source counterpart): 00a08101 00a081bd 00a08241 00a082d3 00a08401
00a084be 00a08542 00a085d2 00a08702 00a087bd 00a08841 00a088f0 00a08a03 00a08abf 00a08b43 00a08bf0 00a08c74.
Last element TextureBumpEnvMat11[7] (+0x11F8) stored at 00a08cf7..00a08d02, then `00a08d05 ret`.
Never written: each element's +0x0C (union state id) and +0x12 (TextureStage).

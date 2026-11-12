// CGraphicDataBank::GetBlankTexture(ETextureType type) const -- the bank's blank texture for a texture type (BlankTextures[type], 8-byte CTextures from +0x2B4)
// Leaf: ?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD150: `lea eax,[ecx+eax*8+2B4h]`-style address of BlankTextures[type], `ret 4`.
// Name-fix 2026-11-12 (was a stub without a Leaf line: non-const `CTexture* CGraphicDataBank::GetBlankTexture(ETextureType)` with a
//   global enum): Ego_r.pdb 00b566f0 + FableWin.pdb 03117b60 ?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z
//   (const, nested enum, const result); engine/CGraphicDataBank.h (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) declares the same form
//   and the landed caller CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures 0x00BA4A40 binds it so.
#include "fable_compiler.h"
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CGraphicDataBank.h"

const CTexture* CGraphicDataBank::GetBlankTexture(ETextureType type) const
{
    return &BlankTextures[type];
}

#ifndef FABLE_ENGINE_C3DTEXTURESTAGES_H
#define FABLE_ENGINE_C3DTEXTURESTAGES_H
#include "rebuild_abi.h"
// Original Ego_r sizeof12, fields0/4/8; native00ba48f0 reads both stage indices.
#ifdef FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
// Opt-in (chunk 2060, row 0x00BA5260 RenderMeshPrimitive): FableWin.pdb ??0C3DTextureStages@@QAE@XZ (14 debug instructions), inline in
// retail: 0x00BA5564 `or eax,-1 ; mov [ebp-40h],eax ; mov [ebp-3Ch],eax` and 0x00BA558B `mov byte ptr [ebp-38h],0` (both stage
// indices -1, no blank textures). No layout change.
class C3DTextureStages {public:C3DTextureStages():DiffuseTextureStage(-1),BumpTextureStage(-1),UseBlankTextures(false){}long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
#else
class C3DTextureStages {public:long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
#endif
FABLE_STATIC_ASSERT(sizeof(C3DTextureStages)==12);
#endif

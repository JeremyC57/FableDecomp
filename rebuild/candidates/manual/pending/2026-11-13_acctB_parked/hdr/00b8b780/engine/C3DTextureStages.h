#ifndef FABLE_ENGINE_C3DTEXTURESTAGES_H
#define FABLE_ENGINE_C3DTEXTURESTAGES_H
#include "rebuild_abi.h"
// Original Ego_r sizeof12, fields0/4/8; native00ba48f0 reads both stage indices.
#ifdef FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
// Opt-in (row 0x00B8B780): FableWin.pdb ??0C3DTextureStages@@QAE@XZ (inline; debug size 40 = the three stores).
class C3DTextureStages {public:C3DTextureStages(){UseBlankTextures=false;DiffuseTextureStage=0;BumpTextureStage=1;}long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
#else
class C3DTextureStages {public:long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
#endif
FABLE_STATIC_ASSERT(sizeof(C3DTextureStages)==12);
#endif

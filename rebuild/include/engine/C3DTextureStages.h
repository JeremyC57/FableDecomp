#ifndef FABLE_ENGINE_C3DTEXTURESTAGES_H
#define FABLE_ENGINE_C3DTEXTURESTAGES_H
#include "rebuild_abi.h"
// Original Ego_r sizeof12, fields0/4/8; native00ba48f0 reads both stage indices.
class C3DTextureStages {public:long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
FABLE_STATIC_ASSERT(sizeof(C3DTextureStages)==12);
#endif

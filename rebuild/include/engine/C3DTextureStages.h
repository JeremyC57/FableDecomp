#ifndef FABLE_ENGINE_C3DTEXTURESTAGES_H
#define FABLE_ENGINE_C3DTEXTURESTAGES_H
#include "rebuild_abi.h"
// Original Ego_r sizeof12, fields0/4/8; native00ba48f0 reads both stage indices.
class C3DTextureStages {public:long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;
#ifdef FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS
    // Retail opt-in (wf8, row 0x00BA4A40): FableWin.pdb ??8C3DTextureStages@@QBE_NABV0@@Z (inline in retail: the row compares
    // +0 / +4 as dwords and +8 as a byte, 0x00BA4A6B..0x00BA4A80). No layout change.
    bool operator==(const C3DTextureStages& other) const { return DiffuseTextureStage==other.DiffuseTextureStage && BumpTextureStage==other.BumpTextureStage && UseBlankTextures==other.UseBlankTextures; }
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DTextureStages)==12);
#endif

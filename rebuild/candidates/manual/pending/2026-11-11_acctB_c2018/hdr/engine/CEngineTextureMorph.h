#ifndef FABLE_ENGINE_CENGINETEXTUREMORPH_H
#define FABLE_ENGINE_CENGINETEXTUREMORPH_H
// New header (chunk 2018, CEngineSurfaceCompositionManager rows). Ego_r.pdb CEngineTextureMorph: OriginalTextureIndex +0x00,
// MorphID +0x04, Blends +0x08 (vector<CCompositeBlendDesc>; the retail STLport vector is 12 bytes, so sizeof 0x14 -- the node of
// GeneratedMorphs, map<pair<long,long>,CEngineTextureMorph>, is malloc(0x2C) at 0x00B2CBDD = 0x10 + 8 + 0x14; RenderToTexture
// 0x00B2B458 reads Blends at +0x08 / +0x0C with the 16-byte CCompositeBlendDesc stride). The landed ~CEngineSurfaceCompositionManager
// 0x00B2D1B0 models the same record locally.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CCompositeBlendDesc.h"
class CEngineTextureMorph {
public:
    long                             OriginalTextureIndex;   // +0x00
    long                             MorphID;                // +0x04
    std::vector<CCompositeBlendDesc> Blends;                 // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEngineTextureMorph) == 0x14);
#endif // FABLE_ENGINE_CENGINETEXTUREMORPH_H

#ifndef FABLE_ENGINE_CENGINETEXTUREDECAL_H
#define FABLE_ENGINE_CENGINETEXTUREDECAL_H
#include "stlport/fable_containers.h"
#include "engine/CCompositeBlendDesc.h"
// Original PDB class fields; native vector is12 bytes, giving size20.
class CEngineTextureDecal {
public:
 long OriginalTextureIndex,DecalID;
 std::vector<CCompositeBlendDesc> OverlaidBlends;
};
FABLE_STATIC_ASSERT(sizeof(CEngineTextureDecal)==20);
#endif

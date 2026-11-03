#ifndef FABLE_ENGINE_CENGINETEXTUREMORPH_H
#define FABLE_ENGINE_CENGINETEXTUREMORPH_H
#include "engine/CCompositeBlendDesc.h"
#include "stlport/fable_containers.h"
// Original fields, native twenty-byte stride.
class CEngineTextureMorph {public:CEngineTextureMorph():OriginalTextureIndex(0),MorphID(0){}long OriginalTextureIndex,MorphID;std::vector<CCompositeBlendDesc> Blends;};
FABLE_STATIC_ASSERT(sizeof(CEngineTextureMorph)==20);
#endif

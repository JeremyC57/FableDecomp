#ifndef FABLE_CIDRAWENVIRONMENTTEXTURESET_H
#define FABLE_CIDRAWENVIRONMENTTEXTURESET_H
#include "engine/CArray.h"
// Original two CArray members; 24 bytes in retail, 32/40 in the donor builds.
struct CIDrawEnvironmentTextureSet {
public:
    CArray<long> Textures;
    CArray<float> Blends;
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentTextureSet)==24);
#endif

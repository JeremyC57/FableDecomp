#ifndef FABLE_ENGINE_CTVERTEXPOS_H
#define FABLE_ENGINE_CTVERTEXPOS_H
#include "rebuild_abi.h"
#include "engine/CGenericVertex.h"
// Original CTVertexPos is an empty CGenericVertex<CTVertexPosBase> wrapper,12bytes.
// OriginalBuildUnitBox02dc6ba0 and native00b8d0d0 write X,Y,Z at0/4/8 with12-byte stride.
class CTVertexPosBase {public:float X,Y,Z;};
class CTVertexPos:public CGenericVertex<CTVertexPosBase> {};
FABLE_STATIC_ASSERT(sizeof(CTVertexPos)==12);
#endif

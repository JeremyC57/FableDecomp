#ifndef FABLE_ENGINE_CTVERTEXNORMTEX1_H
#define FABLE_ENGINE_CTVERTEXNORMTEX1_H
// New header (chunk 2038): CTVertexNormTex1 = CGenericVertex<CTVertexNormTex1Base> with no own members (the CTVertexSeaBackground
// hierarchy pattern; FableWin.pdb names ?SetPosition / ?SetNormal / ?SetU1V1 / ?SetU1 / ?SetV1 @?$CGenericVertex@VCTVertexNormTex1Base@@@@
// and CArray<CTVertexNormTex1> / vector<CTVertexNormTex1> members, V-keyed). No user constructor: neither PDB name set has
// ??0CTVertexNormTex1@@QAE@XZ, and as a derived (non-aggregate) class `CTVertexNormTex1()` is left uninitialised by VC7.1 -- the landed
// vector<CTVertexNormTex1>::vector(size_type) 0x00BB9FD0 copies an unwritten 32-byte frame temporary into each element.
#include "engine/CTVertexNormTex1Base.h"
#include "engine/CGenericVertex.h"
class CTVertexNormTex1 : public CGenericVertex<CTVertexNormTex1Base> { };
FABLE_STATIC_ASSERT(sizeof(CTVertexNormTex1) == 0x20);
#endif // FABLE_ENGINE_CTVERTEXNORMTEX1_H

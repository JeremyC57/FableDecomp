#ifndef FABLE_ENGINE_CTVERTEXSEABACKGROUND_H
#define FABLE_ENGINE_CTVERTEXSEABACKGROUND_H
#ifdef FABLE_RETAIL_CTVERTEXSEABACKGROUND_HIERARCHY
#include "engine/CTVertexSeaBackgroundBase.h"
#include "engine/CGenericVertex.h"
// Ego_r0x5d329/FableWin0x6a4d9: sole public base at0; no own members.
class CTVertexSeaBackground : public CGenericVertex<CTVertexSeaBackgroundBase> { };
#else
// Retail layout (hand-written, chunk 785): CTVertexSeaBackground, 12 bytes (class key V; Ego_r.pdb names CArray<CTVertexSeaBackground>
// but carries no layout record). Retail: the sea tesselator 0x00BE4FE0 indexes the bridge buffer with `lea [i*3]*4`, stores
// `fild`-converted 16-bit landscape X / Y to +0x00 / +0x04 and the water vertex Z to +0x08; push_back 0x00BE5950 copies three
// dwords. Member names X / Y / Z follow CTVertexWaterBackgroundBase (descriptive for this class).
class CTVertexSeaBackground {
public:
    float X;                                                     // +0x00
    float Y;                                                     // +0x04
    float Z;                                                     // +0x08
};

#endif

#endif // FABLE_ENGINE_CTVERTEXSEABACKGROUND_H

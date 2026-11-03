#ifndef FABLE_ENGINE_CVERTEXCOMPRESSIONPARAMS_H
#define FABLE_ENGINE_CVERTEXCOMPRESSIONPARAMS_H
// New header (chunk 725b): CVertexCompressionParams, Ego_r sizeof 0x20 (PosScaleX..W, PosBiasX..W). The default constructor
// ??0CVertexCompressionParams@@QAE@XZ (FableWin) is inline in retail: scale 1.0f x4 then bias 0 x4, in member order, as
// immediate stores (C3DMeshFileXLoader::C3DMeshFileXLoader 0x00AA5C80; CreateNewInstance 0x00AA59C0).
#include "rebuild_abi.h"
class CVertexCompressionParams {
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
FABLE_STATIC_ASSERT(sizeof(CVertexCompressionParams) == 0x20);
#endif

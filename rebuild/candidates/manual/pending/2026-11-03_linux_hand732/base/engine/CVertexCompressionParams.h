#ifndef FABLE_ENGINE_CVERTEXCOMPRESSIONPARAMS_H
#define FABLE_ENGINE_CVERTEXCOMPRESSIONPARAMS_H
// Hand-written (chunk 723): CVertexCompressionParams, 0x20 bytes (Ego_r.pdb: PosScaleX..W +0x00..+0x0C, PosBiasX..W +0x10..+0x1C).
// ??0CVertexCompressionParams@@QAE@XZ is inline: unit scale, zero bias (CVertexBufferWin32 construction inside 0x00BC14E0 stores
// 1.0f at +0x14..+0x20 and 0 at +0x24..+0x30 of the buffer, i.e. CVertexBufferBase::CompressionParams at +0x14).
#include "rebuild_abi.h"
class CVertexCompressionParams {
public:
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
    float PosScaleX;                                                     // +0x00
    float PosScaleY;                                                     // +0x04
    float PosScaleZ;                                                     // +0x08
    float PosScaleW;                                                     // +0x0C
    float PosBiasX;                                                      // +0x10
    float PosBiasY;                                                      // +0x14
    float PosBiasZ;                                                      // +0x18
    float PosBiasW;                                                      // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CVertexCompressionParams) == 0x20);

#endif // FABLE_ENGINE_CVERTEXCOMPRESSIONPARAMS_H

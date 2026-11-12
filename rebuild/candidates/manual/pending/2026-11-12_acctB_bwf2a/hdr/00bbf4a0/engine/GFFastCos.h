#ifndef FABLE_ENGINE_GFFASTCOS_H
#define FABLE_ENGINE_GFFASTCOS_H
#include "engine/GFFloatToLongNear.h"
// Both original PUBLIC32/GDATA32 records: float GFastCosTable[1025].
// Donor02DCFCD0 and native fade sites bind the same cosine table.
extern float GFastCosTable[1025];
#ifdef FABLE_RETAIL_GFFASTCOS_FORCEINLINE
// Opt-in (row 0x00BBF4A0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive): expanded where plain `inline` is refused
// (retail 0x00BC00E0..0x00BC0145, next to the GFFastSin expansion that reuses its angle * 1024 product), written as the landed out-of-line
// body 0x00A14840 (global_GFFastCos_00a14840.cpp: the argument itself scaled), the text whose scaled angle the sine expansion shares.
FABLE_FORCEINLINE float FABLE_FASTCALL GFFastCos(float angle){
 angle=angle*1024.0f;
 long i=GFFloatToLongNear(angle-0.5f);
 float frac=angle-(float)i;
 i&=0x3ff;
 return (1.0f-frac)*GFastCosTable[i]+frac*GFastCosTable[i+1];
}
#else
inline float FABLE_FASTCALL GFFastCos(float angle){
 float pos=angle*1024.0f;
 long index=GFFloatToLongNear(pos-0.5f);
 float interp=pos-(float)index;
 index&=0x3ff;
 return (1.0f-interp)*GFastCosTable[index]+interp*GFastCosTable[index+1];
}
#endif
#endif

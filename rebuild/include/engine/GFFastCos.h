#ifndef FABLE_ENGINE_GFFASTCOS_H
#define FABLE_ENGINE_GFFASTCOS_H
#include "engine/GFFloatToLongNear.h"
// Both original PUBLIC32/GDATA32 records: float GFastCosTable[1025].
// Donor02DCFCD0 and native fade sites bind the same cosine table.
extern float GFastCosTable[1025];
inline float FABLE_FASTCALL GFFastCos(float angle){
 float pos=angle*1024.0f;
 long index=GFFloatToLongNear(pos-0.5f);
 float interp=pos-(float)index;
 index&=0x3ff;
 return (1.0f-interp)*GFastCosTable[index]+interp*GFastCosTable[index+1];
}
#endif

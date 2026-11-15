#ifndef FABLE_ENGINE_GFFASTSIN_H
#define FABLE_ENGINE_GFFASTSIN_H
#include "engine/GFFastCos.h"
// Original FableWin public ?GFFastSin@@YIMM@Z and02DCFC30, called by the
// original occlusion module973. Same cosine table, quarter-turn index shift.
inline float FABLE_FASTCALL GFFastSin(float angle)
{
    float pos=angle*1024.0f;
    long index=GFFloatToLongNear(pos-0.5f);
    float interp=pos-(float)index;
    index=(index-256)&0x3ff;
    return (1.0f-interp)*GFastCosTable[index]+interp*GFastCosTable[index+1];
}
#endif

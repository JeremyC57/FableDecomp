#ifndef FABLE_ENGINE_GFROUNDFLOATNEAR_H
#define FABLE_ENGINE_GFROUNDFLOATNEAR_H
// GFRoundFloatNear(float value, long places) -- round a float to `places` decimal places with the x87 rounding mode.
// Identity: FableWin.pdb ?GFRoundFloatNear@@YIMMJ@Z (global __fastcall; its sibling ?GFRoundFloat@@YIMMJ@Z truncates through
//   __ftol2, the landed CPlayer::DrawFreeCamDebug 0x00487570 models that one locally). Inline in retail: CPSCDecalRenderer
//   DatabaseSerialise 0x00B1E380 / PreparePrimitives 0x00B1F200 expand it per packed field: `mov eax,places ; fld 1.0 ;
//   dec eax ; fmul 10.0 ; jne` (the multiplier), `fmul ; fstp tmp ; fld tmp ; fistp` (GFFloatToLongNear) and `fild ; fdiv
//   multiplier`. Plain C++ over the shared GFFloatToLongNear helper (engine/GFFloatToLongNear.h, which carries the retail asm).
// Chunk 724 round 3 (new header).
#include "engine/GFFloatToLongNear.h"
inline float __fastcall GFRoundFloatNear(float value, long places)
{
    float multiplier = 1.0f;
    while (places--)
        multiplier *= 10.0f;
    return (float)GFFloatToLongNear(value * multiplier) / multiplier;
}
#endif

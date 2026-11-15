#ifndef FABLE_ENGINE_GFPACKFLOAT_H
#define FABLE_ENGINE_GFPACKFLOAT_H
#include "fable_compiler.h"
// GFRoundFloatNear / GFPackFloat / GFUnpackFloat -- the engine's float quantisers (FableWin.pdb ?GFRoundFloatNear@@YIMMJ@Z,
// ?GFPackFloat@@YIKMKMMK@Z, ?GFUnpackFloat@@YIMKKMMK@Z; __fastcall). Out-of-line copies in retail: GFPackFloat 0x00AF8D50 (landed
// row, the same body) and GFUnpackFloat 0x00AF8DC0 (`fild packed ; fild (1 << bits) ; fsub 1.0 ; fdivp ; fld max ; fsub min ;
// fmulp ; fadd min`, then the GFRoundFloatNear loop / fistp / fdiv). Every CPSC* database member (the bit-field packed floats of
// CPSCRenderSprite: ctor 0x00AFB7C0, AddToBoundingSphere 0x00AFAE50, DatabaseSerialise 0x00AFBCE0, PreparePrimitives 0x00AFCC30)
// expands them in place with constant arguments (the divisions by a constant become reciprocal multiplies, the `+ 0.0f` and
// `* 1.0f` vanish). __forceinline: they contain the inline-asm GFFloatToLongNear, which VC7.1 never auto-inlines.
// GFRoundFloatNear rounds through GFFloatToLongNear (FPU round-to-nearest); GFRoundFloat (?GFRoundFloat@@YIMMJ@Z) is the
// truncating `(long)` twin (CPlayer::DrawFreeCamDebug 0x00487570).
// Shape (chunk 738): `normalised` is its own statement in both directions -- written as one expression VC7.1 folds the constant
// divide and multiply into one reciprocal (retail keeps `fmul 1/2047 ; fmul 20` apart at 0x00AFCD7D). Checked out of line: these
// bodies compiled as plain functions grade RELOCATION_MATCH against the landed GFPackFloat 0x00AF8D50.
// No asm of its own: the bare `fistp dword` comes from the shared engine/GFFloatToLongNear.h (its FABLE_PORTABLE twin covers the
// portable build), so this header needs no dual-path test of its own.
#include "engine/GFFloatToLongNear.h"
FABLE_FORCEINLINE float __fastcall GFRoundFloatNear(float value, long places)
{
    float scale = 1.0f;
    for (long i = places; i != 0; --i)
        scale *= 10.0f;
#ifdef FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
    return GFFloatToLongNear((value * scale)) / scale;
#elif defined(FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_QUOTIENT)
    // Opt-in (chunk 4006 / acctC agentC1, rows 0x005D1F0C / 0x005CE0E6 CTCInventoryStats::ConstructPersonalityListPC /
    // ConstructPersonalityList): the return expression grouped as a whole. Same bytes for the helper and each expansion; the
    // no-code PAREN tuple after the divide moves the scheduler's 81-tuple region cuts of the long caller to retail's (row 1: the
    // MONEY-title bank load 0x005D52F9 / 0x005D5A40; the default body gives 5611 / 5617 instructions, GROUP_PRODUCT 5616 / 5617).
    return (GFFloatToLongNear(value * scale) / scale);
#else
    return GFFloatToLongNear(value * scale) / scale;
#endif
}
FABLE_FORCEINLINE unsigned long __fastcall GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float normalised = (GFRoundFloatNear(value, numDecimalPlaces) - minValue) / (maxValue - minValue);
    return (unsigned long)GFFloatToLongNear(normalised * ((float)(long)(1 << numBits) - 1.0f));
}
FABLE_FORCEINLINE float __fastcall GFUnpackFloat(unsigned long packed, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float normalised = packed / ((float)(long)(1 << numBits) - 1.0f);
#ifdef FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
    return GFRoundFloatNear((normalised * (maxValue - minValue)) + minValue, numDecimalPlaces);
#else
    return GFRoundFloatNear(normalised * (maxValue - minValue) + minValue, numDecimalPlaces);
#endif
}
#endif

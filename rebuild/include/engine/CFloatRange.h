#ifndef FABLE_ENGINE_CFLOATRANGE_H
#define FABLE_ENGINE_CFLOATRANGE_H
// CFloatRange (chunk 777, row 0x00803540): Ego_r.pdb / FableWin.pdb CFloatRange, 8 bytes: HighValue +0, LowValue +4
// (ghidra_out/struct_layouts_egor.tsv). FableWin.pdb names only ??0CFloatRange@@QAE@XZ and the CPersistContext transfer
// templates; no member function, so callers interpolate the two values in place (retail 0x008036F1: `fld High; fsub Low;
// fmul t; fadd Low`). The default constructor's body is unproven and left undeclared (the class stays an aggregate).
#include "rebuild_abi.h"
class CFloatRange {
public:
    float HighValue;                                            // +0x00
    float LowValue;                                             // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CFloatRange) == 0x8);
#endif // FABLE_ENGINE_CFLOATRANGE_H

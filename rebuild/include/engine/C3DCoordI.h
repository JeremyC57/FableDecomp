#ifndef FABLE_ENGINE_C3DCOORDI_H
#define FABLE_ENGINE_C3DCOORDI_H
// NEW shared header (hand3c, row 0x00B51AD0): Ego_r.pdb C3DCoordI (ghidra_out/struct_layouts_egor.tsv: 12 bytes, a C2DCoordI base at +0
// (X +0, Y +4) and long Z +8). Layout plus the two out-of-line compound operators under row opt-ins.
#include "fable_compiler.h"
#include "engine/C2DCoordI.h"
#if defined(FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS) || defined(FABLE_RETAIL_C3DCOORDI_OPERATOR_MINUS_EQUALS)
class C3DVector;
#endif
class C3DCoordI : public C2DCoordI {
public:
#ifdef FABLE_RETAIL_C3DCOORDI_CTOR_WEATHER_PASS29
    C3DCoordI() {}
    C3DCoordI(long x, long y, long z) : C2DCoordI(x,y), Z(z) {}
#endif
    long Z;                                                  // +0x08
#ifdef FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS
    // Retail opt-in (row 0x00A13BE0): Ego_r.pdb ??YC3DCoordI@@QAEABV0@ABVC3DVector@@@Z (out of line, const reference result).
    const C3DCoordI& operator+=(const C3DVector& v);         // 0x00A13BE0
#endif
#ifdef FABLE_RETAIL_C3DCOORDI_OPERATOR_MINUS_EQUALS
    // Retail opt-in (row 0x00A13C20): Ego_r / FableWin.pdb ??ZC3DCoordI@@QAEABV0@ABVC3DVector@@@Z (out of line, const reference result).
    const C3DCoordI& operator-=(const C3DVector& v);         // 0x00A13C20
#endif
};
FABLE_STATIC_ASSERT_AT(22, sizeof(C3DCoordI) == 12);
#endif

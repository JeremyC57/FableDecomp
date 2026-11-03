#ifndef FABLE_ENGINE_CENGINEPRIMITIVELINE_H
#define FABLE_ENGINE_CENGINEPRIMITIVELINE_H
// New shared header (chunk 728, row 0x0074BC20): Ego_r.pdb CEnginePrimitiveLine : CEnginePrimitive, sizeof 0x3C: Start +0xC, End +0x18,
// Colour +0x24, Width +0x28, BankIndex +0x2C, BlendMode +0x30, BlendOp +0x34, UseZBuffer +0x38. The one constructor (Ego_r + FableWin
// ??0CEnginePrimitiveLine@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MW4EEnginePrimitiveBlendMode@@K_NW4EEngineInterpolationSet@@
// W4EEnginePrimitiveBlendOp@@@Z) is a header inline: retail keeps an out-of-line COMDAT (0x004C3090, landed) and also expands it in
// place (DrawDebugInfo 0x0074BF86..: Type 6, the zero channel bytes, the copied vectors and colour, then the scalars in member order).
// The 64-bit owner id is not stored. Type 6 is the value 0x004C3090 stores (the enumerator name is not on disk).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
class CEnginePrimitiveLine : public CEnginePrimitive {
public:
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         EEnginePrimitiveBlendMode blend_mode, unsigned long bank_index, bool use_z_buffer,
                         EEngineInterpolationSet interpolation_set, EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive(EEnginePrimitiveType(6), interpolation_set), Start(start), End(end), Colour(colour), Width(width),
          BankIndex(bank_index), BlendMode(blend_mode), BlendOp(blend_op), UseZBuffer(use_z_buffer) { }
    C3DVector                 Start;           // +0x0C
    C3DVector                 End;             // +0x18
    CRGBColour                Colour;          // +0x24
    float                     Width;           // +0x28
    unsigned long             BankIndex;       // +0x2C
    EEnginePrimitiveBlendMode BlendMode;       // +0x30
    EEnginePrimitiveBlendOp   BlendOp;         // +0x34
    bool                      UseZBuffer;      // +0x38
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveLine) == 0x3C);
#endif

#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DTEXT_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DTEXT_H
// New shared header (chunk 749, row 0x0054EF00 NUISystem::CText::Draw): Ego_r.pdb CEnginePrimitive2DText, sizeof 0x2C
// (ghidra_out/struct_layouts_egor.tsv): CEnginePrimitive base +0 (Type, InterpolationSet, ThemeChannelDesc), Pos (C3DVector)
// +0xC, Layer +0x18 (EEngine2DLayer), Scale +0x1C, Colour +0x20, Text (CWideString) +0x24, PFont (CFontBank*) +0x28.
// Name set: FableWin.pdb ??0..QAE@XZ (default; inline in retail: CText::Draw stores Type 0x27, InterpolationSet 0, the three
// channel bytes, then constructs Text, in place), ??0..QAE@_KABVC2DVector@@W4EEngine2DLayer@@MABVCWideString@@ABVCFontBank@@
// ABVCRGBColour@@W4EEngineInterpolationSet@@@Z (out of line at 0x00543910, `ret 24h`), ??0..QAE@ABV0@@Z (copy; the landed
// 0x00655048), ??4..QAEAAV0@ABV0@@Z (out of line at 0x00543530, `ret 4`), ??1..QAE@XZ (implicit: ~CWideString of Text).
// The primitive type number 0x27 has no recovered enumerator name (EEnginePrimitiveType stays MS forward-declared).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class C2DVector;
class CFontBank;
enum EEngine2DLayer;
class CEnginePrimitive2DText : public CEnginePrimitive {
public:
    CEnginePrimitive2DText() : CEnginePrimitive((EEnginePrimitiveType)0x27, (EEngineInterpolationSet)0) { }
    CEnginePrimitive2DText(unsigned __int64, const C2DVector&, EEngine2DLayer, float, const CWideString&, const CFontBank&,
                           const CRGBColour&, EEngineInterpolationSet);                 // 0x00543910
    CEnginePrimitive2DText(const CEnginePrimitive2DText&);                              // 0x00655048
    CEnginePrimitive2DText& operator=(const CEnginePrimitive2DText&);                   // 0x00543530
    C3DVector Pos;                                                                       // +0x0c
    EEngine2DLayer Layer;                                                                // +0x18
    float Scale;                                                                         // +0x1c
    CRGBColour Colour;                                                                   // +0x20
    CWideString Text;                                                                    // +0x24
    CFontBank* PFont;                                                                    // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DText) == 0x2c);
#endif

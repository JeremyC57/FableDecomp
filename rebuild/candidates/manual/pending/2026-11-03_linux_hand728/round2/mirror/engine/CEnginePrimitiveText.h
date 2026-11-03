#ifndef FABLE_ENGINE_CENGINEPRIMITIVETEXT_H
#define FABLE_ENGINE_CENGINEPRIMITIVETEXT_H
// New shared header (chunk 728, row 0x0074BC20): Ego_r.pdb CEnginePrimitiveText : CEnginePrimitive, sizeof 0x2C: Pos +0xC, Scale +0x18,
// Colour +0x1C, Text +0x20 (CWideString), PFont +0x24 (CFontBank*), ScaleIn3D +0x28. Constructor: Ego_r + FableWin
// ??0CEnginePrimitiveText@@QAE@_KABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@_NW4EEngineInterpolationSet@@@Z, out of
// line in retail (0x004C3120, landed; every caller calls it). The destructor is the implicit one (~CWideString on Text, expanded
// at the caller: DrawDebugInfo 0x0074BE1C). Needs FABLE_RETAIL_CWIDESTRING's CWideString (fable_string.h) for the member.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
class CFontBank;
class CEnginePrimitiveText : public CEnginePrimitive {
public:
    CEnginePrimitiveText(unsigned __int64 id, const C3DVector& pos, float scale, const CWideString& text, const CFontBank& font,
                         const CRGBColour& colour, bool scale_in_3d, EEngineInterpolationSet interpolation_set);   // 0x004C3120
    C3DVector   Pos;                 // +0x0C
    float       Scale;               // +0x18
    CRGBColour  Colour;              // +0x1C
    CWideString Text;                // +0x20
    CFontBank*  PFont;               // +0x24
    bool        ScaleIn3D;           // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveText) == 0x2C);
#endif

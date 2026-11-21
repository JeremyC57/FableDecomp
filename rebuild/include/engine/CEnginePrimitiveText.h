#if defined(FABLE_RETAIL_STEALTH_DRAW) || defined(FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG)
#ifndef FABLE_ENGINE_CENGINEPRIMITIVETEXT_H
#define FABLE_ENGINE_CENGINEPRIMITIVETEXT_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class CFontBank;
// Layout: complete original Egobcb5/FWb9a4, native44 with original four-byte wide string.
class CEnginePrimitiveText:public CEnginePrimitive{
public:
 C3DVector Pos;
 float Scale;
 CRGBColour Colour;
 CWideString Text;
 const CFontBank * PFont;
 bool ScaleIn3D;
 CEnginePrimitiveText(const CEnginePrimitiveText & p0);
 CEnginePrimitiveText(unsigned __int64 p0, const C3DVector & p1, float p2, const CWideString & p3, const CFontBank & p4, const CRGBColour & p5, bool p6, EEngineInterpolationSet p7);
 CEnginePrimitiveText();
 ~CEnginePrimitiveText(){};
 CEnginePrimitiveText & operator=(const CEnginePrimitiveText & p0);
};
inline CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64 id,const C3DVector&pos,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,bool scale_in_3d,EEngineInterpolationSet interpolation_set):CEnginePrimitive((EEnginePrimitiveType)5,id,interpolation_set),Pos(pos),Scale(scale),Colour(colour),Text(text),PFont(&font),ScaleIn3D(scale_in_3d){}
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveText)==44);
#endif
#else
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVETEXT_INLINE_CTOR
    const CFontBank* PFont;          // +0x24 (Ego_r.pdb: const CFontBank*)
#else
    CFontBank*  PFont;               // +0x24
#endif
    bool        ScaleIn3D;           // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveText) == 0x2C);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVETEXT_INLINE_CTOR
// Retail opt-in (p25N, row 0x00833C00): the constructor is an inline header function in the original (Ego_d.pdb holds it once, under
// tc_graphic_appearance.obj; FableWin.pdb emits its COMDAT in several objects; retail keeps one out-of-line copy, 0x004C3120, landed).
// Plain inline and visible (the body of the STEALTH_DRAW branch above): c2 still calls it out of line, as retail does, but the visible
// body changes how the callers copy the position (0x00833C00). Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR.
inline CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64 id,const C3DVector&pos,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,bool scale_in_3d,EEngineInterpolationSet interpolation_set):CEnginePrimitive((EEnginePrimitiveType)5,id,interpolation_set),Pos(pos),Scale(scale),Colour(colour),Text(text),PFont(&font),ScaleIn3D(scale_in_3d){}
#endif
#endif

#endif

// CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, const CFontBank& font, const CRGBColour& colour, unsigned long flags) -- queue a scaled text string at a 3D position (forwarded to AddText) and report success
// Leaf: ?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DBFF0, length 49 bytes (`ret 18h`). Ego_r.pdb 0x00B2FB10 / FableWin.pdb
//   ?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z (reloc_bindings via the
//   landed caller 0x00435530 CDisplayEngine::DoRender).
// Callees: CRenderManager2D::AddText 0x009DB700 (Ego_r 0x00B2F040, private, the font by reference), CWideString::~CWideString 0x0099B510.
// Shape: a pure forwarder on the same `this` (ecx untouched); the by-value CWideString parameter is passed on by address
//   (`lea edx,[esp+18h]`: its own argument slot) and destroyed by this callee afterwards (`lea ecx,[esp+0Ch] ; call
//   ~CWideString`), which also blocks the sibling-call jump; the bool result is the constant `mov al,1`.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT
#define FABLE_RETAIL_CRENDERMANAGER2D_ADDTEXT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
class CFontBank;
#include "engine/CRenderManager2D.h"

bool CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, const CFontBank& font,
                                        const CRGBColour& colour, unsigned long flags)
{
    AddText(position, scale, text, font, colour, flags);
    return true;
}

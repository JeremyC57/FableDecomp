// CRenderManager2D::Draw2DText(const C2DVector& position, CWideString text, const CFontBank& font, const CRGBColour& colour, unsigned long flags) -- queue a text string at a 2D screen position (z = 0, scale 1) and report success
// Leaf: ?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DD8F0, length 109 bytes (`ret 14h`). Ego_r.pdb 0x00B31610 / FableWin.pdb
//   ?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z (engine/CRenderManager2D.h).
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720, CRenderManager2D::AddText 0x009DB700 (private, the font by
//   reference), CWideString::~CWideString 0x0099B510 (x2: the copy, then the by-value parameter).
// Shape: the 3D position and the string copy are NAMED locals built before the argument pushes (VC homes the string
//   copy in the dead position-argument slot); `mov al,1`.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_ADDTEXT
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRenderManager2D.h"

bool CRenderManager2D::Draw2DText(const C2DVector& position, CWideString text, const CFontBank& font, const CRGBColour& colour,
                                  unsigned long flags)
{
    C3DVector screen_position(position.X, position.Y, 0.0f);
    CWideString string(text);
    AddText(screen_position, 1.0f, string, font, colour, flags);
    return true;
}

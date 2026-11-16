// CRenderManager2D::AddText(const C3DVector& position, float scale, const CWideString& text, const CFontBank& font, const CRGBColour& colour, unsigned long flags) -- queue a text record on the 2D quick-draw list (viewport window, screen-space position, string, metrics, colour, font) unless the viewport is offscreen
// Leaf: ?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DB700, length 266 bytes (`ret 18h`). Ego_r.pdb 0x00B2F040 / FableWin.pdb 0x030DF3D0
//   ?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z (private, the font by reference).
// Callees: CWideString::CWideString() 0x0099AED0, CRenderManagerCore::GetCurrentAWindow() const 0x00A0AAA0 (Ego_r 0x00B74050, directly
//   after the SetAWindow pair as retail 0x00A0AA80; `&DisplayManager->ViewportBoxFloat`), GFVXToAX / GFVYToAY 0x009E1E10 / 0x009E1E40
//   (Ego_r ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z), CWideString::operator= 0x0099B7D0, CQuickDrawTriInfo copy ctor 0x009DE890, the
//   vector's _M_insert_overflow 0x009E1750, CTexture::Uninitialise 0x009F9F70 (the inline ~CTexture), ~CWideString 0x0099B510.
// Layout: engine/CRenderManager2D.h retail branch (QuickDraw2DTriInfo +0x3E94), engine/CQuickDrawTriInfo.h.
// Shape: one local record: default-constructed (string, texture), the Line/IsText bit stores, the window copy, the converted
//   X/Y, Z, the text assignment, then font / colour / flags / scale in that statement order; the inline STLport push_back.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_ADDTEXT
#define FABLE_RETAIL_CQUICKDRAWTRIINFO_COPY
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CDISPLAYMANAGER_ISVIEWPORTOFFSCREEN
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CQuickDrawTriInfo.h"
#include "engine/CRenderManager2D.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
float __fastcall GFVXToAX(float x);                     // 0x009E1E10
float __fastcall GFVYToAY(float y);                     // 0x009E1E40

void CRenderManager2D::AddText(const C3DVector& position, float scale, const CWideString& text, const CFontBank& font,
                               const CRGBColour& colour, unsigned long flags)
{
    if (GetDisplayManager().IsViewportOffscreen())
        return;
    CQuickDrawTriInfo info;
    info.Window = GetCurrentAWindow();
    info.Line = false;
    info.IsText = true;
    info.StringX = GFVXToAX(position.X);
    info.StringY = GFVYToAY(position.Y);
    info.StringZ = position.Z;
    info.String = text;
    info.PFont = &font;
    info.Colour = colour;
    info.StringFlags = flags;
    info.StringScale = scale;
    QuickDraw2DTriInfo.push_back(info);
}

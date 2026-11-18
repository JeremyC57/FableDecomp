// CProgressDisplay::DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& vbox) const -- draw multi-line text centred in a window, bottom-aligned: count the lines, then draw each line centred horizontally with three black drop shadows and the white text on top, snapped to pixels
// Leaf: ?DisplayCenteredText@CProgressDisplay@@ABEXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00497FD0, 1,076 bytes, `ret 0Ch` (this unused). Ego_r.pdb / FableWin.pdb
//   ?DisplayCenteredText@CProgressDisplay@@ABEXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z; Ego_d.pdb (progress_debug_display.obj, after
//   the CWStringParser ctor / dtor, before GetCharWidthW) gives the parameter vbox and every local: pdisplay_manager, prender_manager,
//   parser, boxlength, num_lines, max_num_lines, counter, text_line at function scope; left, length, start_draw_pos, draw_pos in the
//   loop block. Called twice by the landed same-TU DrawRetailDisplay 0x00498490.
// Callees: GFGetSystemManager 0x009A4EC0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the parsers: the CWStringParser ctor is
//   expanded, vptr 0x01231718, then CWStringParser::Init 0x00A0C8E0), CWideString operator!= 0x0099B150 / operator= 0x0099B7D0 / copy
//   ctor 0x0099B720 / dtor 0x0099B510, CWStringParser::SkipWhitespace 0x00A0C910 / ReadAsStringUntilCharacter 0x00A0CCD0, GFAXToVX
//   0x009E1DA0, GFAYToVY 0x009E1DD0, GFRoundVXToNearestPixel 0x009E1D00, GFRoundVYToNearestPixel 0x009E1D50 (Ego_r / FableWin
//   ?GFRoundVXToNearestPixel@@YIMN@Z / ?GFRoundVYToNearestPixel@@YIMN@Z; declared here as in the landed sibling 0x00498490),
//   CRenderManager2D::Draw2DText 0x009DD8F0, the CFontBank virtuals GetStringExtents / GetFontMaxHeight.
// Shape: the draw loop is `if (text_line != L"") { num_lines = max_num_lines; do { ... --num_lines; } while (text_line != L""); }`
//   (num_lines homed once after the guard, 0x004980FE). Ego_d's `left` is the half spare width, added to vbox.TLX in the
//   start_draw_pos constructor: that named local (with the paren text below) moves the scheduling-region cut so the white draw's
//   `draw_pos = start_draw_pos` copy interleaves as retail (0x00498328..0x0049834B: load X, store X, load Y, ... store Y, fld Y before
//   `sub esp,8`); the old draft's one-expression form copied as a block (load, load, store, store; 5 hunks).
// FITTED: FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN (existing opt-in: FableWin's GetVFontMaxHeight with the parenthesised return, a
//   no-code c2 tuple; also required by 0x00435530 and 0x00494900). Equivalent measured alternative: the default GetVFontMaxHeight text
//   with `vbox.BRY - (font.GetVFontMaxHeight() * num_lines)` (also RELOCATION_MATCH); neither paren: 2 hunks. The landed same-TU
//   DrawRetailDisplay 0x00498490 does not use GetVFontMaxHeight, so the TU keeps one text.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CWSTRINGPARSER_INIT
#define FABLE_RETAIL_CWSTRINGPARSER_INLINE_CTOR
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CWStringParser.h"
#include "engine/CProgressDisplay.h"

float __fastcall GFRoundVXToNearestPixel(double x);                      // 0x009E1D00 ?GFRoundVXToNearestPixel@@YIMN@Z
float __fastcall GFRoundVYToNearestPixel(double y);                      // 0x009E1D50 ?GFRoundVYToNearestPixel@@YIMN@Z

void CProgressDisplay::DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& vbox) const
{
    CDisplayManager* pdisplay_manager = GFGetSystemManager()->GetDisplayManager();
    if (pdisplay_manager == 0)
        return;
    CRenderManager* prender_manager = &pdisplay_manager->GetRenderManager();
    if (prender_manager == 0)
        return;
    CWStringParser parser(text);
    float boxlength = vbox.BRX - vbox.TLX;
    long num_lines;
    long max_num_lines = 0;
    CWStringParser counter(text);
    CWideString text_line = counter.ReadAsStringUntilCharacter(L'\n');
    while (text_line != L"") {
        counter.SkipWhitespace();
        text_line = counter.ReadAsStringUntilCharacter(L'\n');
        ++max_num_lines;
    }
    text_line = parser.ReadAsStringUntilCharacter(L'\n');
    if (text_line != L"") {
        num_lines = max_num_lines;
        do {
            float length = font.GetVStringExtents(text_line).X;
            float left = (boxlength - length) * 0.5f;
            C2DVector start_draw_pos(left + vbox.TLX, vbox.BRY - font.GetVFontMaxHeight() * num_lines);
            C2DVector draw_pos;
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(1.0f), GFAYToVY(1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            prender_manager->Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(1.0f), GFAYToVY(-1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            prender_manager->Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(-1.0f), GFAYToVY(1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            prender_manager->Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos;
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            prender_manager->Draw2DText(draw_pos, text_line, font, CRGBColour(255, 255, 255, 255), 0);
            parser.SkipWhitespace();
            text_line = parser.ReadAsStringUntilCharacter(L'\n');
            --num_lines;
        } while (text_line != L"");
    }
}
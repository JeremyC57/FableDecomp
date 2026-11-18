// DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& vbox) -- draw multi-line text centred in a window, bottom-aligned: count the lines, then draw each line centred horizontally with three black drop shadows and the white text on top, snapped to pixels
// Leaf: ?DisplayCenteredText@@YIXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x009BCA20, 1,104 bytes, __fastcall `ret 4`. Ego_r.pdb / FableWin.pdb
//   ?DisplayCenteredText@@YIXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z; Ego_d.pdb (BBBLibrary lib_xmv_player.obj, after
//   FormatTextForVWindow, before CXMVPlayer::GetCharWidthW) gives the parameters and every local: MaxNumLines, boxlength, counter,
//   text_line, NumLines, parser, pdisplay at function scope; left, draw_pos, length, start_draw_pos in the loop block. Called by the
//   landed CMovieSubtitles::Play 0x009BD910. Twin of CProgressDisplay::DisplayCenteredText 0x00497FD0 (no null checks here).
// Callees: CWideString operator const unsigned short* 0x0099AF00, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the parsers: the
//   CWStringParser ctor is expanded, vptr 0x01231718, then CWStringParser::Init 0x00A0C8E0), GFGetSystemManager 0x009A4EC0,
//   CWideString operator!= 0x0099B150 / operator= 0x0099B7D0 / copy ctor 0x0099B720 / dtor 0x0099B510, GetAsCharArray 0x0099AF10,
//   CWStringParser::SkipWhitespace 0x00A0C910 / ReadAsStringUntilCharacter 0x00A0CCD0, GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0,
//   GFRoundVXToNearestPixel 0x009E1D00, GFRoundVYToNearestPixel 0x009E1D50, CRenderManager2D::Draw2DText 0x009DD8F0, the CFontBank
//   virtuals GetStringExtents / GetFontMaxHeight.
// Shape: the e12 twin's text with Ego_d's names: `if (text_line != L"") { NumLines = MaxNumLines; do { ... --NumLines; } while (...); }`;
//   the loop block's `length` (the GetVStringExtents X) and `left` (half the spare width, added to vbox.TLX in start_draw_pos's ctor).
// FITTED: FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN (existing opt-in: FableWin's GetVFontMaxHeight with the parenthesised return, a
//   no-code c2 tuple; also required by 0x00435530, 0x00494900 and the twin 0x00497FD0). Without it 4 hunks: the second shadow's
//   GFAYToVY(-1) result stored after `push 1.0` (0x009BCC63) and the third Draw2DText's receiver reload after the `lea` (0x009BCD83).
// Header: FABLE_RETAIL_CWSTRINGPARSER_INLINE_CTOR (engine/CWStringParser.h overlay block of e12, identical body; hdr/README.md).
// Inline-budget: none.
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

float __fastcall GFRoundVXToNearestPixel(double x);                      // 0x009E1D00 ?GFRoundVXToNearestPixel@@YIMN@Z
float __fastcall GFRoundVYToNearestPixel(double y);                      // 0x009E1D50 ?GFRoundVYToNearestPixel@@YIMN@Z

void __fastcall DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& vbox)
{
    CWStringParser parser(text);
    CDisplayManager* pdisplay = GFGetSystemManager()->GetDisplayManager();
    float boxlength = vbox.BRX - vbox.TLX;
    long NumLines;
    long MaxNumLines = 0;
    CWStringParser counter(text);
    CWideString text_line = counter.ReadAsStringUntilCharacter(L'\n');
    while (text_line != L"") {
        counter.SkipWhitespace();
        text_line = counter.ReadAsStringUntilCharacter(L'\n');
        ++MaxNumLines;
    }
    text_line = parser.ReadAsStringUntilCharacter(L'\n');
    if (text_line != L"") {
        NumLines = MaxNumLines;
        do {
            float length = font.GetVStringExtents(text_line).X;
            float left = (boxlength - length) * 0.5f;
            C2DVector start_draw_pos(left + vbox.TLX, vbox.BRY - font.GetVFontMaxHeight() * NumLines);
            C2DVector draw_pos;
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(1.0f), GFAYToVY(1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            pdisplay->GetRenderManager().Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(1.0f), GFAYToVY(-1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            pdisplay->GetRenderManager().Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos + C2DVector(GFAXToVX(-1.0f), GFAYToVY(1.0f));
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            pdisplay->GetRenderManager().Draw2DText(draw_pos, text_line, font, CRGBColour(0, 0, 0, 255), 0);
            draw_pos = start_draw_pos;
            draw_pos.X = GFRoundVXToNearestPixel(draw_pos.X);
            draw_pos.Y = GFRoundVYToNearestPixel(draw_pos.Y);
            pdisplay->GetRenderManager().Draw2DText(draw_pos, text_line, font, CRGBColour(255, 255, 255, 255), 0);
            parser.SkipWhitespace();
            text_line = parser.ReadAsStringUntilCharacter(L'\n');
            --NumLines;
        } while (text_line != L"");
    }
}

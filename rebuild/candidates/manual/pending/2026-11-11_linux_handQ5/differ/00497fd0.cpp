// CProgressDisplay::DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& window) const -- draw multi-line text centred in a window, bottom-aligned: count the lines, then draw each line centred horizontally with three black drop shadows and the white text on top, snapped to whole pixels
// Grade: DIFFER(1076v1076) -- Q5 (14 grades logged: 11 hypotheses + baseline and final regrades), ratio 0.967 (Q4 0.928). FIXED: the noLines spill -- the draw loop is `if (line != L"") { long lineNo = noLines; do { ... --lineNo; } while (line != L""); }`, which homes the count at [esp+24h] and stores it once in the pre-header after the guard (0x004980FE), with text in esi / the count in edi as retail. ONLY residue (3 lines): the white (4th) draw's `shadow = position` copy -- retail interleaves it (`mov ecx,[pX]; mov [sX],ecx; mov edx,[pY]; fld [sX]; sub esp,8; fstp; mov [sY],edx`, then `fld [sY]` BEFORE `sub esp,8` at 0x0049834B); ours copies as a block (load, load, store, store) and loads sY after the sub. Tried for that copy (all worse or inert): memberwise X/Y statements 1072 (position scalarised), C2DVector(position.X, position.Y) 1106, C2DVector(position) temp 1076 with a bigger frame, Initialise() 1072, a user operator= opt-in 1013, scoped shadow + copy-constructed textPosition 1082, const position / copy-init position / shadow outside the loop: inert. Next: a spelling where the copy source is a temporary like draws 1-3 (whose temp->shadow copies retail also interleaves) without a new frame slot.
// Leaf: ?DisplayCenteredText@CProgressDisplay@@ABEXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00497FD0, 1,076 bytes, thiscall (this unused), `ret 0Ch`; FableWin.pdb
//   ?DisplayCenteredText@CProgressDisplay@@ABEXABVCFontBank@@ABVCWideString@@ABVC2DBoxF@@@Z (CProgressDisplay TU, between
//   FormatTextForVWindow 0x00497F80 and GetCharWidthW 0x00498410; called twice by the landed DrawRetailDisplay 0x00498490).
// Callees: GFGetSystemManager 0x009A4EC0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the parsers), CWideString::operator
//   const wchar_t* 0x0099AF00 / GetAsCharArray 0x0099AF10 / operator!= 0x0099B150 / operator= 0x0099B7D0 / copy ctor 0x0099B720 /
//   ~ 0x0099B510, CWStringParser::Init 0x00A0C8E0 / SkipWhitespace 0x00A0C910 / ReadAsStringUntilCharacter 0x00A0CCD0, GFAXToVX
//   0x009E1DA0, GFAYToVY 0x009E1DD0, GFRoundVXToNearestPixel 0x009E1D00, GFRoundVYToNearestPixel 0x009E1D50,
//   CRenderManager2D::Draw2DText 0x009DD8F0.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CWStringParser.h opt-in FABLE_RETAIL_CWSTRINGPARSER_LINES.
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
#define FABLE_RETAIL_CWSTRINGPARSER_LINES
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

void CProgressDisplay::DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& window) const
{
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    if (display == 0)
        return;
    CRenderManager* renderer = &display->GetRenderManager();
    if (renderer == 0)
        return;
    CWStringParser parser(text);
    float windowWidth = window.BRX - window.TLX;
    // count the lines first: the text is bottom-aligned, so the first line sits noLines line heights above the bottom
    long noLines = 0;
    CWStringParser lineCounter(text);
    CWideString line = lineCounter.ReadAsStringUntilCharacter(L'\n');
    while (line != L"") {
        lineCounter.SkipWhitespace();
        line = lineCounter.ReadAsStringUntilCharacter(L'\n');
        ++noLines;
    }
    line = parser.ReadAsStringUntilCharacter(L'\n');
    if (line != L"") {
        long lineNo = noLines;
        do {
        C2DExtentsF extents = font.GetVStringExtents(line);
        float y = window.BRY - font.GetVFontMaxHeight() * lineNo;
        C2DVector position((windowWidth - extents.X) * 0.5f + window.TLX, y);
        C2DVector shadow;
        shadow = position + C2DVector(GFAXToVX(1.0f), GFAYToVY(1.0f));
        shadow.X = GFRoundVXToNearestPixel(shadow.X);
        shadow.Y = GFRoundVYToNearestPixel(shadow.Y);
        renderer->Draw2DText(shadow, line, font, CRGBColour(0, 0, 0, 255), 0);
        shadow = position + C2DVector(GFAXToVX(1.0f), GFAYToVY(-1.0f));
        shadow.X = GFRoundVXToNearestPixel(shadow.X);
        shadow.Y = GFRoundVYToNearestPixel(shadow.Y);
        renderer->Draw2DText(shadow, line, font, CRGBColour(0, 0, 0, 255), 0);
        shadow = position + C2DVector(GFAXToVX(-1.0f), GFAYToVY(1.0f));
        shadow.X = GFRoundVXToNearestPixel(shadow.X);
        shadow.Y = GFRoundVYToNearestPixel(shadow.Y);
        renderer->Draw2DText(shadow, line, font, CRGBColour(0, 0, 0, 255), 0);
        shadow = position;
        shadow.X = GFRoundVXToNearestPixel(shadow.X);
        shadow.Y = GFRoundVYToNearestPixel(shadow.Y);
        renderer->Draw2DText(shadow, line, font, CRGBColour(255, 255, 255, 255), 0);
        parser.SkipWhitespace();
        line = parser.ReadAsStringUntilCharacter(L'\n');
        --lineNo;
        } while (line != L"");
    }
}

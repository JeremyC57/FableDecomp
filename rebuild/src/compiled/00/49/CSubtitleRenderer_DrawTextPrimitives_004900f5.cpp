// 004900f5 CSubtitleRenderer::DrawTextPrimitives
// Leaf: ?DrawTextPrimitives@CSubtitleRenderer@@AAE?AVC2DBoxF@@ABVCEnginePrimitivePrepareInfo@@ABV2@1@Z
// Flags:
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_SUBTITLE_WRAP_BODY_PASS29
#define FABLE_RETAIL_CWSTRINGPARSER_INIT
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_SUBTITLE_WIDTH_DRAW_PASS29
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_CORNERPOS
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_C2DEXTENTSF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE
#define FABLE_RETAIL_CDISPLAYMANAGER
#include "engine/GFLimit.h"
#include "engine/Direct3D9Types.h"
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CWStringParser.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/SubtitleDrawGlobals.h"
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif

C2DBoxF CSubtitleRenderer::DrawTextPrimitives(const CEnginePrimitivePrepareInfo& info,const C2DBoxF& v_full_window,const C2DBoxF& v_television_window)
{
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    CRGBColour text_colour(255,255,255);
    if (IsFlagSet((ESubtitleFlag)8)) text_colour=gui_def.TextBoxTextGameInfoColour;
    if (Text.ToCharString().StartsWith("(HC)")) text_colour=CRGBColour(128,128,128);
    CWideString formatted_text;
    C2DBoxF text_window;
    float screen_width=(float)GPDisplayManager->GetRenderTargetDimensions().X;
    C2DBoxF text_area(GFAXToVX(gui_def.TextBoxTextAreaTLPos.X),
                      ((float)(VIRTUAL_SCREEN_HEIGHT-GFAYToVY(GetBlackBarHeight())-GFAYToVY(gui_def.TextBoxTextAreaTLPos.Y))),
                      GFAXToVX((float)(gui_def.TextBoxTextAreaBRPos.X+screen_width-640.0f)),
                      (float)(VIRTUAL_SCREEN_HEIGHT-GFAYToVY(GetBlackBarHeight())-GFAYToVY(gui_def.TextBoxTextAreaBRPos.Y)));
    FormatTextForVWindow(text_area,*PFont,Text,formatted_text,text_window);
    if (IsFlagSet((ESubtitleFlag)8)) {
        float text_window_height=text_window.GetHeight();
        text_window.TLY=(float)((VIRTUAL_SCREEN_HEIGHT-text_window.GetHeight())/2.0f);
        text_window.BRY=text_window.TLY+text_window_height;
    }
    GPRenderManager->SetVWindow(text_window);
    CWideString visible_text;
    GetVisibleText(formatted_text,visible_text);
    C2DVector text_pos(0,0);
    CWStringParser parser(formatted_text),visible_parser(visible_text);
    CWideString text_line=parser.ReadAsStringUntilCharacter(10);
    while(text_line!=L"") {
        CWideString visible_line=visible_parser.ReadAsStringUntilCharacter(10);
        if(visible_line==L"") break;
        text_pos.X=0;
        if(!GLeftAlignText) text_pos.X=(float)((text_window.GetWidth()-PFont->GetVStringWidth(text_line))/2.0f);
        C2DVector draw_pos=text_pos+text_window.GetTLPos();
        Draw2DText(draw_pos,visible_line,*PFont,text_colour,true,2.0f,1.0f,false);
        text_pos.Y+=PFont->GetVFontMaxHeight();
        parser.SkipWhitespace();visible_parser.SkipWhitespace();
        text_line=parser.ReadAsStringUntilCharacter(10);
    }
    DrawTextEmbeddedGraphics(info,text_window);DrawTextEmbeddedHighlights(info,text_window);
    GPRenderManager->SetVWindow(v_full_window);
    return text_window;
}
// The following landed TU neighbours are visible to VC7.1. Their bodies prove that the box address does not escape.
inline CWideString CSubtitleRenderer::WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const
{
    CWideString wrapped;
    const wchar_t* line = text.GetAsCharArray();
    const wchar_t* next;
    do {
        float left = box.GetTLX();
        const wchar_t* lineEnd = 0;
        float right = box.GetBRX();
        next = WordWrap_FindNextLineW(line, (long)(right - left), &lineEnd);
        CWideString lineText;
        if (next)
            lineText.SetAsSubString(line, next - line);
        else
            lineText = line;
        if (lineText.GetChar(0) == L',') {
            wrapped.SetLength(wrapped.GetLength() - 1);
            wrapped += L",\n";
            lineText = lineText.GetSubString(1, lineText.GetLength());
        }
        wrapped += lineText + L"\n";
        line = next;
    } while (next);
    return wrapped;
}
inline void CSubtitleRenderer::FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                                             C2DBoxF& outBox) const
{
    PWordWrapFont = &font;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    outText = WordWrapText(box, font, text);
    float maxWidth = 0.0f;
    long lines = 0;
    CWStringParser parser(text);
    parser.Init(outText);
    CWideString line = parser.ReadAsStringUntilCharacter(L'\n');
    while (line != L"") {
        float width = font.GetVStringExtents(line).X;
        if (width > maxWidth)
            maxWidth = width;
        ++lines;
        parser.SkipWhitespace();
        line = parser.ReadAsStringUntilCharacter(L'\n');
    }
    if (!(DisplayFlags & 2) || (DisplayFlags & 0x28))
        ++lines;
    if (lines == 0)
        lines = 1;
    float scale = GPDisplayManager->GetRenderTargetDimensions().X * (1.0f / 640.0f);
    const CPlayerGuiDef* def = (&CPlayerGui::PeekGuiDef());
    GFLimit(maxWidth, GFAXToVX(def->TextBoxMinWidth) * scale, GFAXToVX(def->TextBoxMaxWidth) * scale);
    outBox = box;
    outBox.TLX = (float)((VIRTUAL_SCREEN_WIDTH - maxWidth) * 0.5);
    outBox.BRX = outBox.TLX + maxWidth;
    outBox.TLY = outBox.BRY - font.GetVFontMaxHeight() * lines;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
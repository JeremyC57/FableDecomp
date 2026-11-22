// 00491ad2 CSubtitleRenderer::ProcessEmbeddedHighlightTags
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
// Leaf: ?ProcessEmbeddedHighlightTags@CSubtitleRenderer@@AAEXXZ
// Flags: default
#define FABLE_RETAIL_CSUBTITLERENDERER_WIDESTRING_EQ_DECL_PASS28
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_CFONTBANK_STRINGWIDTH_TEXT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_GRAPHICTAG
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_string.h"
#ifdef _MSC_VER
#pragma optimize("t", on)
#endif
// Inline-budget: keep the original separately callable wide comparison visible at 00491D10.
namespace std {
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> int __cdecl __char_traits_base<unsigned short,unsigned short>::compare(const unsigned short* left, const unsigned short* right, size_t n)
{
    for (size_t i=0; i<n; ++i)
        if (left[i] != right[i]) return left[i] < right[i] ? -1 : 1;
    return 0;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
}
namespace std {
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> bool __cdecl operator==(const basic_string<unsigned short>& x,const basic_string<unsigned short>& y) {
    return x.size()==y.size() && char_traits<unsigned short>::compare(x.data(),y.data(),x.size())==0;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
}
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DExtentsI.h"
#include "engine/CFontBank.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CWStringParser.h"
#include "engine/CPlayerGuiDef.h"
#define FABLE_RETAIL_CSUBTITLERENDERER_PROCESSHIGHLIGHT_PASS28
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"


// Wide-string insertion belongs to the bound instantiation at 004128A0.
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma auto_inline(off)
#endif
// Retail 00442870 expands the element destruction loop. The separate storage-clearing helper at 004940E3 calls the tag overload.
namespace std {
template<> inline vector<CSubtitleRenderer::CEmbeddedHighlight>::~vector()
{
    for (iterator current = this->_M_start, last = this->_M_finish; current != last; ++current)
        _Destroy(&*current);
}
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#pragma optimize("s", on)
#endif
namespace std { template<> void vector<CWideString>::push_back(const CWideString&); }

extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern bool g_bLeftAlignText;                                            // 0x013B861B (descriptive: the "LeftAlignText" config flag)
float __fastcall GFVXToAX(float x);                                      // 0x009E1E10
long __fastcall GetNumberOfCharactersOfTypeInString(const CWideString& text, unsigned short character);   // 0x0048DBF9

void CSubtitleRenderer::ProcessEmbeddedHighlightTags()
{
    EmbeddedHighlights.clear();
    CWideString processed_text(Text);
    long start_tag_pos = processed_text.FindCharPos(L'<');
    while (start_tag_pos != -1) {
        long end_tag_pos = processed_text.FindCharPos(L'>');
        if (end_tag_pos == -1)
            break;
        {
            CWideString highlight_text = processed_text.GetSubString(start_tag_pos + 1, end_tag_pos);
            if (!(highlight_text != L""))
                break;
            CWideString prefix_text;
            if (start_tag_pos > 0)
                prefix_text = processed_text.GetSubString(0, start_tag_pos);
            CWideString suffix_text;
            if (end_tag_pos < processed_text.GetLength())
                suffix_text = processed_text.GetSubString(end_tag_pos + 1, processed_text.GetLength());
            processed_text = prefix_text + L"'" + highlight_text + L"'" + suffix_text;
            EmbeddedHighlightActionNames.push_back(L"'" + highlight_text + L"'");
        }
        start_tag_pos = processed_text.FindCharPos(L'<');
    }
    for (long i = 0; i < EmbeddedHighlightActionNames.size(); ++i) {
        CWideString action_name(EmbeddedHighlightActionNames[i]);
        if (action_name != L"") {
            long offset_index = 0;
            long i = 0;
            if (i < EmbeddedHighlights.size()) do {
                if (EmbeddedHighlights[i].Text == action_name)
                    offset_index = action_name.GetLength() + (EmbeddedHighlights.begin() + i)->CharIndex;
            } while (++i < EmbeddedHighlights.size());
            CWideString sub_processed_text = processed_text.GetSubString(offset_index, processed_text.GetLength());
            const unsigned short* pchar = wcsstr(sub_processed_text.GetAsCharArray(), action_name.GetAsCharArray());
            if (!pchar)
                break;
            long start_tag_pos = (long)(pchar - sub_processed_text.GetAsCharArray()) + offset_index;
            CEmbeddedHighlight embedded_highlight;
            embedded_highlight.Text = action_name;
            embedded_highlight.CharIndex = start_tag_pos;
            embedded_highlight.Split = false;
            EmbeddedHighlights.push_back(embedded_highlight);
        }
    }
    if (!EmbeddedHighlights.empty()) {
        const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
        CWideString formatted_text;
        C2DBoxF text_window;
        {
            float screen_width = (float)GPDisplayManager->GetRenderTargetDimensions().X;
            // Fidelity: the grouping around the top edge preserves the load order at 00491E8C.
            C2DBoxF text_area(GFAXToVX(gui_def.TextBoxTextAreaTLPos_X),
                        ((float)(VIRTUAL_SCREEN_HEIGHT - GFAYToVY(GetBlackBarHeight() - gui_def.TextBoxTextAreaTLPos_Y))),
                        GFAXToVX(gui_def.TextBoxTextAreaBRPos_X + screen_width - 640.0f),
                        (float)(VIRTUAL_SCREEN_HEIGHT - GFAYToVY(GetBlackBarHeight() - gui_def.TextBoxTextAreaBRPos_Y)));
            FormatTextForVWindow(text_area, *PFont, processed_text, formatted_text, text_window);
        }
        std::vector<CEmbeddedHighlight> new_highlights;
        for (long i = 0; i < EmbeddedHighlights.size(); ++i) {
            CWideString text_before_highlight = formatted_text.GetSubString(0, EmbeddedHighlights[i].CharIndex);
            long num_newlines_before_highlight = GetNumberOfCharactersOfTypeInString(text_before_highlight, L'\n');
            long offset_char_index = EmbeddedHighlights[i].CharIndex + num_newlines_before_highlight;
            CWideString formatted_highlight = formatted_text.GetSubString(offset_char_index, offset_char_index + EmbeddedHighlights[i].Text.GetLength());
            long newline_pos = formatted_highlight.FindCharPos(L'\n');
            if (newline_pos > -1) {
                formatted_highlight = formatted_text.GetSubString(offset_char_index, EmbeddedHighlights[i].Text.GetLength() + offset_char_index + 1);
                CWideString sub_highlight_1 = formatted_highlight.GetSubString(0, newline_pos);
                CWideString sub_highlight_2 = formatted_highlight.GetSubString(newline_pos + 1, formatted_highlight.GetLength());
                EmbeddedHighlights[i].Text = sub_highlight_1;
                CEmbeddedHighlight new_highlight;
                new_highlight.Text = sub_highlight_2;
                new_highlight.CharIndex = EmbeddedHighlights[i].CharIndex + newline_pos + 1;
                new_highlight.Split = true;
                new_highlights.push_back(new_highlight);
            }
        }
        for (long i = 0; i < new_highlights.size(); ++i)
            EmbeddedHighlights.push_back(new_highlights[i]);
        for (long i = 0; i < EmbeddedHighlights.size(); ++i) {
            CWideString sub_string = formatted_text.GetSubString(0, EmbeddedHighlights[i].CharIndex);
            long num_newlines = 0;
            bool new_lines = true;
            do {
                long new_num_newlines = GetNumberOfCharactersOfTypeInString(sub_string, L'\n');
                if (new_num_newlines == num_newlines)
                    new_lines = false;
                else {
                    EmbeddedHighlights[i].CharIndex += new_num_newlines;
                    sub_string = formatted_text.GetSubString(0, EmbeddedHighlights[i].CharIndex);
                    num_newlines = new_num_newlines;
                }
            } while (new_lines);
            if (sub_string.ToCharString().StartsWith("(HC)"))
                sub_string = sub_string.GetSubString(4, sub_string.GetLength());
            C2DExtentsF sub_string_extents_height = PFont->GetStringExtents(sub_string, false);
            CWideString last_line_sub_string(sub_string);
            CWStringParser sub_string_parser(sub_string);
            CWideString text_line = sub_string_parser.ReadAsStringUntilCharacter(L'\n');
            while (text_line != L"") {
                last_line_sub_string = text_line;
                sub_string_parser.SkipWhitespace();
                text_line = sub_string_parser.ReadAsStringUntilCharacter(L'\n');
            }
            C2DExtentsF sub_string_extents_width = PFont->GetStringExtents(last_line_sub_string, false);
            if (EmbeddedHighlights[i].Split)
                sub_string_extents_width.X = 0.0f;
            EmbeddedHighlights[i].Offset = C2DVector(sub_string_extents_width.X, sub_string_extents_height.Y);
            EmbeddedHighlights[i].Offset.Y -= (float)++num_newlines;
            if (EmbeddedHighlights[i].Offset.Y > 0.0f)
                EmbeddedHighlights[i].Offset.Y -= PFont->GetFontMaxHeight();
            if (!g_bLeftAlignText) {
                CWideString remaining_text = formatted_text.GetSubString(EmbeddedHighlights[i].CharIndex, formatted_text.GetLength());
                CWStringParser remaining_text_parser(remaining_text);
                CWideString remaining_line = remaining_text_parser.ReadAsStringUntilCharacter(L'\n');
                if (remaining_line == L"")
                    remaining_line = remaining_text;
                CWideString full_line = last_line_sub_string + remaining_line;
                float offset_x = (GFVXToAX(text_window.BRX - text_window.TLX) - PFont->GetStringWidth(full_line)) * 0.5f;
                EmbeddedHighlights[i].Offset.X += offset_x;
            }
        }
    }
    Text = processed_text;
}
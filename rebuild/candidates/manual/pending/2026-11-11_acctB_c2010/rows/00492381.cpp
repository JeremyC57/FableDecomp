// CSubtitleRenderer::ProcessEmbeddedGraphicTags() -- turn every [tag] in the subtitle text into an embedded button graphic: the tag is replaced by as many spaces as the graphic is wide, its graphic and character index are recorded; once the text is formatted for the GUI def's text area, each graphic's character index is moved past the inserted line breaks and its offset is set from the font extents of the text up to it (the last line), lowered by a line, and (unless the text is left aligned) shifted to the centred line start
// Leaf: ?ProcessEmbeddedGraphicTags@CSubtitleRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492381, 1522 bytes, thiscall plain `ret`; the frame-pointer /Os TU of the CSubtitleRenderer
//   neighbours (`push ebp ; lea ebp,[esp-78h] ; sub esp,9Ch`, `leave`), unpadded: the previous function (ProcessEmbeddedHighlightTags
//   0x00491AD2) ends with its `ret` at 0x00492380, this one ends `ret` at 0x00492972 (GetTextFromEmbeddedTagGraphic follows).
// Retail-asm: 0x00492485..0x0049249C `fld [ebp+40h] ; fsub [ebp+50h] ; fistp dword ptr [ebp+4Ch] ; fild [ebp+4Ch] ; fadd [ebp+64h] ;
//   fstp [ebp+44h] ; mov eax,[ebp+40h] ; cmp eax,[ebp+44h] ; jne ; inc dword ptr [ebp+4Ch]` -- the engine's inline-assembly
//   GFFloatToLongFloor (engine/GFFloatToLongFloor.h; its C++ attempt table: GOTCHAS rasm1 / ASM_RULES: `(long)x` = __ftol2, /QIfist
//   `fistp qword`), expanded once for the space count; the row reaches it only through the shared helper header.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ProcessEmbeddedGraphicTags@CSubtitleRenderer@@AAEXXZ (private, no arguments);
//   its two callers 0x004929F2 / 0x004933C6 are SetText 0x00492973 and ClickPast 0x00493281 of the same TU; the "[tag]" scan, GetGraphicFromEmbeddedTag
//   0x00490BE0 (its only caller is this row) and the EmbeddedGraphics records (Ego_r.pdb CEmbeddedGraphic {Graphic, CharIndex, Offset},
//   16 bytes, `sar eax,4`) tie the body to the name; ProcessEmbeddedHighlightTags 0x00491AD2 (just before) is the "<tag>" twin.
// Callees: vector<CEnginePrimitiveHandle>::erase 0x00440D70 / resize(n) 0x00443F90, vector<CEmbeddedGraphic>::erase 0x00493E2C /
//   push_back 0x0049417D, vector<CWideString>::erase 0x00412450 (the three clear() calls), FormatTextWithGameActionNames 0x004917B5,
//   CWideString FindCharPos 0x0099B070 / GetSubString 0x0099BDB0 / operator!=(const unsigned short*) 0x0099B150 / () 0x0099AED0 /
//   operator= 0x0099B7D0 / GetLength 0x0099B220 / += 0x0099B940 / += (CWideString) 0x0099B8D0 / copy 0x0099B720 / ~ 0x0099B510 /
//   ToCharString 0x0099B2C0 / operator const unsigned short* 0x0099AF00, the inline operator==(const unsigned short*) (wcscmp
//   0x00BFE94C / 0x00404280), operator+(CWideString, CWideString) 0x0099BE70, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0
//   with the inline StartsWith (strncmp 0x00BFEAF8), GetGraphicFromEmbeddedTag 0x00490BE0, CGraphicDataBank slot 23 GetGraphicMaxWidth,
//   CFontBank::GetStringWidth(const unsigned short*) 0x0048DA89 / GetStringWidth(const CWideString&) 0x0048DA62 / GetStringExtents
//   0x0048DAA2 / slot 2 GetFontMaxHeight, CPlayerGui::PeekGuiDef 0x00438570, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0
//   (GPDisplayManager 0x013B8390), GetBlackBarHeight 0x0048DBC3, GFAYToVY 0x009E1DD0 / GFAXToVX 0x009E1DA0 / GFVXToAX 0x009E1E10,
//   FormatTextForVWindow 0x0048F50C, GetNumberOfCharactersOfTypeInString 0x0048DBF9, CWStringParser(const unsigned short*) 0x00437260 /
//   ReadAsStringUntilCharacter 0x00A0CCD0 / SkipWhitespace 0x00A0C910 / implicit dtor (~CBaseClass 0x0099A2E0).
// Data: L"" 0x0122D70C, L" " 0x0122D820, "(HC)" 0x0123172C, 2^32 0x0122DCB4 (the unsigned width), 640.0f 0x01238168, 0.0f 0x0122DEDC,
//   0.5f 0x0122F59C, VIRTUAL_SCREEN_HEIGHT 0x013961F0, the LeftAlignText config flag 0x013B861B (descriptive g_bLeftAlignText).
// Shape (chunk 2010, measured with the c2 frame-packer trace, tools/pk.sh): the CEmbeddedGraphic record lives in its own block that
//   closes before the GFFloatToLongFloor expansion: VC7.1 counts every local in scope at an inline-asm block as referenced (+2 IL
//   weight each), so a record still in scope there weighs 5 and is packed with the extents / box temporaries; at weight 3 it packs
//   after the box and the parser and shares the parser's frame words as retail ([ebp+14h]). The newline-count loop is a `done` flag
//   loop (retail tests the zero in memory every pass, 0x0049265F..0x004926AA; a for(;;)/break or while(cond) loop is rotated and its
//   first test peeled against the known 0 -- the residue of the parked twin 0x00491AD2). The text-area box is a temporary argument
//   (a named box adds frame words, 0xAC v 0x9C) and its Y arguments convert the double implicitly (an explicit (float)(...) cast
//   leaves a no-code tuple that schedules the TLX store before the TLY copy). The centring offset is a named float added with +=
//   (retail forms the record address after the GetStringWidth call; `X += expr` forms it first and spills it).
#pragma optimize("s", on)
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
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
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
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/GFFloatToLongFloor.h"

extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern bool g_bLeftAlignText;                                            // 0x013B861B (descriptive: the "LeftAlignText" config flag)
float __fastcall GFVXToAX(float x);                                      // 0x009E1E10
long __fastcall GetNumberOfCharactersOfTypeInString(const CWideString& text, unsigned short character);   // 0x0048DBF9

void CSubtitleRenderer::ProcessEmbeddedGraphicTags()
{
    EmbeddedPrimitiveHandles.clear();
    EmbeddedGraphics.clear();
    EmbeddedHighlightActionNames.clear();
    CWideString text = FormatTextWithGameActionNames(Text, &EmbeddedHighlightActionNames);
    long start;
    while ((start = text.FindCharPos(L'[')) != -1) {
        long end = text.FindCharPos(L']');
        if (end == -1)
            break;
        CWideString tag = text.GetSubString(start + 1, end);
        if (!(tag != L""))
            break;
        long graphic = GetGraphicFromEmbeddedTag(tag);
        if (graphic <= 0)
            break;
        {
            CEmbeddedGraphic embedded;
            embedded.Graphic = graphic;
            embedded.CharIndex = start;
            EmbeddedGraphics.push_back(embedded);
        }
        float graphic_width = (float)Graphics->GetGraphicMaxWidth(graphic);
        long spaces = GFFloatToLongFloor(graphic_width / PFont->GetStringWidth(L" "));
        CWideString before;
        if (start > 0)
            before = text.GetSubString(0, start);
        CWideString after;
        if (end < text.GetLength())
            after = text.GetSubString(end + 1, text.GetLength());
        text = before;
        for (long i = 0; i < spaces; ++i)
            text += L" ";
        text += after;
    }
    if (!EmbeddedGraphics.empty()) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        CWideString formatted;
        float width = (float)GPDisplayManager->GetRenderTargetDimensions().X;
        C2DBoxF formatted_box;
        FormatTextForVWindow(C2DBoxF(GFAXToVX(def.TextBoxTextAreaTLPos_X),
                                     VIRTUAL_SCREEN_HEIGHT - GFAYToVY(GetBlackBarHeight() - def.TextBoxTextAreaTLPos_Y),
                                     GFAXToVX(width + def.TextBoxTextAreaBRPos_X - 640.0f),
                                     VIRTUAL_SCREEN_HEIGHT - GFAYToVY(GetBlackBarHeight() - def.TextBoxTextAreaBRPos_Y)),
                             *PFont, text, formatted, formatted_box);
        for (unsigned long i = 0; i < EmbeddedGraphics.size(); ++i) {
            CWideString before = formatted.GetSubString(0, EmbeddedGraphics[i].CharIndex);
            bool done = false;
            long last_line_breaks = 0;
            while (!done) {
                long line_breaks = GetNumberOfCharactersOfTypeInString(before, L'\n');
                if (line_breaks == last_line_breaks)
                    done = true;
                else {
                    EmbeddedGraphics[i].CharIndex += line_breaks;
                    before = formatted.GetSubString(0, EmbeddedGraphics[i].CharIndex);
                    last_line_breaks = line_breaks;
                }
            }
            if (before.ToCharString().StartsWith("(HC)"))
                before = before.GetSubString(4, before.GetLength());
            C2DExtentsF extents = PFont->GetStringExtents(before, false);
            CWideString last_line(before);
            CWStringParser parser(before);
            CWideString line = parser.ReadAsStringUntilCharacter(L'\n');
            while (line != L"") {
                last_line = line;
                parser.SkipWhitespace();
                line = parser.ReadAsStringUntilCharacter(L'\n');
            }
            C2DExtentsF last_extents = PFont->GetStringExtents(last_line, false);
            EmbeddedGraphics[i].Offset = C2DVector(last_extents.X, extents.Y);
            if (EmbeddedGraphics[i].Offset.Y > 0.0f)
                EmbeddedGraphics[i].Offset.Y -= PFont->GetFontMaxHeight();
            if (!g_bLeftAlignText) {
                CWideString rest = formatted.GetSubString(EmbeddedGraphics[i].CharIndex, formatted.GetLength());
                CWStringParser rest_parser(rest);
                CWideString word = rest_parser.ReadAsStringUntilCharacter(L'\n');
                if (word == L"")
                    word = rest;
                CWideString whole_line = last_line + word;
                float box_width = GFVXToAX(formatted_box.BRX - formatted_box.TLX);
                float offset = (box_width - PFont->GetStringWidth(whole_line)) * 0.5f;
                EmbeddedGraphics[i].Offset.X += offset;
            }
        }
        EmbeddedPrimitiveHandles.resize(EmbeddedGraphics.size());
    }
    Text = text;
}

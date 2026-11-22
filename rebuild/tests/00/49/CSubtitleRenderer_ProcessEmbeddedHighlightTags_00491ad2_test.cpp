// Execute the reconstructed subtitle pass using deterministic font metrics and word wrapping.
#include <string>
#include <vector>
#include <cwchar>
#include <cstdio>
#include <cassert>
struct CCharString {
    std::string value;
    bool StartsWith(const char* p) const { return value.find(p)==0; }
};
struct CWideString {
    std::wstring value;
    CWideString() {}
    CWideString(const wchar_t* s):value(s) {}
    CWideString(const std::wstring& s):value(s) {}
    long FindCharPos(wchar_t c) const {
        std::wstring::size_type n=value.find(c);
        return n==std::wstring::npos ? -1L : (long)n;
    }
    CWideString GetSubString(long first,long end) const {
        assert(first>=0 && first<=GetLength() && end>=first);
        return value.substr(first,end-first);
    }
    const wchar_t* GetAsCharArray() const { return value.c_str(); }
    long GetLength() const { return (long)value.size(); }
    CCharString ToCharString() const {
        CCharString s; for(size_t i=0;i<value.size();++i) s.value+=static_cast<char>(value[i]); return s;
    }
};
static CWideString operator+(const CWideString& lhs,const CWideString& rhs) { return lhs.value+rhs.value; }
static bool operator==(const CWideString& lhs,const CWideString& rhs) { return lhs.value==rhs.value; }
static bool operator!=(const CWideString& lhs,const CWideString& rhs) { return !(lhs==rhs); }
struct CWStringParser {
    std::wstring value; size_t at;
    CWStringParser(const CWideString& s):value(s.value),at(0) {}
    CWideString ReadAsStringUntilCharacter(wchar_t c) {
        size_t end=value.find(c,at);
        if(end==std::wstring::npos) end=value.size();
        CWideString s(value.substr(at,end-at)); at=end; return s;
    }
    void SkipWhitespace() { while(at<value.size() && (value[at]==L'\n' || value[at]==L' ')) ++at; }
};
struct C2DVector {
    float X,Y;
    C2DVector(float x=0,float y=0):X(x),Y(y) {}
};
typedef C2DVector C2DExtentsF;
struct C2DBoxF {
    float TLX,TLY,BRX,BRY;
    C2DBoxF(float l=0,float t=0,float r=0,float b=0):TLX(l),TLY(t),BRX(r),BRY(b) {}
};
struct CPlayerGuiDef {float TextBoxTextAreaTLPos_X,TextBoxTextAreaTLPos_Y,TextBoxTextAreaBRPos_X,TextBoxTextAreaBRPos_Y;};
struct CPlayerGui {static const CPlayerGuiDef& PeekGuiDef() {static const CPlayerGuiDef d={0,0,80,16}; return d;}};
struct DisplayManager {
    C2DVector GetRenderTargetDimensions() const {return C2DVector(640,480);}
};
static DisplayManager display;
static DisplayManager* GPDisplayManager=&display;
static float GetBlackBarHeight(){return 0;}
static const double VIRTUAL_SCREEN_HEIGHT=480;
static float GFAXToVX(float v){return v;}
static float GFAYToVY(float v){return v;}
static float GFVXToAX(float v){return v;}
static bool g_bLeftAlignText=true;
static long GetNumberOfCharactersOfTypeInString(const CWideString& s,wchar_t c){
    long total=0;for(size_t i=0;i<s.value.size();++i) if(s.value[i]==c)++total;return total;
}
struct Font {
    float GetFontMaxHeight()const{return 8;}
    float GetStringWidth(const CWideString& s)const{return static_cast<float>(s.GetLength());}
    C2DExtentsF GetStringExtents(const CWideString& s,bool)const{
        return C2DExtentsF(GetStringWidth(s),8.0f*(1+GetNumberOfCharactersOfTypeInString(s,L'\n')));
    }
};
class CSubtitleRenderer {
public:
    struct CEmbeddedHighlight {CWideString Text;long CharIndex;C2DVector Offset;bool Split;};
    CWideString Text;
    std::vector<CWideString> EmbeddedHighlightActionNames;
    std::vector<CEmbeddedHighlight> EmbeddedHighlights;
    Font font;Font* PFont;unsigned wrap;int calls;
    CSubtitleRenderer(const wchar_t* text,unsigned width):Text(text),PFont(&font),wrap(width),calls(0){}
    void FormatTextForVWindow(const C2DBoxF& area,const Font&,const CWideString& input,CWideString& out,C2DBoxF& window) {
        assert(area.TLX==0 && area.BRX==80 && area.TLY==480 && area.BRY==496);
        window=area; ++calls;
        for(size_t i=0;i<input.value.size();++i){if(i && i%wrap==0)out.value+=L'\n';out.value+=input.value[i];}
    }
    void ProcessEmbeddedHighlightTags();
};

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
            const wchar_t* pchar = wcsstr(sub_processed_text.GetAsCharArray(), action_name.GetAsCharArray());
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

int main() {
    CSubtitleRenderer plain(L"plain <open",100);plain.ProcessEmbeddedHighlightTags();
    assert(plain.calls==0 && plain.Text==L"plain <open");
    CSubtitleRenderer empty(L"<>",100);empty.ProcessEmbeddedHighlightTags();
    assert(empty.EmbeddedHighlights.empty() && empty.Text==L"<>");
    CSubtitleRenderer repeated(L"<A> + <A>",100);repeated.ProcessEmbeddedHighlightTags();
    assert(repeated.calls==1 && repeated.Text==L"'A' + 'A'" && repeated.EmbeddedHighlights.size()==2);
    assert(repeated.EmbeddedHighlights[0].CharIndex==0 && repeated.EmbeddedHighlights[1].CharIndex==6);
    assert(repeated.EmbeddedHighlights[1].Offset.X==6 && !repeated.EmbeddedHighlights[0].Split);
    CSubtitleRenderer split(L"abcdefg<XYZ>",8);split.ProcessEmbeddedHighlightTags();
    assert(split.EmbeddedHighlights.size()==2 && split.EmbeddedHighlights[1].Split);
    assert(split.EmbeddedHighlights[0].Text==L"'" && split.EmbeddedHighlights[1].Text==L"XYZ'");
    assert(split.EmbeddedHighlights[1].Offset.X==0 && split.EmbeddedHighlights[1].Offset.Y>0);
    g_bLeftAlignText=false;
    CSubtitleRenderer centered(L"(HC)<X>",100);centered.ProcessEmbeddedHighlightTags();
    assert(centered.EmbeddedHighlights.size()==1 && centered.EmbeddedHighlights[0].Offset.X==38.5f);
    CSubtitleRenderer missing(L"entry",100);missing.EmbeddedHighlightActionNames.push_back(L"");
    missing.EmbeddedHighlightActionNames.push_back(L"unknown");missing.EmbeddedHighlightActionNames.push_back(L"entry");
    missing.ProcessEmbeddedHighlightTags();assert(missing.EmbeddedHighlights.empty() && missing.calls==0);
    puts("00491ad2_TEST PASS");
    return 0;
}
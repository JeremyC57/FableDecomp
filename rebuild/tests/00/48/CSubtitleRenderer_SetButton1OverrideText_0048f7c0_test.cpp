#include <stdio.h>
#include <string.h>
// Behaviour test for CSubtitleRenderer::SetButton1OverrideText (0x0048F7C0): outside cutscene action mode an empty
// text (unassigned or "") restores the default TEXT_GUI_NEXT caption and clears the override flag; any other text is
// looked up as a text tag (falling back to the text itself) and sets the flag.
struct CBasicStringChar {
    const char* PData;
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
class CCharStringData { public: CBasicStringChar Data; };
class CWideString { public:
    const char* Text;
    CWideString() : Text("") {}
    CWideString(const char* text) : Text(text) {}
    const CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; }
};
class CCharString { public:
    CCharStringData* PStringData;
    CWideString ToWideString() const { return CWideString(PStringData->Data.PData); }
};
static const char* s_Default = "TEXT_GUI_NEXT";
class CSubtitleRenderer { public:
    CWideString Button1OverrideText;
    bool CutsceneActionMode;
    bool Button1TextOverridden;
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text)
    {
        if (strcmp(tag.Text, "TEXT_GUI_NEXT") == 0) { text = CWideString("Next"); return true; }
        if (strcmp(tag.Text, "TEXT_TAG") == 0) { text = CWideString("Tagged"); return true; }
        return false;
    }
    void SetButton1OverrideText(const CCharString& text);
};
void CSubtitleRenderer::SetButton1OverrideText(const CCharString& text)
{
    if (CutsceneActionMode)
        return;
    if (text.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (text.PStringData->Data == "")
        goto empty;
    if (!GetTextFromTextTag(text.ToWideString(), Button1OverrideText))
        Button1OverrideText = text.ToWideString();
    Button1TextOverridden = true;
    return;
empty:
    GetTextFromTextTag(s_Default, Button1OverrideText);
    Button1TextOverridden = false;
}
int main()
{
    int ok = 1;
    CSubtitleRenderer r; r.CutsceneActionMode = false; r.Button1TextOverridden = true; r.Button1OverrideText = CWideString("x");
    CCharStringData tag; tag.Data.PData = "TEXT_TAG";
    CCharStringData plain; plain.Data.PData = "Hello";
    CCharStringData emptyData; emptyData.Data.PData = "";
    CCharString s; s.PStringData = 0;
    r.SetButton1OverrideText(s);
    if (strcmp(r.Button1OverrideText.Text, "Next") != 0 || r.Button1TextOverridden) ok = 0;
    s.PStringData = &tag; r.SetButton1OverrideText(s);
    if (strcmp(r.Button1OverrideText.Text, "Tagged") != 0 || !r.Button1TextOverridden) ok = 0;
    s.PStringData = &plain; r.SetButton1OverrideText(s);
    if (strcmp(r.Button1OverrideText.Text, "Hello") != 0 || !r.Button1TextOverridden) ok = 0;
    s.PStringData = &emptyData; r.SetButton1OverrideText(s);
    if (strcmp(r.Button1OverrideText.Text, "Next") != 0 || r.Button1TextOverridden) ok = 0;
    r.CutsceneActionMode = true; s.PStringData = &plain; r.SetButton1OverrideText(s);
    if (strcmp(r.Button1OverrideText.Text, "Next") != 0 || r.Button1TextOverridden) ok = 0;
    if (ok) { printf("0048f7c0_TEST PASS\n"); return 0; }
    printf("0048f7c0_TEST FAIL\n"); return 1;
}
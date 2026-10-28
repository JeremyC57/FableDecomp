#include <stdio.h>
#include <string.h>
// Behaviour test for CSubtitleRenderer::SetButton2OverrideText (0x0048F881): an empty
// text (unassigned or "") restores the default TEXT_GUI_SKIP_SCENE caption (no cutscene-mode guard) and clears the override flag; any other text is
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
static const char* s_Default = "TEXT_GUI_SKIP_SCENE";
class CSubtitleRenderer { public:
    CWideString Button2OverrideText;
    bool CutsceneActionMode;
    bool Button2TextOverridden;
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text)
    {
        if (strcmp(tag.Text, "TEXT_GUI_SKIP_SCENE") == 0) { text = CWideString("Skip"); return true; }
        if (strcmp(tag.Text, "TEXT_TAG") == 0) { text = CWideString("Tagged"); return true; }
        return false;
    }
    void SetButton2OverrideText(const CCharString& text);
};
void CSubtitleRenderer::SetButton2OverrideText(const CCharString& text)
{
    if (text.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (text.PStringData->Data == "")
        goto empty;
    if (!GetTextFromTextTag(text.ToWideString(), Button2OverrideText))
        Button2OverrideText = text.ToWideString();
    Button2TextOverridden = true;
    return;
empty:
    GetTextFromTextTag(s_Default, Button2OverrideText);
    Button2TextOverridden = false;
}
int main()
{
    int ok = 1;
    CSubtitleRenderer r; r.CutsceneActionMode = false; r.Button2TextOverridden = true; r.Button2OverrideText = CWideString("x");
    CCharStringData tag; tag.Data.PData = "TEXT_TAG";
    CCharStringData plain; plain.Data.PData = "Hello";
    CCharStringData emptyData; emptyData.Data.PData = "";
    CCharString s; s.PStringData = 0;
    r.SetButton2OverrideText(s);
    if (strcmp(r.Button2OverrideText.Text, "Skip") != 0 || r.Button2TextOverridden) ok = 0;
    s.PStringData = &tag; r.SetButton2OverrideText(s);
    if (strcmp(r.Button2OverrideText.Text, "Tagged") != 0 || !r.Button2TextOverridden) ok = 0;
    s.PStringData = &plain; r.SetButton2OverrideText(s);
    if (strcmp(r.Button2OverrideText.Text, "Hello") != 0 || !r.Button2TextOverridden) ok = 0;
    s.PStringData = &emptyData; r.SetButton2OverrideText(s);
    if (strcmp(r.Button2OverrideText.Text, "Skip") != 0 || r.Button2TextOverridden) ok = 0;
    r.CutsceneActionMode = true; s.PStringData = &plain; r.SetButton2OverrideText(s);
    if (strcmp(r.Button2OverrideText.Text, "Hello") != 0 || !r.Button2TextOverridden) ok = 0;
    if (ok) { printf("0048f881_TEST PASS\n"); return 0; }
    printf("0048f881_TEST FAIL\n"); return 1;
}
// Behaviour test for 0x0048E012 CSubtitleRenderer::GetCharWidthW (model copy over stand-in string/font).
#include <cstdio>

typedef unsigned short wchar_t16;

static int g_liveStrings = 0;

struct CWideStringModel {
    wchar_t16 m_buf[4];
    CWideStringModel() { m_buf[0] = 0; ++g_liveStrings; }
    ~CWideStringModel() { --g_liveStrings; }
};

static void FormatChar(CWideStringModel* text, wchar_t16 c) { text->m_buf[0] = c; text->m_buf[1] = 0; }

class C2DExtentsF { public: float Width, Height; C2DExtentsF(float w, float h) : Width(w), Height(h) { } };

struct CFontBankModel {
    C2DExtentsF GetVStringExtents(const CWideStringModel& text) const
    {
        // 'W' is wide, everything else narrow; fractional widths are truncated by the caller
        return C2DExtentsF(text.m_buf[0] == 'W' ? 12.9f : 5.25f, 20.0f);
    }
};
static const CFontBankModel g_font = CFontBankModel();
static const CFontBankModel* g_pWordWrapFont = &g_font;

static unsigned int GetCharWidthModel(wchar_t16 character)
{
    CWideStringModel text;
    FormatChar(&text, character);
    return (unsigned int)g_pWordWrapFont->GetVStringExtents(text).Width;
}

int main()
{
    unsigned int w = GetCharWidthModel('W');
    unsigned int i = GetCharWidthModel('i');
    if (w != 12 || i != 5 || g_liveStrings != 0) { std::printf("0048e012_TEST FAIL\n"); return 1; }
    std::printf("0048e012_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x004168DC (CMainGameComponent::InitialiseFonts): progress text then the named font fetched into the component.
#include <cstdio>
#include <cstring>
static const char* g_progress; static float g_fraction = 0.0f;
static void DisplayProgress(const char* t, bool, float f, bool) { g_progress = t; g_fraction = f; }
struct Font { const char* name; };
static Font g_fonts[2] = { { "ENG_ARIAL_12" }, { "ENG_ARIAL_18" } };
struct Bank { Font* GetFont(const char* name) { for (int i = 0; i < 2; ++i) if (std::strcmp(g_fonts[i].name, name) == 0) return &g_fonts[i]; return 0; } };
static Bank g_bank;
struct Component {
    Font* m_font;
    void InitialiseFonts()
    {
        { DisplayProgress("Init Fonts", false, -1.0f, false); }
        { m_font = g_bank.GetFont("ENG_ARIAL_18"); }
    }
};
int main()
{
    Component c; c.m_font = 0; c.InitialiseFonts();
    int ok = c.m_font == &g_fonts[1] && std::strcmp(g_progress, "Init Fonts") == 0 && g_fraction == -1.0f;
    if (!ok) { std::printf("004168dc_TEST FAIL\n"); return 1; }
    std::printf("004168dc_TEST PASS\n");
    return 0;
}

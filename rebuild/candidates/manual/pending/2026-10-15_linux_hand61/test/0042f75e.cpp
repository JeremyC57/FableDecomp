#include <cstdio>
// Behaviour test for CNewFrontendGameComponent::Init @ 0x0042F75E
class CFontBank { public: int m_id; };
static int g_step = 0; static int g_defsStep = 0, g_xmvStep = 0, g_consoleStep = 0;
static char g_char = 0; static long g_key = 0; static CFontBank* g_font = 0;
class CConsole {
public:
    static CConsole* Get() { static CConsole console; return &console; }
    void Initialise(char toggleCharacter, long toggleKey, CFontBank* font) { g_consoleStep = ++g_step; g_char = toggleCharacter; g_key = toggleKey; g_font = font; }
};
static void StaticLoadXMVCode() { g_xmvStep = ++g_step; }
class CNewFrontendGameComponent;
static CNewFrontendGameComponent* g_pNewFrontendGameComponent = 0;
class CNewFrontendGameComponent {
public:
    CFontBank* m_pMenuFont;
    bool XMVCodeLoaded;
    void InitialiseDefs() { g_defsStep = ++g_step; }
    void Init()
    {
        g_pNewFrontendGameComponent = this;
        InitialiseDefs();
        StaticLoadXMVCode();
        XMVCodeLoaded = true;
        CConsole::Get()->Initialise('`', 0x29, m_pMenuFont);
    }
};
int main()
{
    CFontBank font; CNewFrontendGameComponent c; c.m_pMenuFont = &font; c.XMVCodeLoaded = false;
    c.Init();
    if (g_pNewFrontendGameComponent != &c || !c.XMVCodeLoaded || g_defsStep != 1 || g_xmvStep != 2 || g_consoleStep != 3 ||
        g_char != '`' || g_key != 0x29 || g_font != &font) { std::printf("0042f75e_TEST FAIL\n"); return 1; }
    std::printf("0042f75e_TEST PASS\n");
    return 0;
}

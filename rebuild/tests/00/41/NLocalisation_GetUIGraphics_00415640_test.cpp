// Behaviour test for 0x00415640 NLocalisation::GetUIGraphics: maps each detected language to its UI_LOCALE_GRAPHICS_* def
// (English for every language without its own graphics) and fetches it into the caller's def pointer.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    CCharString(const char* text, long length = -1) : m_text(text) { ++g_live; }
    ~CCharString() { --g_live; }
    const char* m_text;
};
template <class T> class CDefPointer {
public:
    const char* Object;
};
class CUILocaleGraphicsDef;
class CDefinitionManager {
public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* def) const { def->Object = name.m_text; return true; }
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
};

static unsigned long g_Language;
unsigned long GetLanguage() { return g_Language; }

void GetUIGraphics(CDefPointer<const CUILocaleGraphicsDef>& graphics)
{
    switch (GetLanguage()) {
    case 0x0C: CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_FRENCH", &graphics); break;
    case 0x11: CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_JAPANESE", &graphics); break;
    case 7:    CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_GERMAN", &graphics); break;
    case 0x12: CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_KOREAN", &graphics); break;
    case 4:    CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_TCHINESE", &graphics); break;
    case 0x0A: CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_SPANISH", &graphics); break;
    case 0x10: CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_ITALIAN", &graphics); break;
    default:   CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_ENGLISH", &graphics); break;
    }
}

static int Check(unsigned long language, const char* expect)
{
    CDefPointer<const CUILocaleGraphicsDef> def; def.Object = 0;
    g_Language = language;
    GetUIGraphics(def);
    return def.Object != 0 && std::strcmp(def.Object, expect) == 0;
}

int main()
{
    int ok = 1;
    ok &= Check(4, "UI_LOCALE_GRAPHICS_TCHINESE");
    ok &= Check(7, "UI_LOCALE_GRAPHICS_GERMAN");
    ok &= Check(9, "UI_LOCALE_GRAPHICS_ENGLISH");
    ok &= Check(0x0A, "UI_LOCALE_GRAPHICS_SPANISH");
    ok &= Check(0x0C, "UI_LOCALE_GRAPHICS_FRENCH");
    ok &= Check(0x10, "UI_LOCALE_GRAPHICS_ITALIAN");
    ok &= Check(0x11, "UI_LOCALE_GRAPHICS_JAPANESE");
    ok &= Check(0x12, "UI_LOCALE_GRAPHICS_KOREAN");
    ok &= Check(5, "UI_LOCALE_GRAPHICS_ENGLISH");
    ok &= Check(0, "UI_LOCALE_GRAPHICS_ENGLISH");
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00415640_TEST FAIL\n"); return 1; }
    std::printf("00415640_TEST PASS\n");
    return 0;
}
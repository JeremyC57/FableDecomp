// NLocalisation::GetUIGraphics(CDefPointer<const NUISystem::CUILocaleGraphicsDef>& graphics) -- fetch the UI_LOCALE_GRAPHICS_<LANGUAGE> def for the detected language (English by default)
// Leaf: ?GetUIGraphics@NLocalisation@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415640, 421 bytes of code + the 8-entry jump table at 0x004157E8 and the 15-entry
//   byte index table at 0x00415808 (471 in all), `ret`; FableWin.pdb
//   `?GetUIGraphics@NLocalisation@@YIXAAV?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@@Z` (__fastcall, the out
//   def pointer in ecx). Callers: 0x004835CD, 0x006BC393.
// Callees: NLocalisation::GetLanguage 0x00415070, CCharString(const char*, long) 0x0099EBF0, CGameDefinitionManager::Get
//   0x0044C6B0 (static __fastcall, returns the manager), CDefinitionManager::GetDef<NUISystem::CUILocaleGraphicsDef>
//   (const CCharString&, CDefPointer<const T>*) 0x00415890, ~CCharString 0x0099EAE0.
// Switch: `language - 4` through the byte table; verified case-to-literal pairing against the retail tables:
//   4 Chinese -> UI_LOCALE_GRAPHICS_TCHINESE (0x0122EC08), 7 German (0x0122EC40), 0x0A Spanish (0x0122EBEC),
//   0x0C French (0x0122EC78), 0x10 Italian (0x0122EBD0), 0x11 Japanese (0x0122EC5C), 0x12 Korean (0x0122EC24),
//   everything else (English 9 included) -> UI_LOCALE_GRAPHICS_ENGLISH (0x0122EBB4). The case bodies are laid out in
//   source order (French, Japanese, German, Korean, Chinese, Spanish, Italian, default), each with its own frame word for
//   the string and its own `pop esi ; add esp,20h ; ret` tail.
// Shape: the def name is an IMPLICIT CCharString conversion (re-addressed with `lea` after the out-pointer push); both
//   arguments are pushed before the manager getter runs.
class CCharString {
public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
private:
    void* m_pData;
};

class CDefPointeeBase;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
};

namespace NUISystem { class CUILocaleGraphicsDef; }

class CDefinitionManager {
public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* def) const;   // 0x00415890 (T = CUILocaleGraphicsDef)
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};

namespace NLocalisation {

enum ELanguageCode {
    LANGUAGE_CHINESE  = 4,
    LANGUAGE_GERMAN   = 7,
    LANGUAGE_ENGLISH  = 9,
    LANGUAGE_SPANISH  = 0x0A,
    LANGUAGE_FRENCH   = 0x0C,
    LANGUAGE_ITALIAN  = 0x10,
    LANGUAGE_JAPANESE = 0x11,
    LANGUAGE_KOREAN   = 0x12
};

unsigned long __fastcall GetLanguage();                  // 0x00415070

void __fastcall GetUIGraphics(CDefPointer<const NUISystem::CUILocaleGraphicsDef>& graphics)
{
    switch (GetLanguage()) {
    case LANGUAGE_FRENCH:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_FRENCH", &graphics);
        break;
    case LANGUAGE_JAPANESE:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_JAPANESE", &graphics);
        break;
    case LANGUAGE_GERMAN:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_GERMAN", &graphics);
        break;
    case LANGUAGE_KOREAN:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_KOREAN", &graphics);
        break;
    case LANGUAGE_CHINESE:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_TCHINESE", &graphics);
        break;
    case LANGUAGE_SPANISH:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_SPANISH", &graphics);
        break;
    case LANGUAGE_ITALIAN:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_ITALIAN", &graphics);
        break;
    default:
        CGameDefinitionManager::Get().GetDef("UI_LOCALE_GRAPHICS_ENGLISH", &graphics);
        break;
    }
}

}
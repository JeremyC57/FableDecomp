// NLocalisation::GetLanguage() -- detect (once) the installed text language by probing which TEXT_<LANGUAGE>_MAIN bank exists, and cache it
// Leaf: ?GetLanguage@NLocalisation@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415070, 495 bytes (496 with padding), `ret`; FableWin.pdb names it
//   `?GetLanguage@NLocalisation@@YIKXZ` (unsigned long __fastcall NLocalisation::GetLanguage(void)).
// Callers: the language-dependent bank-name builders 0x00415263/0x00415353/0x00415443/0x00415553/0x00415646, 0x0042F13D.
// Callees: CCharString(const char*, long = -1) 0x0099EBF0, CBankFileManager::BankExists(const CCharString&) 0x009A80B0 on
//   the global bank-file manager 0x013CA79C, ~CCharString 0x0099EAE0.
// Global: the cached language 0x013B8684 (0 = not detected yet).
// Shape: one `push ecx` frame word holds every probe's CCharString; each probe is a `__forceinline` helper (plain `inline`
//   stays out of line at eight call sites) with a NAMED string local
//   (`lea eax,[esp+4] ; push eax` after the constructor -- a temporary bound straight to the reference reuses the
//   constructor's eax), the result kept in bl across the destructor; each hit is `return g = N;` (`mov eax,N ; mov [g],eax`).
//   The cached value is an early `if (g != 0) return g;` (retail pushes ebx only after that test; the `if (g == 0) { ... }`
//   form hoists the push above the branch), and the final fall-through re-reads the global.
// Probe order and codes are retail's: English 9, French 0x0C, Japanese 0x11, German 7, Korean 0x12, Chinese 4,
//   Spanish 0x0A, Italian 0x10 (descriptive enumerator names; the PDB enum is not in the name sets).
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
private:
    void* m_pData;
};

class CBankFileManager {
public:
    bool BankExists(const CCharString& bank) const;     // 0x009A80B0
};

extern CBankFileManager g_BankFileManager;               // 0x013CA79C
extern unsigned long g_Language;                         // 0x013B8684

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

static __forceinline bool LanguageBankExists(const char* bank)
{
    CCharString name(bank);
    return g_BankFileManager.BankExists(name);
}

unsigned long __fastcall GetLanguage()
{
    if (g_Language != 0)
        return g_Language;
    if (LanguageBankExists("TEXT_ENGLISH_MAIN"))
        return g_Language = LANGUAGE_ENGLISH;
    if (LanguageBankExists("TEXT_FRENCH_MAIN"))
        return g_Language = LANGUAGE_FRENCH;
    if (LanguageBankExists("TEXT_JAPANESE_MAIN"))
        return g_Language = LANGUAGE_JAPANESE;
    if (LanguageBankExists("TEXT_GERMAN_MAIN"))
        return g_Language = LANGUAGE_GERMAN;
    if (LanguageBankExists("TEXT_KOREAN_MAIN"))
        return g_Language = LANGUAGE_KOREAN;
    if (LanguageBankExists("TEXT_CHINESE_MAIN"))
        return g_Language = LANGUAGE_CHINESE;
    if (LanguageBankExists("TEXT_SPANISH_MAIN"))
        return g_Language = LANGUAGE_SPANISH;
    if (LanguageBankExists("TEXT_ITALIAN_MAIN"))
        return g_Language = LANGUAGE_ITALIAN;
    return g_Language;
}

}
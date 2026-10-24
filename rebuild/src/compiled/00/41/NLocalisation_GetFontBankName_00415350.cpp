// NLocalisation::GetFontBankName() -- name of the main font bank for the current game language
// Leaf: ?GetFontBankName@NLocalisation@@YI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415350, length 0xE3 bytes of code plus the inline
// jump table (8 dwords @ 0x00415404) and byte index table (15 bytes @ 0x00415424), `ret` (no stack args).
// Name: Ego_r.pdb `?GetFontBankName@NLocalisation@@YI?AVCCharString@@XZ` (a `__fastcall` free function in the
// NLocalisation namespace returning CCharString by value; the hidden result pointer arrives in ecx and is
// returned in eax). It sits between GetTextBankName (0x00415260, "TEXT_*_MAIN") and GetStreamingFontBankName
// (0x00415440, "STREAMING_FONT_*_PC") exactly as the donor PDB orders them; the strings it builds are
// "FONT_<LANGUAGE>_MAIN".
// Callees: NLocalisation::GetLanguage @ 0x00415070, CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Shape: `switch (GetLanguage())` over the seven localised languages; the language ids come from GetLanguage's
// own string tests (0x00415070: ENGLISH 9, FRENCH 12, JAPANESE 17, GERMAN 7, KOREAN 18, CHINESE 4,
// SPANISH 10, ITALIAN 16). VC7.1 biases by the lowest case (`add eax,-4`), bounds-checks 0..14 and goes through
// a byte index table into an 8-entry jump table; the arms are laid out in source order (French, Japanese,
// German, Korean, Chinese, Spanish, Italian, then the English default, which is also the `ja` target).
// The table entries prove the mapping: index 12-4=8 -> the FONT_FRENCH_MAIN arm, 16-4=12 -> FONT_ITALIAN_MAIN.
class CCharString
{
public:
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
private:
    void* m_pData;                                       // +0 CCharStringData*
};

namespace NLocalisation
{
enum ELanguage
{
    LANGUAGE_CHINESE  = 4,
    LANGUAGE_GERMAN   = 7,
    LANGUAGE_ENGLISH  = 9,
    LANGUAGE_SPANISH  = 10,
    LANGUAGE_FRENCH   = 12,
    LANGUAGE_ITALIAN  = 16,
    LANGUAGE_JAPANESE = 17,
    LANGUAGE_KOREAN   = 18
};

unsigned long __fastcall GetLanguage();                  // 0x00415070

CCharString __fastcall GetFontBankName()
{
    switch (GetLanguage())
    {
    case LANGUAGE_FRENCH:
        return CCharString("FONT_FRENCH_MAIN", -1);
    case LANGUAGE_JAPANESE:
        return CCharString("FONT_JAPANESE_MAIN", -1);
    case LANGUAGE_GERMAN:
        return CCharString("FONT_GERMAN_MAIN", -1);
    case LANGUAGE_KOREAN:
        return CCharString("FONT_KOREAN_MAIN", -1);
    case LANGUAGE_CHINESE:
        return CCharString("FONT_CHINESE_MAIN", -1);
    case LANGUAGE_SPANISH:
        return CCharString("FONT_SPANISH_MAIN", -1);
    case LANGUAGE_ITALIAN:
        return CCharString("FONT_ITALIAN_MAIN", -1);
    default:
        return CCharString("FONT_ENGLISH_MAIN", -1);
    }
}
}
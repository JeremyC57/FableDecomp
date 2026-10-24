// NLocalisation::GetStreamingFontBankName() -- name of the streaming font bank for the current game language
// Leaf: ?GetStreamingFontBankName@NLocalisation@@YI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415440, length 0xE3 bytes of code plus the inline
// jump table (8 dwords @ 0x004154F4) and byte index table (15 bytes @ 0x00415514), `ret` (no stack args).
// Name: Ego_r.pdb `?GetStreamingFontBankName@NLocalisation@@YI?AVCCharString@@XZ` (a `__fastcall` free function in the
// NLocalisation namespace returning CCharString by value; the hidden result pointer arrives in ecx and is
// returned in eax). It follows GetFontBankName (0x00415350, "FONT_*_MAIN") exactly as the donor PDB orders the
// NLocalisation functions; the strings it builds are "STREAMING_FONT_<LANGUAGE>_PC". Caller: 0x00403230.
// Callees: NLocalisation::GetLanguage @ 0x00415070, CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Shape: `switch (GetLanguage())` over the seven localised languages; the language ids come from GetLanguage's
// own string tests (0x00415070: ENGLISH 9, FRENCH 12, JAPANESE 17, GERMAN 7, KOREAN 18, CHINESE 4,
// SPANISH 10, ITALIAN 16). VC7.1 biases by the lowest case (`add eax,-4`), bounds-checks 0..14 and goes through
// a byte index table into an 8-entry jump table; the arms are laid out in source order (French, Japanese,
// German, Korean, Chinese, Spanish, Italian, then the English default, which is also the `ja` target).
// The table entries prove the mapping: index 12-4=8 -> the STREAMING_FONT_FRENCH_PC arm, 16-4=12 -> STREAMING_FONT_ITALIAN_PC.
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

CCharString __fastcall GetStreamingFontBankName()
{
    switch (GetLanguage())
    {
    case LANGUAGE_FRENCH:
        return CCharString("STREAMING_FONT_FRENCH_PC", -1);
    case LANGUAGE_JAPANESE:
        return CCharString("STREAMING_FONT_JAPANESE_PC", -1);
    case LANGUAGE_GERMAN:
        return CCharString("STREAMING_FONT_GERMAN_PC", -1);
    case LANGUAGE_KOREAN:
        return CCharString("STREAMING_FONT_KOREAN_PC", -1);
    case LANGUAGE_CHINESE:
        return CCharString("STREAMING_FONT_CHINESE_PC", -1);
    case LANGUAGE_SPANISH:
        return CCharString("STREAMING_FONT_SPANISH_PC", -1);
    case LANGUAGE_ITALIAN:
        return CCharString("STREAMING_FONT_ITALIAN_PC", -1);
    default:
        return CCharString("STREAMING_FONT_ENGLISH_PC", -1);
    }
}
}
// _global::GetFontBankName() -- name of the main text/font bank for the current language (Windows primary language id), English by default
// Leaf: ?GetFontBankName@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415260, 240 bytes including the embedded jump table
// (0x00415314, 8 dwords) and byte index table (0x00415334, 15 bytes) (+26 int3 pad). Callees:
// GetLanguage 0x00415070, CCharString::CCharString(const char*, int) 0x0099EBF0; literals
// 0x0122E960..0x0122E974.
//
// Retail bytes:  push esi ; mov esi,ecx ; call GetLanguage ; add eax,-4 ; cmp eax,0xe ; ja D ;
//   movzx eax,byte [eax+index] ; jmp [eax*4+table] ; then eight bodies
//   `push -1 ; push literal ; mov ecx,esi ; call CCharString ; mov eax,esi ; pop esi ; ret`
//   in the order FRENCH, JAPANESE, GERMAN, KOREAN, CHINESE, SPANISH, ITALIAN, ENGLISH (default) ;
//   mov edi,edi ; jump table ; index table.
// The value is a CCharString returned through the hidden pointer, which a no-argument __fastcall
// receives in ecx. The switch keys are Windows primary language ids (LANG_CHINESE 0x04,
// LANG_GERMAN 0x07, LANG_SPANISH 0x0A, LANG_FRENCH 0x0C, LANG_ITALIAN 0x10, LANG_JAPANESE 0x11,
// LANG_KOREAN 0x12): the index table maps 4..18 to case slots numbered by key value and the jump
// table sends 0x0C to the FRENCH body and 0x10 to the ITALIAN one (decoded from the retail
// tables; relocation masking cannot check the key->literal pairing, so it is taken from the data).
// The cases are written in retail's body order. NOTE: the earlier-lane landing
// Global_GetFontBankName_00415260.cpp pairs 16 with FRENCH and 12 with ITALIAN (swapped).
// Grading: the body embeds a jump table (verify_land_jumptable.py at landing).
class CCharString {
public:
    CCharString(const char* text, int length);    // 0x0099EBF0
private:
    void* m_pData;
};

namespace NLocalisation { unsigned long __fastcall GetLanguage(); }   // 0x00415070                    // 0x00415070

enum {
    LANG_CHINESE  = 0x04,
    LANG_GERMAN   = 0x07,
    LANG_SPANISH  = 0x0A,
    LANG_FRENCH   = 0x0C,
    LANG_ITALIAN  = 0x10,
    LANG_JAPANESE = 0x11,
    LANG_KOREAN   = 0x12
};

CCharString __fastcall GetFontBankName()
{
    switch (NLocalisation::GetLanguage())
    {
    case LANG_FRENCH:
        return CCharString("TEXT_FRENCH_MAIN", -1);
    case LANG_JAPANESE:
        return CCharString("TEXT_JAPANESE_MAIN", -1);
    case LANG_GERMAN:
        return CCharString("TEXT_GERMAN_MAIN", -1);
    case LANG_KOREAN:
        return CCharString("TEXT_KOREAN_MAIN", -1);
    case LANG_CHINESE:
        return CCharString("TEXT_CHINESE_MAIN", -1);
    case LANG_SPANISH:
        return CCharString("TEXT_SPANISH_MAIN", -1);
    case LANG_ITALIAN:
        return CCharString("TEXT_ITALIAN_MAIN", -1);
    default:
        return CCharString("TEXT_ENGLISH_MAIN", -1);
    }
}
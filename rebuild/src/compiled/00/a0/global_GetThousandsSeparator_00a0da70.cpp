// GetThousandsSeparator() -- the digit-group separator for the system language: a space for French, a full stop for German, Spanish, Italian and Portuguese, a comma otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0DA70, length 76 bytes including its jump table (3 entries) and byte
// index table (16 entries), + int3 padding.
// Callees: KERNEL32 GetSystemDefaultLangID (IAT 0x0143FE60). Caller: the number formatter 0x00A0DAC0 (0x00A0DAEF).
// Name descriptive (no PDB name).
// Shape: PRIMARYLANGID (`and eax,3FFh`) biased by LANG_GERMAN (`add eax,-7`), range check 0..15 (LANG_GERMAN ..
// LANG_PORTUGUESE) into a byte index table and a three-entry jump table; WCHAR results through `mov ax,imm16`.
// Retail table, checked by hand: entry 0 -> 0x00A0DA96 ('.'), entry 1 -> 0x00A0DA91 (' '), entry 2 -> 0x00A0DA9B
// (','/default); index bytes for languages 7..0x16: 00 02 02 00 02 01 02 02 02 00 02 02 02 02 02 00, i.e. German 7,
// Spanish 0x0A, Italian 0x10, Portuguese 0x16 -> '.', French 0x0C -> ' ', every other language -> ','.
// VC7.1 lays the case bodies out in source order (' ' block first, then '.', then the default ',').
// Leaf: ?GetThousandsSeparator@@YAGXZ
extern "C" __declspec(dllimport) unsigned short __stdcall GetSystemDefaultLangID();   // IAT 0x0143FE60

enum {
    LANG_GERMAN_ID     = 0x07,
    LANG_SPANISH_ID    = 0x0A,
    LANG_FRENCH_ID     = 0x0C,
    LANG_ITALIAN_ID    = 0x10,
    LANG_PORTUGUESE_ID = 0x16
};

unsigned short GetThousandsSeparator()
{
    switch (GetSystemDefaultLangID() & 0x3FF) {
    case LANG_FRENCH_ID:
        return 0x0020;
    case LANG_GERMAN_ID:
    case LANG_SPANISH_ID:
    case LANG_ITALIAN_ID:
    case LANG_PORTUGUESE_ID:
        return 0x002E;
    default:
        return 0x002C;
    }
}
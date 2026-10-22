// CCharString::RemoveWhiteSpaceEnd() -- strip trailing spaces, tabs, carriage returns and line feeds: find the last other character and, when it is not the last, replace the string with the substring up to it (an all-blank string is left untouched)
// Grade: RELOCATION_MATCH -- GetLength() is written with a result local assigned in an if/else (`if (PStringData == 0) length = 0; else length = PStringData->StringLength; return length;`): VC7.1 then keeps the `xor eax,eax ; jmp` null arm and does not thread `length > 0` into a non-null proof, so the per-iteration GetText null test and the guard's [edx+4] re-read stay; the ternary/early-return forms thread (120v135), `length = 0; if (...)` gives 133, the inverted if/else 135v135 off by registers
// Leaf: RemoveWhiteSpaceEnd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F4D0, length 135 bytes.
// Callees: CCharString::GetSubString(unsigned long, unsigned long) const @ 0x0099EC70 (hidden CCharString result in
// the `push ecx` frame word), CCharString::UnassignString @ 0x0099E9B0 (in the inlined operator= and destructor); the
// instance counter @ 0x013BD800 is decremented by the inlined destructor; the empty literal @ 0x0129AAF4.
// Layout: the 17-byte CCharStringData block (PData +0, StringLength +4, NoRefs +0xd).
// Shape: the twin of RemoveWhiteSpaceBegin (0x0099F450) scanning down from GetLength(): the loop is bottom-tested
// (`dec ecx ; jne`) and reads `GetText()[i - 1]` with its null test kept (nothing in the loop tests the block); an
// all-blank string returns straight from the loop; the trim is guarded by `i < GetLength()` and takes
// GetSubString(0, i) into a named local assigned through the inline operator=.
#pragma pack(push, 1)
struct CCharStringData {                      // 17 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength;                 // +0x08 (DataLength:31 + a flag bit)
    unsigned char UseFastExtend;              // +0x0c (bit 0)
    long          NoRefs;                     // +0x0d
};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;       // 0x013BD800
extern char g_EmptyCharString[];              // 0x0129AAF4

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    ~CCharString()                            // 0x0099EAE0, inlined
    {
        UnassignString();
        --g_CCharStringInstanceCount;
    }
    CCharString& operator=(const CCharString& other)   // 0x0099EFB0, inlined
    {
        if (&other != this) {
            if (PStringData != 0)
                UnassignString();
            if (other.PStringData != 0) {
                PStringData = other.PStringData;
                ++PStringData->NoRefs;
            }
        }
        return *this;
    }
    void UnassignString();                    // 0x0099E9B0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    unsigned long GetLength() const { unsigned long length; if (PStringData == 0) length = 0; else length = PStringData->StringLength; return length; }
    const char* GetText() const { return PStringData == 0 ? g_EmptyCharString : PStringData->PData; }
    void RemoveWhiteSpaceBegin();
    void RemoveWhiteSpaceEnd();
};

void CCharString::RemoveWhiteSpaceEnd()
{
    unsigned long i = GetLength();
    while (i > 0) {
        char c = GetText()[i - 1];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            --i;
            continue;
        }
        if (i < GetLength()) {
            CCharString rest = GetSubString(0, i);
            *this = rest;
        }
        return;
    }
}
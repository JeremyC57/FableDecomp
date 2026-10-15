// CCharString::RemoveWhiteSpaceBegin() -- strip leading spaces, tabs, carriage returns and line feeds: find the first other character and, when it is not the first, replace the string with the substring from there to the end (an all-blank string is left untouched)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F450, length 117 bytes.
// Callees: CCharString::GetSubString(unsigned long, unsigned long) const @ 0x0099EC70 (hidden CCharString result in
// the `push ecx` frame word), CCharString::UnassignString @ 0x0099E9B0 (in the inlined operator= and destructor); the
// instance counter @ 0x013BD800 is decremented by the inlined destructor.
// Layout: the 17-byte CCharStringData block (PData +0, StringLength +4, NoRefs +0xd).
// Shape: the scan tests the block and the length at the loop head every step (`GetLength()` null-case-first: a null
// block exits like the end of the string); the character is read through the null-case-first `GetText()[i]` (its null
// test folds against the loop test; `PStringData->PData[i]` swaps the address operands and the later registers); the
// blank test is a four-way `==` chain that jumps back to the increment; the substring runs to GetLength() (re-null-
// tested) into a NAMED local (re-addressed with `lea` for the self test; a bare temporary reuses the call's eax) and
// is assigned through the landed operator= (self test, release of a held block, share + NoRefs bump) inline, then the
// local's inline destructor runs.
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
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetText() const { return PStringData == 0 ? g_EmptyCharString : PStringData->PData; }
    void RemoveWhiteSpaceBegin();
    void RemoveWhiteSpaceEnd();
};

void CCharString::RemoveWhiteSpaceBegin()
{
    unsigned long i = 0;
    while (i < GetLength()) {
        char c = GetText()[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            ++i;
            continue;
        }
        if (i > 0) {
            CCharString rest = GetSubString(i, GetLength());
            *this = rest;
        }
        return;
    }
}

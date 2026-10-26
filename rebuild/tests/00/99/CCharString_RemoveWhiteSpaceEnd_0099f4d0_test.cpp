// behaviour test for 0x0099F4D0 CCharString::RemoveWhiteSpaceEnd (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
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
class CCharStringData { public:                      // 17 bytes
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


// ---- stubs ----
long g_CCharStringInstanceCount;
char g_EmptyCharString[] = "";
static int g_unassigns;
void CCharString::UnassignString()
{
    g_unassigns++;
    if (PStringData) { --PStringData->NoRefs; PStringData = 0; }
}
static CCharStringData g_sub; static char g_subBuf[64]; static bool g_subNull;
static unsigned long g_subStart, g_subCount; static int g_subCalls; static const CCharString* g_subThis;
static CCharString g_subHolder;   // copied out bitwise (no NRVO): the copy is the new reference
CCharString CCharString::GetSubString(unsigned long start, unsigned long count) const
{
    g_subCalls++; g_subStart = start; g_subCount = count; g_subThis = this;
    memcpy(g_subBuf, PStringData->PData + start, count); g_subBuf[count] = 0;
    g_sub.PData = g_subBuf; g_sub.StringLength = count; g_sub.NoRefs = 0;
    g_subHolder.PStringData = g_subNull ? 0 : &g_sub;
    if (!g_subNull) g_sub.NoRefs++;
    return g_subHolder;
}

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static CCharStringData g_data; static char g_buf[64];
static void Set(CCharString& s, const char* text)
{
    strcpy(g_buf, text); g_data.PData = g_buf; g_data.StringLength = (unsigned long)strlen(text); g_data.NoRefs = 1;
    s.PStringData = &g_data;
}

int main()
{
    static CCharString str;
    // 1: empty (null data) -> nothing
    str.PStringData = 0;
    str.RemoveWhiteSpaceEnd();
    check(str.PStringData == 0 && g_subCalls == 0 && g_unassigns == 0, "null");
    // 2: nothing to trim -> untouched
    Set(str, "abc");
    str.RemoveWhiteSpaceEnd();
    check(str.PStringData == &g_data && g_subCalls == 0 && g_data.NoRefs == 1, "no trailing");
    // 3: all blank -> untouched
    Set(str, " \t\r\n ");
    str.RemoveWhiteSpaceEnd();
    check(str.PStringData == &g_data && g_subCalls == 0, "all blank");
    // 4: trailing blanks of every kind -> substring up to the last other character
    Set(str, "a b \t\r\n");
    long instances = g_CCharStringInstanceCount;
    str.RemoveWhiteSpaceEnd();
    check(g_subCalls == 1 && g_subThis == &str && g_subStart == 0 && g_subCount == 3, "trim args");
    check(str.PStringData == &g_sub && strcmp(str.PStringData->PData, "a b") == 0 && g_sub.NoRefs == 1 && g_data.NoRefs == 0, "trimmed");
    check(g_unassigns == 2 && g_CCharStringInstanceCount == instances - 1, "temp destroyed");
    // 5: single trailing space
    Set(str, "x ");
    str.RemoveWhiteSpaceEnd();
    check(g_subCount == 1 && strcmp(str.PStringData->PData, "x") == 0, "one space");
    // 6: substring comes back empty (null data) -> this released, stays null
    Set(str, "yz  "); g_subNull = true;
    str.RemoveWhiteSpaceEnd();
    check(str.PStringData == 0 && g_data.NoRefs == 0 && g_subCount == 2, "null substring");
    if (!fails) printf("0099f4d0_TEST PASS\n");
    return fails;
}
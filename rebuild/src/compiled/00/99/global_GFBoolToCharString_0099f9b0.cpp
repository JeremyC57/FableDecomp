// GFBoolToCharString(bool value) -- returns the text "TRUE" or "FALSE" for a bool, by value
// Leaf: ?GFBoolToCharString@@YI?AVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F9B0, 145 bytes (+ int3 padding). PDB name (Ego_r):
// `?GFBoolToCharString@@YI?AVCCharString@@_N@Z` = `CCharString __fastcall GFBoolToCharString(bool)`.
// Callees: CCharString::UnassignString @ 0x0099E9B0, operator new @ 0x00BFEA1A (null-checked),
// CBasicString<char>::CBasicString(const char*) @ 0x009A0590. Globals: g_CCharStringInstanceCount @ 0x013BD800.
// Literals: "TRUE" @ 0x0122D6D4, "FALSE" @ 0x0122D6CC.
// Shape: a `__fastcall` free function returning the class by value: the hidden result pointer arrives in ecx (esi),
// the bool in dl. GFBoolToCharString lives in the CCharString translation unit (0x0099E4B0..0x0099FA80), so the
// (const char*, long = -1) constructor (landed out of line @ 0x0099EBF0) and AllocStringData (@ 0x0099EA60) are
// visible bodies that /Ob2 inlines: each `return CCharString(literal, -1)` builds straight into the return slot
// (null block, counter++, the kept `if (PStringData) UnassignString();`, `length == -1` and the literal's non-empty
// test folded, `new CCharStringData(text)`; a failed allocation stores the null block). VC7.1 hoists the identical
// head of both arms (the null store, the counter bump and the PStringData load) above the `cmp dl,1` (the
// `value == true` test; a plain `if (value)` would be `test dl,dl`) and tail-merges the failed-allocation exits.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength    : 31;           // +0x08 bits 0..30
    unsigned long m_reserved    : 1;            // +0x08 bit 31 (not initialised here)
    unsigned char UseFastExtend : 1;            // +0x0c bit 0
    CBasicStringChar(const char* text);         // 0x009A0590
    CBasicStringChar(const char* text, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
    {
        AssignFromData(text, length);
    }
    void AssignFromData(const char* text, unsigned long length);   // 0x009A0300
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
    CCharStringData(const char* text, unsigned long length) : Data(text, length), NoRefs(1) { }
};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;         // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (inlined here)
    void UnassignString();                      // 0x0099E9B0
    CCharStringData* AllocStringData(const char* text, long length);   // 0x0099EA60 (inlined here)
};

CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    if (length == -1)
        return new CCharStringData(text);
    return new CCharStringData(text, length);
}

CCharString::CCharString(const char* text, long length)
    : PStringData(0)
{
    ++g_CCharStringInstanceCount;
    if (PStringData != 0)
        UnassignString();
    if (text != 0 && text[0] != '\0')
        PStringData = AllocStringData(text, length);
}

CCharString __fastcall GFBoolToCharString(bool value)
{
    if (value == true)
        return CCharString("TRUE", -1);
    return CCharString("FALSE", -1);
}
// CCharString::FindCharPos(char c) -- index of the first occurrence of c, or -1. The text pointer is an
// inline accessor (block ? block->PData : the static empty string) evaluated twice: once for strchr and
// once to form the difference. The char argument is sign-extended (movsx) into strchr's int parameter.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099E5A0, length 69 bytes.
// Callees: strchr @ 0x00BFEDA0 (cdecl). Globals: the CCharString empty text @ 0x0129AAF4.
// CCharString model (Ego_r.pdb layout, byte-exact with retail): a 4-byte handle on a 17-byte shared
// block. CBasicString<char> is {PData +0, StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc}
// (13 bytes, packed) and CCharStringData appends the reference count NoRefs at +0xd.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08 (the top bit is never touched)
    bool          UseFastExtend : 1;            // +0x0c
    CBasicStringChar(const char* text);         // 0x009A0590 (manifest: CCharString_InitFromCStr)
    void Truncate(unsigned long length);        // 0x009A0080
    void Insert(long position, const char* text); // 0x009A06B0
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)
extern "C" char* __cdecl strchr(const char* text, int c);   // 0x00BFEDA0
extern char g_emptyCharString[1];                            // 0x0129AAF4

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    const char* GetText() const { return PStringData == 0 ? g_emptyCharString : PStringData->Data.PData; }
    long FindCharPos(char c) const;
};

long CCharString::FindCharPos(char c) const
{
    const char* found = strchr(GetText(), c);
    if (found != 0)
        return found - GetText();
    return -1;
}
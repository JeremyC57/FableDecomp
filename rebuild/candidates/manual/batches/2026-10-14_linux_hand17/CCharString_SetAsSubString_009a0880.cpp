// CCharString::SetAsSubString(const char* text, long length) -- release the current block and, for a
// non-empty text, allocate a block holding the first `length` characters.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A0880, length 45 bytes (0x0099EF40 and 0x009A0880 are
// byte-identical bodies: the same member compiled into two translation units).
// Callees: CCharString::UnassignString @ 0x0099E9B0, CCharString::AllocStringData @ 0x0099EA60.
// Same tail as the landed (const char*, long) constructor @ 0x0099EBF0.
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
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    void UnassignString();                      // 0x0099E9B0
    CCharStringData* AllocStringData(const char* text, long length); // 0x0099EA60
    void SetAsSubString(const char* text, long length);
};

void CCharString::SetAsSubString(const char* text, long length)
{
    if (PStringData != 0)
        UnassignString();
    if (text != 0 && *text != '\0')
        PStringData = AllocStringData(text, length);
}

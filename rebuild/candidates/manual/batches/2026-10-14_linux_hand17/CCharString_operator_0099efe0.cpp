// CCharString::operator=(const char*) -- release the current block, then (for a non-empty text) allocate
// a fresh 17-byte block initialised from the text with one owner.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099EFE0, length 81 bytes.
// Callees: CCharString::UnassignString @ 0x0099E9B0, operator new @ 0x00BFEA1A (null-checked),
// CBasicString<char>::CBasicString(const char*) @ 0x009A0590. A failed allocation stores a null block.
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
    const CCharString& operator=(const char* text);
};

const CCharString& CCharString::operator=(const char* text)
{
    if (PStringData != 0)
        UnassignString();
    if (text != 0 && *text != '\0')
        PStringData = new CCharStringData(text);
    return *this;
}

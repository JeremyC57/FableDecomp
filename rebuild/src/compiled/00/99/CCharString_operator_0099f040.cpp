// CCharString::operator=(char) -- assign a one-character string: a two-byte buffer {c, 0} is built on the
// stack (VC7.1 homes it in the dead argument slot once c lives in bl) and handed to the block constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F040, length 89 bytes.
// Callees: CCharString::UnassignString @ 0x0099E9B0, operator new @ 0x00BFEA1A (null-checked),
// CBasicString<char>::CBasicString(const char*) @ 0x009A0590. A NUL character leaves the string unassigned.
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
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    protected: void UnassignString(); public:                      // 0x0099E9B0
    const CCharString& operator=(char c);
};

const CCharString& CCharString::operator=(char c)
{
    char text[2];
    text[0] = c;
    text[1] = '\0';
    if (PStringData != 0)
        UnassignString();
    if (c != '\0')
        PStringData = new CCharStringData(text);
    return *this;
}
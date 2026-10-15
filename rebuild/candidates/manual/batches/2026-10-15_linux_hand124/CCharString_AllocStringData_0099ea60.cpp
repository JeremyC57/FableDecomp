// CCharString::AllocStringData(const char* text, long length) -- allocate a shared 17-byte string block for `text`: the whole NUL-terminated text when length is -1, else `length` characters; the new block has one owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099EA60, length 124 bytes (ret 8).
// Callees: operator new @ 0x00BFEA1A (0x11 bytes), CBasicString<char>::CBasicString(const char*) @ 0x009A0590
// (manifest "CCharString_InitFromCStr"), CBasicString<char>::AssignFromData(const char*, unsigned long) @ 0x009A0300.
// Layout (the landed CCharString block model, Ego_r.pdb): CBasicString<char> {PData +0, StringLength +4,
// DataLength:31 +8 (bit 31 a second, untouched bitfield), UseFastExtend:1 +0xc} (13 bytes, packed) then
// CCharStringData::NoRefs +0xd.
// Shape: the two `new CCharStringData(...)` expressions share ONE operator new call hoisted above the length test
// (each arm keeps its own null test returning 0); the (text, length) arm inlines the landed CBasicString(text, length)
// constructor (0x009A05E0: bitfield read/modify/write, immediate zero stores, AssignFromData; `__forceinline` on it and
// on the block constructor, else VC keeps the block constructor out of line) while the text-only
// constructor stays out of line; NoRefs = 1 is stored after the base constructor in both arms.
#pragma pack(push, 1)
struct CBasicStringChar {                     // CBasicString<char>, 13 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength    : 31;         // +0x08 bits 0..30
    unsigned long m_reserved    : 1;          // +0x08 bit 31 (not initialised here)
    unsigned char UseFastExtend : 1;          // +0x0c bit 0
    CBasicStringChar(const char* text);       // 0x009A0590
    __forceinline CBasicStringChar(const char* text, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
    {
        AssignFromData(text, length);
    }
    void AssignFromData(const char* text, unsigned long length);   // 0x009A0300
};
struct CCharStringData {                      // 17 bytes
    CBasicStringChar Data;                    // +0x00
    long             NoRefs;                  // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
    __forceinline CCharStringData(const char* text, unsigned long length) : Data(text, length), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    CCharStringData* AllocStringData(const char* text, long length);
};

CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    if (length == -1)
        return new CCharStringData(text);
    return new CCharStringData(text, length);
}

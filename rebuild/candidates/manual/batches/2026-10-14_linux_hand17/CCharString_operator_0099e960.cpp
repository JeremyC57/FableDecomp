// CCharString::operator!=(const char*) -- negation of the inline operator==(const char*): an unassigned
// string equals only "" (the strcmp intrinsic: repe cmpsb over the 1-byte pooled empty literal), otherwise the block's
// CBasicString<char>::operator==(const char*) decides.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099E960, length 65 bytes.
// Callees: CBasicString<char>::operator==(const char*) @ 0x004115A0 (this = the block). Literal "" @ 0x0122D70E.
#include <string.h>
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
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const;
};

bool CCharString::operator!=(const char* text) const
{
    return !(*this == text);
}

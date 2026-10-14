// CCharString::Erase(long position, long count) -- remove `count` characters at `position`: make the block
// private, slide the tail down with memmove and truncate the block to the new length.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F400, length 69 bytes.
// Callees: CCharString::MakeOwnCopyOfData @ 0x0099EAF0, memmove @ 0x00BFEAE6 (cdecl, 3 args popped together),
// CBasicString<char>::Truncate @ 0x009A0080 on the block. The block pointer replaces `this` in esi after the
// copy (this is dead from then on); StringLength is re-read for the Truncate argument.
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
    void MakeOwnCopyOfData();                   // 0x0099EAF0
    void Erase(long position, long count);
};

void CCharString::Erase(long position, long count)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        CCharStringData* data = PStringData;
        long end = position + count;
        memmove(data->Data.PData + position, data->Data.PData + end, data->Data.StringLength - end);
        data->Data.Truncate(data->Data.StringLength - count);
    }
}
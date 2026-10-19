// Grade: HELD(bytes match retail) -- calls scalar operator new/delete where retail calls operator new[] 0xbfeb22 / operator delete[] 0xbfeb1c | Codex source 00/9a/CBasicStringchar_Truncate_009a0080.cpp, flags , pass pattern 009a0080_TEST PASS
// CBasicString<char>::Truncate(unsigned long) -- allocate the aligned prefix, release the old buffer, update the packed capacity/length, and clear tail padding; retail 0x009a0080 length 0xa6. Allocator calls are operator new[] @ 0x00bfeb22 and operator delete[] @ 0x00bfeb1c.
#include <string.h>

// Ego_r.pdb CBasicString<char> is a packed 13-byte subobject: PData +0,
// StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc. The actual packed
// CCharStringData layout corroborates this template field placement.
#pragma pack(push, 1)
struct CBasicStringChar {
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    void Truncate(unsigned long length);
};
#pragma pack(pop)

void CBasicStringChar::Truncate(unsigned long length) {
    if (length < StringLength) {
        unsigned long allocation = (length + 4) & ~3UL;
        char* data = new char[allocation];
        data = (char*)memcpy(data, PData, length);
        data[length] = 0;
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        DataLength = allocation;
        PData = data;
        StringLength = length;
        PData[StringLength] = 0;
        unsigned long used = StringLength + 1;
        unsigned long end = (used + 3) & ~3UL;
        memset(PData + used, 0, end - used);
    }
}
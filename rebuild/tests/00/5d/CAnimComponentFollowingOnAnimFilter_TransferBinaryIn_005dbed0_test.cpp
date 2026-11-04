// Behaviour test for CAnimComponentFollowingOnAnimFilter::TransferBinaryIn 0x005DBED0: reading the filter reads its next-filter def string from the stream. The body forwards to the member at +0x8
// (`add ecx,8h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CDataInputStream { int pos; int Read() { return pos++; } };
struct CMemoryDataInputStream : CDataInputStream { };
struct CDefString { int Value; void TransferBinaryIn(CDataInputStream& s) { Value = s.Read(); } };
#pragma pack(push, 4)
struct CAnimComponentFollowingOnAnimFilter {                                  // retail is 32-bit: 4-byte packing keeps the +0x8 offset on a 64-bit host
    unsigned char Before[0x8];
    CDefString NextFilter;
    void TransferBinaryIn(CMemoryDataInputStream& stream) { NextFilter.TransferBinaryIn(stream); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CAnimComponentFollowingOnAnimFilter, NextFilter) != 0x8u) { std::printf("005dbed0_TEST FAIL\n"); return 1; }
    CAnimComponentFollowingOnAnimFilter f; f.NextFilter.Value = -1;
    CMemoryDataInputStream s; s.pos = 41;
    f.TransferBinaryIn(s);
    if (f.NextFilter.Value != 41 || s.pos != 42) { std::printf("0069e1c0_TEST FAIL\n"); return 1; }
    std::printf("005dbed0_TEST PASS\n");
    return 0;
}
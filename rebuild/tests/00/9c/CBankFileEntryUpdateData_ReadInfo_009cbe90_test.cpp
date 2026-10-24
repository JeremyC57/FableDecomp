// Behaviour test for CBankFileEntryUpdateData::ReadInfo (0x009CBE90).
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif

class CBankFileEntryUpdateData
{
public:
    unsigned int StateBlockCRC;
    unsigned int InfoSize;
    char*        InfoBuffer;
    static bool IsValidPosition(Int64 position) { return position >= 0 && position <= 0x7fffffff; }
    void ReadInfo(unsigned int offset, void* dest, unsigned int size) const
    {
        if (IsValidPosition((Int64)(unsigned int)(offset + size)))
            std::memcpy(dest, InfoBuffer + offset, size);
    }
};

int main()
{
    char info[16] = "ABCDEFGHIJKLMNO";
    CBankFileEntryUpdateData d;
    d.StateBlockCRC = 0; d.InfoSize = 16; d.InfoBuffer = info;
    char out[8] = { 0 };
    d.ReadInfo(2, out, 5);
    if (std::memcmp(out, "CDEFG", 5) != 0 || out[5] != 0) { std::printf("009cbe90_TEST FAIL\n"); return 1; }
    char guard[4] = { 'x', 'x', 'x', 'x' };
    d.ReadInfo(0x7ffffff0u, guard, 0x20u);           // end beyond 0x7fffffff: nothing copied
    if (guard[0] != 'x') { std::printf("009cbe90_TEST FAIL\n"); return 1; }
    d.ReadInfo(0, guard, 0);                           // empty copy is fine
    if (guard[0] != 'x') { std::printf("009cbe90_TEST FAIL\n"); return 1; }
    std::printf("009cbe90_TEST PASS\n");
    return 0;
}
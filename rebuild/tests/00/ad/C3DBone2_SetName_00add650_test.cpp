// Behaviour test for 0x00ADD650 C3DBone2::SetName (NameCRC = CRC of the name, "" for a data-less string).
#include <cstdio>
#include <cstring>

static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)   // stand-in for CCRC::Calc 0x004014A0
{
    const unsigned char* p = (const unsigned char*)data;
    unsigned long crc = ~seed;
    for (unsigned long i = 0; i < length; ++i) { crc ^= p[i]; for (int k = 0; k < 8; ++k) crc = (crc >> 1) ^ (0xEDB88320ul & (0ul - (crc & 1ul))); }
    return ~crc & 0xFFFFFFFFul;
}
struct SStringData { const char* Data; long Length; };
struct SCharString { SStringData* PStringData; };
class C3DBone2 { public:
    unsigned long NameCRC; long Parent; long OriginalNoChildren;
    // reconstructed body
    void SetName(const SCharString& name)
    {
        unsigned long length = name.PStringData == 0 ? 0 : (unsigned long)name.PStringData->Length;
        const char* data = name.PStringData == 0 ? "" : name.PStringData->Data;
        NameCRC = Calc(0, data, length);
    }
};

int main()
{
    SStringData d = { "Bip01 Spine", 11 };
    SCharString s = { &d };
    C3DBone2 b; b.NameCRC = 0; b.Parent = -1; b.OriginalNoChildren = 0;
    b.SetName(s);
    if (b.NameCRC != Calc(0, "Bip01 Spine", 11) || b.NameCRC == 0 || b.Parent != -1) { std::printf("00add650_TEST FAIL\n"); return 1; }
    SStringData d2 = { "Bip01 Head", 10 };
    SCharString s2 = { &d2 };
    b.SetName(s2);
    if (b.NameCRC != Calc(0, "Bip01 Head", 10) || b.NameCRC == Calc(0, "Bip01 Spine", 11)) { std::printf("00add650_TEST FAIL\n"); return 1; }
    SCharString empty = { 0 };
    b.SetName(empty);
    if (b.NameCRC != Calc(0, "", 0) || b.NameCRC != 0) { std::printf("00add650_TEST FAIL\n"); return 1; }   // zero-length CRC of seed 0 is 0
    std::printf("00add650_TEST PASS\n");
    return 0;
}
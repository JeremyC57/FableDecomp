// Behaviour test for 0x0073E040 CAnimFilterOf2Groups::SetCurrentGroupNames(const CCharString&, const CCharString&)
#include <cstdio>
#include <cstring>

class CCharStringData { public:
    const char*  PData;
    unsigned int StringLength;
};

static int g_assigns = 0, g_copies = 0;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
    const CCharString& operator=(const CCharString& other) { ++g_assigns; PStringData = other.PStringData; return *this; }
    unsigned int GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
};

// stand-in for the CRC-32 kernel 0x004014A0 (bitwise CRC-32, seed in, no final xor)
static unsigned int Calc_004014a0(unsigned int seed, const void* data, unsigned int length)
{
    unsigned int crc = seed;
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned int i = 0; i < length; ++i) {
        crc ^= p[i];
        for (int k = 0; k < 8; ++k)
            crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1u)));
    }
    return crc;
}

static CCharStringData g_dWalk = { "walk", 4 }, g_dRun = { "run_fast", 8 }, g_dIdle = { "idle", 4 };
static CCharString Str(CCharStringData* d) { CCharString s; s.PStringData = d; return s; }

struct CAnimFilterOf2Groups {
    void*        vptr_stand_in;
    CCharString  Group1;
    CCharString  Group2;
    unsigned int Group1CRC;
    unsigned int Group2CRC;
    void SetCurrentGroupNames(const CCharString& group1, const CCharString& group2)
    {
        Group1 = group1;
        Group1CRC = Calc_004014a0(0, group1.GetPString(), group1.GetLength());
        Group2 = group2;
        Group2CRC = Calc_004014a0(0, group2.GetPString(), group2.GetLength());
    }
};

int main()
{
    CAnimFilterOf2Groups f;
    CCharString walk = Str(&g_dWalk), run = Str(&g_dRun), empty;
    int ok = 1;
    f.SetCurrentGroupNames(walk, run);
    if (f.Group1.PStringData != &g_dWalk || f.Group2.PStringData != &g_dRun) ok = 0;
    if (f.Group1CRC != Calc_004014a0(0, "walk", 4) || f.Group2CRC != Calc_004014a0(0, "run_fast", 8)) ok = 0;
    if (f.Group1CRC == f.Group2CRC) ok = 0;
    f.SetCurrentGroupNames(run, empty);
    if (f.Group1CRC != Calc_004014a0(0, "run_fast", 8) || f.Group2CRC != 0 || f.Group2.PStringData != 0) ok = 0;
    if (g_assigns != 4) ok = 0;
    if (!ok) { std::printf("0073e040_TEST FAIL\n"); return 1; }
    std::printf("0073e040_TEST PASS\n");
    return 0;
}
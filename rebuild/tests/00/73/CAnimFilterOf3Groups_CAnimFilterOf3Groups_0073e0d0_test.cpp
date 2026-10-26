// Behaviour test for 0x0073E0D0 CAnimFilterOf3Groups::CAnimFilterOf3Groups(const CCharString&, const CCharString&, const CCharString&)
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
    CCharString& operator=(const CCharString& other) { ++g_assigns; PStringData = other.PStringData; return *this; }
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

class CAnimFilterBase { public:
    virtual ~CAnimFilterBase() {}
};

struct CAnimFilterOf3Groups : CAnimFilterBase {
    CCharString  Group1;
    CCharString  Group2;
    CCharString  Group3;
    unsigned int Group1CRC;
    unsigned int Group2CRC;
    unsigned int Group3CRC;
    CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3)
        : Group1(group1)
        , Group2(group2)
        , Group3(group3)
    {
        Group1CRC = Calc_004014a0(0, group1.GetPString(), group1.GetLength());
        Group2CRC = Calc_004014a0(0, group2.GetPString(), group2.GetLength());
        Group3CRC = Calc_004014a0(0, group3.GetPString(), group3.GetLength());
    }
};

int main()
{
    CCharString walk = Str(&g_dWalk), run = Str(&g_dRun), idle = Str(&g_dIdle), empty;
    int ok = 1;
    g_copies = 0;
    CAnimFilterOf3Groups f(walk, run, idle);
    if (g_copies != 3 || g_assigns != 0) ok = 0;
    if (f.Group1.PStringData != &g_dWalk || f.Group2.PStringData != &g_dRun || f.Group3.PStringData != &g_dIdle) ok = 0;
    if (f.Group1CRC != Calc_004014a0(0, "walk", 4) || f.Group2CRC != Calc_004014a0(0, "run_fast", 8)
        || f.Group3CRC != Calc_004014a0(0, "idle", 4)) ok = 0;
    CAnimFilterOf3Groups g(empty, walk, empty);
    if (g.Group1CRC != 0 || g.Group3CRC != 0 || g.Group2CRC != f.Group1CRC) ok = 0;
    if (!ok) { std::printf("0073e0d0_TEST FAIL\n"); return 1; }
    std::printf("0073e0d0_TEST PASS\n");
    return 0;
}
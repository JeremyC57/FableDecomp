// Behaviour test for 0x0073DFF0 CAnimFilterOfGroup::SetCurrentGroupName(const CCharString&)
#include <cstdio>
#include <cstring>

struct CCharStringData {
    const char*  PData;
    unsigned int StringLength;
};

static int g_assigns = 0, g_copies = 0;
struct CCharString {
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

struct CAnimFilterOfGroup {
    void*        vptr_stand_in;
    CCharString  Group;
    unsigned int GroupCRC;
    void SetCurrentGroupName(const CCharString& name)
    {
        Group = name;
        GroupCRC = Calc_004014a0(0, name.GetPString(), name.GetLength());
    }
};

int main()
{
    CAnimFilterOfGroup f;
    f.GroupCRC = 12345;
    CCharString walk = Str(&g_dWalk), empty;
    int ok = 1;
    f.SetCurrentGroupName(walk);
    if (f.Group.PStringData != &g_dWalk || f.GroupCRC != Calc_004014a0(0, "walk", 4) || g_assigns != 1) ok = 0;
    if (f.GroupCRC == 0) ok = 0;
    f.SetCurrentGroupName(empty);                         // no data block: CRC over zero bytes = the seed
    if (f.Group.PStringData != 0 || f.GroupCRC != 0 || g_assigns != 2) ok = 0;
    if (!ok) { std::printf("0073dff0_TEST FAIL\n"); return 1; }
    std::printf("0073dff0_TEST PASS\n");
    return 0;
}
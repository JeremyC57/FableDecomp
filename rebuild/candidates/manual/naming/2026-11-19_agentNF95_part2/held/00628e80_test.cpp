// Test for CGameEventDataPackage::GetUHUGE() @ 0x00628E80 (NF95 2026-11-19: PDB names; was the stand-in CGameEvent::ReadData8).
#include <cstdio>
#include <cstring>
static long g_count;
class CGameEventDataPackage { public:
    unsigned char Data[32]; long Length; long StartPos; long EndPos;
    void GetData(long size, void* data) { g_count = size; std::memcpy(data, Data + StartPos, size); StartPos += size; }
    unsigned long long GetUHUGE() { unsigned long long value; GetData(8, &value); return value; }
};
int main()
{
    CGameEventDataPackage p; std::memset(&p, 0, sizeof(p));
    unsigned long long in = 0xFFFFFFF911223344ULL; std::memcpy(p.Data, &in, 8);
    unsigned long long v = p.GetUHUGE();
    if (g_count != 8 || v != in || p.StartPos != 8) { std::printf("00628e80_TEST FAIL\n"); return 1; }
    std::printf("00628e80_TEST PASS\n");
    return 0;
}

// Test for CGameEventDataPackage::GetUHUGE() @ 0x00628E80 (NF95 2026-11-19: PDB names; was the stand-in CGameEvent::ReadData8).
// NF97 2026-11-19 (port): the class is the shared engine/CGameEventDataPackage.h (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE + _GETUHUGE), no
// longer a local model; the test supplies GetData (0x009F18F0, not in this row) and links the row's GetUHUGE.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE
#include <cstdio>
#include <cstring>
#include "engine/CGameEventDataPackage.h"
static long g_count;
static unsigned char g_source[8];
void CGameEventDataPackage::GetData(long size, void* data) { g_count = size; std::memcpy(data, g_source, size); }
int main()
{
    CGameEventDataPackage p;
    unsigned __int64 in = 0xFFFFFFF911223344ULL; std::memcpy(g_source, &in, 8);
    unsigned __int64 v = p.GetUHUGE();
    if (g_count != 8 || v != in) { std::printf("00628e80_TEST FAIL\n"); return 1; }
    std::printf("00628e80_TEST PASS\n");
    return 0;
}

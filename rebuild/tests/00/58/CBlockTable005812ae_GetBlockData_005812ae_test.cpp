#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_bank = 0, g_block = 0; static int g_find = 0;
struct SBlockKey_005812ae { long m_bank; long m_block; }; struct SFindResult_005812ae { unsigned char _body[8]; };
struct CRetailTree_005812ae { void Find(SFindResult_005812ae&, const SBlockKey_005812ae& k) { ++g_find; g_bank = k.m_bank; g_block = k.m_block; } };
struct CBlockTable_005812ae { unsigned char _pad[0x18]; CRetailTree_005812ae m_tree; void* Recalculate() { return this; }
    void* GetBlockData(long bank, long block) { SBlockKey_005812ae key; key.m_bank = bank; key.m_block = block; SFindResult_005812ae result; m_tree.Find(result, key); return Recalculate(); } };
int main() {
    CBlockTable_005812ae t; void* r = t.GetBlockData(3, 4); if (r != (void*)&t || g_bank != 3 || g_block != 4 || g_find != 1) { std::printf("005812ae_TEST FAIL\n"); return 1; }
    std::printf("005812ae_TEST PASS\n"); return 0;
}
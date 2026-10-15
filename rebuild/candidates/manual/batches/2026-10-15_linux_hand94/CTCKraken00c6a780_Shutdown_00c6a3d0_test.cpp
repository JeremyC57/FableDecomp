#include <cstdio>
#include <cstring>
// Behaviour test for 0x00C6A3D0: flag cleared, block freed and nulled (only when set), base block released, holder cleared.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
static void* g_freed = 0;
static void Free_00c6a780(void* block) { g_freed = block; Log('F'); }
struct CHolder_00c6a780 { int body; void Clear() { Log('C'); } };
struct CCountedBlock_00c6a780 { virtual void v00() {} void ReleaseBlock() { Log('R'); } };
struct CTCKraken_00c6a780 : CCountedBlock_00c6a780 {
    CHolder_00c6a780 m_25c;
    void* m_pBlock;
    unsigned char m_294;
    void Shutdown();
};
void CTCKraken_00c6a780::Shutdown()
{
    m_294 = 0;
    if (m_pBlock) {
        Free_00c6a780(m_pBlock);
        m_pBlock = 0;
    }
    ReleaseBlock();
    m_25c.Clear();
}
int main()
{
    int block;
    CTCKraken_00c6a780 k; k.m_294 = 1; k.m_pBlock = &block;
    k.Shutdown();
    if (std::strcmp(g_log, "FRC") != 0 || g_freed != &block || k.m_pBlock != 0 || k.m_294 != 0) { std::printf("00c6a3d0_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0; k.m_294 = 1;
    k.Shutdown();
    if (std::strcmp(g_log, "RC") != 0 || k.m_294 != 0) { std::printf("00c6a3d0_TEST FAIL\n"); return 1; }
    std::printf("00c6a3d0_TEST PASS\n");
    return 0;
}

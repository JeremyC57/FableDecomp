#include <cstdio>
typedef long long LONGLONG;
static int g_n; static LONGLONG g_sq, g_tot;
struct CBaseVideoRenderer {
    int m_cFramesDrawn; LONGLONG m_iTotAcc, m_iSumSqAcc, m_iSumSqFrameTime, m_iSumFrameTime;
    long GetStdDev(int nSamples, int* piResult, LONGLONG llSumSq, LONGLONG iTot) { g_n = nSamples; g_sq = llSumSq; g_tot = iTot; *piResult = 42; return 0; }
    long get_DevSyncOffset(int* piResult) { return GetStdDev(m_cFramesDrawn - 1, piResult, m_iSumSqAcc, m_iTotAcc); }
};
int main()
{
    CBaseVideoRenderer r; r.m_cFramesDrawn = 10; r.m_iTotAcc = 1; r.m_iSumSqAcc = 2; r.m_iSumSqFrameTime = 3; r.m_iSumFrameTime = 4;
    int v = 0;
    long hr = r.get_DevSyncOffset(&v);
    if (hr != 0 || v != 42 || g_n != 10 - 1 || g_sq != r.m_iSumSqAcc || g_tot != r.m_iTotAcc) { std::printf("00ca6020_TEST FAIL\n"); return 1; }
    std::printf("00ca6020_TEST PASS\n");
    return 0;
}
#include <cstdio>
// CDeferredCommand::GetHResult: E_POINTER for null; E_ABORT while still queued; else the stored result.
struct DeferredCommand {
    void* m_pQueue; int m_hrResult;
    int GetHResult(int* p) { if (p == 0) return (int)0x80004003; if (m_pQueue != 0) return (int)0x80004004; *p = m_hrResult; return 0; }
};
int main()
{
    int q; DeferredCommand c; c.m_pQueue = &q; c.m_hrResult = (int)0x80004005; int r = 0;
    if (c.GetHResult(0) != (int)0x80004003) { std::printf("00caecc0_TEST FAIL\n"); return 1; }
    if (c.GetHResult(&r) != (int)0x80004004 || r != 0) { std::printf("00caecc0_TEST FAIL\n"); return 1; }
    c.m_pQueue = 0; if (c.GetHResult(&r) != 0 || r != (int)0x80004005) { std::printf("00caecc0_TEST FAIL\n"); return 1; }
    std::printf("00caecc0_TEST PASS\n");
    return 0;
}
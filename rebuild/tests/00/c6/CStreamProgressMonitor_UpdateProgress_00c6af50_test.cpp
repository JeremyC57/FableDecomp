#include <cstdio>
// Behaviour model of CStreamProgressMonitor::UpdateProgress (0x00C6AF50): an accepted, non-quiet step adds to the
// count and fails once the count exceeds twice the limit; any other step resets the count; returns the verdict.
struct Monitor {
    bool accept; long m_limit, m_count;
    bool Update(bool quiet, long amount)
    {
        bool accepted = accept;
        if (accepted && !quiet) {
            m_count += amount;
            if (m_count > m_limit * 2) return false;
        } else {
            m_count = 0;
        }
        return accepted;
    }
};
int main()
{
    Monitor m = { true, 5, 0 };
    if (!m.Update(false, 6) || m.m_count != 6) { std::printf("00c6af50_TEST FAIL\n"); return 1; }
    if (m.Update(false, 5) || m.m_count != 11) { std::printf("00c6af50_TEST FAIL\n"); return 1; }
    if (!m.Update(true, 5) || m.m_count != 0) { std::printf("00c6af50_TEST FAIL\n"); return 1; }
    m.accept = false; m.m_count = 4;
    if (m.Update(false, 1) || m.m_count != 0) { std::printf("00c6af50_TEST FAIL\n"); return 1; }
    std::printf("00c6af50_TEST PASS\n");
    return 0;
}
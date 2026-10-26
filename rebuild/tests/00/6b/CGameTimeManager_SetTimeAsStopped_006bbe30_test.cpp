#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static const long* g_handle = 0; static long g_value = -1; struct SSetIterator { long m_a, m_b; };
struct CStopperSet { unsigned char _body[12]; SSetIterator* Insert(SSetIterator* result, const long* handle) { std::strcat(g_seq, "I"); g_handle = handle; g_value = *handle; return result; } void Erase(const long* handle) { std::strcat(g_seq, "E"); g_handle = handle; g_value = *handle; } };
class CGameTimeManager { public: unsigned char _pad[0x2c]; CStopperSet m_stoppers; long m_nextStopperHandle;
    void SetTimeAsStopped(bool stopped, long* handle) { if (stopped) { *handle = m_nextStopperHandle++; SSetIterator where; m_stoppers.Insert(&where, handle); } else { m_stoppers.Erase(handle); } } };

int main() {
    CGameTimeManager m; std::memset(&m, 0, sizeof m); m.m_nextStopperHandle = 7; long h = -1; m.SetTimeAsStopped(true, &h); if (h != 7 || m.m_nextStopperHandle != 8 || std::strcmp(g_seq, "I") || g_handle != &h || g_value != 7) { std::printf("006bbe30_TEST FAIL\n"); return 1; }
    m.SetTimeAsStopped(true, &h); if (h != 8 || m.m_nextStopperHandle != 9 || std::strcmp(g_seq, "II")) { std::printf("006bbe30_TEST FAIL\n"); return 1; } m.SetTimeAsStopped(false, &h); if (std::strcmp(g_seq, "IIE") || g_handle != &h || g_value != 8 || m.m_nextStopperHandle != 9) { std::printf("006bbe30_TEST FAIL\n"); return 1; }
    std::printf("006bbe30_TEST PASS\n"); return 0;
}
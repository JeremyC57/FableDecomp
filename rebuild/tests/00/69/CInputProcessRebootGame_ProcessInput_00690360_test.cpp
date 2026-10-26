#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CInputEvent { public: long m_kind; long GetKind() const { std::strcat(g_seq, "K"); return m_kind; } void MarkAsUsed() const { std::strcat(g_seq, "M"); } }; class CProcessedInput { public: void RequestReboot() { std::strcat(g_seq, "R"); } };
class CInputProcessRebootGame { public: virtual void v000() { } virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed) { if (event.GetKind() == 0x15) { event.MarkAsUsed(); processed.RequestReboot(); } } };

int main() {
    CInputProcessRebootGame r; CInputEvent e; CProcessedInput p; e.m_kind = 0x15; r.ProcessInput(e, p); if (std::strcmp(g_seq, "KMR")) { std::printf("00690360_TEST FAIL\n"); return 1; } g_seq[0] = 0; e.m_kind = 0x14; r.ProcessInput(e, p); if (std::strcmp(g_seq, "K")) { std::printf("00690360_TEST FAIL\n"); return 1; }
    std::printf("00690360_TEST PASS\n"); return 0;
}
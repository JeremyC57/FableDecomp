#include <cstdio>
#include <cstdlib>
#include <cstring>
static bool g_conscious = true, g_alert = false; struct CThing { virtual bool IsConscious() { return g_conscious; } virtual bool IsAlert() { return g_alert; } };
struct CCounter { unsigned char _pad[0x1c8]; long m_other; long m_alert; long m_unconscious; void Apply(CThing* thing) { if (!thing->IsConscious()) ++m_unconscious; else if (thing->IsAlert()) ++m_alert; else ++m_other; } };

int main() {
    CCounter c; std::memset(&c, 0, sizeof c); CThing t; g_conscious = false; c.Apply(&t); if (c.m_unconscious != 1 || c.m_alert || c.m_other) { std::printf("00c3eaa0_TEST FAIL\n"); return 1; } g_conscious = true; g_alert = true; c.Apply(&t); if (c.m_alert != 1) { std::printf("00c3eaa0_TEST FAIL\n"); return 1; } g_alert = false; c.Apply(&t); if (c.m_other != 1) { std::printf("00c3eaa0_TEST FAIL\n"); return 1; }
    std::printf("00c3eaa0_TEST PASS\n"); return 0;
}
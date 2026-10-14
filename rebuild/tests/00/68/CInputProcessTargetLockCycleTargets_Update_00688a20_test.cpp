#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_held = true; static long g_a = 0, g_b = 0; struct CInputEvent { unsigned char _body[0x34]; CInputEvent() { std::strcat(g_seq, "c"); } ~CInputEvent() { std::strcat(g_seq, "d"); } };
struct CObject { virtual void v00() { } unsigned char _pad04[0x24]; long m_cycleIndex; long m_2c; long m_target; bool IsButtonHeld(long a, long b) { std::strcat(g_seq, "Q"); g_a = a; g_b = b; return g_held; }
    void Update() { CInputEvent event; if (!IsButtonHeld(1, 2)) { m_cycleIndex = 0; m_target = -1; } } };

int main() {
    CObject o; o.m_cycleIndex = 3; o.m_target = 9; g_held = true; o.Update(); if (std::strcmp(g_seq, "cQd") || o.m_cycleIndex != 3 || g_a != 1 || g_b != 2) { std::printf("00688a20_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_held = false; o.Update(); if (std::strcmp(g_seq, "cQd") || o.m_cycleIndex != 0 || o.m_target != -1) { std::printf("00688a20_TEST FAIL\n"); return 1; }
    std::printf("00688a20_TEST PASS\n"); return 0;
}
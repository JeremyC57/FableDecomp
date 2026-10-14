#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static long g_asked = -1;
struct CInputEvent { long m_device; long m_number; long GetDevice() const { std::strcat(g_seq, "D"); return m_device; } long GetDeviceNumber() const { std::strcat(g_seq, "N"); return m_number; } };
struct CPlayer { long m_using; bool IsUsingJoystickDeviceNumber(long n) { std::strcat(g_seq, "U"); g_asked = n; return n == m_using; } };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface; };
struct CInputProcessMain : CInputProcessGameBase { bool IsEventFromPlayerDevice(const CInputEvent& event) { if (event.GetDevice() == 1) return MyPlayer->IsUsingJoystickDeviceNumber(event.GetDeviceNumber()); return true; } };
int main()
{
    CPlayer player; player.m_using = 3; CInputProcessMain m; m.MyPlayer = &player; CInputEvent e; e.m_device = 2; e.m_number = 9;
    if (!m.IsEventFromPlayerDevice(e) || std::strcmp(g_seq, "D")) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; e.m_device = 1; if (m.IsEventFromPlayerDevice(e) || std::strcmp(g_seq, "DNU") || g_asked != 9) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    e.m_number = 3; if (!m.IsEventFromPlayerDevice(e)) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    std::printf("00687db0_TEST PASS\n");
    return 0;
}

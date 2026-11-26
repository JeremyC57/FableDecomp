// Behaviour test for 0x00687DB0 CInputProcessGameBase::IsInputEventToBeSentToThisInputProcess (NF478 respell): links the row object,
// supplies the event accessors and CPlayer::IsUsingJoystickDeviceNumber, and calls the protected member through a derived accessor.
#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static long g_asked = -1;
class CInputEvent { public: long m_device; long m_number;
    long GetDevice() const;
    long GetDeviceNumber() const;
};
long CInputEvent::GetDevice() const { std::strcat(g_seq, "D"); return m_device; }
long CInputEvent::GetDeviceNumber() const { std::strcat(g_seq, "N"); return m_number; }
class CPlayer { public: long m_using;
    bool IsUsingJoystickDeviceNumber(long deviceNumber) const;
};
bool CPlayer::IsUsingJoystickDeviceNumber(long n) const { std::strcat(g_seq, "U"); g_asked = n; return n == m_using; }
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessGameBase.h"
struct Probe : CInputProcessGameBase {
    Probe(); // never constructed: the test works on raw storage
    bool Ask(const CInputEvent& e) const { return CInputProcessGameBase::IsInputEventToBeSentToThisInputProcess(e); }
};
static unsigned char g_process[0x40];
int main()
{
    CPlayer player; player.m_using = 3; Probe* m = (Probe*)g_process; m->MyPlayer = &player;
    CInputEvent e; e.m_device = 2; e.m_number = 9;
    if (!m->Ask(e) || std::strcmp(g_seq, "D")) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; e.m_device = 1; if (m->Ask(e) || std::strcmp(g_seq, "DNU") || g_asked != 9) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    e.m_number = 3; if (!m->Ask(e)) { std::printf("00687db0_TEST FAIL\n"); return 1; }
    std::printf("00687db0_TEST PASS\n");
    return 0;
}

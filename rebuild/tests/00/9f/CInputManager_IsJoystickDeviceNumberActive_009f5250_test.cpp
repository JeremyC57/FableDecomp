#include <cstdio>
static long g_calls = 0;
struct CJoystick { long m_number; virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual long GetDeviceNumber() { ++g_calls; return m_number; } };
struct CCountedPointer_CJoystick { CJoystick* m_pObject; void* m_pCount; };
struct CInputManager {
    virtual void v00() { }
    unsigned char _pad04[0x28];
    CCountedPointer_CJoystick* m_pJoysticksFirst; CCountedPointer_CJoystick* m_pJoysticksLast; CCountedPointer_CJoystick* m_pJoysticksEnd;
    bool IsJoystickDeviceNumberActive(long deviceNumber)
    {
        long count = m_pJoysticksLast - m_pJoysticksFirst;
        for (long i = 0; i < count; ++i) { if (deviceNumber == m_pJoysticksFirst[i].m_pObject->GetDeviceNumber()) return true; }
        return false;
    }
};
int main()
{
    CInputManager m; CJoystick j1, j2, j3; j1.m_number = 4; j2.m_number = 9; j3.m_number = 2; CCountedPointer_CJoystick js[3]; js[0].m_pObject = &j1; js[1].m_pObject = &j2; js[2].m_pObject = &j3;
    m.m_pJoysticksFirst = js; m.m_pJoysticksLast = js + 3;
    if (!m.IsJoystickDeviceNumberActive(9) || g_calls != 2) { std::printf("009f5250_TEST FAIL\n"); return 1; }
    g_calls = 0; if (m.IsJoystickDeviceNumberActive(5) || g_calls != 3) { std::printf("009f5250_TEST FAIL\n"); return 1; }
    g_calls = 0; m.m_pJoysticksLast = js; if (m.IsJoystickDeviceNumberActive(4) || g_calls != 0) { std::printf("009f5250_TEST FAIL\n"); return 1; }
    std::printf("009f5250_TEST PASS\n");
    return 0;
}
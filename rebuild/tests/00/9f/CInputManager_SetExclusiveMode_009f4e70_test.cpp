#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static long g_last = -1;
class CMouse { public: virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void SetExclusiveMode(long f) { std::strcat(g_seq, "M"); g_last = f; } };
class CKeyboard { public: virtual void v00() { } virtual void v01() { } virtual void SetExclusiveMode(long f) { std::strcat(g_seq, "K"); g_last = f; } };
class CJoystick { public: virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { }
    virtual void SetExclusiveMode(long f) { std::strcat(g_seq, "J"); g_last = f; } };
template<class T> struct scoped_ptr { T* ptr; T* get() const { return ptr; } T* operator->() const { return ptr; } };
struct CCountedPointer_CJoystick { CJoystick* m_pObject; void* m_pCount; };
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x20];
    scoped_ptr<CMouse> PMouse; scoped_ptr<CKeyboard> PKeyboard;
    CCountedPointer_CJoystick* m_pJoysticksFirst; CCountedPointer_CJoystick* m_pJoysticksLast; CCountedPointer_CJoystick* m_pJoysticksEnd;
    bool SetExclusiveMode(long exclusive)
    {
        if (PMouse.get()) PMouse->SetExclusiveMode(exclusive);
        if (PKeyboard.get()) PKeyboard->SetExclusiveMode(exclusive);
        long count = m_pJoysticksLast - m_pJoysticksFirst;
        for (long i = 0; i < count; ++i) { CJoystick* joystick = m_pJoysticksFirst[i].m_pObject; if (joystick) joystick->SetExclusiveMode(exclusive); }
        return true;
    }
};
int main()
{
    CInputManager m; CMouse mouse; CKeyboard kb; CJoystick j1; CCountedPointer_CJoystick js[3]; js[0].m_pObject = &j1; js[1].m_pObject = 0; js[2].m_pObject = &j1;
    m.PMouse.ptr = &mouse; m.PKeyboard.ptr = &kb; m.m_pJoysticksFirst = js; m.m_pJoysticksLast = js + 3;
    if (!m.SetExclusiveMode(1) || std::strcmp(g_seq, "MKJJ") || g_last != 1) { std::printf("009f4e70_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.PKeyboard.ptr = 0; m.m_pJoysticksLast = js; if (!m.SetExclusiveMode(0) || std::strcmp(g_seq, "M") || g_last != 0) { std::printf("009f4e70_TEST FAIL\n"); return 1; }
    std::printf("009f4e70_TEST PASS\n");
    return 0;
}
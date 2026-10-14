#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static long g_last = -1;
struct CMouse { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { } virtual void v12() { } virtual void v13() { }
    virtual void SetHasInputFocus(long f) { std::strcat(g_seq, "M"); g_last = f; } };
struct CKeyboard { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void SetHasInputFocus(long f) { std::strcat(g_seq, "K"); g_last = f; } };
struct CJoystick { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { } virtual void v12() { } virtual void v13() { } virtual void v14() { } virtual void v15() { }
    virtual void SetHasInputFocus(long f) { std::strcat(g_seq, "J"); g_last = f; } };
template<class T> struct scoped_ptr { T* ptr; T* get() const { return ptr; } T* operator->() const { return ptr; } };
struct CCountedPointer_CJoystick { CJoystick* m_pObject; void* m_pCount; };
struct CInputManager {
    virtual void v00() { }
    unsigned char _pad04[0x20];
    scoped_ptr<CMouse> PMouse; scoped_ptr<CKeyboard> PKeyboard;
    CCountedPointer_CJoystick* m_pJoysticksFirst; CCountedPointer_CJoystick* m_pJoysticksLast; CCountedPointer_CJoystick* m_pJoysticksEnd;
    void SetHasInputFocus(long hasFocus)
    {
        if (PMouse.get()) PMouse->SetHasInputFocus(hasFocus);
        if (PKeyboard.get()) PKeyboard->SetHasInputFocus(hasFocus);
        long count = m_pJoysticksLast - m_pJoysticksFirst;
        for (long i = 0; i < count; ++i) m_pJoysticksFirst[i].m_pObject->SetHasInputFocus(hasFocus);
    }
};
int main()
{
    CInputManager m; CMouse mouse; CKeyboard kb; CJoystick j1, j2; CCountedPointer_CJoystick js[2]; js[0].m_pObject = &j1; js[1].m_pObject = &j2;
    m.PMouse.ptr = &mouse; m.PKeyboard.ptr = &kb; m.m_pJoysticksFirst = js; m.m_pJoysticksLast = js + 2;
    m.SetHasInputFocus(1); if (std::strcmp(g_seq, "MKJJ") || g_last != 1) { std::printf("009f4e20_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.PMouse.ptr = 0; m.m_pJoysticksLast = js; m.SetHasInputFocus(0); if (std::strcmp(g_seq, "K") || g_last != 0) { std::printf("009f4e20_TEST FAIL\n"); return 1; }
    std::printf("009f4e20_TEST PASS\n");
    return 0;
}
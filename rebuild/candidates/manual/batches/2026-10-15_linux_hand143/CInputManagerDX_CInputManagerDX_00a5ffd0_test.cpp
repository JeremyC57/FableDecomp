// Behaviour test for 0x00A5FFD0 (CInputManagerDX destructor): devices deleted, joysticks emptied, DirectInput released, then the base.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct Mouse { virtual ~Mouse() { g_log[g_n++] = 1; } };
struct Keyboard { virtual ~Keyboard() { g_log[g_n++] = 2; } };
struct DirectInput { int refs; unsigned int Release() { g_log[g_n++] = 4; return (unsigned int)--refs; } };
template <class T> struct ScopedPtr { T* m_p; void reset() { T* old = m_p; m_p = 0; if (old) delete old; } };
struct Joysticks { int count; void clear() { if (count) g_log[g_n++] = 3; count = 0; } };
struct InputManager { ScopedPtr<Mouse> PMouse; ScopedPtr<Keyboard> PKeyboard; Joysticks PJoysticks; virtual ~InputManager() { g_log[g_n++] = 5; } };
struct InputManagerDX : InputManager {
    DirectInput* m_pDirectInput;
    ~InputManagerDX() { PMouse.reset(); PKeyboard.reset(); PJoysticks.clear(); if (m_pDirectInput) m_pDirectInput->Release(); }
};
int main()
{
    DirectInput di; di.refs = 2;
    InputManagerDX* m = new InputManagerDX; m->PMouse.m_p = new Mouse; m->PKeyboard.m_p = new Keyboard; m->PJoysticks.count = 2; m->m_pDirectInput = &di;
    delete m;
    int ok = g_n == 5 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[3] == 4 && g_log[4] == 5 && di.refs == 1;
    if (!ok) { std::printf("00a5ffd0_TEST FAIL\n"); return 1; }
    std::printf("00a5ffd0_TEST PASS\n");
    return 0;
}

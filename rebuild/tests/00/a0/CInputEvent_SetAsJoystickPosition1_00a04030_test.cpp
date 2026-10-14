#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; }; struct C3DVector { float X, Y, Z; };
struct CInputEvent { long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsJoystickPosition1(const C2DVector& position, double time) {
        m_device = 1; m_kind = 17; *(C2DVector*)&m_key = position; m_time2 = (float)time; m_time = (float)time;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; e.SetAsJoystickPosition1(p, 9.0); if (e.m_kind != 17 || e.m_device != 1 || ((C2DVector*)&e.m_key)->X != 1 || ((C2DVector*)&e.m_key)->Y != 2 || e.m_time != 9.0f || e.m_time2 != 9.0f) { std::printf("00a04030_TEST FAIL\n"); return 1; }
    std::printf("00a04030_TEST PASS\n"); return 0;
}
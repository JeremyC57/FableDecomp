#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsJoystickButton3(bool pressed, long button, const C2DVector& position, double time) {
        m_8 = pressed; m_kind = 21; m_device = 1; *(C2DVector*)&m_key = position; m_time2 = (float)time; m_time = (float)time; *(long*)&m_movement.X = button;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; e.SetAsJoystickButton3(true, 4, p, 9.0); if (!e.m_8 || e.m_kind != 21 || e.m_device != 1 || ((C2DVector*)&e.m_key)->X != 1 || ((C2DVector*)&e.m_key)->Y != 2 || *(long*)&e.m_movement.X != 4 || e.m_time != 9.0f || e.m_time2 != 9.0f) { std::printf("00a04110_TEST FAIL\n"); return 1; }
    std::printf("00a04110_TEST PASS\n"); return 0;
}
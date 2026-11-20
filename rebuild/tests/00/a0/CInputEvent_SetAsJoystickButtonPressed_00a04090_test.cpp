// Behaviour test for 0x00A04090 CInputEvent::SetAsJoystickButtonPressed(long button, float pressure, const C2DVector&, double) (host model;
// NF133 respelled it from SetAsJoystickButton1(bool, long, ...) to the PDB parameter order: button byte at +8, pressure at +0xc).
#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; unsigned char m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsJoystickButtonPressed(long button, float pressure, const C2DVector& position, double time) {
        m_8 = (unsigned char)button; m_kind = 19; m_device = 1; *(C2DVector*)&m_key = position; m_time2 = (float)time; m_time = (float)time; m_movement.X = pressure;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; e.SetAsJoystickButtonPressed(4, 0.5f, p, 9.0); if (e.m_8 != 4 || e.m_kind != 19 || e.m_device != 1 || ((C2DVector*)&e.m_key)->X != 1 || ((C2DVector*)&e.m_key)->Y != 2 || e.m_movement.X != 0.5f || e.m_time != 9.0f || e.m_time2 != 9.0f) { std::printf("00a04090_TEST FAIL\n"); return 1; }
    std::printf("00a04090_TEST PASS\n"); return 0;
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsExtraMouseButtonHeld(const C2DVector& position, const C3DVector& movement, double time2, double time, long button) {
        m_kind = button + 0x1b; m_device = 3; m_position = position; m_time2 = (float)time2; m_time = (float)time; *(C3DVector*)&m_key = movement;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; C3DVector m = { 5, 6, 7 }; e.SetAsExtraMouseButtonHeld(p, m, 1.0, 2.0, 1); if (e.m_kind != 0x1c || e.m_device != 3 || e.m_position.X != 1 || e.m_position.Y != 2 || e.m_time2 != 1.0f || e.m_time != 2.0f || ((C3DVector*)&e.m_key)->X != 5 || ((C3DVector*)&e.m_key)->Z != 7) { std::printf("00a03f20_TEST FAIL\n"); return 1; }
    std::printf("00a03f20_TEST PASS\n"); return 0;
}
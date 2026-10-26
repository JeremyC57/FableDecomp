#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsMouseMovement(const C2DVector& position, const C3DVector& movement, double time) {
        m_device = 3; m_kind = 13; m_movement = movement; m_position = position; m_time2 = (float)time; m_time = (float)time;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; C3DVector m = { 5, 6, 7 }; e.SetAsMouseMovement(p, m, 9.0); if (e.m_kind != 13 || e.m_device != 3 || e.m_movement.X != 5 || e.m_movement.Z != 7 || e.m_position.Y != 2 || e.m_time != 9.0f || e.m_time2 != 9.0f) { std::printf("00a03fb0_TEST FAIL\n"); return 1; }
    std::printf("00a03fb0_TEST PASS\n"); return 0;
}
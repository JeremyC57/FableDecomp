#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; }; struct C3DVector { float X, Y, Z; };
struct CInputEvent { long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsExtraMouseButtonReleased(const C2DVector& position, double time2, double time, long button) {
        m_kind = button + 0x20; m_device = 3; m_position = position; m_time2 = (float)time2; m_time = (float)time;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); C2DVector p = { 1, 2 }; e.SetAsExtraMouseButtonReleased(p, 1.0, 2.0, 3); if (e.m_kind != 0x23 || e.m_device != 3 || e.m_position.X != 1 || e.m_position.Y != 2 || e.m_time2 != 1.0f || e.m_time != 2.0f) { std::printf("00a03f70_TEST FAIL\n"); return 1; }
    std::printf("00a03f70_TEST PASS\n"); return 0;
}

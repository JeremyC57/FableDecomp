#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; }; struct C3DVector { float X, Y, Z; };
struct CInputEvent { long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsMouseWheel(long delta, double time) {
        m_time2 = (float)time; m_device = 3; m_time = (float)time; m_kind = 14; *(long*)&m_movement.Z = delta;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); e.SetAsMouseWheel(-120, 9.0); if (e.m_kind != 14 || e.m_device != 3 || *(long*)&e.m_movement.Z != -120 || e.m_time != 9.0f || e.m_time2 != 9.0f) { std::printf("00a04000_TEST FAIL\n"); return 1; }
    std::printf("00a04000_TEST PASS\n"); return 0;
}
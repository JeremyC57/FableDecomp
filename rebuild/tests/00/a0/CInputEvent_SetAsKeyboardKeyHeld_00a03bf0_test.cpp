#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; }; struct C3DVector { float X, Y, Z; };
struct CInputEvent { long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsKeyboardKeyHeld(long key, double time) {
        m_time = (float)time; m_kind = 1; m_time2 = (float)time; m_device = 2; m_key = key;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); e.SetAsKeyboardKeyHeld(65, 2.5); if (e.m_key != 65 || e.m_kind != 1 || e.m_device != 2 || e.m_time != 2.5f || e.m_time2 != 2.5f) { std::printf("00a03bf0_TEST FAIL\n"); return 1; }
    std::printf("00a03bf0_TEST PASS\n"); return 0;
}
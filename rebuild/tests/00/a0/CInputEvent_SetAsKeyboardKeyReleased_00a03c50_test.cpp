#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsKeyboardKeyReleased(long key, double time2, double time) {
        m_time2 = (float)time2; m_device = 2; m_time = (float)time; m_kind = 3; m_key = key;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); e.SetAsKeyboardKeyReleased(66, 1.5, 2.5); if (e.m_key != 66 || e.m_kind != 3 || e.m_device != 2 || e.m_time != 2.5f || e.m_time2 != 1.5f) { std::printf("00a03c50_TEST FAIL\n"); return 1; }
    std::printf("00a03c50_TEST PASS\n"); return 0;
}
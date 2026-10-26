#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; class C3DVector { public: float X, Y, Z; };
class CInputEvent { public: long m_key; long m_4; bool m_8; unsigned char _pad09[3]; C3DVector m_movement; C2DVector m_position; long m_device; long m_24; long m_kind; float m_time; float m_time2;
    void SetAsKeyboardCharacter(unsigned short character, double time) {
        m_time2 = (float)time; m_device = 2; m_time = (float)time; m_kind = 15; *(unsigned short*)&m_key = character;
    }
};

int main() {
    CInputEvent e; std::memset(&e, 0, sizeof e); e.m_key = -1; e.SetAsKeyboardCharacter(0x4142, 3.0); if ((unsigned short)e.m_key != 0x4142 || (e.m_key >> 16) != -1 || e.m_kind != 15 || e.m_device != 2 || e.m_time != 3.0f || e.m_time2 != 3.0f) { std::printf("00a03cb0_TEST FAIL\n"); return 1; }
    std::printf("00a03cb0_TEST PASS\n"); return 0;
}
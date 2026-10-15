#include <cstdio>
struct CTCEnflameMoveComponent { bool m_moving; long m_state; };
struct CThing {
    bool hasMove; CTCEnflameMoveComponent* move;
    template <class T> bool GetInterface(long type, T** out) { if (type == 0xA7 && hasMove) { *out = (T*)move; return true; } return false; }
};
struct CTCWillEnflame {
    CThing* ParentThing; long m_moveState;
    void CancelMove()
    {
        CTCEnflameMoveComponent* move;
        if (ParentThing->GetInterface(0xA7, &move)) {
            if (move->m_state != 5) return;
            move->m_moving = false; move->m_state = 0;
        }
        m_moveState = 3;
    }
};
int main()
{
    CTCEnflameMoveComponent m; m.m_moving = true; m.m_state = 5;
    CThing t; t.hasMove = true; t.move = &m; CTCWillEnflame w; w.ParentThing = &t; w.m_moveState = 1;
    w.CancelMove(); if (m.m_moving || m.m_state != 0 || w.m_moveState != 3) { std::printf("007c6a40_TEST FAIL\n"); return 1; }
    w.m_moveState = 1; m.m_state = 4; m.m_moving = true; w.CancelMove(); if (!m.m_moving || m.m_state != 4 || w.m_moveState != 1) { std::printf("007c6a40_TEST FAIL\n"); return 1; }
    t.hasMove = false; w.CancelMove(); if (w.m_moveState != 3) { std::printf("007c6a40_TEST FAIL\n"); return 1; }
    std::printf("007c6a40_TEST PASS\n");
    return 0;
}
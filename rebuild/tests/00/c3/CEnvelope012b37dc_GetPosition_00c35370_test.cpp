// Behaviour test for CEnvelope_012b37dc::GetPosition (0x00C35370): update, then fraction = ticks done / (attack+loop+release), loops left (0 once releasing; done -> 1.0).
#include <stdio.h>
struct Pos { long m_loopsLeft; float m_fraction; };
struct Env {
    int updates; long m_loopsLeft; unsigned long m_pos, m_state, m_attack, m_loop, m_release;
    void Update(float) { ++updates; }
    void GetPosition(void*, Pos* out)
    {
        Update(1.0f);
        float total = (float)(m_release + m_attack + m_loop);
        unsigned long done;
        switch (m_state) {
        case 0: done = m_pos; out->m_loopsLeft = m_loopsLeft; out->m_fraction = (float)done / total; break;
        case 1: done = m_pos + m_attack; out->m_loopsLeft = m_loopsLeft; out->m_fraction = (float)done / total; break;
        case 2: done = m_pos + m_attack + m_loop; out->m_loopsLeft = 0; out->m_fraction = (float)done / total; break;
        default: out->m_loopsLeft = 0; out->m_fraction = 1.0f; break;
        }
    }
};
int main()
{
    Env e = { 0, 3, 10, 0, 20, 40, 40 }; Pos p;
    e.GetPosition(0, &p); if (p.m_loopsLeft != 3 || p.m_fraction != 0.1f || e.updates != 1) { printf("00c35370_TEST FAIL\n"); return 1; }
    e.m_state = 1; e.GetPosition(0, &p); if (p.m_loopsLeft != 3 || p.m_fraction != 0.3f) { printf("00c35370_TEST FAIL\n"); return 1; }
    e.m_state = 2; e.GetPosition(0, &p); if (p.m_loopsLeft != 0 || p.m_fraction != 0.7f) { printf("00c35370_TEST FAIL\n"); return 1; }
    e.m_state = 3; e.GetPosition(0, &p); if (p.m_loopsLeft != 0 || p.m_fraction != 1.0f) { printf("00c35370_TEST FAIL\n"); return 1; }
    printf("00c35370_TEST PASS\n"); return 0;
}
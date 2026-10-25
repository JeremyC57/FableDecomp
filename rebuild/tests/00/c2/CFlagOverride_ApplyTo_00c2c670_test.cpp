// Behaviour test for CFlagOverride::ApplyTo @ 0x00C2C670 (apply the two (mask, value) pairs, group A first, and return the target).
#include <cstdio>

class CFlagState {
public:
    unsigned short m_a, m_b;
    int calls;
    int order[2];
    void SetGroupA(unsigned short mask, unsigned short value) { m_a = (unsigned short)((m_a & ~mask) | (mask & value)); order[calls++] = 1; }
    void SetGroupB(unsigned short mask, unsigned short value) { m_b = (unsigned short)((m_b & ~mask) | (mask & value)); order[calls++] = 2; }
};

class CFlagOverride {
public:
    unsigned short m_maskB, m_valueB, m_maskA, m_valueA;
    CFlagState& ApplyTo(CFlagState& target) const
    {
        target.SetGroupA(m_maskA, m_valueA);
        target.SetGroupB(m_maskB, m_valueB);
        return target;
    }
};

int main()
{
    CFlagState state;
    state.m_a = 0x00F0; state.m_b = 0xFF00; state.calls = 0;
    CFlagOverride o;
    o.m_maskA = 0x000F; o.m_valueA = 0x0005;
    o.m_maskB = 0x0F00; o.m_valueB = 0x0000;
    CFlagState& r = o.ApplyTo(state);
    bool ok = &r == &state && state.m_a == 0x00F5 && state.m_b == 0xF000 && state.calls == 2;
    ok = ok && state.order[0] == 1 && state.order[1] == 2;
    if (!ok) { std::printf("00c2c670_TEST FAIL\n"); return 1; }
    std::printf("00c2c670_TEST PASS\n");
    return 0;
}
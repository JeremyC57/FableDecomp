// Behaviour test for 00c20840: CSoundEvent::IsActive -- non-zero state word means active
#include <cstdio>
struct CSoundEvent { char pad[0xAC]; unsigned short m_state; long IsActive() const { return m_state != 0; } };
int main()
{
    CSoundEvent e; e.m_state = 0;
    if (e.IsActive() != 0) { std::printf("00c20840_TEST FAIL\n"); return 1; }
    e.m_state = 5;
    if (e.IsActive() != 1) { std::printf("00c20840_TEST FAIL\n"); return 1; }
    std::printf("00c20840_TEST PASS\n");
    return 0;
}

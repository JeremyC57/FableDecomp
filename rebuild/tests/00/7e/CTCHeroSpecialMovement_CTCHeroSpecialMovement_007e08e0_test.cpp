#include <cstdio>
static void* g_owner = 0;
struct CTCBase_00686800 { virtual void v00() { } unsigned char _pad[8]; CTCBase_00686800(void* o) { g_owner = o; } };
struct SZeroWord_007e08e0 { long m_value; SZeroWord_007e08e0() : m_value(0) { } };
class CTCHeroSpecialMovement : public CTCBase_00686800 { public: bool m_active; bool m_pending; unsigned char _pad0e[6]; long m_target; SZeroWord_007e08e0 m_timer;
    CTCHeroSpecialMovement(void* owner) : CTCBase_00686800(owner), m_active(false), m_pending(false), m_target(0), m_timer() { } };
int main() {
    int owner = 0; CTCHeroSpecialMovement m(&owner); if (g_owner != &owner || m.m_active || m.m_pending || m.m_target || m.m_timer.m_value) { std::printf("007e08e0_TEST FAIL\n"); return 1; }
    std::printf("007e08e0_TEST PASS\n"); return 0;
}
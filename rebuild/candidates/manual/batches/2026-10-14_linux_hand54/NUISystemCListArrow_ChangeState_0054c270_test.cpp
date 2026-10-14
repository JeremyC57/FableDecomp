#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_state = -1; struct CBase { unsigned char _pad[0x18c]; void ChangeState(long s) { g_state = s; } }; struct CListArrow : CBase { bool m_armed; void ChangeState(long s) { if (m_armed) { CBase::ChangeState(s); m_armed = false; } } };

int main() {
    CListArrow a; a.m_armed = true; a.ChangeState(5); if (g_state != 5 || a.m_armed) { std::printf("0054c270_TEST FAIL\n"); return 1; } a.ChangeState(6); if (g_state != 5) { std::printf("0054c270_TEST FAIL\n"); return 1; }
    std::printf("0054c270_TEST PASS\n"); return 0;
}

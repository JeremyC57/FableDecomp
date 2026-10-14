#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseIP { virtual void v00() { } void* m_pItem; CBaseIP() : m_pItem(0) { std::strcat(g_seq, "c"); } void SetPItem(void* i) { m_pItem = i; std::strcat(g_seq, "S"); } };
struct CIP : CBaseIP { CIP() : CBaseIP() { } CIP(void* i) : CBaseIP() { SetPItem(i); } }; struct CLightningBranch { CIP m_target; CIP m_other; long m_10; CLightningBranch(void* t) : m_target(t), m_other(), m_10(0) { } };

int main() {
    int item; CLightningBranch b(&item); if (b.m_target.m_pItem != &item || b.m_other.m_pItem != 0 || b.m_10 != 0 || std::strcmp(g_seq, "cSc")) { std::printf("007f3ad0_TEST FAIL\n"); return 1; }
    std::printf("007f3ad0_TEST PASS\n"); return 0;
}

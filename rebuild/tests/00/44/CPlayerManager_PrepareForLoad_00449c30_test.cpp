// Behaviour test for CPlayerManager::PrepareForLoad (0x00449C30): every player in the vector is prepared once, in order.
#include <cstdio>
static int g_order[8]; static int g_count = 0;
struct CPlayer { int id; void PrepareForLoad() { g_order[g_count++] = id; } };
struct CPlayerVector {
    CPlayer** m_pFirst; CPlayer** m_pLast; CPlayer** m_pEnd;
    unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); }
    CPlayer*& operator[](unsigned int i) { return m_pFirst[i]; }
};
struct CPlayerManager {
    CPlayerVector Players;
    void PrepareForLoad() { for (unsigned int i = 0; i < Players.size(); ++i) Players[i]->PrepareForLoad(); }
};
int main()
{
    CPlayer a, b, c; a.id = 1; b.id = 2; c.id = 3;
    CPlayer* list[3] = { &a, &b, &c };
    CPlayerManager m; m.Players.m_pFirst = list; m.Players.m_pLast = list + 3; m.Players.m_pEnd = list + 3;
    m.PrepareForLoad();
    bool ok = g_count == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3;
    m.Players.m_pLast = list; g_count = 0; m.PrepareForLoad();
    ok = ok && g_count == 0;
    if (!ok) { std::printf("00449c30_TEST FAIL\n"); return 1; }
    std::printf("00449c30_TEST PASS\n");
    return 0;
}
#include <cstdio>
enum EPlayerMode { PLAYER_MODE_NULL = 0, PLAYER_MODE_A = 3, PLAYER_MODE_B = 7, PLAYER_MODE_C = 11 };
struct SModeNode { SModeNode* m_pNext; SModeNode* m_pPrev; EPlayerMode m_value; };
struct CPlayer {
    SModeNode* PlayerModes;
    bool HasMode(EPlayerMode mode) const
    {
        SModeNode* head = PlayerModes;
        SModeNode* node = head->m_pNext;
        while (node != head && node->m_value != mode)
            node = node->m_pNext;
        return node != head;
    }
};
int main()
{
    SModeNode head; head.m_pNext = &head; head.m_pPrev = &head; head.m_value = PLAYER_MODE_C;
    CPlayer p; p.PlayerModes = &head;
    if (p.HasMode(PLAYER_MODE_C)) { std::printf("00634160_TEST FAIL\n"); return 1; }
    SModeNode a, b; head.m_pNext = &a; a.m_pNext = &b; b.m_pNext = &head; a.m_value = PLAYER_MODE_A; b.m_value = PLAYER_MODE_B;
    if (!p.HasMode(PLAYER_MODE_A) || !p.HasMode(PLAYER_MODE_B) || p.HasMode(PLAYER_MODE_C)) { std::printf("00634160_TEST FAIL\n"); return 1; }
    std::printf("00634160_TEST PASS\n");
    return 0;
}
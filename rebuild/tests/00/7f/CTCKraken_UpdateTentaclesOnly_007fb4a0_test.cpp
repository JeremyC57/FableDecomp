#include <cstdio>
struct CTCKrakenTentacle { bool Tracking; };
struct CThing {
    unsigned long m_presentBits[9]; CTCKrakenTentacle* m_tentacle;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) { *out = (T*)m_tentacle; return true; }
        return false;
    }
};
struct CIntelligentPointer { CThing* m_pItem; CThing* Get() const { return m_pItem; } };
struct STentacleNode { STentacleNode* _M_next; STentacleNode* _M_prev; CIntelligentPointer _M_data; };
struct STentacleIterator { STentacleNode* _M_node; CIntelligentPointer& operator*() const { return _M_node->_M_data; } };
struct CTCKraken {
    STentacleIterator AttackingTentacleIterator; STentacleNode* m_closest;
    bool GetClosestTentacleToTarget(STentacleIterator* closest) { if (!m_closest) return false; closest->_M_node = m_closest; return true; }
    void UpdateTentaclesOnly()
    {
        if (GetClosestTentacleToTarget(&AttackingTentacleIterator)) {
            CTCKrakenTentacle* tentacle = 0;
            (*AttackingTentacleIterator).Get()->GetInterface(227, &tentacle);
            tentacle->Tracking = true;
        }
    }
};
int main()
{
    CTCKrakenTentacle tc; tc.Tracking = false;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[7] = 8; thing.m_tentacle = &tc;
    STentacleNode node; node._M_next = node._M_prev = &node; node._M_data.m_pItem = &thing;
    CTCKraken k; k.AttackingTentacleIterator._M_node = 0; k.m_closest = 0;
    k.UpdateTentaclesOnly();
    if (tc.Tracking || k.AttackingTentacleIterator._M_node) { std::printf("007fb4a0_TEST FAIL\n"); return 1; }
    k.m_closest = &node; k.UpdateTentaclesOnly();
    if (!tc.Tracking || k.AttackingTentacleIterator._M_node != &node) { std::printf("007fb4a0_TEST FAIL\n"); return 1; }
    std::printf("007fb4a0_TEST PASS\n");
    return 0;
}
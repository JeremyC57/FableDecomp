// Behaviour test for 0x008C7480 CCreatureAction_TrollGroundPound::FinishAction: this action leaves the haste component's speed sources.
#include <cstdio>
static int g_base = 0;
struct CAction;
struct CActionSet {                         // minimal set<CAction*>: erase by key
    CAction* m_items[4]; int m_count;
    void insert(CAction* a) { m_items[m_count++] = a; }
    int erase(CAction* const& key) { for (int i = 0; i < m_count; ++i) if (m_items[i] == key) { m_items[i] = m_items[--m_count]; return 1; } return 0; }
    int size() const { return m_count; }
    int count(CAction* key) const { for (int i = 0; i < m_count; ++i) if (m_items[i] == key) return 1; return 0; }
};
class CTCHaste { public:
    CActionSet SpeedSources; int m_recalcs;
    void RecalculateSpeed() { ++m_recalcs; }
};
class CThing { public:
    unsigned int m_presentBits[9];
    CTCHaste* m_haste;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_haste; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    void FinishAction()
    {
        CTCHaste* pHaste;
        if (PCreature.GetPItem()->GetInterface(&pHaste, 0x8c)) {
            pHaste->SpeedSources.erase(this);
            pHaste->RecalculateSpeed();
        }
        ++g_base;
    }
};
int main()
{
    CTCHaste haste; haste.m_recalcs = 0; haste.SpeedSources.m_count = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_haste = &haste;
    CAction a, other; a.PCreature.m_pItem = &me;
    haste.SpeedSources.insert(&a); haste.SpeedSources.insert(&other);
    a.FinishAction();
    if (haste.SpeedSources.size() != 2 || haste.m_recalcs != 0 || g_base != 1) { std::printf("008c7480_TEST FAIL\n"); return 1; }
    me.m_presentBits[0x8c >> 5] |= 1u << (0x8c & 31);
    a.FinishAction();
    if (haste.SpeedSources.size() != 1 || haste.SpeedSources.count(&other) != 1 || haste.m_recalcs != 1 || g_base != 2) { std::printf("008c7480_TEST FAIL\n"); return 1; }
    std::printf("008c7480_TEST PASS\n");
    return 0;
}
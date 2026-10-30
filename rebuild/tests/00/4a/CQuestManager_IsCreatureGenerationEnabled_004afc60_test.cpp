// Behaviour test for CQuestManager::IsCreatureGenerationEnabled (retail 0x004AFC60): false while any quest named in
// CreatureGenerationDisabledQuests is active, true otherwise (including an empty list).
#include <cstdio>
#include <cstring>
class CCharString { public: const char* m_pString; };
class CQuestManager { public:
    CCharString* m_pFirst; CCharString* m_pLast;
    const char* active;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    bool IsQuestActive(const CCharString& n) { return active && std::strcmp(n.m_pString, active) == 0; }
    bool IsCreatureGenerationEnabled()
    {
        unsigned long i = 0;
        if (i < size()) {
            CCharString* questName = m_pFirst;
            do {
                if (IsQuestActive(*questName))
                    return false;
                ++i;
                ++questName;
            } while (i < size());
        }
        return true;
    }
};
int main()
{
    CCharString q[3] = { {"Q_A"}, {"Q_B"}, {"Q_C"} };
    CQuestManager m = { q, q + 3, 0 };
    bool ok = m.IsCreatureGenerationEnabled();
    m.active = "Q_C"; ok = ok && !m.IsCreatureGenerationEnabled();
    m.active = "Q_X"; ok = ok && m.IsCreatureGenerationEnabled();
    CQuestManager e = { q, q, "Q_A" }; ok = ok && e.IsCreatureGenerationEnabled();
    if (!ok) { std::printf("004afc60_TEST FAIL\n"); return 1; }
    std::printf("004afc60_TEST PASS\n");
    return 0;
}
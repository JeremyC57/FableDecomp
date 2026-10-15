#include <cstdio>
// Behaviour model of 0x006E5600: CScriptConversation::IsValid -- non-empty and every person alive.
struct CThing { unsigned char m_flags91; };
struct CIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
struct CScriptConversation {
    CIntelligentPointer* m_pFirst; CIntelligentPointer* m_pLast;
    bool IsValid() const
    {
        if (m_pFirst == m_pLast) return false;
        for (unsigned long i = 0; i < (unsigned long)(m_pLast - m_pFirst); ++i)
            if (!IS_THING_ALIVE(m_pFirst[i])) return false;
        return true;
    }
};
int main()
{
    CThing alive, dead; alive.m_flags91 = 0x10; dead.m_flags91 = 0x11;
    CIntelligentPointer people[3]; people[0].m_pItem = &alive; people[1].m_pItem = &alive; people[2].m_pItem = &dead;
    CScriptConversation c; c.m_pFirst = people; c.m_pLast = people;
    if (c.IsValid()) { std::printf("006e5600_TEST FAIL\n"); return 1; }
    c.m_pLast = people + 2;
    if (!c.IsValid()) { std::printf("006e5600_TEST FAIL\n"); return 1; }
    c.m_pLast = people + 3;
    if (c.IsValid()) { std::printf("006e5600_TEST FAIL\n"); return 1; }
    people[2].m_pItem = 0;
    if (c.IsValid()) { std::printf("006e5600_TEST FAIL\n"); return 1; }
    std::printf("006e5600_TEST PASS\n");
    return 0;
}

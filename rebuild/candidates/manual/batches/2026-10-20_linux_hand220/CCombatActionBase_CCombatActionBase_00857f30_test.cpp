#include <cstdio>
#include <cstdlib>
struct CThing { int id; };
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(CThing* item) : m_pItem(item) { }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; return *this; }
};
struct Node { Node* next; Node* prev; void* a; void* b; };
struct CSoundPair { long FirstSound, LastSound; };
struct CCreatureActionBase {
    const CSoundPair* Sound; long Group; long Priority;
    CCreatureActionBase(const CSoundPair& s, long g, long p) : Sound(&s), Group(g), Priority(p) { }
};
struct CCombatActionBase : CCreatureActionBase {
    CIntelligentPointer PTarget, PCameraPoint; void* PMeleeAbilityDef; Node* StrikeEvents;
    bool SentMissedEvent, HasResetMeleeCombatAbilityEffects; void* Anim[3]; long ActiveStrikeEventIndex;
    CIntelligentPointer LastWeaponUsedInStrike; int inits;
    CCombatActionBase(CThing* target, long actionCode, const CSoundPair& sound)
        : CCreatureActionBase(sound, 100, actionCode), PMeleeAbilityDef(0), SentMissedEvent(false),
          HasResetMeleeCombatAbilityEffects(false), ActiveStrikeEventIndex(-1), LastWeaponUsedInStrike(0), inits(0)
    {
        StrikeEvents = (Node*)std::malloc(sizeof(Node)); StrikeEvents->next = StrikeEvents; StrikeEvents->prev = StrikeEvents;
        Anim[0] = Anim[1] = Anim[2] = 0;
        PTarget = CIntelligentPointer(target);
        ++inits;
    }
};
int main()
{
    CThing t = { 3 }; CSoundPair s = { 1, 2 };
    CCombatActionBase a(&t, 77, s);
    if (a.PTarget.m_pItem != &t || a.PCameraPoint.m_pItem || a.Group != 100 || a.Priority != 77 || a.Sound != &s || a.ActiveStrikeEventIndex != -1
        || a.StrikeEvents->next != a.StrikeEvents || a.SentMissedEvent || a.HasResetMeleeCombatAbilityEffects || a.inits != 1 || a.LastWeaponUsedInStrike.m_pItem) { std::printf("00857f30_TEST FAIL\n"); return 1; }
    std::printf("00857f30_TEST PASS\n");
    return 0;
}

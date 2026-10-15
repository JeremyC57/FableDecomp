#include <cstdio>
struct CThing { int m_id; };
struct CBaseIntelligentPointer { CThing* m_pItem; long m_handle; CThing* GetPItem() const { return m_pItem; } };
template <class V> struct CList {
    struct node { node* next; node* prev; V value; };
    node* head; node sentinel; node items[3];
    void init() { sentinel.next = sentinel.prev = &sentinel; head = &sentinel; }
    void push(const V& v, int i) { node* n = &items[i]; n->value = v; n->prev = sentinel.prev; n->next = &sentinel; sentinel.prev->next = n; sentinel.prev = n; }
};
typedef CList<CBaseIntelligentPointer> CThingPointerList;
static CBaseIntelligentPointer P(CThing* t) { CBaseIntelligentPointer p; p.m_pItem = t; p.m_handle = 0; return p; }
struct CTCTargetingAI {
    CBaseIntelligentPointer PBestEnemyCreatureForAttack; CBaseIntelligentPointer PScriptTargetOverride; CThingPointerList ThingsToNotAttack;
    bool IsThingAttackable(const CThing* thing) const
    {
        for (CThingPointerList::node* n = ThingsToNotAttack.head->next; n != ThingsToNotAttack.head; n = n->next)
            if (n->value.GetPItem() == thing) return false;
        return true;
    }
};
int main()
{
    CThing a, b, c; CTCTargetingAI t; t.ThingsToNotAttack.init();
    bool all = t.IsThingAttackable(&a);
    t.ThingsToNotAttack.push(P(&a), 0); t.ThingsToNotAttack.push(P(&b), 1);
    if (!all || t.IsThingAttackable(&a) || t.IsThingAttackable(&b) || !t.IsThingAttackable(&c)) { std::printf("007f6170_TEST FAIL\n"); return 1; }
    std::printf("007f6170_TEST PASS\n");
    return 0;
}

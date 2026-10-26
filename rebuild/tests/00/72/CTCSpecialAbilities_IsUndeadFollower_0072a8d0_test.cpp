#include <cstdio>
class CThing { public: int m_id; };
struct CBaseIntelligentPointer { CThing* m_pItem; long m_handle; CThing* GetPItem() const { return m_pItem; } };
template <class V> struct CList {
    struct node { node* next; node* prev; V value; };
    node* head; node sentinel; node items[3];
    void init() { sentinel.next = sentinel.prev = &sentinel; head = &sentinel; }
    void push(const V& v, int i) { node* n = &items[i]; n->value = v; n->prev = sentinel.prev; n->next = &sentinel; sentinel.prev->next = n; sentinel.prev = n; }
};
typedef CList<CBaseIntelligentPointer> CThingPointerList;
static CBaseIntelligentPointer P(CThing* t) { CBaseIntelligentPointer p; p.m_pItem = t; p.m_handle = 0; return p; }
struct CUndeadFollower { long m_value; CBaseIntelligentPointer PThing; };
typedef CList<CUndeadFollower> CUndeadFollowerList;
static CUndeadFollower U(long v, CThing* t) { CUndeadFollower u; u.m_value = v; u.PThing = P(t); return u; }
class CTCSpecialAbilities { public:
    bool AbilityMutexLock; bool UsingBed; long CurrentAbility; CUndeadFollowerList UndeadFollowers;
    bool IsUndeadFollower(const CThing* thing) const
    {
        for (CUndeadFollowerList::node* n = UndeadFollowers.head->next; n != UndeadFollowers.head; n = n->next)
            if (n->value.PThing.GetPItem() == thing) return true;
        return false;
    }
};
int main()
{
    CThing a, b, c; CTCSpecialAbilities s; s.UndeadFollowers.init();
    bool empty = !s.IsUndeadFollower(&a);
    s.UndeadFollowers.push(U(1, &a), 0); s.UndeadFollowers.push(U(2, &b), 1);
    if (!empty || !s.IsUndeadFollower(&a) || !s.IsUndeadFollower(&b) || s.IsUndeadFollower(&c)) { std::printf("0072a8d0_TEST FAIL\n"); return 1; }
    std::printf("0072a8d0_TEST PASS\n");
    return 0;
}
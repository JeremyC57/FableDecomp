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
class CTCFollowed { public:
    CThingPointerList Followers; CThingPointerList AIFollowers;
    bool IsAIFollower(const CThing* thing) const
    {
        for (CThingPointerList::node* n = AIFollowers.head->next; n != AIFollowers.head; n = n->next)
            if (n->value.GetPItem() == thing) return true;
        return false;
    }
};
int main()
{
    CThing a, b, c; CTCFollowed f; f.Followers.init(); f.AIFollowers.init();
    bool empty = !f.IsAIFollower(&a);
    f.AIFollowers.push(P(&a), 0); f.AIFollowers.push(P(&b), 1); f.Followers.push(P(&c), 0);
    if (!empty || !f.IsAIFollower(&a) || !f.IsAIFollower(&b) || f.IsAIFollower(&c)) { std::printf("006d4db0_TEST FAIL\n"); return 1; }
    std::printf("006d4db0_TEST PASS\n");
    return 0;
}
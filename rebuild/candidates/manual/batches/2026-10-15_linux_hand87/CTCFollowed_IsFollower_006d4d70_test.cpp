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
struct CTCFollowed {
    CThingPointerList Followers; CThingPointerList AIFollowers;
    bool IsFollower(const CThing* thing) const
    {
        for (CThingPointerList::node* n = Followers.head->next; n != Followers.head; n = n->next)
            if (n->value.GetPItem() == thing) return true;
        return false;
    }
};
int main()
{
    CThing a, b, c; CTCFollowed f; f.Followers.init(); f.AIFollowers.init();
    bool empty = !f.IsFollower(&a);
    f.Followers.push(P(&a), 0); f.Followers.push(P(&b), 1); f.AIFollowers.push(P(&c), 0);
    if (!empty || !f.IsFollower(&a) || !f.IsFollower(&b) || f.IsFollower(&c)) { std::printf("006d4d70_TEST FAIL\n"); return 1; }
    std::printf("006d4d70_TEST PASS\n");
    return 0;
}

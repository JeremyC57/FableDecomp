#include <cstdio>
// Behaviour test: FindMostRecentMessage returns the first queued message passing all four filters (the third negated).
struct CMessageEvent { int type; bool alive; int originator; bool extra; };
struct IsOfType { int t; bool operator()(const CMessageEvent& m) const { return m.type == t; } };
struct IsOriginatorAlive { bool operator()(const CMessageEvent& m) const { return m.alive; } };
struct IsOriginatedByThing { typedef CMessageEvent argument_type; int thing; bool operator()(const CMessageEvent& m) const { return m.originator == thing; } };
struct HasExtraData { bool operator()(const CMessageEvent& m) const { return m.extra; } };
template <class P> struct unary_negate { P _M_pred; bool operator()(const typename P::argument_type& x) const { return !_M_pred(x); } };
template <class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    F1* PF1; F2* PF2; F3* PF3; F4* PF4;
    bool operator()(const CMessageEvent& m) const { return (*PF1)(m) && (*PF2)(m) && (*PF3)(m) && (*PF4)(m); }
};
struct node { node* next; node* prev; CMessageEvent value; };
struct CMessageEventManager {
    void* World; node* head;
    template <class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) const
    {
        for (node* n = head->next; n != head; n = n->next) {
            const CMessageEvent* message = &n->value;
            if (filter(*message))
                return message;
        }
        return 0;
    }
};
int main()
{
    node h, a, b, c;
    h.next = &a; a.next = &b; b.next = &c; c.next = &h;
    a.value.type = 1; a.value.alive = true; a.value.originator = 7; a.value.extra = true;   // originated by the excluded thing
    b.value.type = 1; b.value.alive = false; b.value.originator = 3; b.value.extra = true;  // dead originator
    c.value.type = 1; c.value.alive = true; c.value.originator = 4; c.value.extra = true;   // match
    IsOfType f1; f1.t = 1; IsOriginatorAlive f2; unary_negate<IsOriginatedByThing> f3; f3._M_pred.thing = 7; HasExtraData f4;
    CFilter_Bind4<IsOfType, IsOriginatorAlive, unary_negate<IsOriginatedByThing>, HasExtraData> filter;
    filter.PF1 = &f1; filter.PF2 = &f2; filter.PF3 = &f3; filter.PF4 = &f4;
    CMessageEventManager m; m.World = 0; m.head = &h;
    if (m.FindMostRecentMessage(filter) != &c.value) { std::printf("00959040_TEST FAIL\n"); return 1; }
    c.value.extra = false;
    if (m.FindMostRecentMessage(filter) != 0) { std::printf("00959040_TEST FAIL\n"); return 1; }
    std::printf("00959040_TEST PASS\n");
    return 0;
}
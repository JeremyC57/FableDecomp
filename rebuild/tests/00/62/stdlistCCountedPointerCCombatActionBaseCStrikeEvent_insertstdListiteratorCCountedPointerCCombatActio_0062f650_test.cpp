// Behaviour test for std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::insert<const_iterator> (0x0062F650):
// the same `for (; first != last; ++first) insert(pos, *first);` loop over a stand-in sentinel list of 8-byte counted
// pointers, with a heap stand-in for the single-element insert 0x0062F690.
#include <stdio.h>
struct StrikeEvent { int id; };
struct CountedPtr { StrikeEvent* Data; void* Info; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CountedPtr data; };
struct Iter {
    NodeBase* node;
    explicit Iter(NodeBase* n) : node(n) { }
    Iter(const Iter& o) : node(o.node) { }                     // user copy ctor, as STLport's iterator
};
struct CIter {
    NodeBase* node;
    explicit CIter(NodeBase* n) : node(n) { }
    bool operator!=(const CIter& o) const { return node != o.node; }
    CIter& operator++() { node = node->next; return *this; }
    const CountedPtr& operator*() const { return ((Node*)node)->data; }
};
struct List {
    Node* sentinel;
    Iter insert(Iter pos, const CountedPtr& x)
    {
        Node* tmp = new Node; tmp->data = x;
        NodeBase* n = pos.node; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        return Iter(tmp);
    }
    void insert(Iter pos, CIter first, CIter last)
    {
        for (; first != last; ++first)
            insert(pos, *first);
    }
};
static List make() { List l; l.sentinel = new Node; l.sentinel->next = l.sentinel; l.sentinel->prev = l.sentinel; return l; }
int main()
{
    static StrikeEvent ev[3] = { { 1 }, { 2 }, { 3 } };
    List src = make(), dst = make();
    for (int i = 0; i < 3; ++i) { CountedPtr p; p.Data = &ev[i]; p.Info = 0; src.insert(Iter(src.sentinel), p); }
    CountedPtr z; z.Data = 0; z.Info = 0; dst.insert(Iter(dst.sentinel), z);
    dst.insert(Iter(dst.sentinel->next), CIter(src.sentinel->next), CIter(src.sentinel));   // before the zero entry
    int ids[5]; int n = 0;
    for (NodeBase* q = dst.sentinel->next; q != dst.sentinel && n < 5; q = q->next, ++n) {
        StrikeEvent* e = ((Node*)q)->data.Data; ids[n] = e ? e->id : 0; }
    int ok = n == 4 && ids[0] == 1 && ids[1] == 2 && ids[2] == 3 && ids[3] == 0;
    dst.insert(Iter(dst.sentinel), CIter(src.sentinel), CIter(src.sentinel));                // empty range: no change
    n = 0; for (NodeBase* q = dst.sentinel->next; q != dst.sentinel; q = q->next) ++n;
    ok = ok && n == 4;
    printf(ok ? "0062f650_TEST PASS\n" : "0062f650_TEST FAIL\n");
    return ok ? 0 : 1;
}

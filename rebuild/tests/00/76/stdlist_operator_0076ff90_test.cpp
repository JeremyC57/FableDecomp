// Behaviour test for 0x0076FF90 list<CWeaponTrailEntry>::operator=: the 28-byte entries of the common prefix are overwritten, then the surplus erased or x's tail inserted.
#include <cstdio>
#include <cstdlib>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Entry { float Top[3]; float Bottom[3]; float CreationTime; };
struct Node : NodeBase { Entry e; };
struct List {
    NodeBase* s;
    void init() { s = (NodeBase*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; }
    void insert(NodeBase* pos, long v)
    { Entry e; for (int k = 0; k < 3; ++k) { e.Top[k] = (float)v; e.Bottom[k] = -(float)v; } e.CreationTime = (float)v; insertE(pos, e); }
    void insertE(NodeBase* pos, const Entry& v)
    {
        Node* n = (Node*)std::malloc(sizeof(Node)); n->e = v;
        n->next = pos; n->prev = pos->prev; pos->prev->next = n; pos->prev = n;
    }
    void erase(NodeBase* first, NodeBase* last)
    {
        while (first != last) { NodeBase* n = first; first = first->next; n->prev->next = n->next; n->next->prev = n->prev; std::free(n); }
    }
    void insertRange(NodeBase* pos, NodeBase* first, NodeBase* last) { for (; first != last; first = first->next) insertE(pos, ((Node*)first)->e); }
    List& assign(const List& x)
    {
        if (this != &x) {
            NodeBase* first1 = s->next; NodeBase* last1 = s;
            NodeBase* first2 = x.s->next; NodeBase* last2 = x.s;
            while (first1 != last1 && first2 != last2) {
                ((Node*)first1)->e = ((Node*)first2)->e;
                first1 = first1->next; first2 = first2->next;
            }
            if (first2 == last2) erase(first1, last1);
            else insertRange(last1, first2, last2);
        }
        return *this;
    }
    int size() const { int n = 0; for (NodeBase* p = s->next; p != s; p = p->next) ++n; return n; }
    long at(int i) const { NodeBase* p = s->next; while (i--) p = p->next; const Entry& e = ((Node*)p)->e; return e.Bottom[2] == -e.Top[0] ? (long)e.CreationTime : -1; }
};
int main()
{
    if (sizeof(Entry) != 28) { std::printf("0076ff90_TEST FAIL\n"); return 1; }
    List a, b; a.init(); b.init();
    for (int i = 0; i < 5; ++i) a.insert(a.s, 100 + i);
    for (int i = 0; i < 2; ++i) b.insert(b.s, 7 + i);
    a.assign(b);   // shrink
    if (a.size() != 2 || a.at(0) != 7 || a.at(1) != 8) { std::printf("0076ff90_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) b.insert(b.s, 20 + i);
    a.assign(b);   // grow
    if (a.size() != 5 || a.at(2) != 20 || a.at(4) != 22) { std::printf("0076ff90_TEST FAIL\n"); return 1; }
    if (&a.assign(a) != &a || a.size() != 5) { std::printf("0076ff90_TEST FAIL\n"); return 1; }
    std::printf("0076ff90_TEST PASS\n");
    return 0;
}
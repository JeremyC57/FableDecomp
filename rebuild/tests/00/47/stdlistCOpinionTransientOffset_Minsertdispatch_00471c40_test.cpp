// Behaviour test for 0x00471C40 list<COpinionTransientOffset>::_M_insert_dispatch: copies of [first, last) linked in order
// before position (a circular list with a header node)
#include <cstdio>
#include <cstdlib>
#include <new>
struct COpinionTransientOffset { int Opinion; float RunIn; int FramesIn; int Peak; float RunOut; int FramesOut; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { COpinionTransientOffset data; };
struct List {
    NodeBase* header; int created;
    Node* CreateNode(const COpinionTransientOffset& x) { ++created; Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->data) COpinionTransientOffset(x); return n; }
    void Insert(NodeBase* position, const COpinionTransientOffset& x)
    {
        Node* tmp = CreateNode(x); NodeBase* n = position; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    }
    void InsertDispatch(NodeBase* position, NodeBase* first, NodeBase* last) { for (; first != last; first = first->next) Insert(position, ((Node*)first)->data); }
};
int main()
{
    NodeBase srcHead; srcHead.next = srcHead.prev = &srcHead;
    List src = { &srcHead, 0 };
    for (int i = 0; i < 3; ++i) { COpinionTransientOffset o = { i, 0.5f * i, 10 + i, 0, 0.0f, 0 }; src.Insert(&srcHead, o); }
    NodeBase dstHead; dstHead.next = dstHead.prev = &dstHead;
    List dst = { &dstHead, 0 };
    COpinionTransientOffset tail = { 99, 0, 0, 0, 0, 0 };
    dst.Insert(&dstHead, tail);
    NodeBase* tailNode = dstHead.next;
    dst.InsertDispatch(tailNode, srcHead.next, &srcHead);      // insert the three before the 99 node
    bool ok = dst.created == 4;
    int expect[4] = { 0, 1, 2, 99 }, i = 0;
    for (NodeBase* n = dstHead.next; n != &dstHead; n = n->next, ++i) if (i >= 4 || ((Node*)n)->data.Opinion != expect[i] || n->next->prev != n) ok = false;
    ok = ok && i == 4 && dstHead.prev == tailNode && ((Node*)dstHead.next)->data.FramesIn == 10;
    dst.InsertDispatch(&dstHead, srcHead.next, srcHead.next);
    ok = ok && dst.created == 4;
    if (!ok) { std::printf("00471c40_TEST FAIL\n"); return 1; }
    std::printf("00471c40_TEST PASS\n");
    return 0;
}
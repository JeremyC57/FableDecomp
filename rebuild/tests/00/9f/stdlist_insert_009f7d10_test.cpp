#include <cstdio>
#include <cstdlib>
// Behaviour test for std::list<CPendingQuery>::insert (0x009F7D10): the new node is linked before the position.
struct Q { void* Query; int Frame; double TimeStamp; };
struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct Node : NodeBase { Q _M_data; };
struct It { NodeBase* _M_node; It(NodeBase* n) : _M_node(n) {} };
struct List {
    Node* _M_node;
    Node* _M_create_node(const Q& x) { Node* t = (Node*)malloc(sizeof(Node)); t->_M_data = x; return t; }
    It insert(It __position, const Q& __x)
    {
        Node* __tmp = _M_create_node(__x);
        NodeBase* __n = __position._M_node;
        NodeBase* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return It(__tmp);
    }
};
int main()
{
    List l; Node sentinel; sentinel._M_next = sentinel._M_prev = &sentinel; l._M_node = &sentinel;
    Q a = { 0, 1, 1.0 }, b = { 0, 2, 2.0 };
    It ia = l.insert(It(&sentinel), a);          // push_back a
    It ib = l.insert(ia, b);                      // b before a
    bool ok = sentinel._M_next == ib._M_node && ib._M_node->_M_next == ia._M_node && ia._M_node->_M_next == &sentinel
           && sentinel._M_prev == ia._M_node && ((Node*)ib._M_node)->_M_data.Frame == 2 && ((Node*)ia._M_node)->_M_data.Frame == 1;
    free(ia._M_node); free(ib._M_node);
    if (!ok) { std::printf("009f7d10_TEST FAIL\n"); return 1; }
    std::printf("009f7d10_TEST PASS\n");
    return 0;
}
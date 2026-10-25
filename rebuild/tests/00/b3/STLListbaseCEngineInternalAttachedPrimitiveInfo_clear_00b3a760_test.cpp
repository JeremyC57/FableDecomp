// Behaviour test for 0x00B3A760 std::_List_base<CEngineInternalAttachedPrimitiveInfo>::clear(): each node's value is
// destroyed through its virtual destructor, the node is freed and the sentinel re-linked to itself.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0;
class CEngineInternalAttachedPrimitiveInfo {
public:
    virtual ~CEngineInternalAttachedPrimitiveInfo() { ++g_destroyed; }
    void* HandleVtable; void* HandleData;
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> struct _List_base {
    _List_node<_Tp>* _M_node;
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node->_M_next;
        while (__cur != this->_M_node) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            std::free(__tmp);
        }
        this->_M_node->_M_next = this->_M_node;
        this->_M_node->_M_prev = this->_M_node;
    }
};
typedef _List_node<CEngineInternalAttachedPrimitiveInfo> Node;
int main()
{
    _List_base<CEngineInternalAttachedPrimitiveInfo> l;
    Node* head = (Node*)std::malloc(sizeof(Node));
    l._M_node = head; head->_M_next = head; head->_M_prev = head;
    for (int i = 0; i < 3; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        new (&n->_M_data) CEngineInternalAttachedPrimitiveInfo;
        n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n;
    }
    l.clear();
    bool ok = g_destroyed == 3 && head->_M_next == head && head->_M_prev == head;
    l.clear();
    ok = ok && g_destroyed == 3;
    std::free(head);
    if (!ok) { std::printf("00b3a760_TEST FAIL\n"); return 1; }
    std::printf("00b3a760_TEST PASS\n");
    return 0;
}

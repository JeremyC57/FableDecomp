// Behaviour test for 006a6290 (STLport list push_back model copy).
#include <cstdio>
#include <cstdlib>
#include <new>
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* x) : _M_node(x) { }
        iterator(const iterator& x) : _M_node(x._M_node) { }
    };
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& x) { _Node* p = (_Node*)std::malloc(sizeof(_Node)); new ((void*)&p->_M_data) _Tp(x); return p; }
    iterator insert(iterator position, const _Tp& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const _Tp& x) { insert(end(), x); }
    int size() const { int n = 0; for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    _Tp& at(int i) { _List_node_base* p = _M_node->_M_next; while (i--) p = p->_M_next; return ((_Node*)p)->_M_data; }
};
class CThingPhysical {
public:
    class CHealthOverTimeHelper { public:
        long FramesLeft; float HealthPerFrame;
        CHealthOverTimeHelper(float amount, long frames) : FramesLeft(frames), HealthPerFrame(amount) { }
    };
    list<CHealthOverTimeHelper> HealthOverTimeEffects;
    void ModifyHealthOverTime(float amount, long frames) { HealthOverTimeEffects.push_back(CHealthOverTimeHelper(amount, frames)); }
};
int main()
{
    CThingPhysical t; int fail = 0;
    t.ModifyHealthOverTime(2.5f, 30);
    t.ModifyHealthOverTime(-1.0f, 5);
    if (t.HealthOverTimeEffects.size() != 2) fail = 1;
    if (t.HealthOverTimeEffects.at(0).FramesLeft != 30 || t.HealthOverTimeEffects.at(0).HealthPerFrame != 2.5f) fail = 1;
    if (t.HealthOverTimeEffects.at(1).FramesLeft != 5 || t.HealthOverTimeEffects.at(1).HealthPerFrame != -1.0f) fail = 1;
    if (fail) { std::printf("006a6290_TEST FAIL\n"); return 1; }
    std::printf("006a6290_TEST PASS\n");
    return 0;
}
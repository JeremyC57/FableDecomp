// Behaviour test for 0x009E4A20 _STL::list<CMaintainedEvent>::insert(iterator): default event linked before pos.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gCtors = 0, gCopies = 0, gDtors = 0;

class CMaintainedEvent
{
public:
    int Value;
    CMaintainedEvent() : Value(7) { ++gCtors; }
    CMaintainedEvent(const CMaintainedEvent& o) : Value(o.Value) { ++gCopies; }
    virtual ~CMaintainedEvent() { ++gDtors; }
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : public _List_node_base { CMaintainedEvent _M_data; };
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) {}
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}
};

struct list {
    _List_node_base* _M_node;
    _List_node* _M_create_node(const CMaintainedEvent& x)
    {
        _List_node* p = (_List_node*)std::malloc(sizeof(_List_node));
        new (&p->_M_data) CMaintainedEvent(x);
        return p;
    }
    _List_iterator insert(_List_iterator position, const CMaintainedEvent& x)
    {
        _List_node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return _List_iterator(tmp);
    }
    _List_iterator insert(_List_iterator position) { return insert(position, CMaintainedEvent()); }
};

int main()
{
    _List_node_base head;
    head._M_next = head._M_prev = &head;
    list l; l._M_node = &head;
    _List_iterator a = l.insert(_List_iterator(&head));          // push_back on an empty list
    bool ok = head._M_next == a._M_node && head._M_prev == a._M_node && a._M_node->_M_next == &head && a._M_node->_M_prev == &head;
    ok = ok && ((_List_node*)a._M_node)->_M_data.Value == 7 && gCtors == 1 && gCopies == 1 && gDtors == 1;
    _List_iterator b = l.insert(a);                               // before the first node
    ok = ok && head._M_next == b._M_node && b._M_node->_M_next == a._M_node && a._M_node->_M_prev == b._M_node
            && b._M_node->_M_prev == &head && head._M_prev == a._M_node && gCtors == 2 && gCopies == 2 && gDtors == 2;
    if (!ok) { std::printf("009e4a20_TEST FAIL\n"); return 1; }
    std::printf("009e4a20_TEST PASS\n");
    return 0;
}
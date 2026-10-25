// Behaviour test for 0x009E47E0 std::list<CMaintainedEvent>::erase(iterator).
#include <cstdio>
#include <cstdlib>
#include <new>

static int gDestroyed = 0, gLastId = -1;

class CMaintainedEvent
{
public:
    int Id;
    CMaintainedEvent() : Id(0) {}
    virtual ~CMaintainedEvent() { ++gDestroyed; gLastId = Id; }
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
    _List_iterator erase(_List_iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node* n = (_List_node*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        (&n->_M_data)->~CMaintainedEvent();
        std::free(n);
        return _List_iterator(next);
    }
};

static _List_node* NewNode(int id)
{
    _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
    new (&n->_M_data) CMaintainedEvent;
    n->_M_data.Id = id;
    return n;
}

int main()
{
    _List_node_base head;
    head._M_next = head._M_prev = &head;
    _List_node* a = NewNode(1); _List_node* b = NewNode(2); _List_node* c = NewNode(3);
    head._M_next = a; a->_M_prev = &head; a->_M_next = b; b->_M_prev = a; b->_M_next = c; c->_M_prev = b; c->_M_next = &head; head._M_prev = c;
    list l; l._M_node = &head;
    _List_iterator it = l.erase(_List_iterator(b));
    bool ok = it._M_node == c && a->_M_next == c && c->_M_prev == a && gDestroyed == 1 && gLastId == 2;
    it = l.erase(_List_iterator(c));
    ok = ok && it._M_node == &head && a->_M_next == &head && head._M_prev == a && gDestroyed == 2 && gLastId == 3;
    it = l.erase(_List_iterator(a));
    ok = ok && it._M_node == &head && head._M_next == &head && head._M_prev == &head && gDestroyed == 3;
    if (!ok) { std::printf("009e47e0_TEST FAIL\n"); return 1; }
    std::printf("009e47e0_TEST PASS\n");
    return 0;
}
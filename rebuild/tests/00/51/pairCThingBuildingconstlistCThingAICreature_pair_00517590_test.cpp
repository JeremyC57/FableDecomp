// Behaviour test for 0x00517590 pair<CThingBuilding* const, list<CThingAICreature*> > copy constructor: the key is copied
// and the list is deep-copied in order into fresh nodes behind a fresh self-linked sentinel; an empty list copies empty.
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
class CThingBuilding;
class CThingAICreature;

struct __false_type { __false_type() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
    _List_base(const allocator<_List_node<T> >& a) : _M_node(a, (_List_node<T>*)0)
    {
        _List_node<T>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    allocator<_Node> get_allocator() const { return allocator<_Node>(); }
    _List_iterator begin() { return _List_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator begin() const { return _List_const_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(this->_M_node._M_data); }
    void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x00513B60 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _Node* n = this->_M_node.allocate(1);
            new (&n->_M_data) T(((_Node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list<T>& x) : _List_base<T>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};


typedef pair<CThingBuilding* const, list<CThingAICreature*> > PairT;
static int check(const list<CThingAICreature*>& l, CThingAICreature** v, int n, const list<CThingAICreature*>& src)
{
    _List_node_base* h = l._M_node._M_data;
    if (h == 0 || h == src._M_node._M_data) return 0;
    _List_node_base* p = h->_M_next;
    for (int i = 0; i < n; ++i) {
        if (p == h || ((list<CThingAICreature*>::_Node*)p)->_M_data != v[i] || p->_M_next->_M_prev != p) return 0;
        for (_List_node_base* q = src._M_node._M_data->_M_next; q != src._M_node._M_data; q = q->_M_next) if (q == p) return 0;
        p = p->_M_next;
    }
    return p == h && h->_M_prev->_M_next == h;
}
int main()
{
    static int dummy[8];
    CThingAICreature* v[3] = { (CThingAICreature*)&dummy[1], (CThingAICreature*)&dummy[5], (CThingAICreature*)&dummy[3] };
    // hand-build a source list
    typedef list<CThingAICreature*>::_Node N;
    N* h = (N*)malloc(sizeof(N)); h->_M_next = h; h->_M_prev = h;
    for (int i = 0; i < 3; ++i) {
        N* n = (N*)malloc(sizeof(N)); n->_M_data = v[i];
        n->_M_next = h; n->_M_prev = h->_M_prev; h->_M_prev->_M_next = n; h->_M_prev = n;
    }
    char raw[sizeof(PairT)];
    PairT* src = (PairT*)raw;
    *(CThingBuilding**)&src->first = (CThingBuilding*)&dummy[7];
    src->second._M_node._M_data = h;
    PairT copy(*src);
    int ok = copy.first == (CThingBuilding*)&dummy[7] && check(copy.second, v, 3, src->second);
    N* e = (N*)malloc(sizeof(N)); e->_M_next = e; e->_M_prev = e;
    src->second._M_node._M_data = e;
    PairT copy2(*src);
    ok &= check(copy2.second, v, 0, src->second);
    if (!ok) { std::printf("00517590_TEST FAIL\n"); return 1; }
    std::printf("00517590_TEST PASS\n");
    return 0;
}
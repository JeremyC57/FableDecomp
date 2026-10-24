// Behaviour test for 0x00517130 list<CIntelligentPointer<CThingAICreature> >::_M_insert_dispatch: every element of
// [first, last) is copy-constructed (base ctor + SetPItem(GetPItem())) into a fresh node linked in before pos, in order;
// an empty range adds nothing.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_base;
class CBaseObject;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() {}
    CBaseIntelligentPointer() : PData(0) { ++g_base; }
    CBaseObject* GetPItem() const { return (CBaseObject*)PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
    void* PData;                               // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7E0 for T = CThingAICreature
public:
    virtual ~CIntelligentPointer() {}
    CIntelligentPointer(CBaseObject* p) : CBaseIntelligentPointer() { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

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
    bool operator!=(const _List_const_iterator& y) const { return _M_node != y._M_node; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    _List_iterator insert(_List_iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class _InputIter>
    void _M_insert_dispatch(_List_iterator pos, _InputIter first, _InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(pos, ((_Node*)first._M_node)->_M_data);
    }
};


typedef CIntelligentPointer<CThingAICreature> P;
typedef list<P> L;
typedef _List_node<P> N;
int main()
{
    static char objs[4];
    // source list: sentinel s + 3 nodes
    _List_node_base s; s._M_next = &s; s._M_prev = &s;
    N* src[3];
    for (int i = 0; i < 3; ++i) {
        src[i] = (N*)std::malloc(sizeof(N));
        new (&src[i]->_M_data) P((CBaseObject*)&objs[i]);
        src[i]->_M_next = &s; src[i]->_M_prev = s._M_prev; s._M_prev->_M_next = src[i]; s._M_prev = src[i];
    }
    // destination: sentinel d + one node holding objs[3]; insert before that node
    _List_node_base d; d._M_next = &d; d._M_prev = &d;
    N* last = (N*)std::malloc(sizeof(N)); new (&last->_M_data) P((CBaseObject*)&objs[3]);
    last->_M_next = &d; last->_M_prev = &d; d._M_next = last; d._M_prev = last;
    L l;
    __false_type tag;
    g_base = 0;
    l._M_insert_dispatch(_List_iterator(last), _List_const_iterator(s._M_next), _List_const_iterator(&s), tag);
    int ok = g_base == 3;
    _List_node_base* p = d._M_next;
    for (int i = 0; i < 3; ++i) {
        ok &= p != &d && p != src[i] && ((N*)p)->_M_data.GetPItem() == (CBaseObject*)&objs[i] && p->_M_next->_M_prev == p;
        p = p->_M_next;
    }
    ok &= p == last && last->_M_next == &d && d._M_prev == last;
    l._M_insert_dispatch(_List_iterator(&d), _List_const_iterator(&s), _List_const_iterator(&s), tag);
    ok &= g_base == 3 && d._M_prev == last;
    if (!ok) { std::printf("00517130_TEST FAIL\n"); return 1; }
    std::printf("00517130_TEST PASS\n");
    return 0;
}
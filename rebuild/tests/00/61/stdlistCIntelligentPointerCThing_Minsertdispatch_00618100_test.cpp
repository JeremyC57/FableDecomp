// Behaviour test for 0x00618100 std::list<CIntelligentPointer<CThing> >::_M_insert_dispatch(pos, first, last, __false_type):
// for every element of [first, last) a new node is linked in before pos, holding a copy of the smart pointer
// (base ctor 0x00A01B10, own vtable, SetPItem(other.GetPItem()) -- 0x00A01B50 / 0x00A01B90, modelled).
#include <cstdio>
#include <cstdlib>
#include <new>

class CBaseObject { public: int m_id; };
class CThing : public CBaseObject { };

static int g_sets = 0;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() { }
    CBaseIntelligentPointer() : PData(0) { }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++g_sets; }
    CBaseObject* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer() { }
    explicit CIntelligentPointer(T* p) : CBaseIntelligentPointer() { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace stl {
struct __false_type { __false_type() {} };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
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
    bool operator!=(const _List_const_iterator& y) const { return _M_node != y._M_node; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class list {
public:
    typedef _List_node<T> _Node;
    _List_node_base* _M_node;
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)std::malloc(sizeof(_Node));
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
}

typedef CIntelligentPointer<CThing> TP;
typedef stl::list<TP> L;

static void Init(stl::_List_node_base& h) { h._M_next = h._M_prev = &h; }

int main()
{
    int ok = 1;
    CThing things[3];
    for (int i = 0; i < 3; ++i) things[i].m_id = i;
    // source list: three nodes
    stl::_List_node_base srcHead;
    Init(srcHead);
    L src;
    src._M_node = &srcHead;
    for (int i = 0; i < 3; ++i) src.insert(stl::_List_iterator(&srcHead), TP(&things[i]));
    // destination list with one existing node; insert the whole source before it
    stl::_List_node_base dstHead;
    Init(dstHead);
    L dst;
    dst._M_node = &dstHead;
    CThing tail;
    tail.m_id = 99;
    dst.insert(stl::_List_iterator(&dstHead), TP(&tail));
    stl::_List_node_base* tailNode = dstHead._M_next;
    int setsBefore = g_sets;
    dst._M_insert_dispatch(stl::_List_iterator(tailNode), stl::_List_const_iterator(srcHead._M_next),
                           stl::_List_const_iterator(&srcHead), stl::__false_type());
    if (g_sets - setsBefore != 3) ok = 0;
    int n = 0;
    int want[4] = { 0, 1, 2, 99 };
    for (stl::_List_node_base* p = dstHead._M_next; p != &dstHead; p = p->_M_next, ++n) {
        const TP& tp = ((L::_Node*)p)->_M_data;
        if (n >= 4 || tp.GetPItem()->m_id != want[n] || p->_M_next->_M_prev != p) ok = 0;
    }
    if (n != 4) ok = 0;
    // an empty source range inserts nothing
    dst._M_insert_dispatch(stl::_List_iterator(&dstHead), stl::_List_const_iterator(&srcHead),
                           stl::_List_const_iterator(&srcHead), stl::__false_type());
    if (dstHead._M_prev != tailNode) ok = 0;
    stl::_List_node_base* heads[2] = { &srcHead, &dstHead };
    for (int h = 0; h < 2; ++h) {
        stl::_List_node_base* p = heads[h]->_M_next;
        while (p != heads[h]) { stl::_List_node_base* nx = p->_M_next; ((L::_Node*)p)->_M_data.~TP(); std::free(p); p = nx; }
    }
    if (!ok) { std::printf("00618100_TEST FAIL\n"); return 1; }
    std::printf("00618100_TEST PASS\n");
    return 0;
}
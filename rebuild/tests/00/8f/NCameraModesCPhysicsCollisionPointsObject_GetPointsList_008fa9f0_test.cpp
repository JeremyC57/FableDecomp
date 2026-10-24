// Behaviour test for 0x008FA9F0 NCameraModes::CPhysicsCollisionPointsObject::GetPointsList(): Points is cleared and
// rebuilt as the concatenation of every SmartPointList entry's point list, in order.
#include <stdio.h>
#include <stdlib.h>
#include <new>


struct C3DVector {                              // retail C3DVector, 12 bytes
    float X, Y, Z;
};

namespace _STL {

struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
    template <class _InputIter>
    void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
};


template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

}

namespace NCameraModes {

class CPhysicsCollisionPointsObject {
public:
    _STL::list<C3DVector>                                    Points;            // +0x00
    _STL::list<_STL::pair<float, _STL::list<C3DVector> > >   SmartPointList;    // +0x04
    _STL::list<_STL::pair<float, long> >                     NumTestsPerTheta;  // +0x08
    long  ScanDensity;                                                          // +0x0c
    float StartTheta;                                                           // +0x10
    float DeltaTheta;                                                           // +0x14
    long  DebugGraphicsRed;                                                     // +0x18
    long  DebugGraphicsGreen;                                                   // +0x1c
    long  DebugGraphicsBlue;                                                    // +0x20
    void GetPointsList();
};

void CPhysicsCollisionPointsObject::GetPointsList()
{
    Points.clear();
    for (_STL::list<_STL::pair<float, _STL::list<C3DVector> > >::iterator it = SmartPointList.begin();
         it != SmartPointList.end(); ++it)
        for (_STL::list<C3DVector>::iterator point = (*it).second.begin(); point != (*it).second.end(); ++point)
            Points.push_back(*point);
}

}


typedef _STL::list<C3DVector> TPoints;
typedef _STL::pair<float, TPoints> TEntry;
typedef _STL::list<TEntry> TEntries;

template <class L> static void InitList(L& l)
{
    _STL::_List_node_base* s = (_STL::_List_node_base*)malloc(sizeof(_STL::_List_node_base));
    s->_M_next = s; s->_M_prev = s;
    l._M_node._M_data = s;
}
static C3DVector V(float x) { C3DVector v; v.X = x; v.Y = x + 0.5f; v.Z = -x; return v; }

int main()
{
    int ok = 1;
    NCameraModes::CPhysicsCollisionPointsObject* o =
        (NCameraModes::CPhysicsCollisionPointsObject*)malloc(sizeof(NCameraModes::CPhysicsCollisionPointsObject));
    InitList(o->Points); InitList(o->SmartPointList);
    o->Points.push_back(V(99.0f));                     // stale content, must be cleared
    TEntry& e1 = *(TEntry*)malloc(sizeof(TEntry));
    TEntry& e2 = *(TEntry*)malloc(sizeof(TEntry));
    TEntry& e3 = *(TEntry*)malloc(sizeof(TEntry));
    e1.first = 1.0f; InitList(e1.second); e1.second.push_back(V(1.0f)); e1.second.push_back(V(2.0f));
    e2.first = 2.0f; InitList(e2.second);              // empty entry
    e3.first = 3.0f; InitList(e3.second); e3.second.push_back(V(3.0f));
    o->SmartPointList.push_back(e1); o->SmartPointList.push_back(e2); o->SmartPointList.push_back(e3);

    o->GetPointsList();
    float want[3] = { 1.0f, 2.0f, 3.0f };
    unsigned int n = 0;
    for (TPoints::iterator it = o->Points.begin(); it != o->Points.end(); ++it, ++n) {
        if (n >= 3) { ok = 0; break; }
        float d = (*it).X - want[n]; if (d < 0) d = -d;
        float dy = (*it).Y - (want[n] + 0.5f); if (dy < 0) dy = -dy;
        if (d > 1e-6f || dy > 1e-6f) ok = 0;
    }
    if (n != 3) ok = 0;
    // back links are consistent
    _STL::_List_node_base* s = o->Points._M_node._M_data;
    if (s->_M_prev->_M_next != s || s->_M_next->_M_prev != s) ok = 0;

    // an empty SmartPointList leaves Points empty
    InitList(o->SmartPointList);
    o->GetPointsList();
    if (o->Points.begin() != o->Points.end()) ok = 0;

    printf("008fa9f0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of list<CMiniMapDisplay::CMarkerInfo>::remove_if<CIsMarkerAutoMarker> @ 0x0082C6E0: erase every automatic
// minimap marker.

class CBaseObject { };
class CThing : public CBaseObject { };
class CBaseObjectPointer;
static int gIPDtors = 0;

class CBaseIntelligentPointer {                             // Ego_r.pdb: vptr +0, PData +4 (vtable 0x0129C95C)
public:
    virtual ~CBaseIntelligentPointer() { ++gIPDtors; }                     // 0x00A01C10 (slot 0: the scalar deleting destructor)
    CBaseObject* GetPItem() const { return (CBaseObject*)(void*)PData; }   // stand-in: PData is the object                          // 0x00A01B50
    CBaseObjectPointer* PData;                              // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // no declared destructor in retail
public:
    T* Get() const { return (T*)GetPItem(); }
};

template <class T>
bool __fastcall operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b)
{
    return a.GetPItem() == b.GetPItem();
}

class CMiniMapDisplay {
public:
    class CMarkerInfo {                                     // Ego_r.pdb, 0x10 bytes
    public:
        CIntelligentPointer<const CThing> PThing;           // +0x0
        long                              GraphicIndex;     // +0x8
        bool                              AutoMarker;       // +0xc
    };
    class CIsMarkerAutoMarker {                             // Ego_r.pdb: empty
    public:
        CIsMarkerAutoMarker() { }
        bool operator()(const CMarkerInfo& marker) const { return marker.AutoMarker; }
    };
    class CIsMarkerBoundToThing {                           // Ego_r.pdb: PThing +0
    public:
        const CThing* PThing;
        CIsMarkerBoundToThing(const CThing& thing) : PThing(&thing) { }
        bool operator()(const CMarkerInfo& marker) const { return marker.PThing.Get() == PThing && !marker.AutoMarker; }
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};
}


template <class L> void InitList(L& l)
{
    typedef typename L::_Node Node;
    Node* s = (Node*)malloc(sizeof(Node));
    s->_M_next = s;
    s->_M_prev = s;
    l._M_node._M_data = s;
}
template <class L> typename L::_Node* NewBack(L& l)
{
    typedef typename L::_Node Node;
    Node* n = (Node*)malloc(sizeof(Node));
    _STL::_List_node_base* s = l._M_node._M_data;
    n->_M_next = s;
    n->_M_prev = s->_M_prev;
    s->_M_prev->_M_next = n;
    s->_M_prev = n;
    return n;
}
template <class L> int CountList(L& l)
{
    int c = 0;
    for (_STL::_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next)
        ++c;
    return c;
}
template <class L> bool LinksOk(L& l)
{
    _STL::_List_node_base* s = l._M_node._M_data;
    for (_STL::_List_node_base* p = s->_M_next; p != s; p = p->_M_next)
        if (p->_M_next->_M_prev != p || p->_M_prev->_M_next != p)
            return false;
    return s->_M_next->_M_prev == s && s->_M_prev->_M_next == s;
}
int main()
{
    bool ok = true;
    int bound[4] = { 0, 1, 0, 0 };
    bool autos[4] = { true, false, true, false };
    typedef CMiniMapDisplay::CMarkerInfo M;
    typedef _STL::list<M> L;
    L l;
    InitList(l);
    static CThing things[2];
    for (int i = 0; i < 4; ++i) {
        L::_Node* n = NewBack(l);
        new (&n->_M_data) M;
        n->_M_data.PThing.PData = (CBaseObjectPointer*)(void*)static_cast<const CBaseObject*>(&things[bound[i]]);
        n->_M_data.GraphicIndex = i;
        n->_M_data.AutoMarker = autos[i];
    }
    l.remove_if(CMiniMapDisplay::CIsMarkerAutoMarker());
    long expect[2] = { 1, 3 };
    int k = 0;
    for (L::iterator it = l.begin(); it != l.end(); ++it, ++k)
        if (k >= 2 || (*it).GraphicIndex != expect[k])
            ok = false;
    if (k != 2 || gIPDtors != 2 || !LinksOk(l))
        ok = false;
    if (!ok) { printf("0082c6e0_TEST FAIL\n"); return 1; }
    printf("0082c6e0_TEST PASS\n");
    return 0;
}
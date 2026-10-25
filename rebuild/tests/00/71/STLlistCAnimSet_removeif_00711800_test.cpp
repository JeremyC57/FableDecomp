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
// Model of list<CAnimSet>::remove_if<CAnimSet::CIsType> @ 0x00711800: erase every animation set of the predicate's
// type (each erased node frees its two vector buffers, then the node).

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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }                                           // _Destroy over a trivial element type: nothing
};
}

class CAnimateThing;
enum EAnimUpdateMode { ANIM_UPDATE_MODE_NONE };

class CTransitionBlendAlpha {                               // Ego_r.pdb, 8 bytes
public:
    float CurrentAlpha;                                     // +0x0
    float LastAlpha;                                        // +0x4
};

class CAnimSet {                                            // Ego_r.pdb with the retail 12-byte STLport vectors: 0x54 bytes
public:
    EAnimUpdateMode                        UpdateMode;                 // +0x00
    _STL::vector<short>                    Anims;                      // +0x04
    _STL::vector<CTransitionBlendAlpha>    InternalAlphas;             // +0x10
    float                                  CurrentPercentageComplete;  // +0x1c
    float                                  LastPercentageComplete;     // +0x20
    float                                  Speed;                      // +0x24
    unsigned long                          FrameInternalAlphasUpdated; // +0x28
    float                                  CurrentAlpha;               // +0x2c
    float                                  LastAlpha;                  // +0x30
    float                                  LastUpdateChange;           // +0x34
    float                                  ChangeAlphaBy;              // +0x38
    CAnimateThing*                         PAnimator;                  // +0x3c
    unsigned long                          Flags;                      // +0x40
    unsigned long                          TypeFlags;                  // +0x44
    long                                   AnimDelay;                  // +0x48
    long                                   Priority;                   // +0x4c
    unsigned char                          Type;                       // +0x50
    bool                                   Looping : 1;                // +0x51
    bool                                   Finished : 1;
    bool                                   Forwards : 1;
    bool                                   HadUpdate : 1;
    bool                                   OwnedByMainAnim : 1;

    class CIsType {                                         // Ego_r.pdb: Type +0
    public:
        long Type;
        CIsType(long type) : Type(type) { }
        bool operator()(const CAnimSet& set) const { return set.Type == Type; }
    };
    class CHasTypeFlags {                                   // Ego_r.pdb: Flags +0
    public:
        long Flags;
        CHasTypeFlags(long flags) : Flags(flags) { }
        bool operator()(const CAnimSet& set) const { return (set.TypeFlags & Flags) != 0; }
    };
};

namespace _STL {
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
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
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
    unsigned int types[4] = { 1, 2, 1, 3 };
    unsigned int flags[4] = { 1, 6, 8, 2 };
    typedef _STL::list<CAnimSet> L;
    L l;
    InitList(l);
    for (int i = 0; i < 4; ++i) {
        L::_Node* n = NewBack(l);
        CAnimSet& set = n->_M_data;
        set.Anims._M_start = (short*)malloc(4 * sizeof(short));
        set.Anims._M_finish = set.Anims._M_start + 2;
        set.Anims._M_end_of_storage._M_data = set.Anims._M_start + 4;
        set.InternalAlphas._M_start = i == 0 ? 0 : (CTransitionBlendAlpha*)malloc(2 * sizeof(CTransitionBlendAlpha));
        set.InternalAlphas._M_finish = set.InternalAlphas._M_start;
        set.InternalAlphas._M_end_of_storage._M_data = i == 0 ? 0 : set.InternalAlphas._M_start + 2;
        set.Type = (unsigned char)types[i];
        set.TypeFlags = flags[i];
        set.Priority = i;
    }
    l.remove_if(CAnimSet::CIsType(1));
    long expect[2] = { 1, 3 };
    int k = 0;
    for (L::iterator it = l.begin(); it != l.end(); ++it, ++k)
        if (k >= 2 || (*it).Priority != expect[k])
            ok = false;
    if (k != 2 || !LinksOk(l))
        ok = false;
    if (!ok) { printf("00711800_TEST FAIL\n"); return 1; }
    printf("00711800_TEST PASS\n");
    return 0;
}
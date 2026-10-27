// std::list::insert(iterator __position, const value_type& __x) -- STLport single-element list insert: malloc a 0x18-byte node, copy-construct the {CWideString, 3 words} value into it, link it before position and return an iterator to it
// Leaf: ?insert@?$list@UCUnprovenValue_004405f0@@V?$allocator@UCUnprovenValue_004405f0@@@std@@@std@@QAE?AU?$_List_iterator@UCUnprovenValue_004405f0@@U?$_Nonconst_traits@UCUnprovenValue_004405f0@@@std@@@2@U32@ABUCUnprovenValue_004405f0@@@Z
// Retail: Fable.exe @ 0x004405F0, 81 bytes, __fastcall `ret 0Ch` with `this` never read (hidden iterator result,
// position by value, &x).
// Identity: STLport 4.6 `list<_Tp>::insert(iterator, const _Tp&)` (_M_create_node: allocate(1) = malloc(0x18), the
// placement construct null-tests &node->_M_data; then the four link stores and the iterator returned through the hidden
// pointer), the shape of the landed list<CWeaponTrailEntry>::insert 0x0076FC60. The value is 16 bytes: a CWideString
// (copy constructor 0x0099B720) followed by three words copied by the implicit copy constructor. No caller in retail
// (gap-scan COMDAT), so the value type is unproven: CUnprovenValue_004405f0 (descriptive; the landed
// std::_Construct 0x0043F400 of this region guesses pair<CWideString, C3DVector> for a 16-byte value of the same form).
// Callees: malloc @ 0x00BFEA0E, CWideString::CWideString(const CWideString&) @ 0x0099B720.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};

struct CUnprovenValue_004405f0 {
    CWideString First;                                  // +0x00
    long Word1;                                         // +0x04
    long Word2;                                         // +0x08
    long Word3;                                         // +0x0C
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _Tp value_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
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
};

template list<CUnprovenValue_004405f0, allocator<CUnprovenValue_004405f0> >::iterator
list<CUnprovenValue_004405f0, allocator<CUnprovenValue_004405f0> >::insert(iterator, const CUnprovenValue_004405f0&);
}
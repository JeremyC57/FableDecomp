// CClassTypeInfo::CClassTypeInfo(const CClassTypeInfo& other) -- copy a class-type record: a fresh TypeList holding the other's class pointers, the counters and flags, the allocator function and the name
// Leaf: ??0CClassTypeInfo@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37B50, 101 bytes (+ int3 padding), `ret 4`.
// Identity: the (implicit, memberwise) copy constructor of the 20-byte vector<CClassTypeInfo> element of CClassFactory;
//   the field order NoAllocated +4, Used +8, Local +9, AllocFunc +0xc, Name +0x10 follows the landed default constructor
//   0x00A37880. The catalog's "list_node_allocate" is descriptive.
// Callees: malloc @ 0x00BFEA0E (the sentinel node), list<CClass*>::_M_insert_dispatch(pos, first, last, const __false_type&)
//   @ 0x00A37800, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.6 `list(const list& x) : _List_base(alloc) { insert(begin(), x.begin(), x.end()); }` inlined: the node
//   pointer is zeroed by the alloc proxy before the allocation, the sentinel linked to itself, then the range insert with
//   pos built in its argument slot (the non-const iterator's user copy constructor), the const iterators pushed, and the
//   __false_type tag homed in the dead `other` argument slot (`lea ebx,[esp+10h]`); the scalars copy in member order.
#include <stdlib.h>

class CClass;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};

namespace _STL {
struct __false_type { __false_type() { } };
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> struct _Nonconst_traits { typedef T& reference; };
template <class T> struct _Const_traits { typedef const T& reference; };
template <class T, class Traits> struct _List_iterator {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const iterator& x) : _M_node(x._M_node) { }      // the non-const iterator's user copy ctor
};
template <class T> class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    list(const list& x)
    {
        _M_node = 0;
        _List_node_base* n = (_List_node_base*)malloc(sizeof(_List_node<T>));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
        insert(begin(), x.begin(), x.end());
    }
    iterator begin() { return iterator(_M_node->_M_next); }
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&);   // 0x00A37800
    template <class InputIter>
    void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
};
}

typedef CClass* (__cdecl* ClassAllocFunc)();

class CClassTypeInfo {
public:
    _STL::list<CClass*> TypeList;                        // +0x00
    unsigned long       NoAllocated;                     // +0x04
    bool                Used;                            // +0x08
    bool                Local;                           // +0x09
    ClassAllocFunc      AllocFunc;                       // +0x0c
    CCharString         Name;                            // +0x10
    CClassTypeInfo(const CClassTypeInfo& other);
};

CClassTypeInfo::CClassTypeInfo(const CClassTypeInfo& other)
    : TypeList(other.TypeList)
    , NoAllocated(other.NoAllocated)
    , Used(other.Used)
    , Local(other.Local)
    , AllocFunc(other.AllocFunc)
    , Name(other.Name)
{
}
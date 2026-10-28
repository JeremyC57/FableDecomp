// std::list::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- (list<CFileInstaller::CPendingWrite>, const_iterator range) STLport 4.6 range insert: for each source element malloc a 0x14-byte node, placement-copy the 12-byte CPendingWrite and link it before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C69C0, 78 bytes, thiscall `ret 10h` (pos, first, last, tag).
// Identity: proven -- the out-of-line callee of the landed list<CFileInstaller::CPendingWrite> copy constructor 0x009C6F70
//   (which binds this exact symbol) and of the landed CActiveFile::CActiveFile(const CActiveFile&) 0x009C72B0 for its
//   PendingWrites list; 0x14-byte nodes = links + the 12-byte CPendingWrite (Ego_r.pdb). Callers 0x009C6D38, 0x009C6FA3,
//   0x009C7347. Catalog label "CActiveFile::CopyList_20byte" was already corrected to std::list::_M_insert_dispatch (chunk 220).
// Callees: malloc @ 0x00BFEA0E (_STLP_alloc_proxy::allocate(1)).
// Body: STLport 4.6 `for ( ; first != last; ++first) insert(pos, *first);` with the inline insert(pos, x) =
//   _M_create_node (allocate + placement _Construct: the `lea ecx,[eax+8] ; test ecx,ecx` null test) and the four link
//   stores; the model is the landed 0x009C6F70 TU with the iterator operators and the body added.
#include <stddef.h>
#include <new>
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
class CFileInstaller {
public:
    class CPendingWrite {                           // 12 bytes (Ego_r.pdb)
    public:
        void*         PBufferChunk;
        unsigned long FilePos;
        unsigned long WriteLength;
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(size_t __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(size_t __n) { return _MaybeReboundAlloc::allocate(__n); }
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
    _List_iterator_base() { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _AllocProxy;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _Node_allocator_type&)_M_node); }
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _AllocProxy _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
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
    template <class _InputIter>                                  // 0x009C69C0
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
};
template void list<CFileInstaller::CPendingWrite >::_M_insert_dispatch<list<CFileInstaller::CPendingWrite >::const_iterator>(
    list<CFileInstaller::CPendingWrite >::iterator, list<CFileInstaller::CPendingWrite >::const_iterator,
    list<CFileInstaller::CPendingWrite >::const_iterator, const __false_type&);
}
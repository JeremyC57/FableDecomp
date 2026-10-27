// std::list<CFileInstaller::CActiveRead*>::_M_insert_dispatch(iterator __pos, const_iterator __first, const_iterator __last, const __false_type&) -- insert a copy of [first, last) before pos
// Leaf: ??$_M_insert_dispatch@U?$_List_iterator@PAVCActiveRead@CFileInstaller@@U?$_Const_traits@PAVCActiveRead@CFileInstaller@@@std@@@std@@@?$list@PAVCActiveRead@CFileInstaller@@V?$allocator@PAVCActiveRead@CFileInstaller@@@std@@@std@@QAEXU?$_List_iterator@PAVCActiveRead@CFileInstaller@@U?$_Nonconst_traits@PAVCActiveRead@CFileInstaller@@@std@@@1@U?$_List_iterator@PAVCActiveRead@CFileInstaller@@U?$_Const_traits@PAVCActiveRead@CFileInstaller@@@std@@@1@1ABU__false_type@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6A10, 66 bytes, thiscall, `ret 10h` (this unused: the node allocator is malloc).
// Identity (proven): called by the landed list<CFileInstaller::CActiveRead*> copy constructor 0x009C6FB0 and the landed CActiveFile copy constructor 0x009C72B0 (PendingDecompression).
// Callees: malloc 0x00BFEA0E (0xC-byte nodes).
// Shape (family M132, representative 0x0055C400): STLport 4.6 `for (; __first != __last; ++__first) insert(__pos, *__first);`
//   with insert / _M_create_node inline (placement copy behind the null test on node+8, both neighbours cached; the landed
//   0x00517130 model).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CFileInstaller { public: class CActiveRead; };
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __value) { new (__p) _T1(__value); }
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;
};
typedef list<CFileInstaller::CActiveRead*> _L;
template void _L::_M_insert_dispatch<_L::const_iterator>(_L::iterator, _L::const_iterator, _L::const_iterator, const __false_type&);
}
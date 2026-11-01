// std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >::~_List_base() -- free every node (trivially destructible
//   pointers: clear() inlined), re-link the sentinel to itself, then free the sentinel node
// Leaf: ??1?$_List_base@PAVCSoundObjectLH@@V?$allocator@PAVCSoundObjectLH@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3FF60, 62 bytes, thiscall.
// Identity (proven, chunk c670): called by ~CSoundManagerLH 0x00A3EE3B on Sounds (+0x4C, the list<CSoundObjectLH*> of the landed
//   vfunc_5/6/10) and jumped to from 0x00A40190; body = STLport ~_List_base with clear() inlined (free nodes, reset sentinel, free
//   sentinel); the element class CSoundObjectLH is the landed descriptive spelling (the sound module is in neither PDB).
// Callees: free 0x00BFEA14 (allocator<_Node>::deallocate, per node and for the sentinel).
// Shape: the real STLport 4.6 text, copied from the landed ~CSoundManagerLH 0x00A3EDE0 row (whose TU emits this COMDAT) with
//   STLport's null-tested allocator<T>::deallocate, explicitly instantiated for the proven element type.
// Naming wave 16: landed as the free __fastcall SortTreeRecursively_00436900(TreeList*) copy of the 0x00436900 model;
//   re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base { public:
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    ~_List_base() { clear(); _M_node.deallocate((_List_node<_Tp>*)_M_node._M_data, 1); }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> { public:
};
}
class CSoundObjectLH;
template class std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >;

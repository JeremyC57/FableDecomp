// std::vector<std::list<C2DLineF>>::operator=(const vector& x) -- STLport vector assignment of the per-node line lists: reallocate and copy-construct when x outgrows the capacity, else assign over the live lists and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4?$vector@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A83D70, 268 bytes, thiscall `ret 4`, returns *this.
// Identity (proven): the landed CNavQuadTree::CNavigationLayer::operator= 0x00A85700 assigns LineLists
//   (vector<list<C2DLineF> >, Ego_r.pdb) through this row and binds exactly this symbol; its callees are the landed
//   list<C2DLineF> helpers __uninitialized_copy 0x00A82730, __destroy_aux 0x00A827D0, __copy_ptrs 0x00A82AC0.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the inline _M_allocate_and_copy / _M_clear), the three helpers above
//   (__fastcall first/last + result/tag on the stack).
// Shape: STLport 4.6 vector.c `operator=` verbatim; the tag-taking helpers are same-TU noinline stand-ins that never
//   read their tags, so the tags are homed on x's argument slot as in retail.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class C2DVector { public: float X, Y; };
class C2DLineF {                                           // 16 bytes
public:
    C2DVector Start;                                       // +0x0
    C2DVector End;                                         // +0x8
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                         // +0x00 (4-byte STLport list: the sentinel pointer)
};

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{   // stand-in for 0x00A82730 (only the operator= is graded)
    for (; __first != __last; ++__first, ++__result)
        __result->_M_node = __first->_M_node;
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{   // stand-in for 0x00A827D0
    for (; __first != __last; ++__first)
        __first->_M_node = 0;
}
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{   // stand-in for 0x00A82AC0
    for (; __first != __last; ++__first, ++__result)
        __result->_M_node = __first->_M_node;
    return __result;
}
#pragma auto_inline(on)
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}
template class std::vector<std::list<C2DLineF> >;
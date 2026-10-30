// std::vector::_M_fill_insert(CPrimitiveIterator* position, unsigned int n, const CPrimitiveIterator& x) -- (vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>) STLport insert of n copies of a particle group's child-primitive slot: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@V?$allocator@VCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@@std@@@std@@QAEXPAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBD950, 280 bytes, thiscall `ret 0Ch`.
// Identity: the landed vector<CPrimitiveIterator>::resize 0x00BBDD40 (Ego_r.pdb ?resize@?$vector@VCPrimitiveIterator@
//   CEngineInternalPrimitiveParticleGroup@@...@@QAEXI@Z; the group's PrimitiveIDToIterator table) calls this row through the
//   inline `insert(pos, n, x)` and binds this exact `_M_fill_insert` spelling; the landed _M_insert_overflow 0x00BBD7F0 and
//   __copy_backward_ptrs 0x00BBD550 already carry the CPrimitiveIterator element. STLport 4.6 vector.c verbatim.
// Callees (all over CPrimitiveIterator): __uninitialized_copy<CPrimitiveIterator*, CPrimitiveIterator*> 0x00BBD520 (twice),
//   __copy_backward_ptrs 0x00BBD550, fill 0x00BBD380 (both fills tail-merged), uninitialized_fill_n 0x00BBD580,
//   _M_insert_overflow 0x00BBD7F0. The landed 0x00BBD520 / 0x00BBD380 / 0x00BBD580 rows model the element as a descriptive
//   pair<bool, long>; it is CPrimitiveIterator (NAME_FIXES.tsv). Free helpers are __fastcall (first/last in ecx/edx).
// Shape: x_copy in the 8-byte frame; STLport's _List_iterator has a user copy constructor, so the element's implicit copy is
//   memberwise (Valid as one byte, the iterator as one dword; a trivial copy would move two dwords); retail homes
//   both `__false_type()` tags on the dead x argument slot, which holds a live value across the calls (finish - n, then
//   elems_after), so __uninitialized_copy and __copy_backward_ptrs get same-TU noinline bodies that never touch the tag
//   (the live-slot rule of 0x00AE82C0; the same source shape as vector<C3DQuad2>::_M_fill_insert 0x00AE6E60, without /GS).
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CEnginePrimitiveHandle;
namespace std {
struct _List_node_base;
template <class _Tp> struct _Nonconst_traits { };
struct _List_iterator_base {                    // STLport 4.6 _list.h: one node pointer
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }   // user copy ctor: memberwise element copy
};
}

class CEngineInternalPrimitiveParticleGroup {
public:
    class CPrimitiveIterator {                  // Ego_r.pdb, 8 bytes
    public:
        bool Valid;                             // +0x0
        std::_List_iterator<CEnginePrimitiveHandle, std::_Nonconst_traits<CEnginePrimitiveHandle> > Iterator;   // +0x4
        CPrimitiveIterator() : Valid(false) { }
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00BBD520
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00BBD580
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _BidirectionalIter>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                       _BidirectionalIter __result, const __false_type&)
{                                                                                              // 0x00BBD550
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00BBD380

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00BBD7F0
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>::_M_fill_insert(
    CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, unsigned int,
    const CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator&);
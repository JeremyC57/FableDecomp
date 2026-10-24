// _STL::vector::insert(iterator position, const value_type& x) -- STLport single-element insert into the occlusion manager's sorted handle map (vector<pair<unsigned long, unsigned long> >): construct at the end, or shift the tail up one slot and assign x into position, or grow; return the iterator at the insert index
// Leaf: ?insert@?$vector@U?$pair@KK
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8F650, 218 bytes, thiscall, `ret 8`, the iterator (a plain pointer) in eax.
// Identity: called from the landed CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID 0x00B8D6C0 (insert into
//   HandleMapper, Ego_r.pdb vector<pair<unsigned long, unsigned long> >). Callee 0x009B6300 is the shared
//   _M_insert_overflow(pos, x, const __false_type&, size_type fill_len, bool atend) of the 8-byte pair vector (manifest
//   "vector_insert"). Catalog "Vector_InsertElement_WithReallocation" is descriptive.
// Callees: _M_insert_overflow 0x009B6300 (tag homed in the dead `x` argument slot).
// Shape: STLport 4.x `vector::insert(position, x)` as the landed vector<CKeyCamera>::insert 0x0075FAE0, with the 8-byte
//   pair's implicit copy inline (placement-new null tests), x_copy held in ebx / a frame word, and the random-access
//   __copy_backward loop inline (count = (finish - 2) - position, `dec eax` counter); `return begin() + n` duplicated
//   into each tail with n parked in the dead `position` argument slot. __copy_backward_ptrs and __copy_backward are
//   __forceinline: with plain `inline` VC7.1 calls __copy_backward_ptrs out of line here.
inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(iterator __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x009B6300
    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
{
    size_type __n = __position - begin();
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        if (__position == end()) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else {
            _Construct(this->_M_finish, *(this->_M_finish - 1));
            ++this->_M_finish;
            _Tp __x_copy = __x;
            __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
            *__position = __x_copy;
        }
    }
    else
        _M_insert_overflow(__position, __x, __false_type(), 1UL);
    return begin() + __n;
}

template class vector<pair<unsigned long, unsigned long> >;

}   // namespace _STL
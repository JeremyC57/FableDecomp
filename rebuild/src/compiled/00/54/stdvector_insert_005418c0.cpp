// std::vector::insert(iterator position, const value_type& x) -- (vector<NUISystem::CComponent*>) STLport single insert: construct at the end or shift the tail up one slot (memmove) and store x, else reallocate; return begin() + offset
// Leaf: ?insert@?$vector@PAVCComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005418C0, 167 bytes, thiscall, `ret 8`, returns the inserted iterator.
// Identity: the manifest's "std::vector::insert_single" is descriptive. The only caller is the landed
//   NUISystem::CDynamicList::InsertChild 0x005414A0, which declares `Insert(CComponent**, CComponent* const&)` @
//   0x005418C0 on its child vector (+0x174) next to the shared pointer-vector overflow 0x00485DBC.
// Callees: memmove 0x00BFEAE6, vector::_M_insert_overflow(pos, x, const __true_type&, fill_len, atend)
//   0x00485DBC (the pointer-vector body that the landed CTCCreditsUI::Activate 0x007E4B60 and CTCInventoryMap::
//   OpenInventory 0x006116E0 also call).
// Shape: STLport 4.6 `n = pos - begin(); if (finish != end_of_storage) { if (pos == end()) { _Construct(finish, x);
//   ++finish; } else { _Construct(finish, *(finish - 1)); ++finish; _Tp x_copy = x; __copy_backward_ptrs(pos,
//   finish - 2, finish - 1, __true_type()); *pos = x_copy; } } else _M_insert_overflow(pos, x, __true_type(), 1UL);
//   return begin() + n;` -- placement new behind its null test (no throw() on the placement operator), the trivial
//   backward copy as `_Num > 0 ? memmove(result - _Num, first, _Num) : 0`, begin() re-read for the result.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

namespace NUISystem { class CComponent; }

namespace std {

struct __true_type { __true_type() { } };

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : 0;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);          // 0x00485DBC

    iterator insert(iterator __position, const _Tp& __x)
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
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __true_type());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, __true_type(), 1UL);
        return begin() + __n;
    }
};

template vector<NUISystem::CComponent*>::iterator vector<NUISystem::CComponent*>::insert(iterator, NUISystem::CComponent* const&);

} // namespace std
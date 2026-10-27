// std::vector::resize(size_type new_size, const MinimapTheme& x) -- (vector<CMinimapData::MinimapTheme>) shrink by erasing the tail, else append new_size - size() copies of x
// Leaf: ?resize@?$vector@W4MinimapTheme@CMinimapData@@V?$allocator@W4MinimapTheme@CMinimapData@@@std@@@std@@QAEXIABW4MinimapTheme@CMinimapData@@@Z
// Retail: Fable.exe @ 0x00643890, 76 bytes, thiscall `ret 8`.
// Identity (proven element type): its callers are CMinimapData::ResizeMinimapThemeList 0x00643070 (`MinimapThemeList.resize(
// MinimapExtents.Width * MinimapExtents.Height, MinimapTheme())` on the vector at CMinimapData +0x08; Ego_r.pdb
// ?ResizeMinimapThemeList@CMinimapData@@QAEXXZ), a sibling at 0x00643090, and the one-argument resize(n) 0x006438E0 (x = 0 in
// its frame slot). CMinimapData::MinimapThemeList is Ego_r.pdb vector<CMinimapData::MinimapTheme>.
// Callees: _M_fill_insert(iterator, size_type, const MinimapTheme&) 0x006436C0.
// Shape: STLport 4.6 `resize`: size() computed once; the erase(begin() + n, end()) inlines __copy_ptrs over a non-trivial
// (enum) element as the counted `__copy` loop of the empty [end, finish) range, then `_M_finish = __i`; insert(end(), n -
// size(), x) stays a call to the out-of-line _M_fill_insert.
// Layout: STLport vector (12 bytes): _M_start +0, _M_finish +4, _M_end_of_storage +8.
class CMinimapData {
public:
    enum MinimapTheme { MINIMAP_THEME_FORCE_DWORD = 0x7fffffff };
};

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x006436C0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template class vector<CMinimapData::MinimapTheme>;
}
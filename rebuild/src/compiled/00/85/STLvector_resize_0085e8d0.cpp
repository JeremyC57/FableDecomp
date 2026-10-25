// std::vector::resize(unsigned int new_size, const NThingMap::CThingMapCell& x) -- resize a mapwho cell row: erase the surplus tail (inline element-wise copy-down), or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085E8D0, 76 bytes, thiscall `ret 8`.
// Callers: 0x0085D317 in CThingMap::InitMapwhoLevels 0x0085D180 and the one-argument resize wrapper after this body
//   (0x0085E982); CThingMap's Cells member is a vector<vector<NThingMap::CThingMapCell> > (Ego_r.pdb).
// Callees: vector<CThingMapCell>::_M_fill_insert(pos, n, x) 0x0085E6C0.
// Element: Ego_r.pdb NThingMap::CThingMapCell (4 bytes, CMapwhoNode* PFirstNode) -- a class, so STLport's type traits give
//   __false_type and erase copies element-wise (the landed 0x00412A50 is the same body over an enum).
// Label: catalog "vector<unsigned int>::resize" is propagated (unsigned int would copy with memmove).
// Shape: STLport 4.6 `resize(n, x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }`:
//   erase inline -- `__copy_ptrs(last, _M_finish, first, __false_type())` = the random-access `__copy` loop of
//   `_M_finish - last` (= 0) elements, a no-op _Destroy, `_M_finish = i` -- and insert forwarding to _M_fill_insert.
class CMapwhoNode;
namespace NThingMap {
class CThingMapCell {                           // Ego_r.pdb, 4 bytes
public:
    CMapwhoNode* PFirstNode;                    // +0x0
};
}

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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
    return __copy(__first, __last, __result, random_access_iterator_tag(), (long*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
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
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x0085E6C0
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<NThingMap::CThingMapCell>::resize(unsigned int, const NThingMap::CThingMapCell&);
}
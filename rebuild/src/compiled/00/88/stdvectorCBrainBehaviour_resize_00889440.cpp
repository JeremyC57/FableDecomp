// std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight>::resize(size_type n, const value_type& x) -- shrink by erasing the tail, else grow by inserting n - size() copies of x at the end
// Leaf: ?resize@?$vector@UCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@UCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@@std@@@std@@QAEXIABUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00889440, 87 bytes, thiscall, `ret 8`.
// Identity (probable): its caller 0x00887330 (landed) builds a zeroed 12-byte CThemeWeight (Ego_r.pdb Name / Priority / ScriptAIPriority; the first element then gets the name and priority 1.0f) and calls resize(1, value) on the vector at +0x08; other callers 0x00887380 and 0x00889650; bound only as the stand-in ?resize@CRetailVector_CBrainBehaviour@@QAEXKABVCBrainBehaviour@@@Z.
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00885E10 (erase's __copy_ptrs,
//   __fastcall), vector<T>::_M_fill_insert(pos, n, x) 0x008890F0.
// Body: STLport 4.6 `if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x);` -- erase's _Destroy loop is
//   empty (trivial destructor), so only `_M_finish = __copy(...)` remains; the tag is homed on the n argument's slot (same-TU
//   noinline __copy body, as the landed STLport helpers).
// Shape (family L078).
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                        // +0x00
};
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry { public:
    struct CThemeWeight {                                 // Ego_r.pdb, 12 bytes
        long  ThemeID;                                    // +0x00
        float Weight;                                     // +0x04
        bool  LockWeight;                                 // +0x08
    };
};
}
typedef NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight CThemeWeight;
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
#pragma auto_inline(off)
template <class _InputIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)   // 0x00885E10
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline()
template <class _Tp> inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {     // STLport 4.6, 12 bytes
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x008890F0
};
}   // namespace std
template void std::vector<CThemeWeight >::resize(unsigned int, const CThemeWeight&);
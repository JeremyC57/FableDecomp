// std::vector<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >>::resize(unsigned int __new_size, const std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >& __x) -- shrink by erasing the tail, or grow by filling copies of x at the end
// Leaf: ?resize@?$vector@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@V?$allocator@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@@2@@std@@QAEXIABU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCF90, 104 bytes, thiscall, `ret 8`.
// Identity (proven): its callees are the landed pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > __copy 0x006FBC50 and __destroy_aux 0x006FB8A0 (bound with that element) and _M_fill_insert 0x006FC7A0, 12-byte stride; called from NAnimationEvents::CManager::LoadBinary 0x006F9F90 (AnimToExtraEventsMap).
// Callees: std::__copy<T*, T*, int> 0x006FBC50, std::__destroy_aux<T*> 0x006FB8A0, vector<T>::_M_fill_insert 0x006FC7A0.
// Shape (family M114, representative 0x0051B770): STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end());
//   else insert(end(), __new_size - size(), __x);` -- erase is `__copy_ptrs(__last, _M_finish, __first, __false_type())`
//   (the counted __copy out of line, its tag homed in the dead `__new_size` slot, the distance pointer null), `_Destroy(__i,
//   _M_finish)` (__destroy_aux out of line), `_M_finish = __i`; insert is the out-of-line _M_fill_insert.
namespace NAnimationEvents { class CEvent; }
template <class T, int N> class CSmallVector {          // 8 bytes: Start, Size
public:
    CSmallVector& operator=(const CSmallVector& other);
    ~CSmallVector();
    T* Start;
    int Size;
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
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
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template void vector<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > >::resize(unsigned int, const std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >&);
}
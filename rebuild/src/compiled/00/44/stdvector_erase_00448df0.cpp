// std::vector::erase(iterator __first, iterator __last) -- (vector<CCountedPointer<const CInputTypeBase> >) STLport range erase: assign the tail down over the gap, destroy the vacated tail, shrink _M_finish, return first
// Leaf: ?erase@?$vector@V?$CCountedPointer@$$CBVCInputTypeBase@@@@V?$allocator@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@@std@@QAEPAV?$CCountedPointer@$$CBVCInputTypeBase@@@@PAV3@0@Z
// Retail: Fable.exe @ 0x00448DF0, 57 bytes, __fastcall `ret 8`, returns __first in eax.
// Identity: the element type is proven by the callees: 0x00448860 is bound as
// ??$__destroy_aux@PAV?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@YIXPAV?$CCountedPointer@$$CBVCInputTypeBase@@@@0ABU__false_type@0@@Z,
// and 0x00448300 assigns 8-byte CCountedPointer elements (`sar edx,3`, count-block release / add-ref) -- STLport's
// __copy(first, last, result, const random_access_iterator_tag&, int*) for that element (catalog stand-in
// "Move_CountedPointer_00448300"). The vector<CCountedPointer<const CInputTypeBase> > instantiation is in the PDB name
// sets (_Allocate / _Cons_val / _Construct over it). No direct caller in retail (gap-scan COMDAT).
// Shape: STLport 4.6 `pointer __i = copy(__last, _M_finish, __first); _Destroy(__i, _M_finish); _M_finish = __i;
// return __first;` -- copy reaches the non-trivial __copy(.., random_access_iterator_tag(), (int*)0) and _Destroy the
// out-of-line __destroy_aux(.., __false_type()); both empty tags are homed in __first's dead argument slot (`lea eax,
// [esp+14h]` / `lea ecx,[esp+10h]`), the callees are __fastcall (first in ecx, last in edx).
class CInputTypeBase;
class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer<T>& other);
    ~CCountedPointer();
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other);
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);          // 0x00448300
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00448860
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template CCountedPointer<const CInputTypeBase>*
vector<CCountedPointer<const CInputTypeBase>, allocator<CCountedPointer<const CInputTypeBase> > >::erase(
    CCountedPointer<const CInputTypeBase>*, CCountedPointer<const CInputTypeBase>*);
}
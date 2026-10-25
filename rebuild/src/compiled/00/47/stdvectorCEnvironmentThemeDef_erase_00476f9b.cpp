// std::vector<CEnvironmentThemeDef>::erase(iterator first, iterator last) -- STLport range erase: assign the tail down over [first, last), destroy the surplus, move finish, return first
// Leaf: ?erase@?$vector@VCEnvironmentThemeDef@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00476F9B, 92 bytes, thiscall, `ret 8`, returns first; size-optimised
//   frame-pointer def TU (unaligned entry, `push ebp ; mov ebp,esp`, `leave`) -- `#pragma optimize("s", on)`.
// Identity: the manifest's "std::vector::erase" is right but unparameterised. The element is 0xC4 bytes (the `idiv 0C4h`)
//   and the callees are the landed CEnvironmentThemeDef::operator= 0x00473046 and the __destroy_aux 0x00450B1B that the
//   landed CEnvironmentThemeDefVector::operator= 0x00472EDA (same def TU) also calls.
// Callees: CEnvironmentThemeDef::operator=(const CEnvironmentThemeDef&) 0x00473046, __destroy_aux(first, last,
//   const __false_type&) 0x00450B1B (`__fastcall`, the tag by reference, `ret 4`).
// Shape: STLport 4.6 `pointer i = __copy_ptrs(last, finish, first, __false_type()); _Destroy(i, finish); finish = i;
//   return first;` with the counted random-access __copy inline (the count `(finish - last) / 0xC4` homed in the dead
//   `last` slot, the source kept as an offset from the destination in [ebp-4]) and _Destroy's
//   `__destroy_aux(first, last, __false_type())` tag temporary placed in the top byte of the `first` slot
//   (`lea eax,[ebp+0Bh]`; the tag is never written). That slot needs the same-TU stand-in body of __destroy_aux (it
//   never reads its tag); declared only, VC7.1 puts the tag on the dead `last` slot (`[ebp+0Fh]`).
#pragma optimize("s", on)

class CEnvironmentThemeDef {                                // retail 0xC4 bytes (only the size and the assignment matter here)
public:
    virtual ~CEnvironmentThemeDef();
    unsigned char _body[0xC0];
    CEnvironmentThemeDef& operator=(const CEnvironmentThemeDef& other);   // 0x00473046
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

// Same-TU stand-in body of the out-of-line element destruction 0x00450B1B (it never reads the tag).
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->~CEnvironmentThemeDef();
}
#pragma auto_inline(on)

template <class _ForwardIterator, class _Tp>
__forceinline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> __forceinline random_access_iterator_tag __iterator_category(const _Tp*)
{
    return random_access_iterator_tag();
}
template <class _Tp> __forceinline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _ForwardIterator> __forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __value_type(__first));
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, __iterator_category(__first), __distance_type(__first));
}

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
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template vector<CEnvironmentThemeDef>::iterator vector<CEnvironmentThemeDef>::erase(iterator, iterator);

} // namespace std
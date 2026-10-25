// std::vector<unsigned int>::_M_fill_insert(unsigned int* position, unsigned int n, const unsigned int& x) -- STLport insert of n copies of a 4-byte scalar: memmove the tail inside the spare capacity and fill the gap, or grow through _M_insert_overflow
// Grade: DIFFER(276v277) -- register allocation only: retail keeps n in ecx and pushes edi after the capacity test (x_copy then elems_after spilled to the x / n slots); ours holds n in edi pushed at entry. The by-value fill_n value was needed for retail's `rep stosd` (a const& to the escaped x_copy reloads it per pass, 279v277); five more forms (the _IsPOD tag copy, a same-TU fill body, tag without ctor, reordered locals, /Oa, /Ow) did not move n.
// Leaf: ?_M_fill_insert@?$vector@I
// Retail: Fable.exe (TLC, VC7.1) @ 0x00441020, 277 bytes, thiscall `ret 0Ch`; /O2; no frame.
// Identity: the out-of-line fill insert of the landed vector<unsigned int>::resize(n, x) 0x00442770 (its "insert at
//   end()" arm); shared (COMDAT-folded) by every vector of a 4-byte trivially assignable element: callers are
//   insert(position, n, x) (tail jump 0x004416D0), resize 0x004427BD and three engine sites (0x00887319
//   CEnvironmentThemePriorityStackEntry::Reset, 0x00ACA02C CParticleMorphs::BuildMorphRemap, 0x00B67F68). The catalog
//   label "vector_insert_with_fill" is behavioural.
// Callees: memmove 0x00BFEAE6 (three inlined __copy_trivial / __copy_trivial_backward), fill(first, last, x) 0x0043DFC0
//   (__fastcall, `ret 4`; both fills), _M_insert_overflow(position, x, const __true_type&, n, atend) 0x004408A0 (the
//   folded 4-byte growth path, labelled vector<float>).
// Shape: STLport 4.x `_M_fill_insert` with the POD (`__true_type`) helpers: x_copy lives in the dead `x` argument slot
//   and elems_after in the `n` slot; `uninitialized_fill_n` is VC7.1's `rep stosd` (with the empty-count guard; the
//   POD fill_n takes its value by value here, else the escaped x_copy is reloaded every pass); the
//   _M_insert_overflow tag is homed on the live `x` slot, which VC7.1 does only with a same-TU body for that member
//   whose tag is unnamed (GOTCHAS 504/667; body as the landed POD overflow 0x00467831).
// Model: STLport's three vector words; `unsigned int` per the landed resize 0x00442770.
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, (const char*)__last - (const char*)__first)) +
          ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial_backward(__first, __last, __result);
}

template <class _Tp> inline _Tp* fill_n(_Tp* __first, unsigned int __n, _Tp __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _Tp> inline _Tp* __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _Tp> inline _Tp* uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                  // 0x0043DFC0

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x004408A0
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __true_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __true_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __true_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __true_type(), __n);
    }
}

template void vector<unsigned int>::_M_fill_insert(unsigned int*, unsigned int, const unsigned int&);

}

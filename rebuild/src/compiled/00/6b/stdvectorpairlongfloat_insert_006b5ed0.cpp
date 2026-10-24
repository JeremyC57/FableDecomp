// std::vector<pair<long,float> >::insert(iterator pos, const value_type& x) -- STLport single-element insert: append, shift the tail up one slot, or grow through _M_insert_overflow; returns begin() + offset
// Leaf: ?insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5ED0, length 220 bytes (+ int3 padding), thiscall `ret 8`.
// Callees: vector<pair<long,float> >::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool)
//   @ 0x006B5BE0 (declared only).
// Identity: the catalog's "Vector_InsertElement" is a descriptive auto label; the caller is 0x006B3F4B in
//   CEnvironment::SetWeightedThemes, inserting into the function-static `ThemeWeights` vector (0x013BA8EC) at the
//   std::lower_bound (0x006B5A50) position.
// Element: pair<long, float> (8 bytes) with the implicit copy, so every copy is two dword moves.
// Shape: STLport 4.x insert. The element offset is computed first and homed in the dead pos-argument slot;
//   the shift is _Construct(finish, finish[-1]), ++finish, a copy of x, then __copy_backward_ptrs with the
//   __false_type assignment trait -> __copy_backward over a count loop (inlined), and *pos = the copy. The
//   overflow call passes (pos, x, __false_type() homed in the dead x slot, 1, false). Two details are
//   load-bearing: the tags carry a user constructor (a POD empty tag gets a `mov byte ptr [esp+30h],0`
//   zero fill retail lacks), and __copy_backward_ptrs is __forceinline (plain inline stays a call, and the
//   offset then lives in ebx instead of the spilled ebp: DIFFER 200v220).
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
typedef int ptrdiff_t;
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp>
class vector {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // 0x006B5BE0
    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage) {
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
};
template class vector<pair<long, float> >;
}
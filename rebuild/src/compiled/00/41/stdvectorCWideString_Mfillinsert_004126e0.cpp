// std::vector<CWideString>::_M_fill_insert(CWideString* position, unsigned int n, const CWideString& x) -- STLport insert of n copies of a wide string: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCWideString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004126E0, 254 bytes, thiscall `ret 0Ch`; /O2; no frame.
// Identity: stride 4 with CWideString's copy constructor 0x0099B720, assignment 0x0099B7D0 and destructor 0x0099B510;
//   the growth path is vector<CWideString>::_M_insert_overflow 0x00412330 (this chunk). Callers: insert(position, n, x)
//   (tail jump 0x004129B0) and the resize / insert sites 0x0040C587, 0x0040CC41, 0x00412B58, 0x00412BC0. The catalog
//   label "vector_resize_with_fill" is behavioural.
// Callees: CWideString(const CWideString&) 0x0099B720 (x_copy), __uninitialized_copy(first, last, result, const
//   __false_type&) 0x0040DD60 (twice), __copy_backward_ptrs(first, last, result, const __false_type&) 0x0040FBF0,
//   uninitialized_fill_n(first, n, x) 0x0040F5B0, fill(first, last, x) 0x0040E0D0 (both fills tail-merged),
//   ~CWideString 0x0099B510 (x_copy), _M_insert_overflow(position, x, const __false_type&, n, atend) 0x00412330; every
//   free helper is __fastcall (first/last in ecx/edx).
// Shape: STLport 4.x `_M_fill_insert` verbatim. x_copy is copy-constructed into the dead `x` argument slot; the empty
//   tags sit on the `n` slot ([esp+18h]) that later holds old_finish - n / n - elems_after. The helpers are the TU's
//   `inline` STLport templates (VC7.1 leaves them out of line here), except __copy_backward_ptrs, which needs a
//   same-TU noinline body (as plain `inline` VC7.1 inlines it down to a cdecl __copy_backward call, 255 v 256).
// Model: CWideString is the engine's one-pointer wide-string handle; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    CWideString(const CWideString& other);                // 0x0099B720
    const CWideString& operator=(const CWideString& other);     // 0x0099B7D0
    ~CWideString();                                       // 0x0099B510
private:
    void* m_pData;                                        // +0x00
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0040DD60
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)       // 0x0040F5B0
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                           _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)   // 0x0040FBF0
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
inline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)                    // 0x0040E0D0
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00412330
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CWideString>::_M_fill_insert(CWideString*, unsigned int, const CWideString&);


}
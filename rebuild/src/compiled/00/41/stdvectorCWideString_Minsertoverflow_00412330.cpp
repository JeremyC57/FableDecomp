// std::vector<CWideString>::_M_insert_overflow(CWideString* position, const CWideString& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path of a wide-string vector: allocate size + max(size, n), copy-construct the head, n copies of x and (unless at the end) the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCWideString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412330, 280 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd); /O2.
// Identity: stride 4 with CWideString's copy constructor 0x0099B720 and destructor 0x0099B510; 28 callers across the
//   engine (push_back / insert of every vector<CWideString>, 0x00409326 .. 0x00E81C54): one COMDAT. The catalog label
//   "Std_Vector_Insert_Range_Copies" is wrong: this is _M_insert_overflow.
// Callees: malloc 0x00BFEA0E, CWideString(const CWideString&) 0x0099B720, __uninitialized_copy(first, last, result,
//   const __false_type&) 0x0040DD60 (__fastcall `ret 8`; the tail copy only), ~CWideString 0x0099B510, free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` with `inline` helpers: VC7.1's /Ob1 budget
//   inlines the head __uninitialized_copy, the n == 1 `_Construct` and the __uninitialized_fill_n loop, then calls the
//   tail __uninitialized_copy out of line (its tag homed on the live `atEnd` slot, [esp+28h]); `_M_clear` inlines the
//   destructor loop and the free; the new start is kept in the dead fillLength slot.
// Model: CWideString is the engine's one-pointer wide-string handle; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
private:
    void* m_pData;                                        // +0x00
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0040DD60 (tail call site)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                     // inlined
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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
                            bool __atend = false);
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

template void vector<CWideString>::_M_insert_overflow(CWideString*, const CWideString&, const __false_type&,
                                                          unsigned int, bool);

}
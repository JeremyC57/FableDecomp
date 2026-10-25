// std::vector<CDateAndTime>::_M_insert_overflow(CDateAndTime* position, const CDateAndTime& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path of a date-and-time vector: allocate size + max(size, n), copy the head, construct n copies of x, copy the tail unless at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCDateAndTime@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004125A0, 310 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd); /O2.
// Identity: called by vector<CDateAndTime>::_M_fill_insert 0x0048608C (landed; it declares this callee), by
//   CUserProfileManager::LoadFileList 0x00409372 and by the identical-folded 28-byte push_back 0x004129A7. The catalog
//   label "Std_Vector_Insert_Range_Copies_28byte" is behavioural (the corrections table already names it).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 (everything else is inlined).
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` with every helper inlined (the two copy/fill
//   loops `__forceinline`: as plain `inline` VC7.1's /Ob1 budget sends the tail copy out of line, 284 v 310): the signed divide by 28 for size(), the const-reference `max` (old size homed in [esp+10h]), the
//   null-checked placement copies (`rep movsd` x7: the implicit copy of the 7-dword record) for the head, the single
//   element, the fill and the tail, and the trivial-destructor `_M_clear` free.
// Model: Ego_r.pdb CDateAndTime (Year, Month, Day, Hour, Minute, Second, Millisecond; 28 bytes) with its declared
//   default constructor 0x009E9AB0 and the implicit copy, as the landed 0x0048608C; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CDateAndTime {                                      // Ego_r.pdb, 28 bytes
public:
    long          Year;                                   // +0x00
    unsigned long Month;                                  // +0x04
    unsigned long Day;                                    // +0x08
    unsigned long Hour;                                   // +0x0C
    unsigned long Minute;                                 // +0x10
    unsigned long Second;                                 // +0x14
    unsigned long Millisecond;                            // +0x18
    CDateAndTime();                                       // 0x009E9AB0
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

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

template void vector<CDateAndTime>::_M_insert_overflow(CDateAndTime*, const CDateAndTime&, const __false_type&,
                                                          unsigned int, bool);

}
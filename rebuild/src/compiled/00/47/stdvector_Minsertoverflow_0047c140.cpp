// std::vector::_M_insert_overflow(CHeroLogBookEntry* position, const CHeroLogBookEntry& x, const __false_type&, unsigned int fillLength, bool atEnd) -- (vector<CHeroLogBook::CHeroLogBookEntry>) STLport growth path of the hero log book's entry list: allocate size + max(size, n), copy-construct the head, n copies of x and (unless at the end) the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCHeroLogBookEntry@CHeroLogBook@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C140, 270 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd); /O2.
// Identity: stride 12 with the landed out-of-line helpers of this element, __uninitialized_copy 0x0047B670,
//   __uninitialized_fill_n 0x0047B6C0 and __destroy_aux 0x0047BAF0; callers are CHeroLogBook::AddLogEntry 0x0047AD46 /
//   0x0047ADD6 and the entry push_back 0x0047C4C0. The catalog label "Vector_Reserve_And_Assign" is wrong: this is
//   _M_insert_overflow.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x0047B670 (head and tail), CWideString(const CWideString&) 0x0099B720
//   (x3, the inlined n == 1 copy), __uninitialized_fill_n 0x0047B6C0, __destroy_aux(first, last, const __false_type&)
//   0x0047BAF0, free 0x00BFEA14; the helpers are __fastcall with the tag reference on the stack.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` as 0x0047C030, but `_M_clear`'s `_Destroy` calls
//   __destroy_aux out of line; every empty tag (four calls) is homed on the live `atEnd` slot ([esp+28h]), which needs
//   same-TU bodies (tag unnamed) for the three helpers (GOTCHAS 504/667).
// Model: Ego_r.pdb CHeroLogBook::CHeroLogBookEntry {CWideString Name, AbbreviatedName, Content} (12 bytes, implicit copy
//   and destructor), as the landed 0x0047B670 / 0x0047BAF0; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CWideStringData;

class CWideString {
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
private:
    CWideStringData* PStringData;                         // +0x00
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {                             // Ego_r.pdb, 12 bytes
    public:
        CWideString Name;                                 // +0x00
        CWideString AbbreviatedName;                      // +0x04
        CWideString Content;                              // +0x08
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0047B670
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                     // 0x0047B6C0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)   // 0x0047BAF0
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
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

template void vector<CHeroLogBook::CHeroLogBookEntry>::_M_insert_overflow(CHeroLogBook::CHeroLogBookEntry*, const CHeroLogBook::CHeroLogBookEntry&, const __false_type&,
                                                          unsigned int, bool);

}
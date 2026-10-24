// std::__uninitialized_fill_n(basic_string* first, unsigned int n, const basic_string& x, const __false_type&) -- STLport placement fill of n raw string slots with copies of x
// Leaf: ??$__uninitialized_fill_n
// Grade: DIFFER(143v142) -- int3-after-noreturn row: RELOCATION_MATCH against the 143-byte row read straight from Fable.exe (byte 142 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00568F10, 142 bytes (the noreturn length-error call is the last instruction),
//   /Gr `__fastcall` (first in ecx, n in edx, &x and the tag on the stack: `ret 8`). Caller:
//   vector<string>::_M_insert_overflow 0x00569E30 (the fill_len != 1 arm). The catalog label
//   "ElectrocutedEffect_FillFromRange" is propagated/descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator<char>::allocate), memmove @ 0x00BFEAE6 (__copy_trivial),
//   _String_base<char>::_M_throw_length_error() const @ 0x00565A30 (never returns).
// Layout: STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (unsigned n: `jbe`/`ja`)
//   with the string copy constructor inline exactly as in the sibling __uninitialized_copy 0x00568E70; n lives
//   in the first frame word, x's finish in the second.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;

    unsigned int max_size() const { return (unsigned int)(-1) / sizeof(char) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        } else
            _M_throw_length_error();
    }
    __declspec(noreturn) void _M_throw_length_error() const;                     // 0x00565A30
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};

class basic_string : public _String_base {
public:
    allocator_type get_allocator() const { return _M_end_of_storage; }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
    void _M_range_initialize(const char* __f, const char* __l)
    {
        int __n = __l - __f;
        _M_allocate_block(__n + 1);
        _M_finish = uninitialized_copy(__f, __l, _M_start);
        _M_terminate_string();
    }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template basic_string* __fastcall __uninitialized_fill_n<basic_string*, unsigned int, basic_string>(
    basic_string*, unsigned int, const basic_string&, const __false_type&);

}
